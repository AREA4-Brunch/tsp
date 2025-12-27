#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <numeric>
#include <string>
#include <memory>
#include <unordered_map>
#include <variant>
#include <random>
#include <iomanip>
#include <list>

#include "cut_3_opt.hpp"
#include "history.hpp"
#include "heuristic.hpp"
#include "heuristic_best_cut.hpp"
#include "heuristic_classical.hpp"
#include "heuristic_funky.hpp"
#include "heuristic_rand.hpp"
#include "cut_k_opt.hpp"
#include "vertex_concept.hpp"
#include "vertex.hpp"
#include "cut_strategy.hpp"
#include "../common/random.hpp"
#include "../common/timing.hpp"
#include "../common/problem_loader.hpp"


template <typename cost_t>
cost_t Solve(
    const std::string &selection_name,
    const std::string &cut_name,
    std::vector<std::vector<cost_t>> &distances,
    const bool is_searching_for_cycle,
    k_opt::History<cost_t> &history,
    const unsigned int seed
);

namespace detail {

template<typename cost_t, k_opt::IntrusiveVertex vertex_t>
std::unique_ptr<k_opt::Heuristic<cost_t, vertex_t>> selectAlgo(
    const std::string &selection_algo_name,
    const std::string &cut_algo_name,
    const unsigned int seed
);

template<typename cost_t, k_opt::IntrusiveVertex vertex_t>
std::variant<
    k_opt::Cut3Opt<cost_t, vertex_t>,
    k_opt::Cut3OptNo2Opt<cost_t, vertex_t>,
    k_opt::CutKOpt<cost_t, vertex_t, 4>,
    k_opt::CutKOpt<cost_t, vertex_t, 5>,
    k_opt::CutKOpt<cost_t, vertex_t, -1>
> createCut(const std::string &cut_name, int &k);

template<
    typename cost_t,
    typename cut_t,
    k_opt::IntrusiveVertex vertex_t,
    int K
>
requires k_opt::CutStrategy<cut_t, cost_t, vertex_t, K>
std::unique_ptr<k_opt::Heuristic<cost_t, vertex_t>>
createHeuristic(
    const std::string &heur_name,
    const cut_t &cut,
    const unsigned int seed,
    const int k = -1
);

bool hasFlag(int argc, const char **argv, const std::string& flag) {
    for (int i = 1; i < argc; ++i) {
        if (argv[i] == flag) return true;
    }
    return false;
}

template<k_opt::IntrusiveVertex vertex_t>
void logPath(
    typename vertex_t::traits::const_node_ptr path,
    const bool is_searching_for_cycle
);

}  // namespace detail


