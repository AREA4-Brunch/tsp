#ifndef TSP_K_OPT_HEURISTIC_CLASSICAL_HPP
#define TSP_K_OPT_HEURISTIC_CLASSICAL_HPP

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
class HeuristicClassical
    : public k_opt::Heuristic<cost_t, vertex_t>
{
 public:

    HeuristicClassical(cut_strategy_t cut_strategy)
        : cut(cut_strategy) { }

    ~HeuristicClassical() = default;

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
cost_t k_opt::HeuristicClassical<cost_t, cut_strategy_t, vertex_t>::run(
    std::vector<vertex_t> &path,
    cost_t cur_cost,
    History<cost_t> &history,
    const std::vector<std::vector<cost_t>> &weights,
    const int verbose
) const {
    const int n = path.size();
    const int log_freq = 10;  // log best cost every 1 iters
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
        for (int i = 0; i < n - 3 && !did_update; ++i) {
            for (int j = i + 1; j < n - 2 && !did_update; ++j) {
                for (int k = j + 2; k < n; ++k) {
                    std::vector<std::pair<int, int>> segs = {
                        { k, i }, { i + 1, j }, { j + 1, k - 1 }
                    };
                    this->cut.selectCut(
                        path, segs, cur_cost_change, weights
                    );
                    // add slight amount to negative side when comparing
                    // the change to avoid swaps of the same element
                    if (cur_cost_change < -1e-10) {
                        did_update = true;
                        this->cut.applyCut(path, segs);
                        cur_cost += cur_cost_change;
                        history.addCost(cur_cost);
                        history.addPath(path, i, j, k, iter);
                        break;
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
        std::cout << std::fixed << std::setprecision(6)
                  << "Last ITERATION " << iter << ": "
                  << cur_cost << std::defaultfloat
                  << std::endl;
    }
    return cur_cost;
}

#endif
