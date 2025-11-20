#ifndef TSP_3_OPT_HPP
#define TSP_3_OPT_HPP

#include <vector>
#include <functional>
#include "history.hpp"

template<typename cost_t, typename vertex_t>
class LocalSearch3Opt {
 public:
    LocalSearch3Opt() = default;
    virtual ~LocalSearch3Opt() = default;

    /**
     * @param solution Initial tour which will be overriden
     *                 with best found tour. If cyclical should
     *                 not contain path[0] == path.back().
     */
    cost_t search(
        std::vector<std::vector<cost_t>> &weights,
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
        const std::vector<std::vector<cost_t>> &weights,
        const int verbose = 0
    ) const = 0;

    inline int selectCut(
        const int i,
        const int j,
        const int k,
        cost_t &change,
        const std::vector<vertex_t> &path,
        const std::vector<std::vector<cost_t>> &weights
    ) const;

    inline void applyCut(
        const int i,
        const int j,
        const int k,
        const int patch_ordinal,
        std::vector<vertex_t> &path
    ) const;

    cost_t calcCost(
        const std::vector<vertex_t> &solution,
        const bool is_searching_for_path,
        const std::vector<std::vector<cost_t>> &weights
    ) const;

    cost_t calcCutCost(
        const int v0,
        const int v1,
        const int v2,
        const int v3,
        const int v4,
        const int v5,
        const std::vector<std::vector<cost_t>> &weights
    ) const {
        const cost_t cut = weights[v0][v1]
                         + weights[v2][v3]
                         + weights[v4][v5];
        return cut;
    }

 private:

    static void removeArtificialVertex(
        std::vector<vertex_t> &solution
    );
};

template<typename cost_t, typename vertex_t>
cost_t LocalSearch3Opt<cost_t, vertex_t>::search(
    std::vector<std::vector<cost_t>> &weights,
    std::vector<vertex_t> &solution,
    const bool is_searching_for_path,
    History<cost_t> &history,
    const int verbose
) {
    if (is_searching_for_path) {
        // make path a cycle with artificial vertex
        solution.push_back(weights.size());
        for (auto &row : weights) row.push_back((cost_t) 0);
        weights.push_back(
            std::vector<cost_t>(weights.size() + 1, (cost_t) 0));
    }
    const cost_t init_cost = this->calcCost(
        solution, is_searching_for_path, weights
    );
    const cost_t best_cost = this->run(
        solution, init_cost, history, weights, verbose
    );
    if (is_searching_for_path) {
        for (auto &row : weights) row.pop_back();
        weights.pop_back();
        LocalSearch3Opt::removeArtificialVertex(solution);
    }
    return best_cost;
}

template<typename cost_t, typename vertex_t>
cost_t LocalSearch3Opt<cost_t, vertex_t>::calcCost(
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
int LocalSearch3Opt<cost_t, vertex_t>::selectCut(
    const int i,
    const int j,
    const int k,
    cost_t &change,
    const std::vector<vertex_t> &path,
    const std::vector<std::vector<cost_t>> &weights
) const {
    const int n = path.size();
    const int v0 = path[(k + 1) % n];
    const int v1 = path[i];
    const int v2 = path[i + 1];
    const int v3 = path[j];
    const int v4 = path[j + 1];
    const int v5 = path[k];
    //     0     1       2     3        4    5
    // [ k + 1 : i] + [ i + 1, j ] + [j + 1, k]
    const std::vector<cost_t> changes = {
        this->calcCutCost(v2, v0, v1, v4, v5, v3, weights),
        this->calcCutCost(v2, v0, v1, v5, v4, v3, weights),
        this->calcCutCost(v3, v0, v1, v4, v5, v2, weights),
        this->calcCutCost(v3, v0, v1, v5, v4, v2, weights),
        this->calcCutCost(v4, v0, v1, v2, v3, v5, weights),
        this->calcCutCost(v4, v0, v1, v3, v2, v5, weights),
        this->calcCutCost(v5, v0, v1, v3, v2, v4, weights)
        // this->calcCutCost(v5, v0, v1, v2, v3, v4, weights),
    };
    const auto min_change_it = std::min_element(
        changes.begin(), changes.end()
    );
    const cost_t cur_cut_cost = this->calcCutCost(
        v5, v0, v1, v2, v3, v4, weights
    );
    change = *min_change_it - cur_cut_cost;
    return min_change_it - changes.begin();
}

template<typename cost_t, typename vertex_t>
void LocalSearch3Opt<cost_t, vertex_t>::applyCut(
    const int i,
    const int j,
    const int k,
    const int patch_ordinal,
    std::vector<vertex_t> &path
) const {
    std::vector<int> new_path;
    new_path.reserve(path.size());
    // cut indices
    const int i0 = k + 1;
    const int i1 = i;
    const int i2 = i + 1;
    const int i3 = j;
    const int i4 = j + 1;
    const int i5 = k;
    // add part [k+1 : i]
    new_path.insert(new_path.end(), path.begin() + i0, path.end());
    new_path.insert(new_path.end(), path.begin(), path.begin() + i1 + 1);

    switch (patch_ordinal)
    {
    case 0:
        new_path.insert(new_path.end(), path.begin() + i4, path.begin() + i5 + 1);
        new_path.insert(new_path.end(), path.rend() - 1 - i3, path.rend() - 1 - i2 + 1);
        break;
    case 1:
        new_path.insert(new_path.end(), path.rend() - 1 - i5, path.rend() - 1 - i4 + 1);
        new_path.insert(new_path.end(), path.rend() - 1 - i3, path.rend() - 1 - i2 + 1);
        break;
    case 2:
        new_path.insert(new_path.end(), path.begin() + i4, path.begin() + i5 + 1);
        new_path.insert(new_path.end(), path.begin() + i2, path.begin() + i3 + 1);
        break;
    case 3:
        new_path.insert(new_path.end(), path.rend() - 1 - i5, path.rend() - 1 - i4 + 1);
        new_path.insert(new_path.end(), path.begin() + i2, path.begin() + i3 + 1);
        break;
    case 4:
        new_path.insert(new_path.end(), path.begin() + i2, path.begin() + i3 + 1);
        new_path.insert(new_path.end(), path.rend() - 1 - i5, path.rend() - 1 - i4 + 1);
        break;
    case 5:
        new_path.insert(new_path.end(), path.rend() - 1 - i3, path.rend() - 1 - i2 + 1);
        new_path.insert(new_path.end(), path.rend() - 1 - i5, path.rend() - 1 - i4 + 1);
        break;
    case 6:
        new_path.insert(new_path.end(), path.rend() - 1 - i3, path.rend() - 1 - i2 + 1);
        new_path.insert(new_path.end(), path.begin() + i4, path.begin() + i5 + 1);
        break;
    // case 7:
    //     new_path.insert(new_path.end(), path.begin() + i2, path.begin() + i3 + 1);
    //     new_path.insert(new_path.end(), path.begin() + i4, path.begin() + i5 + 1);
    //     break;
    default:
        break;
    }

    path = std::move(new_path);
}

template<typename cost_t, typename vertex_t>
void LocalSearch3Opt<cost_t, vertex_t>::removeArtificialVertex(
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

#endif
