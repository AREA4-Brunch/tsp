#ifndef TSP_3_OPT_FUNKY_HPP
#define TSP_3_OPT_FUNKY_HPP

#include <iostream>
#include <vector>
#include <utility>
#include "3_opt.hpp"

template<typename cost_t, typename vertex_t=int>
class LocalSearch3OptFunky : public LocalSearch3Opt<cost_t, vertex_t> {
 public:
    LocalSearch3OptFunky() = default;
    ~LocalSearch3OptFunky() = default;

 protected:

    cost_t run(
        std::vector<vertex_t> &solution,
        cost_t cur_cost,
        History<cost_t> &history,
        const int verbose = 0
    ) const override;

};

template<typename cost_t, typename vertex_t>
cost_t LocalSearch3OptFunky<cost_t, vertex_t>::run(
    std::vector<vertex_t> &path,
    cost_t cur_cost,
    History<cost_t> &history,
    const int verbose
) const {
    const int n = path.size();
    const int log_freq = 1;  // log best cost every 1 iters
    const int flush_freq = 10000;  // flush every 10000 costs
    const bool do_record_history = !history.isStopped();
    cost_t cur_cost_change = (cost_t) 0;
    int iter = 1;
    if (do_record_history) history.addCost(cur_cost);
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
                    // add slight amount to negative side when comparing
                    // the change to avoid swaps of the same element when
                    // j=i+1 or k=j+1 or both in patch_ordinals: 6, 4, 5
                    if (cur_cost_change < -1e-11) {
                        did_update = true;
                        this->applyCut(i, j, k, patch_ordinal, path);
                        cur_cost += cur_cost_change;
                        if (do_record_history) {
                            history.addCost(cur_cost);
                        }
                    }
                }
            }
        }
        // store history on flush_freq, or on last iter
        if (do_record_history) {
            if (!did_update || history.size() % flush_freq == 0) {
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
