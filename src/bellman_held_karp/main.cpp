#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <chrono>
#include <variant>

#include "problem_loader.hpp"
#include "dtype_selector.hpp"
#include "scaler.hpp"
#include "bellman_held_karp.hpp"


template<typename Func, typename... Args>
unsigned long long executeAndMeasureExecTime(
    const Func &func, Args&&... args
);

template<typename Func, typename... Args>
int executeAndMeasureAvgExecTime(
    const int num_runs,
    const unsigned long long timeout_ms,
    const Func &func,
    Args&&... args
);

template <typename distance_t>
void Solve(
    const std::vector<std::vector<distance_t>>& distances,
    const distance_t min_dist,  // non 0 diagonal min distance
    const distance_t max_dist,
    const bool is_searching_for_cycle,
    const unsigned long long max_num_bytes,
    const double precision,
    const bool do_not_prefer_cost_t_int,
    const int verbose,
    const bool is_symmetric,
    const unsigned int seed
);

template <typename distance_t, typename cost_t, typename vertex_t>
void logFoundSolution(
    const cost_t scaled_solution,
    const std::vector<vertex_t> &path,
    const int num_points,
    const std::vector<std::vector<distance_t>> &distances
);

template<typename cost_t, typename vertex_t>
void logMemoryUsage(const int n, const bool cycle, const bool is_symmetric);


int main(const int argc, const char **argv)
{
    using point_t = double;
    using distance_t = double;

    const std::string input_point_format = "%lf %lf\n";
    const int num_points = argc < 2 ? 24 : std::atoi(argv[1]);
    const std::string path_in_file = argc < 3
                                   ? "../problems/263.txt"
                                   : argv[2];
    const int num_reruns = argc < 4 ? 1 : std::atoi(argv[3]);
    const double desired_precision = argc < 5 ? 1e12 : std::atof(argv[4]);
    const unsigned long long memory_constraint_bytes = argc < 6 ?
                                                       // default 1 TB of RAM
                                                       1ULL << 40
                                                     : std::atoll(argv[5]);
    // by default try double/float whenever enough memory
    const bool do_not_prefer_cost_t_int = argc < 7 ? true : std::atoi(argv[6]);
    const unsigned long long timeout_ms = argc < 8
                                        ? 1000ULL * 24 * 60 * 60
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
        distance_t min_dist = std::numeric_limits<distance_t>::max();
        distance_t max_dist = std::numeric_limits<distance_t>::lowest();
        bool is_symmetric = true;
        const std::vector<std::vector<distance_t>> distances
            = loadDistances<point_t, distance_t>(
                path_in_file,
                input_point_format,
                num_points,
                min_dist,
                max_dist,
                is_symmetric,
                is_problem_in_pts_format
            );

        int run_idx = 1;
        executeAndMeasureAvgExecTime(
            num_reruns,
            timeout_ms,
            [&] () {
                Solve(
                    distances,
                    min_dist,
                    max_dist,
                    is_searching_for_cycle,
                    memory_constraint_bytes,
                    desired_precision,
                    do_not_prefer_cost_t_int,
                    run_idx == 1 ? 1 : 0,  // verbose only for first run
                    is_symmetric,
                    run_idx
                );
                ++run_idx;
            }
        );

    } catch (const std::bad_alloc &err) {
        // to cout to keep in log files
        std::cout << "OS failed to allocate space." << std::endl;
        std::cerr << "OS failed to allocate space." << std::endl;
        return -2;

    } catch (const std::exception &err) {
        std::cout << "err: " << err.what() << std::endl;
        std::cerr << "err: " << err.what() << std::endl;
        return -3;

    } catch (...) {
        std::cout << "Unknown error." << std::endl;
        std::cerr << "Unknown error." << std::endl;
        return -4;
    }

    return 0;
}


template<typename Func, typename... Args>
unsigned long long executeAndMeasureExecTime(const Func &func, Args&&... args) {
    const auto start_time = std::chrono::steady_clock::now();
    func(std::forward<Args>(args)...);
    const auto end_time = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>
                          (end_time - start_time);
    const auto dur_ms = duration.count();
    std::cout << "Execution time: " << dur_ms << " ms" << std::endl;
    return dur_ms;
}

