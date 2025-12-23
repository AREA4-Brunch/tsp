#ifndef TSP_3_OPT_CUT_HPP
#define TSP_3_OPT_CUT_HPP

#include <vector>
#include <array>
#include "vertex_concept.hpp"
#include "path_algos.hpp"

namespace k_opt {

/// @brief Implements k_opt::CutStrategy concept avoiding virtual overhead.
template<typename cost_t, IntrusiveVertex vertex_t, bool no_2_opt=false>
class Cut3Opt {
    static_assert(std::is_arithmetic_v<cost_t>, "cost_t must be arithmetic");

    using seg_t = std::pair<
        typename vertex_t::traits::node_ptr,
        typename vertex_t::traits::node_ptr
    >;

 public:

    static constexpr int NUM_CUTS = 3;

    Cut3Opt() = default;
    ~Cut3Opt() = default;

    template<bool can_modify_segs>
    [[ gnu::hot ]]
    inline int selectCut(
        const int n,
        const seg_t * __restrict const segs,
        cost_t &change,
        const cost_t * __restrict const weights,
        int &perm_idx,
        [[ maybe_unused ]] const seg_t * __restrict const
    ) const noexcept;

    [[ gnu::hot ]]
    inline bool applyCut(
        typename vertex_t::traits::node_ptr const path,
        const seg_t * __restrict const segs,
        const int move_ord,
        [[ maybe_unused ]] const int swap_mask = -1,
        [[ maybe_unused ]] const int n = -1
    ) const noexcept;
};


template<typename cost_t, IntrusiveVertex vertex_t, bool no_2_opt>
template<bool can_modify_segs>
int Cut3Opt<cost_t, vertex_t, no_2_opt>::selectCut(
    const int n,
    const seg_t * __restrict const segs,
    cost_t &change,
    const cost_t * __restrict const weights,
    int &perm_idx,
    [[ maybe_unused ]] const seg_t * __restrict const
) const noexcept {
    const vertex_t a = vertex_t::v(segs[0].second])->id;
    const vertex_t b = vertex_t::v(segs[1].first])->id;
    const vertex_t c = vertex_t::v(segs[1].second])->id;
    const vertex_t d = vertex_t::v(segs[2].first])->id;
    const vertex_t e = vertex_t::v(segs[2].second])->id;
    const vertex_t f = vertex_t::v(segs[0].first])->id;

    const cost_t* __restrict wa = weights + a * n;
    const cost_t* __restrict wb = weights + b * n;
    const cost_t* __restrict wc = weights + c * n;
    const cost_t* __restrict wd = weights + d * n;
    const cost_t* __restrict we = weights + e * n;

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
    return 0;
}

template<typename cost_t, IntrusiveVertex vertex_t, bool no_2_opt>
bool Cut3Opt<cost_t, vertex_t, no_2_opt>::applyCut(
    typename vertex_t::traits::node_ptr const path,
    const seg_t * __restrict const segs,
    const int move_ord,
    [[ maybe_unused ]] const int swap_mask,
    [[ maybe_unused ]] const int n
) const noexcept {
    // s0f s0s s1f s1s s2f s2s
    // a    b   c   d   e   f
    //         i1   j1  i2  j2



    const int i1 = segs[1].first;
    const int j1 = segs[1].second;
    const int i2 = segs[2].first;
    const int j2 = segs[2].second;
    const int len1 = j1 - i1 + 1;  // b..c
    const int len2 = j2 - i2 + 1;  // d..e

    switch (move_ord) {
        case 0b010: {
            k_opt::path_algos::reverse(
                segs[0].second,
                segs[1].first, segs[1].second,
                segs[2].first
            );
            return false;
        }
        case 0b100: {
            k_opt::path_algos::reverse(
                segs[1].second,
                segs[2].first, segs[2].second,
                segs[0].first
            );
            return false;
        }
        case 0b110: {
            k_opt::path_algos::reverse(
                segs[0].second,
                segs[1].first, segs[1].second,
                segs[2].first
            );
            k_opt::path_algos::reverse(
                segs[1].first,
                segs[2].first, segs[2].second,
                segs[0].first
            );
            return false;
        }
        case 0b001:
        case 0b011:
        case 0b101:
        case 0b111: {
            if (move_ord & 0b100) {
                std::reverse_copy(path + i2,
                                  path + j2 + 1, buf);
            } else {
                std::copy(path + i2, path + j2 + 1, buf);
            }
            if (move_ord & 0b010) {
                std::reverse_copy(path + i1,
                                  path + j1 + 1, buf + len2);
            } else {
                std::copy(path + i1,
                          path + j1 + 1, buf + len2);
            }
            const int buf_len = len1 + len2;
            // copy shorter part
            if (buf_len <= n - buf_len) {
                std::copy_n(buf, buf_len, path + i1);
                return false;
            }
            auto * const buf_tail = std::copy(
                path + i1 + buf_len,
                path + n, 
                buf + buf_len
            );
            std::copy_n(path, i1, buf_tail);
            return true;
        }
        default:
            return false;
    }
}


// ============================================================
// Type aliases:

template<typename cost_t, IntrusiveVertex vertex_t>
using Cut3OptNo2Opt = Cut3Opt<cost_t, vertex_t, true>;


}  // namespace k_opt

#endif
