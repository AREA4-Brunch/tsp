#ifndef TSP_AUTO_OPT_AUTO_OPT_HPP
#define TSP_AUTO_OPT_AUTO_OPT_HPP

#include <vector>
#include <memory>
#include "../k_opt/heuristic.hpp"
#include "../k_opt/heuristic_funky.hpp"
#include "../k_opt/factories.hpp"

namespace k_opt {

template<
    typename cost_t,
    k_opt::IntrusiveVertex vertex_t,
    int K = -1
>
class AutoOpt : public Heuristic<cost_t, vertex_t>
{
    static_assert(K == -1 || K >= 2, "K must be -1 (dynamic) or >= 2 (static)");

 public:

    explicit AutoOpt(const std::string &select_name, const int k = -1);

 protected:

    cost_t run(
        typename vertex_t::traits::node_ptr solution,
        cost_t cur_cost,
        History<cost_t> &history,
        const cost_t * __restrict const flat_weights,
        const int n,
        const int verbose = 0
    ) const noexcept override;

 private:

    const int k;
    std::vector<std::pair<
        int,
        std::unique_ptr<const Heuristic<cost_t, vertex_t>>
    >> heuristics;

};


template<typename cost_t, k_opt::IntrusiveVertex v_t, int K>
AutoOpt<cost_t, v_t, K>::AutoOpt(
    const std::string &selection_name,
    const int k
) : k(k)
{
    for (int cur_k = k; cur_k >= 2; --cur_k) {
        std::string cut_name = std::to_string(cur_k) + "_opt_pure";
        auto pure_opt = factories::createAlgo<cost_t, v_t>(
            selection_name, cut_name, 0U
        );
        this->heuristics.push_back({ cur_k, std::move(pure_opt) });
    }
}

template<typename cost_t, IntrusiveVertex vertex_t, int K>
cost_t AutoOpt<cost_t, vertex_t, K>::run(
    typename vertex_t::traits::node_ptr path,
    cost_t cur_cost,
    History<cost_t> &history,
    const cost_t * __restrict const weights,
    const int n,
    const int verbose
) const noexcept {
    for (const auto &desc : this->heuristics) {
        const int k = desc.first;
        const auto &heur = desc.second;
        std::cout << std::endl << "SWITCHING to heuristic with k = "
                  << k << std::endl;
        cur_cost = heur->run(
            path, cur_cost, history, weights, n, verbose
        );
    }
    return cur_cost;
}

}  // namespace k_opt

#endif
