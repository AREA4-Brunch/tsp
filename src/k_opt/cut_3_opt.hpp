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
    inline void applyCut(
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
    const auto a = vertex_t::v(segs[0].second)->id;
    const auto b = vertex_t::v(segs[1].first)->id;
    const auto c = vertex_t::v(segs[1].second)->id;
    const auto d = vertex_t::v(segs[2].first)->id;
    const auto e = vertex_t::v(segs[2].second)->id;
    const auto f = vertex_t::v(segs[0].first)->id;

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

template<typename cost_t, IntrusiveVertex v_t, bool no_2_opt>
void Cut3Opt<cost_t, v_t, no_2_opt>::applyCut(
    const seg_t * __restrict const segs,
    const int move_ord,
    [[ maybe_unused ]] const int swap_mask,
    [[ maybe_unused ]] const int n
) const noexcept {
    switch (move_ord) {
        case 0b010: {
            k_opt::path_algos::reverse<v_t>(
                segs[0].second,
                segs[1].first, segs[1].second,
                segs[2].first
            );
            return;
        }
        case 0b100: {
            k_opt::path_algos::reverse<v_t>(
                segs[1].second,
                segs[2].first, segs[2].second,
                segs[0].first
            );
            return;
        }
        case 0b110: {
            k_opt::path_algos::reverse<v_t>(
                segs[0].second,
                segs[1].first, segs[1].second,
                segs[2].first
            );
            k_opt::path_algos::reverse<v_t>(
                segs[1].first,
                segs[2].first, segs[2].second,
                segs[0].first
            );
            return;
        }
        case 0b001:
        case 0b011:
        case 0b101:
        case 0b111: {
            bool DEBUG = false;
            // s0f s0s   s1f s1s s2f s2s
            if (move_ord & 0b100) {
                k_opt::path_algos::reverse<v_t>(
                    segs[1].second,
                    segs[2].first, segs[2].second,
                    segs[0].first
                );
                
                if (DEBUG) {
                    std::cout << "After swap 0b100: " << std::endl;
                    for (int s = 0; s < 3; ++s) {
                        std::cout << " Seg " << s << ": ";
                        auto cur = segs[s].first;
                        std::cout << "first"
                            << " v: " << v_t::v(cur)->id << " "
                            << " ptr: " << cur
                            << " prev: " << v_t::traits::get_previous(cur)
                            << " next: " << v_t::traits::get_next(cur)
                            << std::endl;
                        cur = segs[s].second;
                        std::cout << "\t\tsecond"
                            << " v: " << v_t::v(cur)->id << " "
                            << " ptr: " << cur
                            << " prev: " << v_t::traits::get_previous(cur)
                            << " next: " << v_t::traits::get_next(cur)
                            << std::endl;
                    }
                }
            }
            if (move_ord & 0b010) {
                k_opt::path_algos::reverse<v_t>(
                    segs[0].second,
                    segs[1].first, segs[1].second,
                    move_ord & 0b100
                        ? segs[2].second
                        : segs[2].first
                );
                if (DEBUG) {
                    std::cout << "After swap 0b010: " << std::endl;
                    for (int s = 0; s < 3; ++s) {
                        std::cout << " Seg " << s << ": ";
                        auto cur = segs[s].first;
                        std::cout << "first"
                            << " v: " << v_t::v(cur)->id << " "
                            << " ptr: " << cur
                            << " prev: " << v_t::traits::get_previous(cur)
                            << " next: " << v_t::traits::get_next(cur)
                            << std::endl;
                        cur = segs[s].second;
                        std::cout << "\t\tsecond"
                            << " v: " << v_t::v(cur)->id << " "
                            << " ptr: " << cur
                            << " prev: " << v_t::traits::get_previous(cur)
                            << " next: " << v_t::traits::get_next(cur)
                            << std::endl;
                    }
                }
            }
            k_opt::path_algos::swap_sequential_segs<v_t>(
                segs[0].second,
                move_ord & 0b010
                    ? segs[1].second
                    : segs[1].first,
                move_ord & 0b010
                    ? segs[1].first
                    : segs[1].second,
                move_ord & 0b100
                    ? segs[2].second
                    : segs[2].first,
                move_ord & 0b100
                    ? segs[2].first
                    : segs[2].second,
                segs[0].first
            );
            return;
        }
        default:
            return;
    }
}


// ============================================================
// Type aliases:

template<typename cost_t, IntrusiveVertex vertex_t>
using Cut3OptNo2Opt = Cut3Opt<cost_t, vertex_t, true>;


}  // namespace k_opt

#endif
