#ifndef TSP_K_OPT_HEURISTIC_FUNKY_HPP
#define TSP_K_OPT_HEURISTIC_FUNKY_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
#include <algorithm>  // std::min
#include "heuristic.hpp"
#include "cut_strategy.hpp"


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

    [[ nodiscard ]] constexpr int getK() const noexcept {
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

template<
    int K,
    int Depth,
    typename callback_t,
    typename segs_t,
    typename... Limits
>
[[ gnu::always_inline ]]
inline void loopSegmentsStatic(
    const int n,
    segs_t &segs_,
    const int start,
    callback_t &&cb,
    const Limits... limits
) {
    auto * const __restrict segs = &segs_[0];
    if constexpr (Depth == K) {
        // check which value to overwrite as
        // segs[0] may have been reversed
        auto &seg = segs[0];
        if (seg.first >= seg.second) seg.first = start;
        else seg.second = start;
        cb(segs_);
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
            if constexpr (Depth != 0) {
                segs[Depth].first = start;
            } else {
                segs[Depth].first = n;
            }
            segs[Depth].second = i;
            loopSegmentsStatic<K, Depth + 1>(
                n, segs_, i + 1,
                std::forward<callback_t>(cb),
                limits...
            );
        }
    }
}

template<
    typename segs_t,
    typename callback_t,
    typename... Limits
>
[[ gnu::always_inline ]]
inline void loopSegmentsDynamic(
    const int k,
    const int n,
    segs_t &segs_,
    callback_t &&cb,
    const Limits... limits_
) {
    constexpr int num_limits = sizeof...(limits_);
    const int limits[num_limits] = { limits_... };

    auto * const __restrict segs = &segs_[0];

    struct State { int prev; int start; };
    std::array<State, 16> stack_buf;
    std::vector<State> stack_vct;
    if (k + 1 > 16) stack_vct.resize(k + 1);
    State * __restrict state
        = (k + 1 > 16) ? stack_vct.data() : stack_buf.data();

    state->prev = n;
    state->start = 0;
    for (int depth = 0; depth >= 0; ) {
        int &start = state->start;
        if (depth == k) {
            auto &seg = segs[0];
            if (seg.first >= seg.second) seg.first = start;
            else seg.second = start;
            cb(segs_);
            --depth;
            --state;
            continue;
        }
        const int lim = (depth < num_limits)
                      ? limits[depth]
                      : n - k + depth;
        if (start < lim) {
            segs[depth].first = state->prev;
            segs[depth].second = start++;
            (++state)->prev = start;
            state->start = start;
            ++depth;
            continue;
        }
        --state;
        --depth;
    }
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
    const int k = this->getK();
    if (n < k) return cur_cost;
    const int log_freq = 10;  // log best cost every 1 iters
    const int flush_freq = 10000;  // flush every 10000 costs
    const bool do_record_history = !history.isStopped();
    history.addCost(cur_cost);
    history.addPath(path, -1, -1, -1, 0);
    cost_t cur_cost_change = (cost_t) 0;

    int last_update_i = n - k - 1;
    int last_update_j = last_update_i + 1;
    std::vector<seg_t> seg_indices_vct;
    if constexpr (K == -1) seg_indices_vct.resize(k);
    std::array<seg_t, K == -1 ? 0 : K> seg_indices_arr;
    std::vector<vertex_t> buffer(n);

    int iter = 1;
    for (bool did_update = true; did_update; ++iter) {
        if (verbose > 0 && (iter < 10 || iter % log_freq == 0)) {
            std::cout << "ITERATION " << iter << ": "
                      << cur_cost << std::endl;
        }
        did_update = false;

        const auto run = [&] (auto &segs) {
            int perm_idx = -1;
            auto res = this->cut.selectCut(
                path, segs, cur_cost_change, weights, perm_idx
            );
            // add slight amount to negative side when comparing
            // the change to avoid swaps of the same element
            if (cur_cost_change < -1e-10) {
                did_update = true;
                last_update_i = segs[0].second;
                last_update_j = std::min(segs[1].first, segs[1].second);
                if (perm_idx == -1) {
                    this->cut.applyCut(path, res, -1, buffer);
                } else {
                    this->cut.applyCut(
                        path, segs, perm_idx, buffer);
                }
                cur_cost += cur_cost_change;
                history.addCost(cur_cost);
                // history.addPath(path, i, j, k, iter);
            }
        };

        if constexpr (K == -1) {
            detail::loopSegmentsDynamic(
                k, n,
                seg_indices_vct, run,
                last_update_i + 1,
                last_update_j + 1
            );
        } else {
            detail::loopSegmentsStatic<K, 0>(
                n, seg_indices_arr, 0, run,
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
