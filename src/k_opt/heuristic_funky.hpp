#ifndef TSP_K_OPT_HEURISTIC_FUNKY_HPP
#define TSP_K_OPT_HEURISTIC_FUNKY_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
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

    explicit KOptFunky(cut_strategy_t cut_strategy, const int k = -1)
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
        const cost_t * __restrict const flat_weights,
        const int verbose = 0
    ) const noexcept override;

 private:

    int k;

};


namespace detail {

template<int K, int Depth, typename callback_t>
[[ gnu::hot ]]
inline bool loopSegmentsStatic(
    const int start,
    const int n,
    std::pair<int, int> * __restrict const segs,
    const int * __restrict const limits,
    int * __restrict const next_limits,
    callback_t &&cb
) noexcept {
    if constexpr (Depth != 0) {
        segs[Depth].first = start;
    }
    const int max_lim = n - K + Depth;
    int next_lim = -1;
    for (int i = start, lim = limits[Depth]; i < lim; ++i) {
        segs[Depth].second = i;
        if constexpr (Depth == K - 1) {
            segs[0].first = i + 1;
            if (cb()) [[ unlikely ]] {
                next_lim = i + 1;
                lim = max_lim;
            }
        } else {
            if (loopSegmentsStatic<K, Depth + 1>(
                i + 1, n, segs,
                limits, next_limits,
                std::forward<callback_t>(cb)
            )) [[ unlikely ]] {
                next_lim = i + 1;
                lim = max_lim;
            }
        }
    }
    const bool did_update = next_lim >= 0;
    next_limits[Depth] = did_update
                       ? next_lim : max_lim;
    return did_update;
}

template<typename callback_t>
[[ gnu::hot ]]
inline bool loopSegmentsDynamic(
    const int start,
    const int depth,
    const int k,
    const int n,
    std::pair<int, int> * __restrict const segs,
    const int * __restrict const limits,
    int * __restrict const next_limits,
    callback_t &&cb
) noexcept {
    segs[depth].first = start;
    const int max_lim = n - k + depth;
    int next_lim = -1;
    for (int i = start, lim = limits[depth]; i < lim; ++i) {
        segs[depth].second = i;
        if (depth == k - 1) [[ likely ]] {
            segs[0].first = i + 1;
            if (cb()) [[ unlikely ]] {
                next_lim = i + 1;
                lim = max_lim;
            }
        } else if (loopSegmentsDynamic(
            i + 1, depth + 1, k, n, segs,
            limits, next_limits,
            std::forward<callback_t>(cb)
        )) [[ unlikely ]] {
            next_lim = i + 1;
            lim = max_lim;
        }
    }
    const bool did_update = next_lim >= 0;
    next_limits[depth] = did_update
                       ? next_lim : max_lim;
    return did_update;
}

}  // namespace detail


template<typename cost_t, typename cut_strategy_t, int K, typename vertex_t>
requires CutStrategy<cut_strategy_t, cost_t, vertex_t, K>
cost_t KOptFunky<cost_t, cut_strategy_t, K, vertex_t>::run(
    std::vector<vertex_t> &path_,
    cost_t cur_cost,
    History<cost_t> &history,
    const cost_t * __restrict const weights,
    const int verbose
) const noexcept {
    using seg_t = std::pair<int, int>;
    const int n = path_.size();
    const int k = this->getK();
    if (n < k) return cur_cost;
    const int log_freq = 10;  // log best cost every 1 iters
    const int flush_freq = 10000;  // flush every 10000 costs
    const bool do_record_history = !history.isStopped();
    history.addCost(cur_cost);
    // history.addPath(path_, -1, -1, -1, 0);
    cost_t cur_cost_change = (cost_t) 0;

    std::vector<vertex_t> path_buf_(n);
    vertex_t * __restrict path_buf = path_buf_.data();
    vertex_t * __restrict path = path_.data();

    std::array<seg_t, K == -1 ? 16 : K> seg_indices_arr;
    std::array<seg_t, K == -1 ? 16 : K> seg_indices_buf_arr;
    seg_t * __restrict segs_indices;
    seg_t * __restrict segs_indices_buf;

    std::array<int, K == -1 ? 16 : K> limits_arr;
    std::array<int, K == -1 ? 16 : K> next_limits_arr;
    int * __restrict limits;
    int * __restrict next_limits;
    bool use_next_limits = false;
    if constexpr (K == -1) {
        segs_indices = k <= 16 ? seg_indices_arr.data()
                               : new seg_t[k];
        segs_indices_buf = k <= 16 ? seg_indices_buf_arr.data()
                                   : new seg_t[k];
        limits = k <= 16 ? limits_arr.data()
                         : new int[k];
        next_limits = k <= 16 ? next_limits_arr.data()
                              : new int[k];
    } else {
        segs_indices = seg_indices_arr.data();
        segs_indices_buf = seg_indices_buf_arr.data();
        limits = limits_arr.data();
        next_limits = next_limits_arr.data();
    }
    for (int depth = 0; depth < k; ++depth) {
        limits[depth] = n - k + depth;
    }

    const cut_strategy_t * __restrict const cut = &this->cut;
    int iter = 1;
    for (bool did_update = true; did_update; ++iter) {
        if (verbose > 0 && (iter < 10 || iter % log_freq == 0)) {
            std::cout << "ITERATION " << iter << ": "
                      << cur_cost << std::endl;
        }
        did_update = false;

        const auto run = [&] () [[ gnu::hot ]] {
            int perm_idx = -1;
            const int swap_mask = cut->template selectCut<false>(
                n, path,
                segs_indices, cur_cost_change, weights,
                perm_idx, segs_indices_buf
            );
            // add slight amount to negative side when comparing
            // the change to avoid swaps of the same element
            if (cur_cost_change < -1e-10) [[ unlikely ]] {
                const bool is_new_path_in_buf = cut->applyCut(
                    path, path_buf,
                    perm_idx >= 0 ? segs_indices : segs_indices_buf,
                    perm_idx, swap_mask, n
                );
                // swapping __restrict pointers, empirically no errors
                if (is_new_path_in_buf) std::swap(path, path_buf);
                cur_cost += cur_cost_change;
                history.addCost(cur_cost);
                // history.addPath(path, i, j, k, iter);
                return did_update = true;
            }
            return false;
        };

        if constexpr (K == -1) {
            detail::loopSegmentsDynamic(
                0, 0, k, n, segs_indices,
                use_next_limits ? next_limits : limits,
                use_next_limits ? limits : next_limits,
                run
            );
        } else {
            detail::loopSegmentsStatic<K, 0>(
                0, n, segs_indices,
                use_next_limits ? next_limits : limits,
                use_next_limits ? limits : next_limits,
                run
            );
        }
        use_next_limits = !use_next_limits;

        // store history on flush_freq, or on last iter
        if (do_record_history) {
            if (!did_update || history.size() % flush_freq == 0)
                [[ unlikely ]]
            {
                // history.flush(iter > 1);
                history.flush(true);
            }
        }
    }
    if (path == path_buf_.data()) {
        std::swap(path_, path_buf_);
    }
    if constexpr (K == -1) {
        if (segs_indices != seg_indices_arr.data()) {
            delete[] segs_indices;
        }
        if (segs_indices_buf != seg_indices_buf_arr.data()) {
            delete[] segs_indices_buf;
        }
        if (limits != limits_arr.data()) {
            delete[] limits;
        }
        if (next_limits != next_limits_arr.data()) {
            delete[] next_limits;
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
