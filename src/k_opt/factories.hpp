#ifndef TSP_K_OPT_FACTORIES_HPP
#define TSP_K_OPT_FACTORIES_HPP

#include <vector>
#include <variant>
#include <memory>
#include <unordered_map>
#include "vertex_concept.hpp"
#include "vertex.hpp"
#include "cut_2_opt.hpp"
#include "cut_3_opt.hpp"
#include "cut_k_opt.hpp"
#include "heuristic.hpp"
#include "heuristic_best_cut.hpp"
#include "heuristic_classical.hpp"
#include "heuristic_funky.hpp"
#include "heuristic_rand.hpp"

namespace k_opt {
namespace factories {

template<typename cost_t, IntrusiveVertex vertex_t>
std::variant<
    Cut2Opt<cost_t, vertex_t>,
    Cut3Opt<cost_t, vertex_t>,
    Cut3OptNo2Opt<cost_t, vertex_t>,
    CutKOpt<cost_t, vertex_t, 4>,
    CutKOpt<cost_t, vertex_t, 5>,
    CutKOpt<cost_t, vertex_t, -1>
> createCut(const std::string &cut_name, int &k) {
    using cut_t = std::variant<
        Cut2Opt<cost_t, vertex_t>,
        Cut3Opt<cost_t, vertex_t>,
        Cut3OptNo2Opt<cost_t, vertex_t>,
        CutKOpt<cost_t, vertex_t, 4>,
        CutKOpt<cost_t, vertex_t, 5>,
        CutKOpt<cost_t, vertex_t, -1>
    >;
    using factory_t = std::function<cut_t ()>;

    const bool select_first_better = false;
    const bool do_pre_gen_perms = true;
    static const std::unordered_map<std::string, factory_t> cuts = {
        { "2_opt", [] () {
            return Cut2Opt<cost_t, vertex_t>();
        }},
        { "2_opt_pure", [] () {
            return Cut2Opt<cost_t, vertex_t>();
        }},
        { "3_opt", [] () {
            return Cut3Opt<cost_t, vertex_t>();
        }},
        { "3_opt_pure", [] () {
            return Cut3OptNo2Opt<cost_t, vertex_t>();
        }},
        { "4_opt", [] () {
            return CutKOpt<cost_t, vertex_t, 4>(
                4, true, select_first_better, do_pre_gen_perms
            );
        }},
        { "4_opt_pure", [] () {
            return CutKOpt<cost_t, vertex_t, 4>(
                4, false, select_first_better, do_pre_gen_perms
            );
        }},
        { "5_opt", [] () {
            return CutKOpt<cost_t, vertex_t, 5>(
                5, true, select_first_better, do_pre_gen_perms
            );
        }},
        { "5_opt_pure", [] () {
            return CutKOpt<cost_t, vertex_t, 5>(
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
            return CutKOpt<cost_t, vertex_t>(
                k, true, select_first_better, do_pre_gen_perms
            );
        }},
        { "_opt_pure", [] (const int k) {
            return CutKOpt<cost_t, vertex_t>(
                k, false, select_first_better, do_pre_gen_perms
            );
        }}
    };
    return k_cuts.at(cut_name.substr(sep_idx))(k);
}

template<
    typename cost_t,
    typename cut_t,
    IntrusiveVertex vertex_t,
    int K
>
requires CutStrategy<cut_t, cost_t, vertex_t, K>
std::unique_ptr<Heuristic<cost_t, vertex_t>>
createHeuristic(
    const std::string &heur_name,
    const cut_t &cut,
    const unsigned int seed,
    const int k
) {
    using heur_ptr = std::unique_ptr<Heuristic<cost_t, vertex_t>>;
    using factory_t = std::function<heur_ptr ()>;
    const std::unordered_map<std::string, factory_t> heurs = {
        { "best_cut", [&] () {
            return std::make_unique<KOptBestCut<
                cost_t, cut_t, vertex_t, K
            >>(cut, k);
        }},
        { "classical", [&] () {
            return std::make_unique<KOptClassical<
                cost_t, cut_t, vertex_t, K
            >>(cut, k);
        }},
        { "funky", [&] () {
            return std::make_unique<KOptFunky<
                cost_t, cut_t, vertex_t, K
            >>(cut, k);
        }},
        { "rand", [&] () {
            auto psrng = random::initPSRNG(seed);
            return std::make_unique<KOptRand<
                cost_t, cut_t, vertex_t, K
            >>(cut, psrng, k);
        }}
    };
    return heurs.at(heur_name)();
}

template<typename cost_t, k_opt::IntrusiveVertex vertex_t>
std::unique_ptr<k_opt::Heuristic<cost_t, vertex_t>> createAlgo(
    const std::string &selection_algo_name,
    const std::string &cut_algo_name,
    const unsigned int seed
) {
    int k = -1;
    return std::visit([&] (auto &&cut) {
        using cut_t = std::decay_t<decltype(cut)>;
        return createHeuristic<
            cost_t, cut_t, vertex_t, cut_t::NUM_CUTS
        >(
            selection_algo_name, cut, seed, k
        );
    }, createCut<cost_t, vertex_t>(cut_algo_name, k));
}

}  // namespace factories
}  // namespace k_opt

#endif
