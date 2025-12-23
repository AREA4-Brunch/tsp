#ifndef TSP_K_OPT_PATH_ALGOS_HPP
#define TSP_K_OPT_PATH_ALGOS_HPP

#include "vertex_concept.hpp"

namespace k_opt {
namespace path_algos {

template<
    auto get_more_likely,
    auto get_less_likely,
    IntrusiveVertex vertex_t
>
inline typename vertex_t::traits::node_ptr get_neighbour(
    typename vertex_t::traits::const_node_ptr const ptr,
    typename vertex_t::traits::const_node_ptr const opp
) {
    using traits = typename vertex_t::traits;
    traits::const_node_ptr const cand1 = get_more_likely(ptr);
    return cand1 != opp ? cand1 : get_less_likely(ptr);
}

template<IntrusiveVertex v_t, bool is_next_hint = true>
inline typename v_t::traits::node_ptr get_neighbour(
    typename v_t::traits::const_node_ptr const p,
    typename v_t::traits::const_node_ptr const opp
) {
    using t = typename v_t::traits;
    if constexpr (is_next_hint) {
        return get_neighbour<t::get_next, t::get_prev, v_t>(p, opp);
    } else {
        return get_neighbour<t::get_prev, t::get_next, v_t>(p, opp);
    }
}

template<IntrusiveVertex v_t>
inline typename v_t::traits::node_ptr get_neighbour(
    typename v_t::traits::const_node_ptr const ptr,
    typename v_t::traits::const_node_ptr const opp,
    const bool is_next_hint
) {
    using t = typename v_t::traits;
    return is_next_hint
        ? get_neighbour<true, v_t>(ptr, opp)
        : get_neighbour<false, v_t>(ptr, opp);
}

template<
    auto get_more_likely,
    auto set_more_likely,
    auto set_less_likely,
    IntrusiveVertex vertex_t
>
inline void set_neighbour(
    typename vertex_t::traits::const_node_ptr const ptr,
    typename vertex_t::traits::const_node_ptr const cur_next,
    typename vertex_t::traits::const_node_ptr const new_next
) {
    if (get_more_likely(ptr) == cur_next) [[ likely ]] {
        set_more_likely(ptr, new_next);
    } else {
        set_less_likely(ptr, new_next);
    }
}

template<IntrusiveVertex v_t, bool is_next_hint = true>
inline void set_neighbour(
    typename v_t::traits::const_node_ptr const ptr,
    typename v_t::traits::const_node_ptr const cur_next,
    typename v_t::traits::const_node_ptr const new_next
) {
    using t = typename v_t::traits;
    if constexpr (is_next_hint) {
        set_neighbour<t::get_next, t::set_next, t::set_prev, v_t
        >(ptr, cur_next, new_next);
    } else {
        set_neighbour<t::get_prev, t::set_prev, t::set_next, v_t
        >(ptr, cur_next, new_next);
    }
}

template<IntrusiveVertex v_t>
inline void set_neighbour(
    typename v_t::traits::const_node_ptr const ptr,
    typename v_t::traits::const_node_ptr const cur_next,
    typename v_t::traits::const_node_ptr const new_next,
    const bool is_next_hint
) {
    return is_next_hint
        ? set_neighbour<true, v_t>(ptr, cur_next, new_next)
        : set_neighbour<false, v_t>(ptr, cur_next, new_next);
}

template<IntrusiveVertex v_t>
inline void reverse(
    typename v_t::traits::node_ptr before_start,
    typename v_t::traits::node_ptr start,
    typename v_t::traits::node_ptr end,
    typename v_t::traits::node_ptr after_end
) {
    set_neighbour<v_t, false>(before_start, start, end);
    set_neighbour<v_t, true>(end, after_end, before_start);
    set_neighbour<v_t, true>(start, before_start, after_end);
    set_neighbour<v_t, false>(after_end, end, start);
}

}  // namespace path_algos
}  // namespace k_opt

#endif
