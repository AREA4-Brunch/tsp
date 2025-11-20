#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <numeric>
#include <string>
#include <memory>

#include "history.hpp"
#include "3_opt.hpp"
#include "3_opt_funky.hpp"
#include "3_opt_best_cut.hpp"
#include "3_opt_classical.hpp"
#include "3_opt_rand.hpp"
#include "../common/random.hpp"
#include "../common/timing.hpp"
#include "../common/problem_loader.hpp"

template <typename cost_t, typename weight_t>
cost_t Solve(
    LocalSearch3Opt<cost_t, int> &algo,
    std::vector<std::vector<weight_t>> &distances,
    const bool is_searching_for_cycle,
    History<cost_t> &history,
    const unsigned int seed = 0U
);

namespace detail {

template<typename cost_t>
void startNewHistory(
    const int runs_per_history,
    const std::string& path_history_dir,
    const int run_idx,
    History<cost_t>* &cur_history
);

template<typename cost_t, typename vertex_t>
std::unique_ptr<LocalSearch3Opt<cost_t, vertex_t>> selectAlgo(
    const std::string &req_algo,
    const unsigned int seed = 0U
);

}


int main(const int argc, const char **argv)
{
    using point_t = double;
    using cost_t = point_t;

    const std::string input_point_format = "%lf %lf\n";
    const std::string req_algo = argc < 2 ? "3_opt_funky" : argv[1];
    const int num_points = argc < 3 ? 24 : std::atoi(argv[2]);
    const std::string path_in_file = argc < 4
                                   ? "../problems/263.txt"
                                   : argv[3];
    const std::string path_history_dir = argc < 5
            ? "../../results/3_opt_funky/histories/" + path_in_file
            : argv[4];
    const int num_reruns = argc < 6 ? 100 : std::atoi(argv[5]);
    const int runs_per_history = argc < 7 ? 1 : std::atoi(argv[6]);
    const unsigned long long timeout_ms = argc < 8
                                        ? 1000ULL * 105 * 60  // 105 mins
                                        : 1000ULL * std::atoll(argv[7]);
    const bool is_searching_for_cycle = argc < 9
                                      ? false  // by default solve SHP
                                      : std::string(argv[8]) == "tsp";
    const bool is_problem_in_pts_format = argc < 10
                                        ? true  // not TSPLIB format by default
                                        : std::atoi(argv[9]);

    std::cout << "Solving "
              << (is_searching_for_cycle ? "TSP" : "SHP")
              << std::endl;

    try {
        std::vector<std::vector<cost_t>> distances
            = prloader::loadDistances<point_t, cost_t>(
                path_in_file,
                input_point_format,
                num_points,
                is_problem_in_pts_format
            );

        cost_t avg_min_cost_in_n_reruns = (cost_t) 0;
        cost_t best_cost_in_n_reruns = std::numeric_limits<cost_t>::max();
        History<cost_t> *cur_history = nullptr;
        int run_idx = 1;
        const int executed_reruns = timing::executeAndMeasureAvgExecTime(
            num_reruns,
            timeout_ms,
            [&] () {
                if ((run_idx - 1) % runs_per_history == 0) {
                    detail::startNewHistory(
                        runs_per_history, path_history_dir, run_idx, cur_history);
                }
                // make sure it is (ull, int) for python script to work
                cur_history->appendMarkersToLastFlush(0ULL, (int) run_idx);

                const auto seed = random::genRandomSeed();
                const auto algo = detail::selectAlgo<cost_t, int>(req_algo, seed);
                const cost_t min_cost = Solve<cost_t, cost_t>(
                    *algo, distances, is_searching_for_cycle, *cur_history,
                    seed  // e.g. good: 3310318500
                );
                avg_min_cost_in_n_reruns += min_cost;
                best_cost_in_n_reruns = std::min(best_cost_in_n_reruns, min_cost);
                ++run_idx;
            }
        );

        if (cur_history != nullptr) delete cur_history;
        avg_min_cost_in_n_reruns /= executed_reruns;
        std::cout << "AVG MIN COST OVER " << executed_reruns << " RUNS: "
                  << avg_min_cost_in_n_reruns << std::endl;
        std::cout << "BEST COST OVER " << executed_reruns << " RUNS: "
                  << best_cost_in_n_reruns << std::endl;

    } catch (const std::exception &err) {
        // to cout to keep in log files
        std::cout << "err: " << err.what() << std::endl;
        std::cerr << "err: " << err.what() << std::endl;
        return -1;

    } catch (...) {
        std::cout << "Unknown error." << std::endl;
        std::cerr << "Unknown error." << std::endl;
        return -2;
    }

    return 0;
}


