#ifndef TSP_K_OPT_HEURISTIC_RAND_HPP
#define TSP_K_OPT_HEURISTIC_RAND_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <utility>
#include <stack>
#include <random>
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
        std::vector<vertex_t> &solution,
        cost_t cur_cost,
        History<cost_t> &history,
        const cost_t * __restrict const flat_weights,
        const int verbose = 0
    ) const noexcept override;

 private:

    mutable std::mt19937 psrng;
    int k;
    mutable std::stack<int> rand_indices;

    [[ gnu::hot ]]
    inline void genRandomSegments(
        const int n,
        const int k,
        std::pair<int, int> * __restrict const segs,
        const int batch_size = 100
    ) const noexcept;

};


namespace detail {

template<int K = -1>
[[ gnu::hot ]]
inline std::stack<int> genRandomIndices(
    const int n,
    const int k,
    const int batch_size,
    std::mt19937 &psrng
) {
    using distr_t = std::uniform_int_distribution<
        std::mt19937::result_type
    >;
    std::stack<int> generated;
    for (int batch = batch_size; batch > 0; --batch) {
        int rnd = n;
        if constexpr (K == -1) {
            for (int depth = 0; depth < k; ++depth) {
                distr_t rand_idx(k - 1 - depth, rnd - 1);
                rnd = rand_idx(psrng);
                generated.push(rnd);
            }
        } else {
            const auto set_rand_seg = [&] (const int depth) {
                distr_t rand_idx(K - 1 - depth, rnd - 1);
                rnd = static_cast<int>(rand_idx(psrng));
                generated.push(rnd);
            };
            [&] <std::size_t... I> (std::index_sequence<I...>) {
                (set_rand_seg(I), ...);
            } (std::make_index_sequence<K>{});
        }
    }
    return generated;
}

}  // namespace detail


template<typename cost_t, typename cut_strategy_t, int K, typename vertex_t>
requires CutStrategy<cut_strategy_t, cost_t, vertex_t, K>
void KOptRand<cost_t, cut_strategy_t, K, vertex_t>::genRandomSegments(
    const int n,
    const int k,
    std::pair<int, int> * __restrict const segs,
    const int batch_size
) const noexcept {
    if (this->rand_indices.empty()) [[ unlikely ]] {
        this->rand_indices = detail::genRandomIndices<K>(
            n, k, batch_size, this->psrng
        );
    }

    const auto set_rand_seg = [&] (const int idx) {
        int rnd = this->rand_indices.top();
        segs[idx].second = rnd++;
        if constexpr (K == -1) {
            if (idx == k - 1) {
                segs[0].first = rnd == n ? 0 : rnd;
            } else segs[idx + 1].first = rnd;
        } else {
            if (idx == K - 1) {
                segs[0].first = rnd == n ? 0 : rnd;
            } else segs[idx + 1].first = rnd;
        }
        this->rand_indices.pop();
    };

    if constexpr (K == -1) {
        for (int i = 0; i < k; ++i) set_rand_seg(i);
    } else {
        [&] <std::size_t... I> (std::index_sequence<I...>) {
            (set_rand_seg(I), ...);
        } (std::make_index_sequence<K>{});
    }
}


template<typename cost_t, typename cut_strategy_t, int K, typename vertex_t>
requires CutStrategy<cut_strategy_t, cost_t, vertex_t, K>
cost_t KOptRand<cost_t, cut_strategy_t, K, vertex_t>::run(
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

    std::vector<vertex_t> path_buf_(n);
    vertex_t * __restrict path_buf = path_buf_.data();
    vertex_t * __restrict path = path_.data();

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
    for (bool did_update = true, no_collision = true; did_update; ++iter) {
        if (verbose > 0 && (iter < 10 || iter % log_freq == 0)) {
            std::cout << "ITERATION " << iter << ": "
                      << cur_cost << std::endl;
        }
        did_update = false;

        const auto process_cut = [&] () [[ gnu::hot ]] {
            int perm_idx = -1;
            segs_indices_buf[0].first = -1;
            const int swap_mask = no_collision
                ? cut->template selectCut<true>(
                    n, path, segs_indices, cur_cost_change, weights,
                    perm_idx, segs_indices_buf
                ) : cut->template selectCut<false>(
                    n, path, segs_indices, cur_cost_change, weights,
                    perm_idx, segs_indices_buf
                );
            // add slight amount to negative side when comparing
            // the change to avoid swaps of the same element
            if (cur_cost_change < -1e-10) [[ unlikely ]] {
                const bool is_new_path_in_buf = cut->applyCut(
                    path, path_buf,
                    segs_indices_buf[0].first >= 0
                        ? segs_indices_buf : segs_indices,
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

        if (no_collision) [[ likely ]] {
            for (int tries_left = n * n * n / 6;
                tries_left >= 0;
                --tries_left
            ) {
                // batch size = n
                this->genRandomSegments(n, k, segs_indices, n);
                if (process_cut()) [[ unlikely ]] break;
            }
        }

        if (!did_update) [[ unlikely ]] {
            // switch to classical to finish it
            no_collision = false;
            if constexpr (K == -1) {
                detail::loopSegmentsDynamic(
                    0, 0, k, n, segs_indices, process_cut
                );
            } else {
                detail::loopSegmentsStatic<K, 0>(
                    0, n, segs_indices, process_cut
                );
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
    }
    this->rand_indices = {};

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
