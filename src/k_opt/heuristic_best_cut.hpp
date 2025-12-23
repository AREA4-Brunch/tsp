#ifndef TSP_K_OPT_HEURISTIC_BEST_CUT_HPP
#define TSP_K_OPT_HEURISTIC_BEST_CUT_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
#include "heuristic.hpp"
#include "vertex_concept.hpp"
#include "cut_strategy.hpp"

namespace k_opt {

template<
    typename cost_t,
    typename cut_strategy_t,
    IntrusiveVertex vertex_t,
    int K=-1
>
requires CutStrategy<cut_strategy_t, cost_t, vertex_t, K>
class KOptBestCut : public Heuristic<cost_t, vertex_t>
{
    static_assert(K == -1 || K >= 2, "K must be -1 (dynamic) or >= 2 (static)");

 public:

    explicit KOptBestCut(cut_strategy_t cut_strategy, const int k = -1)
        : cut(std::move(cut_strategy)), k(k) { }

    ~KOptBestCut() = default;

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
        typename vertex_t::traits::node_ptr path,
        cost_t cur_cost,
        History<cost_t> &history,
        const cost_t * __restrict const flat_weights,
        const int n,
        const int verbose = 0
    ) const noexcept override;

 private:

    int k;

};


template<typename cost_t, typename cut_strategy_t,
         IntrusiveVertex vertex_t, int K>
requires CutStrategy<cut_strategy_t, cost_t, vertex_t, K>
cost_t KOptBestCut<cost_t, cut_strategy_t, vertex_t, K>::run(
    typename vertex_t::traits::node_ptr path,
    cost_t cur_cost,
    History<cost_t> &history,
    const cost_t * __restrict const weights,
    const int n,
    const int verbose
) const noexcept {
    using seg_ptr = typename vertex_t::traits::node_ptr;
    using seg_t = std::pair<seg_ptr, seg_ptr>;
    const int k = this->getK();
    if (n < k) return cur_cost;
    const int log_freq = 10;  // log best cost every 1 iters
    const int flush_freq = 10000;  // flush every 10000 costs
    const bool do_record_history = !history.isStopped();
    history.addCost(cur_cost);
    // history.addPath(path_, -1, -1, -1, 0);

    std::array<seg_t, K == -1 ? 16 : K> seg_indices_arr;
    std::array<seg_t, K == -1 ? 16 : K> seg_indices_buf_arr;
    std::array<seg_t, K == -1 ? 16 : K> best_segs_arr;
    seg_t * __restrict segs_indices;
    seg_t * __restrict segs_indices_buf;
    seg_t * __restrict best_segs;
    if constexpr (K == -1) {
        segs_indices = k <= 16 ? seg_indices_arr.data()
                               : new seg_t[k];
        segs_indices_buf = k <= 16 ? seg_indices_buf_arr.data()
                                   : new seg_t[k];
        best_segs = k <= 16 ? best_segs_arr.data()
                            : new seg_t[k];
    } else {
        segs_indices = seg_indices_arr.data();
        segs_indices_buf = seg_indices_buf_arr.data();
        best_segs = best_segs_arr.data();
    }

    const cut_strategy_t * __restrict const cut = &this->cut;
    int iter = 1;
    cost_t cur_cost_change = (cost_t) 0;
    cost_t best_cost = cur_cost;
    int best_swap = -1, best_perm_idx = -1;
    for (bool did_update = true; did_update; ++iter) {
        if (verbose > 0 && (iter < 10 || iter % log_freq == 0)) {
            std::cout << "ITERATION " << iter << ": "
                      << cur_cost << std::endl;
        }
        did_update = false;

        const auto process_cut = [&] () [[ gnu::hot ]] {
            int perm_idx = -1;
            const int swap_mask = cut->template selectCut<false>(
                n, segs_indices, cur_cost_change, weights,
                perm_idx, segs_indices_buf
            );
            // add slight amount to negative side when comparing
            // the change to avoid swaps of the same element
            if (cur_cost + cur_cost_change < best_cost - 1e-10) [[ unlikely ]] {
                best_cost = cur_cost + cur_cost_change;
                best_swap = swap_mask;
                best_perm_idx = perm_idx;
                if (perm_idx < 0) std::swap(best_segs, segs_indices_buf);
                else std::copy_n(segs_indices, k, best_segs);
                did_update = true;
            }
            return false;
        };

        if constexpr (K == -1) {
            detail::loopSegmentsDynamic<vertex_t>(
                vertex_t::traits::get_previous(path),
                path,
                0, 0, k, n, segs_indices, process_cut
            );
        } else {
            detail::loopSegmentsStatic<K, 0, vertex_t>(
                vertex_t::traits::get_previous(path),
                path,
                0, n, segs_indices, process_cut
            );
        }

        if (did_update) {
            cut->applyCut(best_segs, best_perm_idx, best_swap, n);
            cur_cost = best_cost;
            history.addCost(cur_cost);
            // history.addPath(path, i, j, k, iter);
        }

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
    if constexpr (K == -1) {
        if (segs_indices != seg_indices_arr.data()) {
            delete[] segs_indices;
        }
        if (segs_indices_buf != seg_indices_buf_arr.data()) {
            delete[] segs_indices_buf;
        }
        if (best_segs != best_segs_arr.data()) {
            delete[] best_segs;
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
