#ifndef TSP_3_OPT_RAND_HPP
#define TSP_3_OPT_RAND_HPP

#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include "3_opt.hpp"
#include "../common/random.hpp"

template<typename cost_t, typename vertex_t=int>
class LocalSearch3OptRand : public LocalSearch3Opt<cost_t, vertex_t> {
 public:
    LocalSearch3OptRand(std::mt19937 &psrng) : psrng(psrng) {}
    ~LocalSearch3OptRand() = default;

 protected:

    cost_t run(
        std::vector<vertex_t> &solution,
        cost_t cur_cost,
        History<cost_t> &history,
        const int verbose = 0
    ) const override;

 private:

    mutable std::mt19937 psrng;

};

template<typename cost_t, typename vertex_t>
cost_t LocalSearch3OptRand<cost_t, vertex_t>::run(
    std::vector<vertex_t> &path,
    cost_t cur_cost,
    History<cost_t> &history,
    const int verbose
) const {
    const int n = path.size();
    const bool do_record_history = !history.isStopped();
    if (do_record_history) history.addCost(cur_cost);
    if (n <= 2) return cur_cost;

    std::uniform_int_distribution<std::mt19937::result_type>
        rand_idx_i(0, n - 1 - 2);
    const auto set_rand_cut_positions = [&] (int &i, int &j, int &k) {
        i = rand_idx_i(this->psrng);
        std::uniform_int_distribution<std::mt19937::result_type>
                rand_idx_j(i + 1, n - 1 - 1);
        j = rand_idx_j(this->psrng);
        std::uniform_int_distribution<std::mt19937::result_type>
                rand_idx_k(j + 1, n - 1);
        k = rand_idx_k(this->psrng);
    };

    int iter = 1;
    const int log_freq = 10;  // log best cost every 10 iters
    const int flush_freq = 10000;  // flush every 10000 costs
    cost_t cur_cost_change = (cost_t) 0;
    for (bool did_update = true; did_update; ++iter) {
        if (verbose > 0 && (iter < 10 || iter % log_freq == 0)) {
            std::cout << "ITERATION " << iter << ": "
                      << cur_cost << std::endl;
        }
        did_update = false;
        int i = 0, j = 0, k = 0;
        const auto process_cut = [&] () {
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
        };
        // try randomly for 1/6 the i, j, k combos until collisions start
        // and if none provide update then check all to see if no better
        // path is possible
        for (int tries_left = n * n * n / 6;
             tries_left >= 0 && !did_update;
             --tries_left
        ) {
            set_rand_cut_positions(i, j, k);
            process_cut();
        }
        for (i = 0; i < n - 2 && !did_update; ++i) {
            for (j = i + 1; j < n - 1 && !did_update; ++j) {
                for (k = j + 1; k < n && !did_update; ++k) {
                    process_cut();
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
