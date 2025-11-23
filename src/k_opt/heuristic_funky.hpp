#ifndef TSP_K_OPT_HEURISTIC_FUNKY_HPP
#define TSP_K_OPT_HEURISTIC_FUNKY_HPP

#include <iostream>
#include <vector>
#include <utility>
#include "heuristic.hpp"
#include "cut_strategy.hpp"

namespace k_opt {

template<
    typename cost_t,
    typename cut_strategy_t,
    typename vertex_t=int
>
requires k_opt::CutStrategy<cut_strategy_t, cost_t, vertex_t>
class HeuristicFunky
    : public k_opt::Heuristic<cost_t, vertex_t>
{
 public:

    HeuristicFunky(cut_strategy_t cut_strategy)
        : cut(cut_strategy) { }

    ~HeuristicFunky() = default;

 protected:

    const cut_strategy_t cut;

    cost_t run(
        std::vector<vertex_t> &solution,
        cost_t cur_cost,
        History<cost_t> &history,
        const std::vector<std::vector<cost_t>> &weights,
        const int verbose = 0
    ) const override;
};

}  // namespace k_opt


template<typename cost_t, typename cut_strategy_t, typename vertex_t>
requires k_opt::CutStrategy<cut_strategy_t, cost_t, vertex_t>
cost_t k_opt::HeuristicFunky<cost_t, cut_strategy_t, vertex_t>::run(
    std::vector<vertex_t> &path,
    cost_t cur_cost,
    History<cost_t> &history,
    const std::vector<std::vector<cost_t>> &weights,
    const int verbose
) const {
    const int n = path.size();
    const int log_freq = 1;  // log best cost every 1 iters
    const int flush_freq = 10000;  // flush every 10000 costs
    const bool do_record_history = !history.isStopped();
    history.addCost(cur_cost);
    history.addPath(path, -1, -1, -1, 0);
    cost_t cur_cost_change = (cost_t) 0;
    int iter = 1;
    for (bool did_update = true; did_update; ++iter) {
        if (verbose > 0 && (iter < 10 || iter % log_freq == 0)) {
            std::cout << "ITERATION " << iter << ": "
                      << cur_cost << std::endl;
        }
        did_update = false;
        for (int i = 0; i < n - 2; ++i) {
            for (int j = i + 1; j < n - 1; ++j) {
                for (int k = j + 1; k < n; ++k) {
                    const int patch_ordinal = this->cut.selectCut(
                        i, j, k, cur_cost_change, path, weights
                    );
                    // add slight amount to negative side when comparing
                    // the change to avoid swaps of the same element when
                    // j=i+1 or k=j+1 or both in patch_ordinals: 6, 4, 5
                    if (cur_cost_change < -1e-11) {
                        did_update = true;
                        this->cut.applyCut(i, j, k, patch_ordinal, path);
                        cur_cost += cur_cost_change;
                        history.addCost(cur_cost);
                        history.addPath(path, i, j, k, iter);
                    }
                }
            }
        }
        // store history on flush_freq, or on last iter
        if (do_record_history) {
            if (!did_update || history.size() % flush_freq == 0) {
                // history.flush(iter > 1);
                history.flush(true);
            }
        }
    }
    if (verbose > 0) {  // log cost after last iteration
        std::cout << "Last ITERATION " << iter << ": "
                  << cur_cost << std::endl;
    }
    return cur_cost;
}

#endif
