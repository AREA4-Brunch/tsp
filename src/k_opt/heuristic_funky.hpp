#ifndef TSP_K_OPT_HEURISTIC_FUNKY_HPP
#define TSP_K_OPT_HEURISTIC_FUNKY_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
#include <algorithm>  // std::min
#include "heuristic.hpp"
#include "cut_strategy.hpp"

#include <cassert>  // for debugging
#include <unordered_set>  // for debugging


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
        const std::vector<std::vector<cost_t>> &weights,
        const int verbose = 0
    ) const noexcept override;

 private:

    int k;

};


namespace detail {

template<int K, int Depth, typename callback_t>
[[ gnu::always_inline ]]
inline void loopSegmentsStatic(
    const int n,
    std::pair<int, int> * __restrict const segs,
    const int start,
    callback_t &&cb,
    const int * __restrict const limits
) noexcept {
    if constexpr (Depth == K) {
        // check which value to overwrite as
        // segs[0] may have been reversed
        auto &seg = segs[0];
        if (seg.first >= seg.second) seg.first = start;
        else seg.second = start;
        cb();
    } else {
        const int lim = limits[Depth];
        for (int i = start; i < lim; ++i) {
            // reset start always as cb may modify
            if constexpr (Depth != 0) {
                segs[Depth].first = start;
            } else {
                segs[Depth].first = n;
            }
            segs[Depth].second = i;
            loopSegmentsStatic<K, Depth + 1>(
                n, segs, i + 1,
                std::forward<callback_t>(cb),
                limits
            );
        }
    }
}

template<typename callback_t>
[[ gnu::always_inline ]]
inline void loopSegmentsDynamic(
    const int k,
    const int n,
    std::pair<int, int> * __restrict const segs,
    callback_t &&cb,
    const int * __restrict const limits
) noexcept {
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
            cb();
            --depth;
            --state;
            continue;
        }
        if (start >= limits[depth]) [[ unlikely ]] {
            --state;
            --depth;
            continue;
        }
        auto &seg = segs[depth];
        seg.first = state->prev;
        seg.second = start++;
        (++state)->prev = start;
        state->start = start;
        ++depth;
    }
}

}  // namespace detail


