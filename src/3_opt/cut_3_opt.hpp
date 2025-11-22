#ifndef TSP_3_OPT_CUT_HPP
#define TSP_3_OPT_CUT_HPP

#include <vector>


/// @brief Implements k_opt::CutStrategy concept avoiding virtual overhead.
template<typename cost_t, typename vertex_t>
class Cut3Opt {
 public:

    [[ gnu::always_inline ]]
    inline int selectCut(
        const int i,
        const int j,
        const int k,
        cost_t &change,
        const std::vector<vertex_t> &path,
        const std::vector<std::vector<cost_t>> &weights
    ) const;

    [[ gnu::always_inline ]]
    inline void applyCut(
        const int i,
        const int j,
        const int k,
        const int patch_ordinal,
        std::vector<vertex_t> &path
    ) const;

 protected:

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

};


template<typename cost_t, typename vertex_t>
int Cut3Opt<cost_t, vertex_t>::selectCut(
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
void Cut3Opt<cost_t, vertex_t>::applyCut(
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

#endif
