#ifndef TSP_K_OPT_CUT_STRATEGY_HPP
#define TSP_K_OPT_CUT_STRATEGY_HPP

#include <concepts>
#include <vector>

namespace k_opt {

template<typename this_t, typename cost_t, typename vertex_t>
concept CutStrategy = requires(
    this_t &t,
    const std::vector<vertex_t> &path_c,
    std::vector<std::pair<int, int>> &segs,
    cost_t &change,
    const std::vector<std::vector<cost_t>> &weights,
    std::vector<vertex_t> &path,
    const std::vector<int> &seg_perm_indices_c,
    const std::vector<bool> &is_rotated_c
) {
    { t.selectCut(path_c, segs, change, weights) }
        -> std::same_as<bool>;
    { t.applyCut(path, segs) } -> std::same_as<void>;
};

}  // namespace k_opt

#endif