template<typename Func, typename... Args>
int executeAndMeasureAvgExecTime(
    const int num_runs,
    const unsigned long long timeout_ms,
    const Func &func,
    Args&&... args
) {
    unsigned long long total_time_ms = 0ULL;
    int executed_runs = 0;
    for (int i = 1; i <= num_runs; ++i) {
        std::cout << "\n\nExecution #" << i << "/" << num_runs << "\n\n";
        total_time_ms += executeAndMeasureExecTime(func, std::forward<Args>(args)...);
        ++executed_runs;
        std::cout << "\nElapsed time: " << total_time_ms << " ms" << std::endl;

        if (total_time_ms >= timeout_ms) {
            std::cout << "\n\nTerminating due to timeout: " << total_time_ms
                      << " ms >= " << timeout_ms << " ms\n\n";
            break;
        }
    }

    double avg_time_ms = 1. * total_time_ms / executed_runs;
    std::cout << "Executed runs: " << executed_runs << "/" << num_runs
              << std::endl
              << "Total execution time: " << (1. * total_time_ms / 1000) << " s"
              << std::endl
              << "Avg execution time: " << avg_time_ms << " ms" << std::endl;
    return executed_runs;
}

template <typename distance_t>
void Solve(
    const std::vector<std::vector<distance_t>> &distances,
    const distance_t min_dist,  // non 0 diagonal min distance
    const distance_t max_dist,
    const bool is_finding_cycle,
    const unsigned long long max_num_bytes,
    const double precision,
    const bool do_not_prefer_cost_t_int,
    const int verbose,
    const bool is_symmetric,
    const unsigned int seed
) {
    std::mt19937 psrng = detail::initPSRNG(seed);
    const distance_t max_cost = detail::estimateMaxPossibleCost(
        distances, is_finding_cycle, psrng, 10
    );
    const int num_points = distances.size();
    const int num_edges = is_finding_cycle ? num_points
                                           : num_points - 1;
    const distance_t max_cost_norm = max_dist - min_dist <= 0
                                ? (distance_t) 0
                                : (max_cost - min_dist * num_edges);
    const auto cost_t_variant = chooseCostType<distance_t>(
        precision, max_cost_norm, num_points, max_num_bytes,
        do_not_prefer_cost_t_int, is_finding_cycle, is_symmetric,
        verbose, false
    );

    std::visit([&] (auto &&cost_t_variant) {
        using cost_t = std::decay_t<decltype(cost_t_variant)>;
        using vertex_t = uint8_t;
        const int num_points = distances.size();
        if (verbose > 0) {
            logMemoryUsage<cost_t, vertex_t>(
                num_points, is_finding_cycle, is_symmetric
            );
        }
        const std::vector<std::vector<cost_t>> scaled_distances
            =  ( std::is_floating_point_v<cost_t>
              || max_cost_norm <= (distance_t) 0 )
            ? recastMatrix<cost_t, distance_t>(distances)
            : scaleAndNormalize<cost_t, distance_t>(
                distances, min_dist,
                max_cost_norm, precision,
                true,  // do round
                verbose
            );
        std::vector<vertex_t> path;
        const cost_t cost = bellmanHeldKarp<cost_t, vertex_t, uint64_t>(
            path,
            scaled_distances,
            is_finding_cycle,
            is_symmetric,
            std::numeric_limits<cost_t>::max(),
            true  // always true since normalized
        );

        if (verbose > 0) {
            logFoundSolution<distance_t>(
                cost, path, num_points, distances
            );
        }
    }, cost_t_variant);
}

template <typename distance_t, typename cost_t, typename vertex_t>
void logFoundSolution(
    const cost_t scaled_solution,
    const std::vector<vertex_t> &path,
    const int num_points,
    const std::vector<std::vector<distance_t>> &distances
) {
    std::cout << "Optimal total distance (scaled) for " << num_points
              << " points: " << static_cast<double> (scaled_solution)
              << std::endl;

    // recalculate exact distance of the found path and log it
    distance_t exact_min_distance_found = (distance_t)0;
    for (int i = 1, n = path.size(); i < n; ++i) {
        exact_min_distance_found += distances[path[i - 1]][path[i]];
    }
    std::cout << "Optimal total distance for " << num_points
            << " points: " << std::fixed << std::setprecision(6)
            << exact_min_distance_found << std::defaultfloat << std::endl;

    std::cout << "Corresponding path (0-indexed):" << std::endl;
    for (const int idx : path) {
        std::cout << "Point #" << ((int) idx) << std::endl;
    }
}

template<typename cost_t, typename vertex_t>
void logMemoryUsage(const int n, const bool cycle, const bool is_symmetric) {
    const auto space = calcSpaceNeeded(n, cycle, is_symmetric);
    const uint64_t bytes_to_use = sizeof(vertex_t) * space.first
                                + sizeof(cost_t) * space.second;
    std::cout << "Solving with cost_t = " << typeid(cost_t).name()
              << std::endl << "Memory [GB] needed: "
              << (1. * bytes_to_use / (1 << 30)) << std::endl;
}