int main(const int argc, const char **argv)
{
    using point_t = double;
    using cost_t = point_t;

    const std::string input_point_format = "%lf %lf\n";
    const std::string selection_name = argc < 2 ? "funky" : argv[1];
    const std::string cut_name = argc < 3 ? "3_opt" : argv[2];
    const int num_points = argc < 4 ? 24 : std::atoi(argv[3]);
    const std::string path_in_file = argc < 5
                                ? "../problems/263.txt"
                                : argv[4];
    const std::string path_history_dir = argc < 6
            ? "../../results/3_opt_funky/histories/" + path_in_file
            : argv[5];
    const int num_reruns = argc < 7 ? 100 : std::atoi(argv[6]);
    const int runs_per_history = argc < 8 ? 1 : std::atoi(argv[7]);
    const unsigned long long timeout_ms = argc < 9
                                        ? 1000ULL * 105 * 60  // 105 mins
                                        : 1000ULL * std::atoll(argv[8]);
    const bool is_searching_for_cycle = argc < 10
                                    ? false  // by default solve SHP
                                    : std::string(argv[9]) == "tsp";
    const bool is_problem_in_pts_format = argc < 11
                                        ? true  // not TSPLIB format by default
                                        : std::atoi(argv[10]);
    const bool is_history_off = detail::hasFlag(argc, argv, "--no-history");

    std::cout << "Solving "
              << (is_searching_for_cycle ? "TSP" : "SHP")
              << std::endl;

    try {
        k_opt::History<cost_t> *cur_history = is_history_off
            ? new k_opt::History<cost_t> ("")
            : nullptr;  // create later
        if (is_history_off) cur_history->stop();

        std::vector<std::vector<cost_t>> distances
            = prloader::loadDistances<point_t, cost_t>(
                path_in_file,
                input_point_format,
                num_points,
                is_problem_in_pts_format
            );
        cost_t avg_min_cost_in_n_reruns = (cost_t) 0;
        cost_t best_cost_in_n_reruns = std::numeric_limits<cost_t>::max();
        int run_idx = 1;
        auto seed = random::genRandomSeed();
        const int executed_reruns = timing::executeAndMeasureAvgExecTime(
            num_reruns,
            timeout_ms,
            [&] () {
                if (!is_history_off) {
                    if ((run_idx - 1) % runs_per_history == 0) {
                        k_opt::startNewHistory(
                            runs_per_history, path_history_dir,
                            run_idx, cur_history);
                    }
                    // make sure it is (ull, int) for python script to work
                    cur_history->appendMarkersToLastFlush(0ULL, (int) run_idx);
                }
                const cost_t min_cost = Solve<cost_t>(
                    selection_name, cut_name,
                    distances, is_searching_for_cycle, *cur_history,
                    seed++  // e.g. good: 3310318500
                );
                avg_min_cost_in_n_reruns += min_cost;
                best_cost_in_n_reruns = std::min(best_cost_in_n_reruns, min_cost);
                ++run_idx;
            }
        );

        if (cur_history != nullptr) delete cur_history;
        avg_min_cost_in_n_reruns /= executed_reruns;
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "AVG MIN COST OVER " << executed_reruns << " RUNS: "
                  << avg_min_cost_in_n_reruns << std::endl;
        std::cout << "BEST COST OVER " << executed_reruns << " RUNS: "
                  << best_cost_in_n_reruns << std::endl;
        std::cout << std::defaultfloat;  

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

    std::cout << std::flush;

    return 0;
}


template <typename cost_t>
cost_t Solve(
    const std::string &selection_name,
    const std::string &cut_name,
    std::vector<std::vector<cost_t>> &distances,
    const bool is_searching_for_cycle,
    k_opt::History<cost_t> &history,
    const unsigned int seed
) {
    using id_t = int;
    using vertex_t = k_opt::Vertex<id_t>;

    const int n = distances.size();
    std::cout << "Seed: " << seed << std::endl;
    const auto algo = detail::selectAlgo<cost_t, vertex_t>(
        selection_name, cut_name, seed);

    std::vector<vertex_t> path_buffer;
    typename vertex_t::traits::node_ptr path;
    const cost_t min_distance = algo->search(
        path,
        path_buffer,
        std::move(distances),
        !is_searching_for_cycle,
        history,
        seed,
        1  // verbose
    );

    // log found cost and path:
    std::cout << "Best found total distance for " << n
              << " points: " << std::fixed << std::setprecision(6)
              << static_cast<double>(min_distance)
              << std::defaultfloat << std::endl;
    std::cout << "Corresponding path (0-indexed):" << std::endl;
    detail::logPath<vertex_t>(path, is_searching_for_cycle);

    return min_distance;
}

template<k_opt::IntrusiveVertex vertex_t>
void detail::logPath(
    typename vertex_t::traits::const_node_ptr path,
    const bool is_searching_for_cycle
) {
    auto cur = path;
    do {
        const auto v = vertex_t::v(cur)->id;
        cur = vertex_t::traits::get_next(cur);
        std::cout << "Point #" << ((int)v) << std::endl;
    } while (cur != path);

    // if cycle then change to format with explicit last edge
    if (is_searching_for_cycle) {
        const auto v = vertex_t::v(path)->id;
        std::cout << "Point #" << ((int)v) << std::endl;
    }
}

template<typename cost_t, k_opt::IntrusiveVertex vertex_t>
std::unique_ptr<k_opt::Heuristic<cost_t, vertex_t>> detail::selectAlgo(
    const std::string &selection_algo_name,
    const std::string &cut_algo_name,
    const unsigned int seed
) {
    int k = -1;
    return std::visit([&] (auto &&cut) {
        using cut_t = std::decay_t<decltype(cut)>;
        return detail::createHeuristic<
            cost_t, cut_t, vertex_t, cut_t::NUM_CUTS
        >(
            selection_algo_name, cut, seed, k
        );
    }, detail::createCut<cost_t, vertex_t>(cut_algo_name, k));
}

template<typename cost_t, k_opt::IntrusiveVertex vertex_t>
std::variant<
    k_opt::Cut3Opt<cost_t, vertex_t>,
    k_opt::Cut3OptNo2Opt<cost_t, vertex_t>,
    k_opt::CutKOpt<cost_t, vertex_t, 4>,
    k_opt::CutKOpt<cost_t, vertex_t, 5>,
    k_opt::CutKOpt<cost_t, vertex_t, -1>
> detail::createCut(const std::string &cut_name, int &k) {
    using cut_t = std::variant<
        k_opt::Cut3Opt<cost_t, vertex_t>,
        k_opt::Cut3OptNo2Opt<cost_t, vertex_t>,
        k_opt::CutKOpt<cost_t, vertex_t, 4>,
        k_opt::CutKOpt<cost_t, vertex_t, 5>,
        k_opt::CutKOpt<cost_t, vertex_t, -1>
    >;
    using factory_t = std::function<cut_t ()>;

    const bool select_first_better = false;
    const bool do_pre_gen_perms = true;
    static const std::unordered_map<std::string, factory_t> cuts = {
        { "3_opt", [] () {
            return k_opt::Cut3Opt<cost_t, vertex_t>();
        }},
        { "3_opt_pure", [] () {
            return k_opt::Cut3OptNo2Opt<cost_t, vertex_t>();
        }},
        { "4_opt", [] () {
            return k_opt::CutKOpt<cost_t, vertex_t, 4>(
                4, true, select_first_better, do_pre_gen_perms
            );
        }},
        { "4_opt_pure", [] () {
            return k_opt::CutKOpt<cost_t, vertex_t, 4>(
                4, false, select_first_better, do_pre_gen_perms
            );
        }},
        { "5_opt", [] () {
            return k_opt::CutKOpt<cost_t, vertex_t, 5>(
                5, true, select_first_better, do_pre_gen_perms
            );
        }},
        { "5_opt_pure", [] () {
            return k_opt::CutKOpt<cost_t, vertex_t, 5>(
                5, false, select_first_better, do_pre_gen_perms
            );
        }}
    };
    if (cuts.count(cut_name)) return cuts.at(cut_name)();

    using k_factory_t = std::function<cut_t (const int)>;
    const int sep_idx = cut_name.find('_');
    k = std::stoi(cut_name.substr(0, sep_idx));
    static const std::unordered_map<std::string, k_factory_t> k_cuts = {
        { "_opt", [] (const int k) {
            return k_opt::CutKOpt<cost_t, vertex_t>(
                k, true, select_first_better, do_pre_gen_perms
            );
        }},
        { "_opt_pure", [] (const int k) {
            return k_opt::CutKOpt<cost_t, vertex_t>(
                k, false, select_first_better, do_pre_gen_perms
            );
        }}
    };
    return k_cuts.at(cut_name.substr(sep_idx))(k);
}

template<
    typename cost_t,
    typename cut_t,
    k_opt::IntrusiveVertex vertex_t,
    int K
>
requires k_opt::CutStrategy<cut_t, cost_t, vertex_t, K>
std::unique_ptr<k_opt::Heuristic<cost_t, vertex_t>>
detail::createHeuristic(
    const std::string &heur_name,
    const cut_t &cut,
    const unsigned int seed,
    const int k
) {
    using heur_ptr = std::unique_ptr<k_opt::Heuristic<cost_t, vertex_t>>;
    using factory_t = std::function<heur_ptr ()>;
    const std::unordered_map<std::string, factory_t> heurs = {
        { "best_cut", [&] () {
            return std::make_unique<k_opt::KOptBestCut<
                cost_t, cut_t, vertex_t, K
            >>(cut, k);
        }},
        { "classical", [&] () {
            return std::make_unique<k_opt::KOptClassical<
                cost_t, cut_t, vertex_t, K
            >>(cut, k);
        }},
        { "funky", [&] () {
            return std::make_unique<k_opt::KOptFunky<
                cost_t, cut_t, vertex_t, K
            >>(cut, k);
        }},
        { "rand", [&] () {
            auto psrng = random::initPSRNG(seed);
            return std::make_unique<k_opt::KOptRand<
                cost_t, cut_t, vertex_t, K
            >>(cut, psrng, k);
        }}
    };
    return heurs.at(heur_name)();
}