template <typename cost_t, typename weight_t>
cost_t Solve(
    LocalSearch3Opt<cost_t, int> &algo,
    std::vector<std::vector<weight_t>> &distances,
    const bool is_searching_for_cycle,
    History<cost_t> &history,
    const unsigned int seed
) {
    // start with a random solution
    std::vector<int> path(distances.size());
    std::iota(path.begin(), path.end(), 0);
    std::mt19937 psrng = random::initPSRNG(seed);
    random::permuteRandomly(path, psrng);

    const cost_t min_distance = algo.template search<weight_t>(
        distances,
        path,
        !is_searching_for_cycle,
        history,
        false,  // is_weights_upper_triangular
        1       // verbose
    );
    // if cycle change to format with path[0] == path.back()
    if (is_searching_for_cycle) path.push_back(path[0]);

    std::cout << "Best found total distance for " << distances.size()
              << " points: " << static_cast<double>(min_distance)
              << std::endl;
    std::cout << "Corresponding path (0-indexed):" << std::endl;
    for (const int idx : path) {
        std::cout << "Point #" << ((int) idx) << std::endl;
    }
    return min_distance;
}

/// clears run(s) dir if it already exists
template<typename cost_t>
void detail::startNewHistory(
    const int runs_per_history,
    const std::string& path_history_dir,
    const int run_idx,
    History<cost_t>* &cur_history
) {
    const std::string path_history_run
        = runs_per_history == 1 ?
        path_history_dir + "/run_" + std::to_string(run_idx)
        : path_history_dir + "/runs_"
        + std::to_string(1 + runs_per_history * (run_idx / runs_per_history))
        + "_"
        + std::to_string(runs_per_history * (1 + run_idx / runs_per_history));

    std::cout << "Run(s) folder: " << path_history_run << std::endl;
    // clear the run folder if it already exists
    if (std::filesystem::is_directory(path_history_run)) {
        std::error_code err_code;
        std::filesystem::remove_all(path_history_run, err_code);
        if (err_code) throw ("Failed to delete run folder: " + path_history_run);
        std::cout << "Deleted already existing run folder: "
            << path_history_run << std::endl;
    }

    if (cur_history != nullptr) delete cur_history;
    cur_history = new History<cost_t>(path_history_run);
}

template<typename cost_t, typename vertex_t>
std::unique_ptr<LocalSearch3Opt<cost_t, vertex_t>> detail::selectAlgo(
    const std::string &req_algo,
    const unsigned int seed
) {
    if (req_algo == "3_opt_funky") {
        return std::make_unique<LocalSearch3OptFunky<cost_t, vertex_t>>();
    }
    if (req_algo == "3_opt_best_cut") {
        return std::make_unique<LocalSearch3OptBestCut<cost_t, vertex_t>>();
    }
    if (req_algo == "3_opt_classical") {
        return std::make_unique<LocalSearch3OptClassical<cost_t, vertex_t>>();
    }
    if (req_algo == "3_opt_rand") {
        auto psrng = random::initPSRNG(seed);
        return std::make_unique<LocalSearch3OptRand<cost_t, vertex_t>>(psrng);
    }
    throw std::invalid_argument("No such algorithm: " + req_algo);
}
