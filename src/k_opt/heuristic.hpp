#ifndef TSP_K_OPT_HEURISTIC_HPP
#define TSP_K_OPT_HEURISTIC_HPP

#include <vector>
#include <functional>
#include <boost/intrusive/circular_list_algorithms.hpp>
#include "history.hpp"
#include "vertex_concept.hpp"
#include "path_algos.hpp"
#include "../common/random.hpp"

namespace k_opt {

template<typename cost_t, IntrusiveVertex vertex_t>
class Heuristic {
 public:

    using path_algos = boost::intrusive::circular_list_algorithms<
        typename vertex_t::traits>;

    Heuristic() = default;
    virtual ~Heuristic() = default;

    /**
     * @param solution Initial tour which will be overriden
     *                 with best found tour. If cyclical should
     *                 not contain path.front() == path.back().
     *                 If provided tour is empty then a random
     *                 tour will be generated.
     *                 In the end will be returned as cyclical.
     * @param path Will be set to head in cyclical linked list.
     * @param seed Seed for random number generator if solution.empty().
     */
    cost_t search(
        typename vertex_t::traits::node_ptr &path,
        std::vector<vertex_t> &solution,
        const std::vector<std::vector<cost_t>> &weights,
        const bool is_searching_for_path,
        History<cost_t> &history,
        const unsigned int seed = 0U,
        const int verbose = 0
    );

    virtual cost_t run(
        typename vertex_t::traits::node_ptr solution,
        cost_t cur_cost,
        History<cost_t> &history,
        const cost_t * __restrict const flat_weights,
        const int n,
        const int verbose = 0
    ) const noexcept = 0;

 protected:

    virtual typename vertex_t::traits::node_ptr
    createInitSolution(
        std::vector<vertex_t> &solution,
        const int n,
        const unsigned int seed = 0U
    ) const;

    cost_t calcCost(
        typename vertex_t::traits::const_node_ptr path,
        const cost_t * __restrict const weights,
        const int n
    ) const;

 private:

    static void removeArtificialVertex(
        typename vertex_t::traits::node_ptr &path,
        const int n
    );

    static std::vector<cost_t> genFlatMatrix(
        const std::vector<std::vector<cost_t>> &weights,
        const bool add_artificial_vertex = false
    );

    static typename vertex_t::traits::node_ptr toLinkedList(
        std::vector<vertex_t> &vec
    );

};

template<typename cost_t, IntrusiveVertex vertex_t>
cost_t Heuristic<cost_t, vertex_t>::search(
    typename vertex_t::traits::node_ptr &path,
    std::vector<vertex_t> &solution,
    const std::vector<std::vector<cost_t>> &weights,
    const bool is_searching_for_path,
    History<cost_t> &history,
    const unsigned int seed,
    const int verbose
) {
    const int n = weights.size() + is_searching_for_path;
    if (solution.empty()) {  // start from random solution
        path = this->createInitSolution(solution, n, seed);
    } else if (solution.size() != weights.size()) {
        throw std::runtime_error(
            "Heuristic::search: solution.size() != 0"
            " && solution.size() != weight.size()"
        );
    } else {
        // make path a cycle with artificial vertex
        if (is_searching_for_path) {
            solution.push_back(
                static_cast<vertex_t>(weights.size()));
        }
        path = this->toLinkedList(solution);
    }
    const auto flat_weights = genFlatMatrix(
        weights,
        is_searching_for_path
    );
    const cost_t init_cost = this->calcCost(
        path, flat_weights.data(), n
    );
    const cost_t best_cost = this->run(
        path, init_cost, history,
        flat_weights.data(), n,
        verbose
    );
    k_opt::path_algos::correct_order<vertex_t>(
        path,
        vertex_t::traits::get_previous(path)
    );
    if (is_searching_for_path) {
       removeArtificialVertex(path, n);
    }
    return best_cost;
}

template<typename cost_t, IntrusiveVertex vertex_t>
typename vertex_t::traits::node_ptr
Heuristic<cost_t, vertex_t>::createInitSolution(
    std::vector<vertex_t> &solution,
    const int n,
    const unsigned int seed
) const {
    solution.reserve(n);
    for (int i = 0; i < n; ++i) {
        solution.push_back(static_cast<vertex_t>(i));
    }
    auto path = toLinkedList(solution);
    auto psrng = random::initPSRNG(seed);
    random::permuteRandomly(solution, psrng,
        [] (vertex_t &x, vertex_t &y) {
            path_algos::swap_nodes(&x, &y);
    });
    return path;
}

template<typename cost_t, IntrusiveVertex vertex_t>
cost_t Heuristic<cost_t, vertex_t>::calcCost(
    typename vertex_t::traits::const_node_ptr path,
    const cost_t * __restrict const weights,
    const int n
) const {
    cost_t cost = static_cast<cost_t>(0);
    for (int i = n; i > 0; --i) {
        const auto src = vertex_t::v(path)->id;
        path = vertex_t::traits::get_next(path);
        const auto dst = vertex_t::v(path)->id;
        cost += weights[src * n + dst];
    }
    return cost;
}

template<typename cost_t, IntrusiveVertex vertex_t>
void Heuristic<cost_t, vertex_t>::removeArtificialVertex(
    typename vertex_t::traits::node_ptr &path,
    const int n
) {
    auto prev = path, next = path;
    for (int i = n - 2; i >= 0; --i) {
        next = vertex_t::traits::get_next(prev);
        if (vertex_t::v(next)->id == n - 1) {
            path = vertex_t::traits::get_next(next);
            path_algos::unlink(next);
            return;
        }
        prev = next;
    }
}

template<typename cost_t, IntrusiveVertex vertex_t>
std::vector<cost_t> Heuristic<cost_t, vertex_t>::genFlatMatrix(
    const std::vector<std::vector<cost_t>> &weights,
    const bool add_artificial_vertex
) {
    const int m = weights.size();
    const int n = m + add_artificial_vertex;
    std::vector<cost_t> flat_weights(n * n);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            flat_weights[i * n + j] = weights[i][j];
        }
        if (add_artificial_vertex) {
            flat_weights[i * n + m] = (cost_t) 0;
            flat_weights[m * n + i] = (cost_t) 0;
        }
    }
    if (add_artificial_vertex) {
        flat_weights.back() = (cost_t) 0;
    }
    return flat_weights;
}

