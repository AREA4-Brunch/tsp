#ifndef TSP_K_OPT_CUT_HPP
#define TSP_K_OPT_CUT_HPP

#include <vector>
#include <numeric>
#include <functional>
#include <utility>
#include <array>


namespace k_opt {

template<typename cost_t, typename vertex_t, int K = -1>
class CutKOpt {
    static_assert(std::is_arithmetic_v<cost_t>, "cost_t must be arithmetic");
    static_assert(K == -1 || K >= 2, "K must be -1 (dynamic) or >= 2 (static)");

    using seg_t = std::pair<
        typename vertex_t::traits::node_ptr,
        typename vertex_t::traits::node_ptr
    >;

 public:

    static constexpr int NUM_CUTS = K;

    explicit CutKOpt(
        const int k = -1,
        const bool is_not_pure_k_opt=true,
        const bool select_first_better=true,
        const bool do_pre_gen_perms=true
    ) :
        is_not_pure_k_opt(is_not_pure_k_opt),
        select_first_better(select_first_better)
    {
        if constexpr (K == -1) this->setK(k, do_pre_gen_perms);
        else {
            if (do_pre_gen_perms) this->generateSegPermIndices();
        }
    }

    ~CutKOpt() = default;

    template<bool can_modify_segs>
    [[ gnu::hot ]]
    inline int selectCut(
        const int n,
        std::conditional_t<can_modify_segs,
            seg_t * __restrict,
            const seg_t * __restrict
        > const segs,
        cost_t &change,
        const cost_t * __restrict const weights,
        int &perm_idx,
        seg_t * __restrict const best_segs
    ) const noexcept;

    [[ gnu::hot ]]
    inline void applyCut(
        const seg_t * __restrict const segs,
        const int perm_idx,
        const int swap_mask,
        const int n
    ) const noexcept;

    [[ nodiscard ]] constexpr int getK() const noexcept {
        return K == -1 ? this->k : K;
    }

    void setK(const int k, const bool do_pre_gen_perms=true) {
        static_assert(K == -1, "Cannot call setK on static K specialization");
        if (k == this->k) return;
        this->k = k;
        this->rot_stack.resize(k);
        this->perm_buf.resize(k);
        if (do_pre_gen_perms) this->generateSegPermIndices();
    }

 private:

    struct RotDesc {
        int_fast8_t step;
        cost_t cost;
        typename vertex_t::traits::const_node_ptr prev;
    };

    int k = -1;
    bool is_not_pure_k_opt = true;
    bool select_first_better = true;
    std::vector<std::vector<int>> seg_perm_indices;
    // buffer for dynamic K only
    mutable std::vector<RotDesc> rot_stack;
    mutable std::vector<seg_t> perm_buf;

