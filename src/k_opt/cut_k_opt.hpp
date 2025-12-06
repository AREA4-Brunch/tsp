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

    [[ gnu::always_inline ]]
    inline bool selectCut(
        const std::vector<vertex_t> &path,
        std::vector<std::pair<int, int>> &segs,
        cost_t &change,
        const std::vector<std::vector<cost_t>> &weights
    ) const;

    [[ gnu::always_inline ]]
    inline void applyCut(
        std::vector<vertex_t> &path,
        const std::vector<std::pair<int, int>> &segs
    ) const;

    [[ nodiscard ]] constexpr int getK() const {
        return K == -1 ? k : K;
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
inline bool all_permutations(iter_t begin, iter_t end, callback_t &&cb) {
    if (cb()) return true;
    const int n = std::distance(begin, end);
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

template<typename cost_t, typename vertex_t>
cost_t calcCutCost(
    const std::vector<vertex_t> &path,
    const std::vector<std::pair<int, int>> &segs,
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

template<typename cost_t, typename vertex_t, int K = -1>
[[ gnu::always_inline ]]
inline void applyCut(
    std::vector<vertex_t> &path,
    const std::vector<std::pair<int, int>> &segs
) {
    const int n = static_cast<int>(path.size());
    std::vector<vertex_t> new_path;
    new_path.reserve(n);
    // first segment is always [k, i]
    const int start = segs[0].first;
    const int end = segs[0].second;
    if (start > end) {
        new_path.insert(
            new_path.end(),
            path.begin() + start,
            path.end()
        );
        new_path.insert(
            new_path.end(),
            path.begin(),
            path.begin() + end + 1
        );
    } else if (start < end) {
        new_path.insert(
            new_path.end(),
            path.rbegin() + (n - 1 - start),
            path.rend()
        );
        new_path.insert(
            new_path.end(),
            path.rbegin(),
            path.rbegin() + (n - end)
        );
    } else {  // single vertex
        new_path.push_back(path[start]);
    }

    const auto insert_seg = [&] (const int s) {
        const int start = segs[s].first;
        const int end = segs[s].second;
        if (start <= end) {  // forward
            new_path.insert(
                new_path.end(), 
                path.begin() + start, 
                path.begin() + end + 1
            );
        } else {  // reversed
            new_path.insert(
                new_path.end(),
                path.rbegin() + (n - 1 - start),
                path.rbegin() + (n - end)
            );
        }
    };

    // remaining k-1 contiguous ranges
    if constexpr (K == -1) {
        for (int s = 1, k = segs.size(); s < k; ++s) {
            insert_seg(s);
        }
    } else {  // unroll using fold expression
        [&] <std::size_t... I> (std::index_sequence<I...>) {
            (insert_seg(I + 1), ...);
        } (std::make_index_sequence<K - 1>{});
    }

    path = std::move(new_path);
}

}  // namespace detail

template<typename cost_t, typename vertex_t, int K>
void CutKOpt<cost_t, vertex_t, K>::generateSegPermIndices() {
    if (this->k - 1 > 9) return;  // would take too much space
    this->seg_perm_indices.reserve(detail::factorial(this->k - 1));
    std::vector<int> indices(this->k);
    std::iota(indices.begin(), indices.end(), 0);
    detail::all_permutations(indices.begin() + 1, indices.end(),
        [&] () {
            this->seg_perm_indices.emplace_back(indices);
            return false;
    });
}

template<typename cost_t, typename vertex_t, int K>
bool CutKOpt<cost_t, vertex_t, K>::selectCut(
    const std::vector<vertex_t> &path,
    std::vector<std::pair<int, int>> &segs,
    cost_t &change,
    const std::vector<std::vector<cost_t>> &weights
) const {
    using seg_t = std::pair<int, int>;
    using rot_t = std::pair<int, cost_t>;

    bool not_inc_single_v_seg = false;
    change = detail::calcCutCost(path, segs, weights,
                                 not_inc_single_v_seg);
    const bool is_not_pure_k_opt = !not_inc_single_v_seg
                                 || this->is_not_pure_k_opt;
    cost_t best_edges_cost = change;
    std::vector<seg_t> best_segs;

    std::array<rot_t, (K != -1 ? K : 0)> rot_stack;
    rot_t *rotations;
    if constexpr (K == -1) rotations = this->rot_stack.data();
    else                   rotations = rot_stack.data();

    const auto rotate_perm = [&] (auto &&seg_at) {
        rot_t *rot_top = rotations;
        *rot_top = { 0, static_cast<cost_t>(0) };
        const int k = this->getK();  // prefer over this->k if static
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
            if (step == 1) {  // other rot, in-place on stack
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
        const auto seg_at = [&segs] (int idx) -> seg_t& {
            return segs[idx];
        };
        detail::all_permutations(segs.begin() + 1, segs.end(), [&] () {
            return rotate_perm(seg_at);
        });
        if (best_edges_cost >= change) return false;
        if (!this->select_first_better) segs = std::move(best_segs);
    } else {
        int best_perm_idx = -1;
        int perm_idx = 0;
        cost_t cur_best = change;
        for (const auto &perm_indices : this->seg_perm_indices) {
            const auto seg_at = [&] (int idx) -> seg_t& {
                return segs[perm_indices[idx]];
            };
            if (rotate_perm(seg_at)) break;
            if (best_edges_cost < cur_best) {
                cur_best = best_edges_cost;
                best_perm_idx = perm_idx;
            }
            ++perm_idx;
        }
        if (best_edges_cost >= change) return false;
        if (best_perm_idx < 0) best_perm_idx = perm_idx;
        if (best_segs.empty()) best_segs = segs;
        const auto &perm_indices
            = this->seg_perm_indices[best_perm_idx];
        for (int i = this->k - 1; i >= 0; --i) {
            segs[i] = std::move(best_segs[perm_indices[i]]);
        }
    }
    change = best_edges_cost - change;
    return true;
}

template<typename cost_t, typename vertex_t, int K>
void CutKOpt<cost_t, vertex_t, K>::applyCut(
    std::vector<vertex_t> &path,
    const std::vector<std::pair<int, int>> &segs
) const {
    detail::applyCut<cost_t, vertex_t, K>(path, segs);
}

}  // namespace k_opt


#endif
