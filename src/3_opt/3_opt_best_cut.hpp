#ifndef TSP_3_OPT_BEST_CUT_HPP
#define TSP_3_OPT_BEST_CUT_HPP

#include <iostream>
#include <vector>
#include <utility>
#include "3_opt.hpp"

template<typename cost_t, typename vertex_t=int>
class LocalSearch3OptBestCut : public LocalSearch3Opt<cost_t, vertex_t> {
 public:
    LocalSearch3OptBestCut() = default;
    ~LocalSearch3OptBestCut() = default;

 protected:

    cost_t run(
        std::vector<vertex_t> &solution,
        cost_t cur_cost,
        History<cost_t> &history,
        const int verbose = 0
    ) const override;

};

template<typename cost_t, typename vertex_t>
cost_t LocalSearch3OptBestCut<cost_t, vertex_t>::run(
    std::vector<vertex_t> &path,
    cost_t cur_cost,
    History<cost_t> &history,
    const int verbose
) const {
    const int n = path.size();
    const int log_freq = 50;  // log best cost every 50 iters
    const int flush_freq = 10000;  // flush every 10000 costs
    const bool do_record_history = !history.isStopped();
    if (do_record_history) history.addCost(cur_cost);
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
                    const int patch_ordinal = this->selectCut(
                        i, j, k, cur_cost_change, path
                    );
                    if (cur_cost + cur_cost_change < best_cost) {
                        did_update = true;
                        best_cost = cur_cost + cur_cost_change;
                        best_cut_desc = { i, j, k, patch_ordinal };
                        if (do_record_history) {
                            history.addCost(cur_cost + cur_cost_change);
                        }
                    }
                }
            }
        }
        if (did_update) {  // update path to new best
            cur_cost = best_cost;
            this->applyCut(
                best_cut_desc[0],  // i
                best_cut_desc[1],  // j
                best_cut_desc[2],  // k
                best_cut_desc[3],   // patch ordinal
                path
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
