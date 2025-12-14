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
 * @return true if new path is in `best_segs` buffer, false if in path.
 */
template<typename this_t, typename cost_t, typename vertex_t, int K>
concept CutStrategy = requires(
    this_t &t,

    const vertex_t * __restrict const path_c,
    std::pair<int, int> * __restrict const segs,
    cost_t &change,
    const std::vector<cost_t> * __restrict const weights,
    int &perm_idx,
    std::pair<int, int> * __restrict const best_segs,

    vertex_t * __restrict const path,
    const std::pair<int, int> * __restrict const segs_c,
    const int perm_idx_c,
    vertex_t * __restrict const buffer,
    const int n
) {
    { this_t::NUM_CUTS } -> std::convertible_to<int>;
    requires (K == -1 || this_t::NUM_CUTS == K);

    { t.selectCut(path_c, segs, change, weights, perm_idx, best_segs) }
        noexcept -> std::same_as<bool>;
    { t.applyCut(path, segs_c, perm_idx_c, buffer, n) }
        noexcept -> std::same_as<bool>;
};

}  // namespace k_opt

#endif
