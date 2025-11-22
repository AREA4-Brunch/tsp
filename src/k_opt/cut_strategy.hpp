#ifndef TSP_K_OPT_CUT_STRATEGY_HPP
#define TSP_K_OPT_CUT_STRATEGY_HPP

#include <concepts>
#include <vector>

namespace k_opt {

template<typename this_t, typename cost_t, typename vertex_t>
concept CutStrategy = requires(
    this_t &t,
    const int i,
    const int j,
    const int k,
    cost_t &change,
    const std::vector<vertex_t> &path_c,
    const std::vector<std::vector<cost_t>> &weights,
    const int patch_ordinal,
    std::vector<vertex_t> &path
) {
    { t.selectCut(i, j, k, change, path_c, weights) } -> std::same_as<int>;
    { t.applyCut(i, j, k, patch_ordinal, path) } -> std::same_as<void>;
};

}  // namespace k_opt


#endif
