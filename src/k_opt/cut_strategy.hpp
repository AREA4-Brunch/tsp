#ifndef TSP_K_OPT_CUT_STRATEGY_HPP
#define TSP_K_OPT_CUT_STRATEGY_HPP

#include <concepts>
#include <vector>
#include <array>
#include <type_traits>

namespace k_opt {

/**
 * @brief Interface for k-opt cut strategies.
 * 
 * - selectCut:
 * @param segs Modified, not permuted, but segs may get reversed.
 * @param change Modifies to gain/delta if improvement found
 *                 else is set to original cut cost.
 * @return Empty if no better cut found, else either perm. reversed
 *         reversed segs, or { { perm_idx, foo } } and segs with
 *         matching reversals.
 * 
 * - applyCut:
 * @param segs It may or may not get modified.
 */
template<typename this_t, typename cost_t, typename vertex_t, int K>
concept CutStrategy = requires(
    this_t &t,
    const std::vector<vertex_t> &path_c,
    std::vector<std::pair<int, int>> &segs_vct,
    std::conditional_t<K == -1,
        std::vector<std::pair<int, int>>,
        std::array<std::pair<int, int>, K>
    > &segs,
    cost_t &change,
    const std::vector<std::vector<cost_t>> &weights,
    int &perm_idx,
    std::vector<vertex_t> &path,
    const std::vector<int> &seg_perm_indices_c,
    const std::vector<bool> &is_rotated_c,
    const int perm_idx_c,
    std::vector<vertex_t> &new_path
) {
    { this_t::NUM_CUTS } -> std::convertible_to<int>;
    requires (K == -1 || this_t::NUM_CUTS == K);

    { t.selectCut(path_c, segs, change, weights, perm_idx) }
        -> std::same_as<std::vector<std::pair<int, int>>>;
    { t.applyCut(path, segs, perm_idx_c, new_path) }
        -> std::same_as<void>;
};

}  // namespace k_opt

#endif
