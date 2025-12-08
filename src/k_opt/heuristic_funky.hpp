#ifndef TSP_K_OPT_HEURISTIC_FUNKY_HPP
#define TSP_K_OPT_HEURISTIC_FUNKY_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
#include "heuristic.hpp"
#include "cut_strategy.hpp"

#include <unordered_set>  // testing
#include <cassert> // testing



namespace k_opt {

template<
    typename cost_t,
    typename cut_strategy_t,
    int K=-1,
    typename vertex_t=int
>
requires CutStrategy<cut_strategy_t, cost_t, vertex_t, K>
class KOptFunky : public Heuristic<cost_t, vertex_t>
{
    static_assert(K == -1 || K >= 2, "K must be -1 (dynamic) or >= 2 (static)");

 public:

    explicit KOptFunky(cut_strategy_t cut_strategy, const int k=-1)
        : cut(std::move(cut_strategy)), k(k) { }

    ~KOptFunky() = default;

    [[ nodiscard ]] constexpr int getK() const {
        return K == -1 ? this->k : K;
    }

    void setK(const int k) {
        static_assert(K == -1, "Cannot call setK on static K specialization");
        this->k = k;
    }

 protected:

    const cut_strategy_t cut;

    cost_t run(
        std::vector<vertex_t> &solution,
        cost_t cur_cost,
        History<cost_t> &history,
        const std::vector<std::vector<cost_t>> &weights,
        const int verbose = 0
    ) const override;

 private:

    int k;

};


namespace detail {

/**
 * @brief Unfolded at compile-time.
 */
template<int K, int Depth, typename callback_t, typename... Limits>
[[ gnu::always_inline ]]
inline bool loopSegmentsStatic(
    const int n,
    std::array<std::pair<int, int>, K> &segs,
    const int start,
    callback_t &&cb,
    const Limits... limits
) {
    if constexpr (Depth == K) {
        segs[0].first = start;
        return cb(segs);
    } else {
        constexpr int num_limits = sizeof...(limits);
        int lim;
        if constexpr (Depth < num_limits) {
            lim = std::get<Depth>(std::forward_as_tuple(limits...));
        } else {
            lim = n - K + Depth;
        }
        for (int i = start; i < lim; ++i) {
            // reset start always as cb may modify
            segs[Depth].first = start;
            segs[Depth].second = i;
            const bool is_done = loopSegmentsStatic<K, Depth + 1>(
                n, segs, i + 1, std::forward<callback_t>(cb)
            );
            if (is_done) return true;
        }
        return false;
    }
}

template<typename callback_t>
[[ gnu::always_inline ]]
inline bool loopSegmentsDynamic(callback_t &cb) {

}


}  // namespace detail


template<typename cost_t, typename cut_strategy_t, int K, typename vertex_t>
requires CutStrategy<cut_strategy_t, cost_t, vertex_t, K>
cost_t KOptFunky<cost_t, cut_strategy_t, K, vertex_t>::run(
    std::vector<vertex_t> &path,
    cost_t cur_cost,
    History<cost_t> &history,
    const std::vector<std::vector<cost_t>> &weights,
    const int verbose
) const {
    using seg_t = std::pair<int, int>;
    const int n = path.size();
    const int log_freq = 10;  // log best cost every 1 iters
    const int flush_freq = 10000;  // flush every 10000 costs
    const bool do_record_history = !history.isStopped();
    history.addCost(cur_cost);
    history.addPath(path, -1, -1, -1, 0);
    cost_t cur_cost_change = (cost_t) 0;

    const int k = this->getK();
    int last_update_i = n - k - 1;
    int last_update_j = last_update_i + 1;
    std::vector<seg_t> cur_seg_indices_;
    if constexpr (K == -1) {
        cur_seg_indices_.resize(k);
    }
    std::array<seg_t, K == -1 ? 0 : K> cur_seg_indices;

    int iter = 1;
    for (bool did_update = true; did_update; ++iter) {
        if (verbose > 0 && (iter < 10 || iter % log_freq == 0)) {
            std::cout << "ITERATION " << iter << ": "
                      << cur_cost << std::endl;
        }
        did_update = false;

        if constexpr (K == -1) {

        } else {
            using segs_t = std::array<seg_t, K>;
            detail::loopSegmentsStatic<K, 0>(
                n, cur_seg_indices, 0, [&] (segs_t &segs) -> bool {
                    auto res = this->cut.selectCut(
                        path, segs, cur_cost_change, weights
                    );
                    // add slight amount to negative side when comparing
                    // the change to avoid swaps of the same element
                    if (cur_cost_change < -1e-10) {
                        did_update = true;
                        last_update_i = segs[0].second;
                        last_update_j = segs[1].second;
                        if (res[0].second == -1) {
                            this->cut.applyCut(path, segs, res[0].first);
                        } else {
                            this->cut.applyCut(path, res);
                        }
                                assert (path.size() == static_cast<size_t>(n));
                                assert (
                                    std::unordered_set<vertex_t>(path.begin(), path.end()).size()
                                    == static_cast<size_t>(n)
                                );
                        cur_cost += cur_cost_change;
                                cost_t total_cost = (cost_t)0;
                                for (int idx = 0; idx < n; ++idx) {
                                    const int next_idx = (idx + 1) % n;
                                    total_cost += weights[path[idx]][path[next_idx]];
                                }
                                assert (std::abs(total_cost - cur_cost) < 1e-10);

                        history.addCost(cur_cost);
                        // history.addPath(path, i, j, k, iter);
                    }
                    return false;
                },
                last_update_i + 1,
                last_update_j + 1
            );
        }

        // store history on flush_freq, or on last iter
        if (do_record_history) {
            if (!did_update || history.size() % flush_freq == 0) {
                // history.flush(iter > 1);
                history.flush(true);
            }
        }
    }
    if (verbose > 0) {  // log cost after last iteration
        std::cout << std::fixed << std::setprecision(6)
                  << "Last ITERATION " << iter << ": "
                  << cur_cost << std::defaultfloat
                  << std::endl;
    }
    return cur_cost;
}

}  // namespace k_opt

#endif
