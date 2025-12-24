#ifndef TSP_K_OPT_VERTEX_HPP
#define TSP_K_OPT_VERTEX_HPP

#include <boost/intrusive/list.hpp>
#include <iostream>

namespace k_opt {

/// @brief Implements k_opt::IntrusiveVertex concept.
template<typename id_t>
struct Vertex
    : public boost::intrusive::list_base_hook<
        boost::intrusive::link_mode<
            boost::intrusive::normal_link
        >>
{
    using traits = boost::intrusive::list_node_traits<void*>;

    static inline Vertex* v(traits::node_ptr ptr) noexcept {
        return static_cast<Vertex*>(ptr);
    }

    static inline const Vertex* v(traits::const_node_ptr ptr) noexcept {
        return static_cast<const Vertex*>(ptr);
    }

    Vertex(const id_t id) : id(id) { }
    const id_t id;

    friend std::ostream& operator<<(std::ostream &os, const Vertex &vertex) {
        return os << vertex.id;
    }
};

}  // namespace k_opt

#endif
