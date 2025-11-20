#ifndef DTYPE_SELECTOR_HPP
#define DTYPE_SELECTOR_HPP

#include <variant>
#include <vector>
#include <iomanip>
#include <iostream>
#include "bellman_held_karp.hpp"
#include "../common/random.hpp"

namespace detail {

using cost_t_variant = std::variant<
    uint8_t,
    uint16_t,
    uint32_t,
    uint64_t,
    int8_t,
    int16_t,
    int32_t,
    int64_t,
    float,
    double
>;

template<typename T>
T estimateMaxPossibleCost(
    const std::vector<std::vector<T>>& weights,
    const bool search_for_cycle,
    std::mt19937 &psrng,
    int num_samples
) {
    const int n = weights.size();
    std::vector<int> path(n);
    std::iota(path.begin(), path.end(), 0);

    const auto calc_cost = [&]() -> T {
        T cur_cost = search_for_cycle
            ? weights[path.back()][path[0]]
            : (T)0;
        for (int i = 1; i < n; ++i) {
            cur_cost += weights[path[i - 1]][path[i]];
        }
        return cur_cost;
    };

    T best_cost_upper_bound = calc_cost();
    std::vector<int> best_path = path;
    for ( ; num_samples > 0; --num_samples) {
        random::permuteRandomly(path, psrng);
        T cost = calc_cost();
        if (cost < best_cost_upper_bound) {
            best_cost_upper_bound = cost;
            best_path = path;
        }
    }
    std::cout << "Estimated max possible cost "
              << std::fixed << std::setprecision(6)
              << best_cost_upper_bound
              << std::defaultfloat << std::endl
              << "for path: "<< std::endl;
    for (const auto& v : best_path) std::cout << v << " ";
    if (search_for_cycle) std::cout << best_path[0];
    std::cout << std::endl;
    return best_cost_upper_bound;
}

}  // detail namesspace


template<typename distance_t>
detail::cost_t_variant chooseCostType(
    const double precision,
    const distance_t max_possible_cost,
    const int num_points,
    const unsigned long long max_num_bytes,
    const bool do_not_prefer_int,
    const bool search_cycle,
    const bool is_symmetric,
    const bool cost_only,
    const int verbose = 1,
    const bool forbid_unsigned = false
) {
    // needed bits to fit precision * max_cost into an integer
    int needed_bits_cost_t_precision = max_possible_cost <= 1e-12 ?
        1 : std::ceil(std::log2(precision * max_possible_cost));

    // since using signed ints last bit is useless
    if (needed_bits_cost_t_precision == 8
        || needed_bits_cost_t_precision == 16
        || needed_bits_cost_t_precision == 32
        || needed_bits_cost_t_precision == 64
    ) {
        needed_bits_cost_t_precision += 1;
    }

    // maxB = 1B * path + xB * costs => x = (max - 1B * path) / costs
    const auto space = calcSpaceNeeded(num_points, search_cycle,
                                       is_symmetric, cost_only);
    if (verbose > 0) {
        std::cout << "Memory needed for path: " << space.first << " locs, for costs: "
                  << space.second << " locs." << std::endl;
    }
    int max_num_bytes_per_cost_t = std::min(
        (long long)sizeof(double),  // cap since more than 64bits are useless
        (long long)((max_num_bytes - space.first) / space.second)
    );

    if (verbose > 0) {
        std::cout << "Desired precision: " << precision << std::endl;
        std::cout << "Mem constraint for cost_t: " << (8LL * max_num_bytes_per_cost_t)
                  << " bits" << std::endl;
    }
    if (max_num_bytes_per_cost_t <= 0) {
        throw std::runtime_error("Not enough memory provided for any cost_t type.");
    }

    // round down to highest power of 2 it contains
    const int pow_bytes = std::ceil(std::log2(max_num_bytes_per_cost_t));
    if (max_num_bytes_per_cost_t < (1 << pow_bytes)) {
        max_num_bytes_per_cost_t = 1 << (pow_bytes - 1);
    }
    const int max_num_bits_cost_t = 8 * max_num_bytes_per_cost_t;
    const bool can_preserve_precision_int = needed_bits_cost_t_precision
                                          <= max_num_bits_cost_t;
    const int bits_to_use = can_preserve_precision_int
                          ? needed_bits_cost_t_precision
                          : max_num_bits_cost_t;
    if (verbose > 0) {
        std::cout << "Rounded to pow of 2 bytes mem constraint (capped at 64bits)"
                  << " for cost_t: " << max_num_bits_cost_t << " bits" << std::endl;
        std::cout << "Bits needed for an int to preserve precision: "
                  << needed_bits_cost_t_precision << std::endl;
    }

    // cannot fit precision * max_cost into an integer
    if (!can_preserve_precision_int || do_not_prefer_int) {
        if (verbose > 0) {
            std::cout << "Warning: cannot preserve precision." << std::endl;
        }
        if (max_num_bits_cost_t >= 64) {
            if (verbose > 0) {
                std::cout << "Selected cost_t = double" << std::endl;
            }
            return double{};
        }
        else if (max_num_bits_cost_t >= 32) {
            if (verbose > 0) {
                std::cout << "Selected cost_t = float" << std::endl;
            }
            return float{};
        }

        // use integer that there is enough memory for
        if (verbose > 0) {
            std::cout << "Warning: Using an integer; there is not"
                      << " enough memory for decimals." << std::endl;
        }
    }
    if (verbose > 0) {
        std::cout << "Bits used per cost_t: " << bits_to_use << std::endl;
    }
    if (bits_to_use > 32) {
        if (verbose > 0) {
            std::cout << "Selected cost_t = (u)int64_t" << std::endl;
        }
        return forbid_unsigned ? detail::cost_t_variant(int64_t{})
                               : detail::cost_t_variant(uint64_t{});
    }
    if (bits_to_use > 16) {
        if (verbose > 0) {
            std::cout << "Selected cost_t = (u)int32_t" << std::endl;
        }
        return forbid_unsigned ? detail::cost_t_variant(int32_t{})
                               : detail::cost_t_variant(uint32_t{});
    }
    if (bits_to_use > 8) {
        if (verbose > 0) {
            std::cout << "Selected cost_t = (u)int16_t" << std::endl;
        }
        return forbid_unsigned ? detail::cost_t_variant(int16_t{})
                               : detail::cost_t_variant(uint16_t{});
    }
    if (verbose > 0) {
        std::cout << "Selected cost_t = (u)int8_t" << std::endl;
    }
    return forbid_unsigned ? detail::cost_t_variant(int8_t{})
                           : detail::cost_t_variant(uint8_t{});
}

#endif