template<typename cost_t, IntrusiveVertex vertex_t>
typename vertex_t::traits::node_ptr
Heuristic<cost_t, vertex_t>::toLinkedList(
    std::vector<vertex_t> &vec
) {
    typename vertex_t::traits::node_ptr head = &vec.front();
    path_algos::init_header(head);
    for (int i = 1, n = vec.size(); i < n; ++i) {
        path_algos::link_after(&vec[i - 1], &vec[i]);
    }
    return head;
}

namespace detail {

template<int K, int Depth, IntrusiveVertex vertex_t, typename callback_t>
[[ gnu::hot ]]
inline bool loopSegmentsStatic(
    typename vertex_t::traits::node_ptr prev,
    typename vertex_t::traits::node_ptr cur,
    const int start,
    const int n,
    std::pair<
        typename vertex_t::traits::node_ptr,
        typename vertex_t::traits::node_ptr
    > * __restrict const segs,
    callback_t &&cb
) noexcept {
    if constexpr (Depth != 0) {
        segs[Depth].first = cur;
    }
    const int lim = n - K + Depth;
    for (int i = start; i <= lim; ++i) {
        segs[Depth].second = cur;
        cur = k_opt::path_algos::get_neighbour<vertex_t>(cur, prev);
        prev = segs[Depth].second;
        if constexpr (Depth == K - 1) {
            segs[0].first = cur;
            if (cb()) [[ unlikely ]] return true;
        } else {
            if (loopSegmentsStatic<K, Depth + 1, vertex_t>(
                prev, cur, i + 1, n, segs,
                std::forward<callback_t>(cb)
            )) [[ unlikely ]] return true;
        }
    }
    return false;
}

template<IntrusiveVertex vertex_t, typename callback_t>
[[ gnu::hot ]]
inline bool loopSegmentsDynamic(
    typename vertex_t::traits::node_ptr prev,
    typename vertex_t::traits::node_ptr cur,
    const int start,
    const int depth,
    const int k,
    const int n,
    std::pair<
        typename vertex_t::traits::node_ptr,
        typename vertex_t::traits::node_ptr
    > * __restrict const segs,
    callback_t &&cb
) noexcept {
    segs[depth].first = cur;
    const int lim = n - k + depth;
    for (int i = start; i <= lim; ++i) {
        segs[depth].second = cur;
        cur = k_opt::path_algos::get_neighbour<vertex_t>(cur, prev);
        prev = segs[depth].second;
        if (depth == k - 1) [[ likely ]] {
            segs[0].first = cur;
            if (cb()) [[ unlikely ]] return true;
        } else {
            if (loopSegmentsDynamic<vertex_t>(
                prev, cur, i + 1, depth + 1, k, n, segs,
                std::forward<callback_t>(cb)
            )) [[ unlikely ]] return true;
        }
    }
    return false;
}

}  // namespace detail

}  // namespace k_opt

#endif
