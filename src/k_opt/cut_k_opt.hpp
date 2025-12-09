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
    }

    ~CutKOpt() = default;

    template<typename segs_t>
    [[ gnu::hot ]]
    inline std::vector<std::pair<int, int>> selectCut(
        const std::vector<vertex_t> &path,
        segs_t &segs,
        cost_t &change,
        const std::vector<std::vector<cost_t>> &weights,
        int &perm_idx
    ) const;

    template<typename segs_t>
    [[ gnu::hot ]]
    inline void applyCut(
        std::vector<vertex_t> &path,
        const segs_t &segs,
        const int perm_idx,
        std::vector<vertex_t> &buffer
    ) const;

    [[ nodiscard ]] constexpr int getK() const {
        return K == -1 ? this->k : K;
    }

    void setK(const int k, const bool do_pre_gen_perms=true) {
        static_assert(K == -1, "Cannot call setK on static K specialization");
        if (k == this->k) return;
        this->seg_perm_indices.clear();
        this->rot_stack.resize(k);
        this->k = k;
        if (do_pre_gen_perms) this->generateSegPermIndices();
    }

 private:

    int k = -1;
    bool is_not_pure_k_opt = true;
    bool select_first_better = true;
    std::vector<std::vector<int>> seg_perm_indices;
    // buffer for dynamic K only
    mutable std::vector<std::pair<int, cost_t>> rot_stack;

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
[[ gnu::always_inline ]]
inline bool all_permutations(iter_t begin, const int n, callback_t &&cb) {
    if (cb()) return true;
    if (n == 2) {
        std::swap(*begin, *(begin + 1));
        return cb();
    }
    std::vector<int> c(n, 0);
    for (int i = 0; i < n; ) {
        if (c[i] < i) {
            if (i & 1) std::swap(*(begin + c[i]), *(begin + i));
            else       std::swap(*(begin), *(begin + i));
            if (cb()) return true;
            ++c[i];
            i = 0;
        } else {
            c[i] = 0;
            ++i;
        }
    }
    return false;
}

template<typename cost_t, typename vertex_t, typename segs_t>
[[ gnu::always_inline ]]
inline cost_t calcCutCost(
    const std::vector<vertex_t> &path,
    const segs_t &segs,
    const std::vector<std::vector<cost_t>> &weights,
    bool &not_inc_single_v_seg
) {
    not_inc_single_v_seg = true;
    cost_t cut = weights[path[segs.back().second]]
                        [path[segs[0].first]];
    for (int i = 1, n = segs.size(); i < n; ++i) {
        const int src = path[segs[i - 1].second];
        const int dst = path[segs[i].first];
        if (not_inc_single_v_seg) {
            not_inc_single_v_seg
                = segs[i].first != segs[i].second;
        }
        cut += weights[src][dst];
    }
    return cut;
}

template<typename cost_t, typename vertex_t, int K = -1, typename segs_t>
[[ gnu::always_inline ]]
inline void applyCut(
    std::vector<vertex_t> &path,
    const segs_t &segs,
    auto &&seg_at,
    std::vector<vertex_t> &buffer
) {
    const int n = path.size();
    vertex_t *new_path = buffer.data();
    vertex_t * const path_begin = path.data();

    // first segment is always [k, i]
    const auto &seg = seg_at(0);
    const int start = seg.first;
    const int end = seg.second;
    if (start > end) {
        new_path = std::copy(
            path_begin + start,
            path_begin + n,
            new_path
        );
        new_path = std::copy(
            path_begin,
            path_begin + end + 1,
            new_path
        );
    } else if (start < end) {
        new_path = std::reverse_copy(
            path_begin,
            path_begin + start + 1,
            new_path
        );
        new_path = std::reverse_copy(
            path_begin + end,
            path_begin + n,
            new_path
        );
    } else {  // single vertex
        *(new_path++) = path[start];
    }

    const auto insert_seg = [&] (const int s) {
        const auto &seg = seg_at(s);
        const int start = seg.first;
        const int end = seg.second;
        if (start <= end) {  // forward
            new_path = std::copy(
                path_begin + start, 
                path_begin + end + 1,
                new_path
            );
        } else {  // reversed
            new_path = std::reverse_copy(
                path_begin + end,
                path_begin + start + 1,
                new_path
            );
        }
    };

    // remaining k-1 contiguous ranges
    if constexpr (K == -1) {
        for (int s = 1, k = segs.size(); s < k; ++s) {
            insert_seg(s);
        }
    } else {
        [&] <std::size_t... I> (std::index_sequence<I...>) {
            (insert_seg(I + 1), ...);
        } (std::make_index_sequence<K - 1>{});
    }

    std::swap(path, buffer);
}

