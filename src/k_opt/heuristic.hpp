#ifndef TSP_K_OPT_HEURISTIC_HPP
#define TSP_K_OPT_HEURISTIC_HPP

#include <vector>
#include <functional>
#include "history.hpp"
#include "../common/random.hpp"

namespace k_opt {

template<typename cost_t, typename vertex_t>
class Heuristic {
 public:

    virtual ~Heuristic() = default;

    /**
     * @param solution Initial tour which will be overriden
     *                 with best found tour. If cyclical should
     *                 not contain path[0] == path.back().
     *                 If provided tour is empty then a random
     *                 tour will be generated.
     */
    cost_t search(
        const std::vector<std::vector<cost_t>> &weights,
        std::vector<vertex_t> &solution,
        const bool is_searching_for_path,
        History<cost_t> &history,
        const int verbose = 0
    );

 protected:

    virtual cost_t run(
        std::vector<vertex_t> &solution,
        cost_t cur_cost,
        History<cost_t> &history,
        const cost_t * __restrict const flat_weights,
        const int verbose = 0
    ) const noexcept = 0;

    virtual void selectInitSolution(
        std::vector<vertex_t> &solution,
        const std::vector<std::vector<cost_t>> &weights
    );

    cost_t calcCost(
        const std::vector<vertex_t> &solution,
        const bool is_searching_for_path,
        const std::vector<std::vector<cost_t>> &weights
    ) const;

 private:

    static void removeArtificialVertex(
        std::vector<vertex_t> &solution
    );

    static std::vector<cost_t> genFlatMatrix(
        const std::vector<std::vector<cost_t>> &weights,
        const bool add_artificial_vertex = false
    );
};


template<typename cost_t, typename vertex_t>
cost_t Heuristic<cost_t, vertex_t>::search(
    const std::vector<std::vector<cost_t>> &weights,
    std::vector<vertex_t> &solution,
    const bool is_searching_for_path,
    History<cost_t> &history,
    const int verbose
) {
    if (solution.empty()) {  // start from random solution
        this->selectInitSolution(solution, weights);
    } else if (solution.size() != weights.size()) {
        throw std::runtime_error(
            "Heuristic::search: solution.size() != 0"
            " && solution.size() != weight.size()"
        );
    }
    if (is_searching_for_path) {
        // make path a cycle with artificial vertex
        solution.push_back(weights.size());
    }
    const cost_t init_cost = this->calcCost(
        solution, is_searching_for_path, weights
    );
    const auto flat_weights = genFlatMatrix(
        weights,
        is_searching_for_path
    );
    const cost_t best_cost = this->run(
        solution, init_cost, history,
        flat_weights.data(),
        verbose
    );
    if (is_searching_for_path) {
       removeArtificialVertex(solution);
    }
    return best_cost;
}

template<typename cost_t, typename vertex_t>
void Heuristic<cost_t, vertex_t>::selectInitSolution(
    std::vector<vertex_t> &solution,
    const std::vector<std::vector<cost_t>> &weights
) {
    solution.resize(weights.size());
    std::iota(solution.begin(), solution.end(), 0);
    std::mt19937 psrng = random::initPSRNG();
    random::permuteRandomly(solution, psrng);
}

template<typename cost_t, typename vertex_t>
cost_t Heuristic<cost_t, vertex_t>::calcCost(
    const std::vector<vertex_t> &path,
    const bool is_searching_for_path,
    const std::vector<std::vector<cost_t>> &weights
) const {
    cost_t cost = is_searching_for_path
                ? (cost_t) 0
                : weights[path.back()][path[0]];
    for (int i = 0, n = path.size(); i < n - 1; ++i) {
        const int src = path[i];
        const int dst = path[i + 1];
        cost += weights[src][dst];
    }
    return cost;
}

template<typename cost_t, typename vertex_t>
void Heuristic<cost_t, vertex_t>::removeArtificialVertex(
    std::vector<vertex_t> &path
) {
    const auto artificial_it = std::find(
        path.begin(), path.end(),
        static_cast<vertex_t>(path.size() - 1)
    );
    std::vector<vertex_t> new_path;
    new_path.reserve(path.size() - 1);
    new_path.insert(new_path.end(), artificial_it + 1, path.end());
    new_path.insert(new_path.end(), path.begin(), artificial_it);
    path = std::move(new_path);
}

template<typename cost_t, typename vertex_t>
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


}  // namespace k_opt

#endif
