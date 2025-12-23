#ifndef TSP_K_OPT_HEURISTIC_RAND_HPP
#define TSP_K_OPT_HEURISTIC_RAND_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
#include <stack>
#include <random>
#include "heuristic.hpp"
#include "heuristic_funky.hpp"
#include "cut_strategy.hpp"

namespace k_opt {

template<
    typename cost_t,
    typename cut_strategy_t,
    IntrusiveVertex vertex_t,
    int K=-1
>
requires CutStrategy<cut_strategy_t, cost_t, vertex_t, K>
class KOptRand : public Heuristic<cost_t, vertex_t>
{
    static_assert(K == -1 || K >= 2, "K must be -1 (dynamic) or >= 2 (static)");

 public:

    explicit KOptRand(
        cut_strategy_t cut_strategy,
        std::mt19937 &psrng,
        const int k = -1
    ) : cut(std::move(cut_strategy)), psrng(psrng), k(k) { }

    ~KOptRand() = default;

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
        typename vertex_t::traits::node_ptr solution,
        cost_t cur_cost,
        History<cost_t> &history,
        const cost_t * __restrict const flat_weights,
        const int n,
        const int verbose = 0
    ) const noexcept override;

 private:

    mutable std::mt19937 psrng;
    int k;

    [[ gnu::hot ]]
    inline void genRandomSegments(
        const int n,
        const int k,
        std::pair<
            typename vertex_t::traits::node_ptr,
            typename vertex_t::traits::node_ptr
        > * __restrict const segs,
        typename vertex_t::traits::node_ptr * __restrict const nodes_map
    ) const noexcept;

};

template<typename cost_t, typename cut_strategy_t,
         IntrusiveVertex vertex_t, int K>
requires CutStrategy<cut_strategy_t, cost_t, vertex_t, K>
void KOptRand<cost_t, cut_strategy_t, vertex_t, K>::genRandomSegments(
    const int n,
    const int k,
    std::pair<
        typename vertex_t::traits::node_ptr,
        typename vertex_t::traits::node_ptr
    > * __restrict const segs,
    typename vertex_t::traits::node_ptr * __restrict const nodes_map
) const noexcept {
    using distr_t = std::uniform_int_distribution<
        std::mt19937::result_type
    >;
    int rnd = 0;
    const int lim = n - k - 1;
    const auto set_rand_seg = [&] (const int idx) {
        distr_t rand_idx(rnd, lim + idx);
        rnd = static_cast<int>(rand_idx(this->psrng));
        segs[idx].second = nodes_map[rnd++];
        if constexpr (K == -1) {
            if (idx == k - 1) {
                segs[0].first = nodes_map[rnd];
            } else [[ likely ]] {
                segs[idx + 1].first = nodes_map[rnd];
            }
        } else {
            if (idx == K - 1) {
                segs[0].first = nodes_map[rnd];
            } else [[ likely ]] {
                segs[idx + 1].first = nodes_map[rnd];
            }
        }
    };

    if constexpr (K == -1) {
        for (int i = 0; i < k; ++i) set_rand_seg(i);
    } else {
        [&] <std::size_t... I> (std::index_sequence<I...>) {
            (set_rand_seg(I), ...);
        } (std::make_index_sequence<K>{});
    }
}

template<typename cost_t, typename cut_strategy_t,
         IntrusiveVertex vertex_t, int K>
requires CutStrategy<cut_strategy_t, cost_t, vertex_t, K>
cost_t KOptRand<cost_t, cut_strategy_t, vertex_t, K>::run(
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
    seg_t * __restrict segs_indices;
    seg_t * __restrict segs_indices_buf;

    // for funky fallback when rand collisions start:
    std::array<int, K == -1 ? 16 : K> limits_arr;
    std::array<int, K == -1 ? 16 : K> next_limits_arr;
    int * __restrict limits;
    int * __restrict next_limits;
    bool use_next_limits = false;
    {
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
    }

    std::vector<seg_ptr> nodes_map_(n);
    seg_ptr * __restrict const nodes_map = nodes_map_.data();
    {
        seg_ptr cur = path;
        for (int i = 0; i < n; ++i) {
            nodes_map[i] = cur;
            cur = vertex_t::traits::get_next(cur);
        }
    }

    const int max_checks = n > k ? n * n * n / 6 : 0;
    const cut_strategy_t * __restrict const cut = &this->cut;
    int iter = 1;
    cost_t cur_cost_change = (cost_t) 0;
    for (bool did_update = true, no_collision = true; did_update; ++iter) {
        if (verbose > 0 && (iter < 10 || iter % log_freq == 0)) {
            std::cout << "ITERATION " << iter << ": "
                      << cur_cost << std::endl;
        }
        did_update = false;

        const auto process_cut = [&] () [[ gnu::hot ]] {
            int perm_idx = -1;
            segs_indices_buf[0].first = nullptr;
            const int swap_mask = no_collision
                ? cut->template selectCut<true>(
                    n, segs_indices, cur_cost_change, weights,
                    perm_idx, segs_indices_buf
                ) : cut->template selectCut<false>(
                    n, segs_indices, cur_cost_change, weights,
                    perm_idx, segs_indices_buf
                );
            // add slight amount to negative side when comparing
            // the change to avoid swaps of the same element
            if (cur_cost_change < -1e-10) [[ unlikely ]] {
                cut->applyCut(
                    segs_indices_buf[0].first != nullptr
                        ? segs_indices_buf : segs_indices,
                    perm_idx, swap_mask, n
                );
                cur_cost += cur_cost_change;
                history.addCost(cur_cost);
                // history.addPath(path, i, j, k, iter);
                return did_update = true;
            }
            return false;
        };

        if (no_collision) [[ likely ]] {
            for (int tries_left = max_checks;
                 tries_left > 0;
                 --tries_left
            ) {
                this->genRandomSegments(n, k, segs_indices, nodes_map);
                if (process_cut()) [[ unlikely ]] break;
            }
        }

        if (!did_update) [[ unlikely ]] {  // funky fallback
            no_collision = false;
            if constexpr (K == -1) {
                detail::loopSegmentsDynamic<vertex_t>(
                    vertex_t::traits::get_previous(path),
                    path,
                    0, 0, k, n, segs_indices,
                    use_next_limits ? next_limits : limits,
                    use_next_limits ? limits : next_limits,
                    process_cut
                );
            } else {
                detail::loopSegmentsStatic<K, 0, vertex_t>(
                    vertex_t::traits::get_previous(path),
                    path,
                    0, n, segs_indices,
                    use_next_limits ? next_limits : limits,
                    use_next_limits ? limits : next_limits,
                    process_cut
                );
            }
            use_next_limits = !use_next_limits;
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
