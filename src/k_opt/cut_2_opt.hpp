#ifndef TSP_2_OPT_CUT_HPP
#define TSP_2_OPT_CUT_HPP

#include <vector>
#include <array>
#include "vertex_concept.hpp"
#include "path_algos.hpp"

namespace k_opt {

/// @brief Implements k_opt::CutStrategy concept avoiding virtual overhead.
template<typename cost_t, IntrusiveVertex vertex_t>
class Cut2Opt {
    static_assert(std::is_arithmetic_v<cost_t>, "cost_t must be arithmetic");

    using seg_t = std::pair<
        typename vertex_t::traits::node_ptr,
        typename vertex_t::traits::node_ptr
    >;

 public:

    static constexpr int NUM_CUTS = 2;

    Cut2Opt() = default;
    ~Cut2Opt() = default;

    template<bool can_modify_segs>
    [[ gnu::always_inline, gnu::hot ]]
    inline int selectCut(
        const int n,
        const seg_t * __restrict const segs,
        cost_t &change,
        const cost_t * __restrict const weights,
        int &perm_idx,
        [[ maybe_unused ]] const seg_t * __restrict const
    ) const noexcept;

    [[ gnu::always_inline, gnu::hot ]]
    inline void applyCut(
        const seg_t * __restrict const segs,
        [[ maybe_unused ]] const int move_ord = 1,
        [[ maybe_unused ]] const int swap_mask = -1,
        [[ maybe_unused ]] const seg_t * __restrict const orig_segs = nullptr
    ) const noexcept;
};


template<typename cost_t, IntrusiveVertex vertex_t>
template<bool can_modify_segs>
int Cut2Opt<cost_t, vertex_t>::selectCut(
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
    const auto d = vertex_t::v(segs[0].first)->id;

    const cost_t* __restrict wa = weights + a * n;
    const cost_t* __restrict wb = weights + b * n;
    const cost_t* __restrict wc = weights + c * n;

    change = wa[b] + wc[d];
    if (wa[c] + wb[d] < change) {  // reverse segs[1]
        change = wa[c] + wb[d] - change;
        perm_idx = 1;
    }
    return 0;
}

template<typename cost_t, IntrusiveVertex v_t>
void Cut2Opt<cost_t, v_t>::applyCut(
    const seg_t * __restrict const segs,
    [[ maybe_unused ]] const int,
    [[ maybe_unused ]] const int,
    [[ maybe_unused ]] const seg_t * __restrict const
) const noexcept {
    k_opt::path_algos::reverse<v_t>(
        segs[0].second,
        segs[1].first, segs[1].second,
        segs[0].first
    );
}

}  // namespace k_opt

#endif
