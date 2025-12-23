#ifndef TSP_K_OPT_VERTEX_HPP
#define TSP_K_OPT_VERTEX_HPP

#include <boost/intrusive/list.hpp>

namespace k_opt {

/// @brief Implements k_opt::IntrusiveVertex concept.
template<typename id_t>
struct Vertex : public boost::intrusive::list_base_hook<> {
    using traits = boost::intrusive::list_node_traits<void*>;

    static inline Vertex* v(traits::node_ptr ptr) noexcept {
        return static_cast<Vertex*>(ptr);
    }

    static inline const Vertex* v(traits::const_node_ptr ptr) noexcept {
        return static_cast<const Vertex*>(ptr);
    }

    Vertex(const id_t id) : id(id) { }
    const id_t id;
};

}  // namespace k_opt

#endif