    void generateSegPermIndices();

};


namespace detail {

constexpr unsigned long long factorial(int n) {
    unsigned long long fact = 1ULL;
    for ( ; n > 1; --n) fact *= n;
    return fact;
}

/// @brief Heap's algo
template<typename iter_t, typename callback_t>
[[ gnu::hot ]]
inline bool all_permutations(iter_t begin, const int n, callback_t &&cb) {
    if (cb()) [[ unlikely ]] return true;
    if (n == 2) {
        std::swap(*begin, *(begin + 1));
        return cb();
    }
    std::vector<int> c(n, 0);
    for (int i = 0; i < n; ) {
        if (c[i] < i) {
            if (i & 1) std::swap(*(begin + c[i]), *(begin + i));
            else       std::swap(*(begin), *(begin + i));
            if (cb()) [[ unlikely ]] return true;
            ++c[i];
            i = 0;
        } else {
            c[i] = 0;
            ++i;
        }
    }
    return false;
}

template<typename cost_t, typename vertex_t, int K = -1>
[[ gnu::hot ]]
inline void applyCut(
    const int n,
    vertex_t * __restrict const path,
    auto &&seg_at,
    vertex_t * __restrict new_path,
    const int swap_mask,
    [[ maybe_unused ]] const int k
) noexcept {
    // first segment is always [k, i]
    const auto &seg = seg_at(0);
    const int start = seg.first;
    const int end = seg.second;
    if (start > end) {
        new_path = std::copy(
            path + start,
            path + n,
            new_path
        );
        new_path = std::copy(
            path,
            path + end + 1,
            new_path
        );
    } else if (start < end) {
        new_path = std::reverse_copy(
            path,
            path + start + 1,
            new_path
        );
        new_path = std::reverse_copy(
            path + end,
            path + n,
            new_path
        );
    } else {  // single vertex
        *(new_path++) = path[start];
    }

    const auto insert_seg = [&] (const int i)
        [[ always_inline ]]
    {
        const auto &s = seg_at(i);
        const int start = s.first;
        const int end = s.second;
        if (swap_mask & (1 << i)) {  // reversed
            new_path = std::reverse_copy(
                path + start,
                path + end + 1,
                new_path
            );
        } else {  // forward
            new_path = std::copy(
                path + start, 
                path + end + 1,
                new_path
            );
        }
    };

    // remaining k-1 contiguous ranges
    if constexpr (K == -1) {
        for (int i = 1; i < k; ++i) {
            insert_seg(i);
        }
    } else {
        [&] <std::size_t... I> (std::index_sequence<I...>) {
            (insert_seg(I + 1), ...);
        } (std::make_index_sequence<K - 1>{});
    }
}

}  // namespace detail

template<typename cost_t, typename vertex_t, int K>
void CutKOpt<cost_t, vertex_t, K>::generateSegPermIndices() {
    const int k = this->getK();
    if (k >= 10) return;  // would take too much space
    this->seg_perm_indices.clear();
    this->seg_perm_indices.reserve(detail::factorial(k - 1));
    std::vector<int> indices(k);
    std::iota(indices.begin(), indices.end(), 0);
    detail::all_permutations(indices.begin() + 1, k - 1, [&] () {
        this->seg_perm_indices.emplace_back(indices);
        return false;
    });
}

template<typename cost_t, typename vertex_t, int K>
template<bool can_modify_segs>
int CutKOpt<cost_t, vertex_t, K>::selectCut(
    const int n,
    std::conditional_t<can_modify_segs,
        seg_t * __restrict,
        const seg_t * __restrict
    > const segs,
    cost_t &change,
    const cost_t * __restrict const weights,
    int &perm_idx,
    seg_t * __restrict const best_segs
) const noexcept {
    return -1;
    // const int k = this->getK();
    // const bool choose_first = this->select_first_better;
    // // true at least for 1st change compute
    // bool is_not_pure_k_opt = true;

    // std::array<RotDesc, (K == -1 ? 16 : K)> rot_stack;
    // RotDesc * __restrict rotations;
    // if constexpr (K == -1) {
    //     rotations = k <= 16
    //               ? rot_stack.data()
    //               : this->rot_stack.data();
    // } else {
    //     rotations = rot_stack.data();
    // }
    // rotations->cost = static_cast<cost_t>(0);
    // rotations->prev = segs[0].second;

    // constexpr auto is_pure = [] (
    //     typename vertex_t::traits::const_node_ptr const x,
    //     typename vertex_t::traits::const_node_ptr const y
    // ) -> bool {
    //     return vertex_t::traits::get_next(x) != y
    //         && vertex_t::traits::get_previous(x) != y;
    // };

    // int swap_mask = 0, best_mask = 0;
    // bool to_set_init_cost = true;
    // cost_t best_edges_cost = std::numeric_limits<cost_t>::max();
    // const auto rotate_perm = [&] (auto &&seg_at)
    //     [[ always_inline, gnu::hot ]]
    // {
    //     RotDesc * __restrict rot_top = rotations;
    //     rot_top->step = 0;

    //     for (int idx = 1; idx > 0; ++idx) {
    //         cost_t &edges_cost = rot_top->cost;
    //         const auto src_ptr = rot_top->prev;
    //         const vertex_t src_v = vertex_t::v(src_ptr)->id;
    //         const cost_t * __restrict const w_src
    //             = weights + src_v * n;

    //         if (idx == k) [[ unlikely ]] {
    //             const auto dst_ptr = seg_at(0).first;
    //             const auto dst_v = vertex_t::v(dst_ptr)->id;
    //             if (to_set_init_cost) [[ unlikely ]] {
    //                 best_edges_cost = change
    //                     = edges_cost + w_src[dst_v];
    //                 to_set_init_cost = false;
    //                 is_not_pure_k_opt = this->is_not_pure_k_opt;
    //             } else if (
    //                 is_not_pure_k_opt || is_pure(src_ptr, dst_ptr)
    //             ) [[ likely ]] {
    //                 edges_cost += w_src[dst_v];
    //                 if (edges_cost < best_edges_cost)
    //                     [[ unlikely ]]
    //                 {
    //                     best_edges_cost = edges_cost;
    //                     best_mask = swap_mask;
    //                     if (choose_first) [[ unlikely ]]
    //                         return true;
    //                 }
    //             }
    //             --rot_top;
    //             idx -= 2;
    //             continue;
    //         }
    //         const seg_t &dst_seg = seg_at(idx);
    //         int_fast8_t &step = rot_top->step;
    //         if (step == 0) {  // try one rotation
    //             const auto dst_ptr = dst_seg.first;
    //             if ( is_not_pure_k_opt
    //               || is_pure(src_ptr, dst_ptr)
    //             ) [[ likely ]] {
    //                 const cost_t new_cost = edges_cost
    //                     + w_src[vertex_t::v(dst_ptr)->id];
    //                 if (new_cost < best_edges_cost)
    //                    [[ unlikely ]]
    //                 {
    //                     swap_mask &= ~(1 << idx);
    //                     (++rot_top)->step = 0;
    //                     rot_top->cost = new_cost;
    //                     rot_top->prev = dst_seg.second;
    //                     ++step;
    //                     continue;
    //                 }
    //             }
    //         }
    //         // other rotation
    //         if (dst_seg.first != dst_seg.second && step == 1) {
    //             const auto dst_ptr = dst_seg.second;
    //             if ( is_not_pure_k_opt
    //               || is_pure(src_ptr, dst_ptr)
    //             ) [[ likely ]] {
    //                 const cost_t new_cost = edges_cost
    //                     + w_src[vertex_t::v(dst_ptr)->id];
    //                 if (new_cost < best_edges_cost)
    //                    [[ unlikely ]]
    //                 {
    //                     swap_mask |= (1 << idx);
    //                     (++rot_top)->step = 0;
    //                     rot_top->cost = new_cost;
    //                     rot_top->prev = dst_seg.first;
    //                     ++step;
    //                     continue;
    //                 }
    //             }
    //         }
    //         --rot_top;
    //         idx -= 2;
    //     }
    //     return false;
    // };

    // if (this->seg_perm_indices.empty()) {
    //     std::array<seg_t, (K == -1 ? 16 : K)> buffer;
    //     seg_t * __restrict segs_perm;
    //     if constexpr (can_modify_segs) {
    //         segs_perm = segs;
    //     } else {
    //         if (choose_first) {
    //             segs_perm = best_segs;
    //         } else {
    //             if constexpr (K == -1) {
    //                 segs_perm = k <= 16
    //                           ? buffer.data()
    //                           : this->perm_buf.data();
    //             } else {
    //                 segs_perm = buffer.data();
    //             }
    //         }
    //         std::copy_n(segs, k, segs_perm);
    //     }
    //     const auto seg_at = [&] (int idx) -> const seg_t& {
    //         return segs_perm[idx];
    //     };
    //     cost_t cur_best = best_edges_cost;
    //     detail::all_permutations(segs_perm + 1, k - 1, [&] () {
    //         if (rotate_perm(seg_at)) [[ unlikely ]] {
    //             return true;
    //         }
    //         if (best_edges_cost < cur_best) [[ unlikely ]] {
    //             cur_best = best_edges_cost;
    //             if (best_edges_cost < change) [[ likely ]] {
    //                 std::copy_n(segs_perm, k, best_segs);
    //             }
    //         }
    //         return false;
    //     });
    //     if (best_edges_cost < change) [[ unlikely ]] {
    //         change = best_edges_cost - change;
    //     }
    //     return best_mask;

    // } else [[ likely ]] {
    //     int best_perm_idx = -1;
    //     perm_idx = 0;
    //     cost_t cur_best = best_edges_cost;

    //     for (const auto &perm_indices_ : this->seg_perm_indices) {
    //         const int * const  perm_indices
    //             = perm_indices_.data();
    //         const auto seg_at = [&] (int idx) -> const seg_t& {
    //             return segs[perm_indices[idx]];
    //         };
    //         if (rotate_perm(seg_at)) [[ unlikely ]] {
    //             change = best_edges_cost - change;
    //             return best_mask;
    //         }
    //         if (best_edges_cost < cur_best) [[ unlikely ]] {
    //             cur_best = best_edges_cost;
    //             if (best_edges_cost < change) [[ likely ]] {
    //                 best_perm_idx = perm_idx;
    //             }
    //         }
    //         ++perm_idx;
    //     }
    //     if (best_perm_idx != -1) [[ unlikely ]] {
    //         perm_idx = best_perm_idx;
    //         change = best_edges_cost - change;
    //     }
    //     return best_mask;
    // }
}

template<typename cost_t, typename vertex_t, int K>
void CutKOpt<cost_t, vertex_t, K>::applyCut(
    const seg_t * __restrict const segs,
    const int perm_idx,
    const int swap_mask,
    const int n
) const noexcept {
    // using seg_t = const seg_t;

    // const int k = this->getK();
    // if (perm_idx <= 0) {
    //     detail::applyCut<cost_t, vertex_t, K>(n, path,
    //         [segs] (const int i) -> seg_t& {
    //             return segs[i];
    //     }, buffer, swap_mask, k);
    //     return;
    // }
    // const int * const  perm_indices
    //     = this->seg_perm_indices[perm_idx].data();

    // detail::applyCut<cost_t, vertex_t, K>(n, path,
    //     [segs, perm_indices] (const int i) -> seg_t& {
    //         return segs[perm_indices[i]];
    // }, buffer, swap_mask, k);
}

}  // namespace k_opt


#endif
