#ifndef TSP_K_OPT_PATH_ALGOS_HPP
#define TSP_K_OPT_PATH_ALGOS_HPP

#include "vertex_concept.hpp"
#include <iostream>

namespace k_opt {
namespace path_algos {

template<IntrusiveVertex v_t, bool is_next_hint = true>
[[ gnu::always_inline, gnu::hot ]]
inline typename v_t::traits::node_ptr get_neighbour(
    typename v_t::traits::const_node_ptr const p,
    typename v_t::traits::const_node_ptr const opp
) noexcept {
    using t = typename v_t::traits;
    if constexpr (is_next_hint) {
        auto cand1 = t::get_next(p);
        return cand1 != opp ? cand1 : t::get_previous(p);
    } else {
        auto cand1 = t::get_previous(p);
        return cand1 != opp ? cand1 : t::get_next(p);
    }
}

template<IntrusiveVertex v_t>
[[ gnu::always_inline, gnu::hot ]]
inline typename v_t::traits::node_ptr get_neighbour(
    typename v_t::traits::const_node_ptr const ptr,
    typename v_t::traits::const_node_ptr const opp,
    const bool is_next_hint
) noexcept {
    return is_next_hint
        ? get_neighbour<true, v_t>(ptr, opp)
        : get_neighbour<false, v_t>(ptr, opp);
}

template<IntrusiveVertex v_t, bool is_next_hint = true>
[[ gnu::always_inline, gnu::hot ]]
inline void set_neighbour(
    typename v_t::traits::node_ptr const ptr,
    typename v_t::traits::const_node_ptr const cur_next,
    typename v_t::traits::node_ptr const new_next
) noexcept {
    using t = typename v_t::traits;
    if constexpr (is_next_hint) {
        if (t::get_next(ptr) == cur_next) [[ likely ]] {
            t::set_next(ptr, new_next);
        } else {
            t::set_previous(ptr, new_next);
        }
    } else {
        if (t::get_previous(ptr) == cur_next) [[ likely ]] {
            t::set_previous(ptr, new_next);
        } else {
            t::set_next(ptr, new_next);
        }
    }
}

template<IntrusiveVertex v_t>
[[ gnu::always_inline, gnu::hot ]]
inline void set_neighbour(
    typename v_t::traits::node_ptr const ptr,
    typename v_t::traits::const_node_ptr const cur_next,
    typename v_t::traits::node_ptr const new_next,
    const bool is_next_hint
) noexcept {
    return is_next_hint
        ? set_neighbour<true, v_t>(ptr, cur_next, new_next)
        : set_neighbour<false, v_t>(ptr, cur_next, new_next);
}

template<IntrusiveVertex v_t>
[[ gnu::always_inline, gnu::hot ]]
inline void reverse(
    typename v_t::traits::node_ptr before_start,
    typename v_t::traits::node_ptr start,
    typename v_t::traits::node_ptr end,
    typename v_t::traits::node_ptr after_end
) noexcept {
    if (start == end) [[ unlikely ]] return;
    set_neighbour<v_t, false>(before_start, start, end);
    set_neighbour<v_t, true>(end, after_end, before_start);
    set_neighbour<v_t, true>(start, before_start, after_end);
    set_neighbour<v_t, false>(after_end, end, start);
}

// template<IntrusiveVertex v_t>
// inline void swap_sequential_segs(
//     typename v_t::traits::node_ptr prev_s1,
//     typename v_t::traits::node_ptr s1_start,
//     typename v_t::traits::node_ptr s1_end,
//     typename v_t::traits::node_ptr s2_start,
//     typename v_t::traits::node_ptr s2_end,
//     typename v_t::traits::node_ptr after_s2
// ) noexcept {
//     set_neighbour<v_t, true>(prev_s1, s1_start, s2_start);
//     if (s2_start != s2_end) [[ likely ]] {
//         set_neighbour<v_t, false>(s2_start, s1_end, prev_s1);
//         set_neighbour<v_t, false>(s2_end, after_s2, s1_start);
//     } else [[ unlikely ]] {
//         v_t::traits::set_previous(s2_start, prev_s1);
//         v_t::traits::set_next(s2_start, s1_start);
//     }
//     if (s1_start != s1_end) [[ likely ]] {
//         set_neighbour<v_t, false>(s1_start, prev_s1, s2_end);
//         set_neighbour<v_t, false>(s1_end, s2_start, after_s2);
//     } else [[ unlikely ]] {
//         v_t::traits::set_previous(s1_start, s2_end);
//         v_t::traits::set_next(s1_start, after_s2);
//     }
//     set_neighbour<v_t, false>(after_s2, s2_end, s1_end);
// }

template<IntrusiveVertex v_t>
[[ gnu::always_inline, gnu::hot ]]
inline void swap_sequential_segs(
    typename v_t::traits::node_ptr prev_s1,
    typename v_t::traits::node_ptr s1_start,
    typename v_t::traits::node_ptr s1_end,
    typename v_t::traits::node_ptr s2_start,
    typename v_t::traits::node_ptr s2_end,
    typename v_t::traits::node_ptr after_s2
) noexcept {
    set_neighbour<v_t, true>(prev_s1, s1_start, s2_start);
    set_neighbour<v_t, false>(s2_start, s1_end, prev_s1);
    set_neighbour<v_t, false>(s2_end, after_s2, s1_start);
    set_neighbour<v_t, false>(s1_start, prev_s1, s2_end);
    set_neighbour<v_t, false>(s1_end, s2_start, after_s2);
    set_neighbour<v_t, true>(after_s2, s2_end, s1_end);
}

template<IntrusiveVertex v_t>
inline void correct_order(
    typename v_t::traits::node_ptr cur,
    typename v_t::traits::node_ptr end
) noexcept {
    auto prev = end;
    do {
        auto next = get_neighbour<v_t>(cur, prev);
        v_t::traits::set_previous(cur, prev);
        v_t::traits::set_next(cur, next);
        prev = cur;
        cur = next;
    } while (prev != end);
}


}  // namespace path_algos


namespace logging {

template<k_opt::IntrusiveVertex v_t>
std::ostream& logList(
    std::ostream &os,
    typename v_t::traits::const_node_ptr start
) noexcept {
    auto cur = start;
    do {
        os << *(v_t::v(cur)) << " ";
        cur = v_t::traits::get_next(cur);
    } while (cur != start);
    return os;
}

}  // namespace logging

}  // namespace k_opt



#endif
