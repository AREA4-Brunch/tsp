#ifndef TSP_K_OPT_HEURISTIC_RAND_HPP
#define TSP_K_OPT_HEURISTIC_RAND_HPP

#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include <numeric>
#include "heuristic.hpp"
#include "cut_strategy.hpp"
#include "../common/random.hpp"

namespace k_opt {

template<
    typename cost_t,
    typename cut_strategy_t,
    typename vertex_t=int
>
requires k_opt::CutStrategy<cut_strategy_t, cost_t, vertex_t>
class HeuristicRand
    : public k_opt::Heuristic<cost_t, vertex_t>
{
 public:

    HeuristicRand(
        cut_strategy_t cut_strategy,
        std::mt19937 &psrng
    ) : cut(cut_strategy), psrng(psrng) { }

    ~HeuristicRand() = default;

 protected:

    const cut_strategy_t cut;

    cost_t run(
        std::vector<vertex_t> &solution,
        cost_t cur_cost,
        History<cost_t> &history,
        const std::vector<std::vector<cost_t>> &weights,
        const int verbose = 0
    ) const override;

    void selectInitSolution(
        std::vector<vertex_t> &solution,
        const std::vector<std::vector<cost_t>> &weights
    ) override;

 private:

    mutable std::mt19937 psrng;

};

}  // namespace k_opt


template<typename cost_t, typename cut_strategy_t, typename vertex_t>
requires k_opt::CutStrategy<cut_strategy_t, cost_t, vertex_t>
cost_t k_opt::HeuristicRand<cost_t, cut_strategy_t, vertex_t>::run(
    std::vector<vertex_t> &path,
    cost_t cur_cost,
    History<cost_t> &history,
    const std::vector<std::vector<cost_t>> &weights,
    const int verbose
) const {
    const int n = path.size();
    const bool do_record_history = !history.isStopped();
    history.addCost(cur_cost);
    history.addPath(path, -1, -1, -1, 0);
    if (n <= 3) return cur_cost;

    std::cout << "N: " << n << std::endl;
    std::cout << "j-n: " << n - 2 - 1 << std::endl;
    std::uniform_int_distribution<std::mt19937::result_type>
        rand_idx_i(0, n - 3 - 1);
    const auto set_rand_cut_positions = [&] (int &i, int &j, int &k) {
        i = rand_idx_i(this->psrng);
        std::uniform_int_distribution<std::mt19937::result_type>
                rand_idx_j(i + 1, n - 2 - 1);
        j = rand_idx_j(this->psrng);
        std::uniform_int_distribution<std::mt19937::result_type>
                rand_idx_k(j + 2, n - 1);
        k = rand_idx_k(this->psrng);
    };

    const int log_freq = 10;  // log best cost every 10 iters
    const int flush_freq = 10000;  // flush every 10000 costs
    cost_t cur_cost_change = (cost_t) 0;
    int iter = 1;
    for (bool did_update = true; did_update; ++iter) {
        if (verbose > 0 && (iter < 10 || iter % log_freq == 0)) {
            std::cout << "ITERATION " << iter << ": "
                      << cur_cost << std::endl;
        }
        did_update = false;
        int i = 0, j = 0, k = 0;
        const auto process_cut = [&] () {
            // std::cout << "Processing cut " << i << " " << j
            //           << " " << k << std::endl;
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
        for (i = 0; i < n - 3 && !did_update; ++i) {
            for (j = i + 1; j < n - 2 && !did_update; ++j) {
                for (k = j + 2; k < n && !did_update; ++k) {
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
        std::cout << std::fixed << std::setprecision(6)
                  << "Last ITERATION " << iter << ": "
                  << cur_cost << std::defaultfloat
                  << std::endl;
    }
    return cur_cost;
}

template<typename cost_t, typename cut_strategy_t, typename vertex_t>
requires k_opt::CutStrategy<cut_strategy_t, cost_t, vertex_t>
void k_opt::HeuristicRand<
    cost_t, cut_strategy_t, vertex_t
>::selectInitSolution(
    std::vector<vertex_t> &solution,
    const std::vector<std::vector<cost_t>> &weights
) {
    solution.resize(weights.size());
    std::iota(solution.begin(), solution.end(), 0);
    random::permuteRandomly(solution, this->psrng);
}

#endif