template<typename T, std::size_t N>
constexpr inline void swap(std::vector<T> &v, std::array<T, N> &a) {
    const auto swap_element = [&] (const std::size_t i) {
        std::swap(v[i], a[i]);
    };
    [&] <std::size_t ...I> (std::index_sequence<I...>) {
        (swap_element(I), ...);
    } (std::make_index_sequence<N>{});
}

template<typename T, std::size_t N>
constexpr inline void swap(std::array<T, N> &a, std::vector<T> &v) {
    detail::swap(v, a);
}

template<typename T>
constexpr inline void swap(std::vector<T> &a, std::vector<T> &b) {
    std::swap(a, b);
}

template<typename T, std::size_t N>
constexpr inline void swap(std::array<T, N> &a, std::array<T, N> &b) {
    std::swap(a, b);
}

}  // namespace detail

template<typename cost_t, typename vertex_t, int K>
void CutKOpt<cost_t, vertex_t, K>::generateSegPermIndices() {
    const int k = this->getK();
    if (k >= 10) return;  // would take too much space
    this->seg_perm_indices.reserve(detail::factorial(k - 1));
    std::vector<int> indices(k);
    std::iota(indices.begin(), indices.end(), 0);
    detail::all_permutations(indices.begin() + 1, k - 1, [&] () {
        this->seg_perm_indices.emplace_back(indices);
        return false;
    });
}

