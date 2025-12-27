#ifndef TSP_K_OPT_VERTEX_CONCEPT_HPP
#define TSP_K_OPT_VERTEX_CONCEPT_HPP

#include <concepts>
#include <type_traits>

namespace k_opt {

template<typename T>
concept IntrusiveVertex = requires {
    typename T::traits;
    typename T::traits::node_ptr;
    typename T::traits::const_node_ptr;
    { T::traits::get_next(std::declval<typename T::traits::const_node_ptr>()) }
        -> std::same_as<typename T::traits::node_ptr>;
    { T::traits::set_next(std::declval<typename T::traits::node_ptr>(),
                          std::declval<typename T::traits::node_ptr>()) }
        -> std::same_as<void>;
    { T::traits::get_previous(std::declval<typename T::traits::const_node_ptr>()) }
        -> std::same_as<typename T::traits::node_ptr>;
    { T::traits::set_previous(std::declval<typename T::traits::node_ptr>(),
                              std::declval<typename T::traits::node_ptr>()) }
        -> std::same_as<void>;
    { T::v(std::declval<typename T::traits::node_ptr>()) }
        -> std::same_as<T*>;
    { T::v(std::declval<typename T::traits::const_node_ptr>()) }
        -> std::same_as<const T*>;
    requires std::is_constructible_v<  // require a constructor(T id_t)
        T,
        const typename std::remove_cvref_t<
            decltype(std::declval<T>().id)>&
    >;
    { T::id };
};

}  // namespace k_opt


#endif
