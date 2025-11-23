#ifndef TSP_K_OPT_HEURISTIC_BEST_CUT_HPP
#define TSP_K_OPT_HEURISTIC_BEST_CUT_HPP

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
class HeuristicBestCut
    : public k_opt::Heuristic<cost_t, vertex_t>
{
 public:

    HeuristicBestCut(cut_strategy_t cut_strategy)
        : cut(cut_strategy) { }

    ~HeuristicBestCut() = default;

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
cost_t k_opt::HeuristicBestCut<cost_t, cut_strategy_t, vertex_t>::run(
    std::vector<vertex_t> &path,
    cost_t cur_cost,
    History<cost_t> &history,
    const std::vector<std::vector<cost_t>> &weights,
    const int verbose
) const {
    const int n = path.size();
    const int log_freq = 50;  // log best cost every 50 iters
    const int flush_freq = 10000;  // flush every 10000 costs
    const bool do_record_history = !history.isStopped();
    history.addCost(cur_cost);
    history.addPath(path, -1, -1, -1, 0);
    int iter = 1;
    cost_t cur_cost_change = (cost_t) 0;
    cost_t best_cost = cur_cost;
    // i, j, k, patch_ord
    std::vector<int> best_cut_desc = { -1, -1, -1, -1 };
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
                    if (cur_cost + cur_cost_change < best_cost) {
                        did_update = true;
                        best_cost = cur_cost + cur_cost_change;
                        best_cut_desc = { i, j, k, patch_ordinal };
                    }
                }
            }
        }
        if (did_update) {  // update path to new best
            cur_cost = best_cost;
            this->cut.applyCut(
                best_cut_desc[0],  // i
                best_cut_desc[1],  // j
                best_cut_desc[2],  // k
                best_cut_desc[3],   // patch ordinal
                path
            );
            history.addCost(cur_cost);
            history.addPath(
                path, best_cut_desc[0], best_cut_desc[1],
                best_cut_desc[2], iter
            );
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
