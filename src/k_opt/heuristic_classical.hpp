#ifndef TSP_K_OPT_HEURISTIC_CLASSICAL_HPP
#define TSP_K_OPT_HEURISTIC_CLASSICAL_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
#include "heuristic.hpp"
#include "cut_strategy.hpp"

#include <unordered_set>


namespace k_opt {

template<
    typename cost_t,
    typename cut_strategy_t,
    IntrusiveVertex vertex_t,
    int K=-1
>
requires CutStrategy<cut_strategy_t, cost_t, vertex_t, K>
class KOptClassical : public Heuristic<cost_t, vertex_t>
{
    static_assert(K == -1 || K >= 2, "K must be -1 (dynamic) or >= 2 (static)");

 public:

    explicit KOptClassical(cut_strategy_t cut_strategy, const int k = -1)
        : cut(std::move(cut_strategy)), k(k) { }

    ~KOptClassical() = default;

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

    int k;

};

template<typename cost_t, typename cut_strategy_t,
         IntrusiveVertex vertex_t, int K>
requires CutStrategy<cut_strategy_t, cost_t, vertex_t, K>
cost_t KOptClassical<cost_t, cut_strategy_t, vertex_t, K>::run(
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
    if constexpr (K == -1) {
        segs_indices = k <= 16 ? seg_indices_arr.data()
                               : new seg_t[k];
        segs_indices_buf = k <= 16 ? seg_indices_buf_arr.data()
                                   : new seg_t[k];
    } else {
        segs_indices = seg_indices_arr.data();
        segs_indices_buf = seg_indices_buf_arr.data();
    }

    const cut_strategy_t * __restrict const cut = &this->cut;
    int iter = 1;
    cost_t cur_cost_change = (cost_t) 0;
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
            if (cur_cost_change < -1e-10) [[ unlikely ]] {
                bool DEBUG = iter == 13;
                DEBUG = false;
                if (DEBUG) {
                    std::cout << "perm idx: " << perm_idx << std::endl;
                    std::cout << "swap_mask: " << swap_mask << std::endl;
                    std::cout << "Orig Segments: " << std::endl;
                    for (int s = 0; s < k; ++s) {
                        std::cout << " Seg " << s << ": ";
                        auto cur = segs_indices[s].first;
                        std::cout << "first"
                            << " v: " << vertex_t::v(cur)->id << " "
                            << " ptr: " << cur
                            << " prev: " << vertex_t::traits::get_previous(cur)
                            << " next: " << vertex_t::traits::get_next(cur)
                            << std::endl;
                        cur = segs_indices[s].second;
                        std::cout << "\t\tsecond"
                            << " v: " << vertex_t::v(cur)->id << " "
                            << " ptr: " << cur
                            << " prev: " << vertex_t::traits::get_previous(cur)
                            << " next: " << vertex_t::traits::get_next(cur)
                            << std::endl;
                    }
                }

                cut->applyCut(
                    perm_idx >= 0 ? segs_indices : segs_indices_buf,
                    perm_idx, swap_mask, n
                );

                auto cur = path, prev = vertex_t::traits::get_previous(path);
                std::unordered_set<int> visited;
                do {
                if (DEBUG) {
                    std::cout << "i: " << iter
                              << " v: " << vertex_t::v(cur)->id << " "
                              << " ptr: " << cur
                              << " actual prev: " << prev
                              << " prev: " << vertex_t::traits::get_previous(cur)
                              << " next: " << vertex_t::traits::get_next(cur)
                              << " mine next: "
                              << k_opt::path_algos::get_neighbour<vertex_t>(cur, prev)
                              << std::endl;
                }
                    if (visited.count(vertex_t::v(cur)->id)) {
                        std::cerr << "Error: Cycle detected in path!" << std::endl;
                        std::cout << "Segments: " << std::endl;
                        for (int s = 0; s < k; ++s) {
                            std::cout << " Seg " << s << ": ";
                            cur = segs_indices[s].first;
                            std::cout << "first"
                              << " v: " << vertex_t::v(cur)->id << " "
                              << " ptr: " << cur
                              << " prev: " << vertex_t::traits::get_previous(cur)
                              << " next: " << vertex_t::traits::get_next(cur)
                              << " mine next: "
                              << k_opt::path_algos::get_neighbour<vertex_t>(cur, prev)
                              << std::endl;
                            cur = segs_indices[s].second;
                            std::cout << "\t\tsecond"
                              << " v: " << vertex_t::v(cur)->id << " "
                              << " ptr: " << cur
                              << " prev: " << vertex_t::traits::get_previous(cur)
                              << " next: " << vertex_t::traits::get_next(cur)
                              << " mine next: "
                              << k_opt::path_algos::get_neighbour<vertex_t>(cur, prev)
                              << std::endl;
                        }
                        throw std::runtime_error("Cycle detected in path");
                    }
                    visited.insert(vertex_t::v(cur)->id);
                    auto tmp = cur;
                    cur = k_opt::path_algos::get_neighbour<vertex_t>(cur, prev);
                    prev = tmp;
                } while (cur != path);

                cur_cost += cur_cost_change;
                history.addCost(cur_cost);
                // history.addPath(path, i, j, k, iter);
                return did_update = true;
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