template<typename cost_t, typename vertex_t, int K>
template<typename segs_t>
std::vector<std::pair<int, int>>
CutKOpt<cost_t, vertex_t, K>::selectCut(
    const std::vector<vertex_t> &path,
    segs_t &segs,
    cost_t &change,
    const std::vector<std::vector<cost_t>> &weights,
    int &perm_idx
) const {
    using seg_t = std::pair<int, int>;
    using rot_t = std::pair<int, cost_t>;

    bool not_inc_single_v_seg = false;
    change = detail::calcCutCost(path, segs, weights,
                                 not_inc_single_v_seg);
    const bool is_not_pure_k_opt = !not_inc_single_v_seg
                                 || this->is_not_pure_k_opt;
    cost_t best_edges_cost = change;
    segs_t best_segs;

    std::array<rot_t, (K == -1 ? 0 : K)> rot_stack;
    rot_t *rotations;
    if constexpr (K == -1) rotations = this->rot_stack.data();
    else                   rotations = rot_stack.data();
    const int k = this->getK();
    const auto rotate_perm = [&] (auto &&seg_at) {
        rot_t *rot_top = rotations;
        *rot_top = { 0, static_cast<cost_t>(0) };
        for (int idx = 1; rot_top >= rotations; ++idx) {
            cost_t &edges_cost = rot_top->second;
            const int src_i = seg_at(idx - 1).second;
            const vertex_t src_v = path[src_i];
            if (idx == k) {
                const int dst_i = seg_at(0).first;
                const int diff = dst_i - src_i;
                if ( is_not_pure_k_opt
                 || (diff != 1 && diff != -1)
                ) {
                    edges_cost += weights[src_v][path[dst_i]];
                    if (edges_cost < best_edges_cost) {
                        best_edges_cost = edges_cost;
                        if (this->select_first_better) return true;
                        best_segs = segs;
                    }
                }
                --rot_top;
                idx -= 2;
                continue;
            }
            seg_t &dst_seg = seg_at(idx);
            int &step = rot_top->first;
            if (step == 0) {  // try one rotation
                const int dst_i = dst_seg.first;
                const int diff = dst_i - src_i;
                if ( is_not_pure_k_opt
                  || (diff != 1 && diff != -1)
                ) {
                    const cost_t new_change = edges_cost
                        + weights[src_v][path[dst_i]];
                    if (new_change < best_edges_cost) {
                        *(++rot_top) = { 0, new_change };
                        ++step;
                        continue;
                    }
                }
            }
            // other rotation
            if (dst_seg.first != dst_seg.second && step == 1) {
                std::swap(dst_seg.first, dst_seg.second);
                const int dst_i = dst_seg.first;
                const int diff = dst_i - src_i;
                if ( is_not_pure_k_opt
                 || (diff != 1 && diff != -1)
                ) {
                    const cost_t new_change = edges_cost
                        + weights[src_v][path[dst_i]];
                    if (new_change < best_edges_cost) {
                        *(++rot_top) = { 0, new_change };
                        ++step;
                        continue;
                    }
                }
            }
            --rot_top;
            idx -= 2;
        }
        return false;
    };

    if (this->seg_perm_indices.empty()) {
        std::array<seg_t, (K == -1 ? 0 : K)> buffer;
        std::vector<seg_t> original_segs;
        seg_t *segs_perm;
        if constexpr (K == -1) {
            original_segs.assign(segs.begin(), segs.end());
            segs_perm = segs.data();
        } else {
            std::copy(segs.begin(), segs.end(), buffer.begin());
            segs_perm = buffer.data();
        }
        const auto seg_at = [segs_perm] (int idx) -> seg_t& {
            return segs_perm[idx];
        };
        detail::all_permutations(segs_perm + 1, k - 1, [&] () {
            return rotate_perm(seg_at);
        });
        if (best_edges_cost >= change) return {};
        change = best_edges_cost - change;
        if constexpr (K == -1) {
            detail::swap(segs, original_segs);  // restore segs
            if (this->select_first_better) return original_segs;
        } else {
            if (this->select_first_better) {
                return std::vector<seg_t> (
                    buffer.begin(), buffer.end());
            }
        }
        std::vector<seg_t> best_segs_vec;
        detail::swap(best_segs_vec, best_segs);
        return best_segs_vec;
    } else {
        int best_perm_idx = -1;
        perm_idx = 0;
        cost_t cur_best = change;
        for (const auto &perm_indices : this->seg_perm_indices) {
            const auto seg_at = [&] (int idx) -> seg_t& {
                return segs[perm_indices[idx]];
            };
            if (rotate_perm(seg_at)) {
                change = best_edges_cost - change;
                return {};
            }
            if (best_edges_cost < cur_best) {
                cur_best = best_edges_cost;
                best_perm_idx = perm_idx;
            }
            ++perm_idx;
        }
        if (best_perm_idx != -1) {
            perm_idx = best_perm_idx;
            change = best_edges_cost - change;
            std::swap(segs, best_segs);
        }
        return {};
    }
}

template<typename cost_t, typename vertex_t, int K>
template<typename segs_t>
void CutKOpt<cost_t, vertex_t, K>::applyCut(
    std::vector<vertex_t> &path,
    const segs_t &segs,
    const int perm_idx,
    std::vector<vertex_t> &new_path
) const {
    using seg_t = const std::pair<int, int>;
    if (perm_idx <= 0) {
        detail::applyCut<cost_t, vertex_t, K>(path, segs,
            [&segs] (const int i) -> seg_t& {
                return segs[i];
        }, new_path);
        return;
    }
    const auto &perm_indices = this->seg_perm_indices[perm_idx];
    detail::applyCut<cost_t, vertex_t, K>(path, segs,
        [&segs, &perm_indices] (const int i) -> seg_t& {
            return segs[perm_indices[i]];
    }, new_path);
}

}  // namespace k_opt


#endif
