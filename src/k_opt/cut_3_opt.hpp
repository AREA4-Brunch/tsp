#ifndef TSP_3_OPT_CUT_HPP
#define TSP_3_OPT_CUT_HPP

#include <vector>
#include <array>

namespace k_opt {

/// @brief Implements k_opt::CutStrategy concept avoiding virtual overhead.
template<typename cost_t, typename vertex_t, bool no_2_opt=false>
class Cut3Opt {
    static_assert(std::is_arithmetic_v<cost_t>, "cost_t must be arithmetic");

 public:

    static constexpr int NUM_CUTS = 3;

    Cut3Opt() = default;
    ~Cut3Opt() = default;

    template<typename segs_t>
    [[ gnu::hot ]]
    inline std::vector<std::pair<int, int>> selectCut(
        const std::vector<vertex_t> &path,
        const segs_t &segs,
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
};


template<typename cost_t, typename vertex_t, bool no_2_opt>
template<typename segs_t>
std::vector<std::pair<int, int>>
Cut3Opt<cost_t, vertex_t, no_2_opt>::selectCut(
    const std::vector<vertex_t> &path,
    const segs_t &segs,
    cost_t &change,
    const std::vector<std::vector<cost_t>> &weights,
    int &perm_idx
) const {
    const vertex_t a = path[segs[0].second];
    const vertex_t b = path[segs[1].first];
    const vertex_t c = path[segs[1].second];
    const vertex_t d = path[segs[2].first];
    const vertex_t e = path[segs[2].second];
    const vertex_t f = path[segs[0].first];

    const cost_t* __restrict wa = weights[a].data();
    const cost_t* __restrict wb = weights[b].data();
    const cost_t* __restrict wc = weights[c].data();
    const cost_t* __restrict wd = weights[d].data();
    const cost_t* __restrict we = weights[e].data();

    const cost_t current = wa[b] + wc[d] + we[f];
    cost_t best = current;

    #define TRY_MOVE(ord, cost_expr) \
        do { \
            const cost_t c_ = (cost_expr); \
            if (c_ < best) { best = c_; perm_idx = (ord); } \
        } while(0)

    if constexpr (!no_2_opt) {
        TRY_MOVE(0b010, wa[c] + wb[d] + we[f]);
        TRY_MOVE(0b100, wa[b] + wc[e] + wd[f]);
    }

    TRY_MOVE(0b110, wa[c] + wb[e] + wd[f]);
    TRY_MOVE(0b001, wa[d] + we[b] + wc[f]);
    TRY_MOVE(0b011, wa[d] + we[c] + wb[f]);
    TRY_MOVE(0b101, wa[e] + wd[b] + wc[f]);

    if constexpr (!no_2_opt) {
        TRY_MOVE(0b111, wa[e] + wd[c] + wb[f]);
    }

    #undef TRY_MOVE

    change = best - current;
    return {};
}

template<typename cost_t, typename vertex_t, bool no_2_opt>
template<typename segs_t>
void Cut3Opt<cost_t, vertex_t, no_2_opt>::applyCut(
    std::vector<vertex_t> &path,
    const segs_t &segs,
    const int move_ord,
    std::vector<vertex_t> &buffer
) const {
    const int i1 = segs[1].first;
    const int j1 = segs[1].second;
    const int i2 = segs[2].first;
    const int j2 = segs[2].second;
    const int len1 = j1 - i1 + 1;  // b..c
    const int len2 = j2 - i2 + 1;  // d..e

    switch (move_ord) {
        case 0b010: {
            std::reverse(path.begin() + i1, path.begin() + j1 + 1);
            return;
        }
        case 0b100: {
            std::reverse(path.begin() + i2, path.begin() + j2 + 1);
            return;
        }
        case 0b110: {
            std::reverse(path.begin() + i1, path.begin() + j1 + 1);
            std::reverse(path.begin() + i2, path.begin() + j2 + 1);
            return;
        }
        case 0b001:
        case 0b011:
        case 0b101:
        case 0b111: {
            vertex_t* const __restrict buf = buffer.data();
            if (move_ord & 0b100) {
                std::reverse_copy(path.begin() + i2,
                                  path.begin() + j2 + 1, buf);
            } else {
                std::copy(path.begin() + i2, path.begin() + j2 + 1, buf);
            }
            if (move_ord & 0b010) {
                std::reverse_copy(path.begin() + i1,
                                  path.begin() + j1 + 1, buf + len2);
            } else {
                std::copy(path.begin() + i1,
                          path.begin() + j1 + 1, buf + len2);
            }
            std::copy(buf, buf + len1 + len2, path.begin() + i1);
            return;
        }
        default:
            return;
    }
}


// ============================================================
// Type aliases:

template<typename cost_t, typename vertex_t>
using Cut3OptNo2Opt = Cut3Opt<cost_t, vertex_t, true>;


}  // namespace k_opt

#endif
