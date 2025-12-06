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
    cost_t cur_cost_change = (cost_t) 0;
    cost_t best_cost = cur_cost;
    std::vector<std::pair<int, int>> best_segs(2 * 3);
    int best_i = 0, best_j = 0, best_k = 0;
    int iter = 1;
    for (bool did_update = true; did_update; ++iter) {
        if (verbose > 0 && (iter < 10 || iter % log_freq == 0)) {
            std::cout << "ITERATION " << iter << ": "
                      << cur_cost << std::endl;
        }
        did_update = false;
        for (int i = 0; i < n - 3; ++i) {
            for (int j = i + 1; j < n - 2; ++j) {
                for (int k = j + 2; k < n; ++k) {
                    std::vector<std::pair<int, int>> segs = {
                        { k, i }, { i + 1, j }, { j + 1, k - 1 }
                    };
                    this->cut.selectCut(
                        path, segs, cur_cost_change, weights
                    );
                    if (cur_cost + cur_cost_change + 1e-10 < best_cost) {
                        did_update = true;
                        best_cost = cur_cost + cur_cost_change;
                        std::swap(best_segs, segs);
                        best_i = i; best_j = j; best_k = k;
                    }
                }
            }
        }
        if (did_update) {  // update path to new best
            cur_cost = best_cost;
            this->cut.applyCut(path, best_segs);
            history.addCost(cur_cost);
            history.addPath(path, best_i, best_j, best_k, iter);
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
        std::cout << std::fixed << std::setprecision(6)
                  << "Last ITERATION " << iter << ": "
                  << cur_cost << std::defaultfloat
                  << std::endl;
    }
    return cur_cost;
}

#endif
