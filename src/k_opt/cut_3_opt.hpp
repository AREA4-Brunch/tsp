#ifndef TSP_3_OPT_CUT_HPP
#define TSP_3_OPT_CUT_HPP

#include <vector>
#include <array>
#include "cut_k_opt.hpp"


namespace k_opt {

/// @brief Implements k_opt::CutStrategy concept avoiding virtual overhead.
template<typename cost_t, typename vertex_t, bool no_2_opt=false>
class Cut3Opt {
    static_assert(std::is_arithmetic_v<cost_t>, "cost_t must be arithmetic");

 public:

    Cut3Opt() = default;
    ~Cut3Opt() = default;

    [[ gnu::always_inline ]]
    inline std::vector<std::pair<int, int>> selectCut(
        const std::vector<vertex_t> &path,
        std::vector<std::pair<int, int>> &segs,
        cost_t &change,
        const std::vector<std::vector<cost_t>> &weights
    ) const;

    [[ gnu::always_inline ]]
    inline void applyCut(
        std::vector<vertex_t> &path,
        std::vector<std::pair<int, int>> &segs,
        const int perm_idx = -1
    ) const;
};


namespace detail {

template<typename cost_t>
struct Move3Opt {
    cost_t cost;
    int8_t perm;
    int8_t rot;
};

template<typename cost_t, typename vertex_t, int num_moves>
[[ gnu::always_inline ]]
inline std::array<Move3Opt<cost_t>, num_moves> getMoves(
    const vertex_t a,
    const vertex_t b,
    const vertex_t c,
    const vertex_t d,
    const vertex_t e,
    const vertex_t f,
    const std::vector<std::vector<cost_t>> &weights
) {
    if constexpr (num_moves == 4) {
        return {{
            { weights[a][c] + weights[b][e] + weights[d][f], 0, 0b11 },
            { weights[a][d] + weights[e][b] + weights[c][f], 1, 0b00 },
            { weights[a][d] + weights[e][c] + weights[b][f], 1, 0b01 },
            { weights[a][e] + weights[d][b] + weights[c][f], 1, 0b10 }
        }};
    } else {
        return {{
            { weights[a][c] + weights[b][d] + weights[e][f], 0, 0b01 },
            { weights[a][b] + weights[c][e] + weights[d][f], 0, 0b10 },
            { weights[a][c] + weights[b][e] + weights[d][f], 0, 0b11 },
            { weights[a][d] + weights[e][b] + weights[c][f], 1, 0b00 },
            { weights[a][d] + weights[e][c] + weights[b][f], 1, 0b01 },
            { weights[a][e] + weights[d][b] + weights[c][f], 1, 0b10 },
            { weights[a][e] + weights[d][c] + weights[b][f], 1, 0b11 }
        }};
    }
}

}  // namespace detail


template<typename cost_t, typename vertex_t, bool no_2_opt>
std::vector<std::pair<int, int>>
Cut3Opt<cost_t, vertex_t, no_2_opt>::selectCut(
    const std::vector<vertex_t> &path,
    std::vector<std::pair<int, int>> &segs,
    cost_t &change,
    const std::vector<std::vector<cost_t>> &weights
) const {
    static constexpr int num_moves = no_2_opt ? 4 : 7;
    const auto &s0 = segs[0];
    auto &s1 = segs[1];
    auto &s2 = segs[2];

    const vertex_t a = path[s0.second];
    const vertex_t b = path[s1.first];
    const vertex_t c = path[s1.second];
    const vertex_t d = path[s2.first];
    const vertex_t e = path[s2.second];
    const vertex_t f = path[s0.first];
    const auto moves = detail::getMoves<cost_t, vertex_t, num_moves>(
        a, b, c, d, e, f, weights
    );
    change = weights[a][b] + weights[c][d] + weights[e][f];
    cost_t best = change;
    int best_idx = -1;
    const auto update_best = [&] (const int i) {
        const cost_t cost = moves[i].cost;
        if (cost < best) {
            best = cost;
            best_idx = i;
        }
    };
    [&] <std::size_t ...I> (std::index_sequence<I...>) {
        (update_best(I), ...);
    } (std::make_index_sequence<num_moves>{});

    if (best_idx < 0) return {};
    change = best - change;
    const auto &m = moves[best_idx];
    if (m.rot & 0b01) std::swap(s1.first, s1.second);
    if (m.rot & 0b10) std::swap(s2.first, s2.second);
    return { { m.perm, -1 } };
}

template<typename cost_t, typename vertex_t, bool no_2_opt>
void Cut3Opt<cost_t, vertex_t, no_2_opt>::applyCut(
    std::vector<vertex_t> &path,
    std::vector<std::pair<int, int>> &segs,
    const int perm_idx
) const {
    using seg_t = const std::pair<int, int>;
    if (perm_idx > 0) std::swap(segs[1], segs[2]);
    detail::applyCut<cost_t, vertex_t, 3>(path, segs,
        [&segs] (const int i) -> seg_t& {
            return segs[i];
    });
    if (perm_idx > 0) std::swap(segs[1], segs[2]);
}

// ============================================================
// Type aliases:

template<typename cost_t, typename vertex_t>
using Cut3OptNo2Opt = Cut3Opt<cost_t, vertex_t, true>;


}  // namespace k_opt

#endif