template<typename cost_t, typename cut_strategy_t, int K, typename vertex_t>
requires CutStrategy<cut_strategy_t, cost_t, vertex_t, K>
cost_t KOptFunky<cost_t, cut_strategy_t, K, vertex_t>::run(
    std::vector<vertex_t> &path_,
    cost_t cur_cost,
    History<cost_t> &history,
    const std::vector<std::vector<cost_t>> &weights_,
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
    history.addPath(path_, -1, -1, -1, 0);
    cost_t cur_cost_change = (cost_t) 0;

    std::vector<vertex_t> path_buf_(n);
    vertex_t * path_buf = path_buf_.data();
    vertex_t * path = path_.data();
    const auto * __restrict const weights = weights_.data();

    std::array<seg_t, K == -1 ? 16 : K> seg_indices_arr;
    std::array<seg_t, K == -1 ? 16 : K> segs_buf_arr;
    seg_t * __restrict segs_indices = nullptr;
    seg_t * __restrict segs_buf = nullptr;

    std::array<int, K == -1 ? 16 : K> limits_arr;
    std::array<int, K == -1 ? 16 : K> next_limits_arr;
    int * __restrict limits = nullptr;
    int * __restrict next_limits = nullptr;
    bool use_next_limits = false;
    if constexpr (K == -1) {
        segs_indices = k <= 16 ? seg_indices_arr.data()
                               : new seg_t[k];
        segs_buf = k <= 16 ? segs_buf_arr.data()
                           : new seg_t[k];
        limits = k <= 16 ? limits_arr.data()
                         : new int[k];
        next_limits = k <= 16 ? next_limits_arr.data()
                              : new int[k];
    } else {
        segs_indices = seg_indices_arr.data();
        segs_buf = segs_buf_arr.data();
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
        // std::cerr << "here i am: " << perm_idx << std::endl;
        // std::cerr << "perm_idx: " << perm_idx << std::endl;
        // std::cerr << "Segs orgiinal\n";
        // for (int i = 0; i < k; ++i) {
        //     std::cerr << "(" << segs_indices[i].first << ", " << segs_indices[i].second << ")\n";
        // }
            const bool is_sol_in_segs = cut->selectCut(
                path,
                segs_indices, cur_cost_change, weights,
                perm_idx, segs_buf
            );
        // std::cerr << std::endl;
        // std::cerr << "cur_cost: " << cur_cost << std::endl;
        // std::cerr << "cur_cost_change: " << cur_cost_change << std::endl;

            // add slight amount to negative side when comparing
            // the change to avoid swaps of the same element
            if (cur_cost_change < -1e-10) {
                did_update = true;
        // std::cerr << "Selected segs, perm idx:" << perm_idx << "\n";
        // for (int i = 0; i < k; ++i) {
        //     if (is_sol_in_segs) {
        //         std::cerr << "(" << segs_indices[i].first << ", " << segs_indices[i].second << ")\n";
        //     } else {
        //         std::cerr << "(" << segs_buf[i].first << ", " << segs_buf[i].second << ")\n";
        //     }
        // }
        // std::cerr  << "Orig path:\n";
        // for (int i = 0; i < n; ++i) {
        //     std::cerr << path[i] << " ";
        // }
        // std::cerr << std::endl;
                const bool is_new_path_in_buf = cut->applyCut(
                    path,
                    is_sol_in_segs ? segs_indices : segs_buf,
                    perm_idx, path_buf, n
                );
                if (is_new_path_in_buf) std::swap(path, path_buf);

        // std::cerr  << "New path:\n";
        // for (int i = 0; i < n; ++i) {
        //     std::cerr << path[i] << " ";
        // }
        // std::cerr << std::endl;
        // std::cerr << "new cost: " << cur_cost << std::endl;

// Check if path and path_buf alias (they shouldn't, as they point to different vectors)
// if ((path >= path_buf && path < path_buf + n) ||
//     (path_buf >= path && path_buf < path + n)) {
//     std::cerr << "Aliasing detected between path and path_buf at iter " << iter << std::endl;
//     assert(false);
// }
// Check segs_indices and segs_buf
// for (int i = 0; i < k; ++i) {
//     if ((segs_indices + i >= segs_buf && segs_indices + i < segs_buf + k) ||
//         (segs_buf + i >= segs_indices && segs_buf + i < segs_indices + k)) {
//         std::cerr << "Aliasing detected in segs at iter " << iter << ", i=" << i << std::endl;
//         assert(false);
//     }
// }
                                // assert (path_.size() == static_cast<size_t>(n));

                                auto &path_tmp = path;
                                assert (
                                    std::unordered_set<vertex_t>(path_tmp, path_tmp + n).size()
                                    == static_cast<size_t>(n)
                                );
                cur_cost += cur_cost_change;
                                cost_t total_cost = (cost_t)0;
                                for (int idx = 0; idx < n; ++idx) {
                                    const int next_idx = (idx + 1) % n;
                                    total_cost += weights[path_tmp[idx]][path_tmp[next_idx]];
                                }
                                if (std::abs(total_cost - cur_cost) >= 1e-10) {
                                    std::cerr << "Error: cost mismatch after k-opt application!\n";
                                    std::cerr << "Computed cost: " << cur_cost << "\n";
                                    std::cerr << "Actual cost:   " << total_cost << "\n";
                                    std::cerr << "Difference:    " << std::abs(total_cost - cur_cost) << "\n";
                                    // std::cerr << "Sol Segs:\n";
                                    // for (int i = 0; i < k; ++i) {
                                    //     if (is_sol_in_segs) {
                                    //         std::cerr << "(" << segs_indices[i].first << ", " << segs_indices[i].second << ")\n";
                                    //     } else {
                                    //         std::cerr << "(" << segs_buf[i].first << ", " << segs_buf[i].second << ")\n";
                                    //     }
                                    // }
                                    // std::cerr << "Path: \n";
                                    // for (int i = 0; i < n; ++i) {
                                    //     std::cerr << path_tmp[i] << " ";
                                    // }
                                }
                                assert (std::abs(total_cost - cur_cost) < 1e-10);

                for (int i = 1; i < k; ++i) {
                    const auto &s = segs_indices[i];
                    next_limits[i - 1] = std::min(s.first, s.second);
                }
                next_limits[k - 1] = std::max(
                    segs_indices[0].first,
                    segs_indices[0].second
                );
                history.addCost(cur_cost);
                // history.addPath(path, i, j, k, iter);
            }
        };

        if constexpr (K == -1) {
            detail::loopSegmentsDynamic(
                k, n, segs_indices, run,
                use_next_limits ? next_limits : limits
            );
        } else {
            detail::loopSegmentsStatic<K, 0>(
                n, segs_indices, 0, run,
                use_next_limits ? next_limits : limits
            );
        }

        use_next_limits = !use_next_limits;

        // store history on flush_freq, or on last iter
        if (do_record_history) {
            if (!did_update || history.size() % flush_freq == 0) {
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
        if (segs_buf != segs_buf_arr.data()) {
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
