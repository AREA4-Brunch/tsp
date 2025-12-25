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
    inline bool genRandomSegments(
        int max_idx,
        const int k,
        std::pair<
            typename vertex_t::traits::node_ptr,
            typename vertex_t::traits::node_ptr
        > * __restrict const segs,
        typename vertex_t::traits::node_ptr * __restrict const nodes_by_idx,
        std::vector<bool> &forbidden
    ) const noexcept;

};

template<typename cost_t, typename cut_strategy_t,
         IntrusiveVertex vertex_t, int K>
requires CutStrategy<cut_strategy_t, cost_t, vertex_t, K>
bool KOptRand<cost_t, cut_strategy_t, vertex_t, K>::genRandomSegments(
    int max_idx,
    const int k,
    std::pair<
        typename vertex_t::traits::node_ptr,
        typename vertex_t::traits::node_ptr
    > * __restrict const segs,
    typename vertex_t::traits::node_ptr * __restrict const nodes_by_idx,
    std::vector<bool> &forbidden
) const noexcept {
    using distr_t = std::uniform_int_distribution<
        std::mt19937::result_type
    >;
    max_idx = max_idx - 1 - k;
    int rnd_idx = 0;
    const auto set_rand_seg = [&] (const int idx) -> bool {
        distr_t rand_idx(rnd_idx, max_idx + idx);
        rnd_idx = static_cast<int>(rand_idx(this->psrng));
        segs[idx].second = nodes_by_idx[rnd_idx++];
        auto id = vertex_t::v(segs[idx].second)->id;
        if (forbidden[id]) [[ unlikely ]] {
            rnd_idx = static_cast<int>(rand_idx(this->psrng));
            segs[idx].second = nodes_by_idx[rnd_idx++];
            id = vertex_t::v(segs[idx].second)->id;
            if (forbidden[id]) [[ unlikely ]] {
                return true;
            }
        }
        auto next = nodes_by_idx[rnd_idx];
        auto next_id = vertex_t::v(next)->id;
        if (forbidden[next_id]) [[ unlikely ]] {
            return true;
        }
        forbidden[id] = true;
        forbidden[next_id] = true;

        if constexpr (K == -1) {
            if (idx == k - 1) [[ unlikely ]] {
                segs[0].first = next;
            } else [[ likely ]] {
                segs[idx + 1].first = next;
            }
        } else {
            if (idx == K - 1) [[ unlikely ]] {
                segs[0].first = next;
            } else [[ likely ]] {
                segs[idx + 1].first = next;
            }
        }
        return false;
    };

    const auto clear_forbidden = [&] (int seg_idx) {
        for ( ; seg_idx >= 0; --seg_idx) {
            auto id = vertex_t::v(segs[seg_idx].first)->id;
            forbidden[id] = false;
            id = vertex_t::v(segs[seg_idx].second)->id;
            forbidden[id] = false;
        }
    };

    for (int i = 0; i < k; ++i) {
        if (set_rand_seg(i)) [[ unlikely ]] {
            clear_forbidden(i - 1);
            return false;
        }
    }

    return true;
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
    seg_t * __restrict segs;
    seg_t * __restrict segs_buf;
    {
        if constexpr (K == -1) {
            segs = k <= 16 ? seg_indices_arr.data()
                                   : new seg_t[k];
            segs_buf = k <= 16 ? seg_indices_buf_arr.data()
                                       : new seg_t[k];
        } else {
            segs = seg_indices_arr.data();
            segs_buf = seg_indices_buf_arr.data();
        }
        segs[0].first = path;
    }

    std::vector<seg_ptr> nodes_by_idx_(n);
    seg_ptr * __restrict const nodes_by_idx = nodes_by_idx_.data();

    std::vector<bool> forbidden(n, false);
    const int forbidden_clear_freq = n / k / 10;  // 10% of distinct k tuples
    int forbidden_count = forbidden_clear_freq;

    const int max_checks = n > k ? n * n * n / 6 : 0;
    const cut_strategy_t * __restrict const cut = &this->cut;
    int iter = 1;
    cost_t cur_cost_change = (cost_t) 0;
    for (bool did_update = true, no_collision = n > 30; did_update; ++iter) {
        if (verbose > 0 && (iter < 10 || iter % log_freq == 0)) {
            std::cout << "ITERATION " << iter << ": "
                      << cur_cost << std::endl;
        }
        if (--forbidden_count == 0) {
            forbidden = std::vector<bool>(n, false);
            forbidden_count = forbidden_clear_freq;
        }

        did_update = false;

        const auto process_cut = [&] () [[ gnu::hot ]] {
            int perm_idx = -1;
            segs_buf[0].first = nullptr;
            const int swap_mask = no_collision
                ? cut->template selectCut<true>(
                    n, segs, cur_cost_change, weights,
                    perm_idx, segs_buf
                ) : cut->template selectCut<false>(
                    n, segs, cur_cost_change, weights,
                    perm_idx, segs_buf
                );
            // add slight amount to negative side when comparing
            // the change to avoid swaps of the same element
            if (cur_cost_change < -1e-10) [[ unlikely ]] {
                cut->applyCut(
                    segs_buf[0].first != nullptr
                        ? segs_buf : segs,
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
            seg_ptr cur = segs[0].first;
            seg_ptr prev = vertex_t::traits::get_previous(cur);
            for (int i = 0; i < n / 2; ++i) {
                nodes_by_idx[i] = cur;
                auto next = path_algos::get_neighbour<vertex_t>(cur, prev);
                prev = cur;
                cur = next;
            }
            for (int i = n / 2; i < n; ++i) {
                nodes_by_idx[i] = cur;
                auto next = path_algos::get_neighbour<vertex_t>(cur, prev);
                prev = cur;
                cur = next;
                const bool did_gen = this->genRandomSegments(
                    i + 1, k, segs, nodes_by_idx, forbidden
                );
                if (did_gen) [[ likely ]] {
                    if (process_cut()) [[ unlikely ]] break;
                }
            }
        }
        if (!did_update) {
            if (no_collision) [[ likely ]] {
                for (int tries_left = max_checks;
                    tries_left > 0;
                    --tries_left
                ) {
                    const bool did_gen = this->genRandomSegments(
                        n, k, segs, nodes_by_idx, forbidden
                    );
                    if (did_gen) [[ likely ]] {
                        if (process_cut()) [[ unlikely ]] break;
                    }
                }
            }

            if (!did_update) [[ unlikely ]] {  // funky fallback
                no_collision = false;
                // start from furthest vertex
                auto start = segs[0].first;
                auto prev = vertex_t::traits::get_previous(start);
                if constexpr (K == -1) {
                    detail::loopSegmentsDynamic<vertex_t>(
                        prev, start,
                        0, 0, k, n, segs, process_cut
                    );
                } else {
                    detail::loopSegmentsStatic<K, 0, vertex_t>(
                        prev, start,
                        0, n, segs, process_cut
                    );
                }
            }
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
        if (segs != seg_indices_arr.data()) {
            delete[] segs;
        }
        if (segs_buf != seg_indices_buf_arr.data()) {
            delete[] segs_buf;
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
