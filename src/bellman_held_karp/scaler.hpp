#ifndef SCALER_HPP
#define SCALER_HPP

#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include "utils.hpp"


template<typename dst_t, typename src_t>
std::vector<std::vector<dst_t>> recastMatrix(
    const std::vector<std::vector<src_t>> &matr
) {
    std::vector<std::vector<dst_t>> new_matr(matr.size());
    for (int i = 0, n = matr.size(); i < n; ++i) {
        new_matr[i].resize(matr[i].size());
        for (int j = 0; j < n; ++j) {
            new_matr[i][j] = static_cast<dst_t>(matr[i][j]);
        }
    }
    return new_matr;
}

template<typename cost_t, typename distance_t>
std::vector<std::vector<cost_t>> scaleAndNormalize(
    const std::vector<std::vector<distance_t>>& weights,
    const distance_t min_dist,
    const distance_t max_cost_norm,
    double precision,
    const bool do_round,
    const int verbose = 0
) {
    const cost_t inf = std::numeric_limits<cost_t>::max();
    double scaling_factor = 1.;
    int rescale_try = 1, max_tries = 100;

    const double rounding_err = do_round ? 1. * weights.size() : 0.;
    const auto is_precision_good = [&] () {
        scaling_factor = (double) inf
                / (rounding_err + max_cost_norm + 1. / precision);
        const double cur_dbl = scaling_factor * max_cost_norm;
        const cost_t cur = static_cast<cost_t>(
            do_round ? std::round(cur_dbl) : cur_dbl
        );
        const double next_dbl = scaling_factor
                * (max_cost_norm + 1. / precision);
        const cost_t next = static_cast<cost_t>(
            do_round ? std::round(next_dbl) : next_dbl
        );
        return cur < next && next <= inf && cur < inf
            && cur_dbl < next_dbl && next_dbl <= (double) inf
            && cur + rounding_err < inf;
    };

    for ( ; rescale_try <= max_tries; ++rescale_try) {
        if (is_precision_good()) break;
        precision /= 2.;
    }
    if (rescale_try >= max_tries) {
        const std::string msg
            = "Could not find a scaling factor to fit"
              " max_cost_norm into inf.";
        throw std::runtime_error(msg);
    }
    double start = precision;
    for (double end = precision * 2; start < end - 1; ) {
        precision = start + (end - start) / 2.;
        if (is_precision_good()) start = precision;
        else end = precision;
    }
    precision = start;
    scaling_factor = (double) inf
                   / (rounding_err + max_cost_norm + 1. / precision);
    if (rescale_try > 1 && verbose > 0) {
        std::cout << "Requested precision is too high;"
                  << "managed to differentiate"
                  << " costs by: " << std::fixed << std::setprecision(6)
                  << 1. / precision << std::defaultfloat << std::endl;
    }
    std::vector<std::vector<cost_t>> scaled_weights(weights.size());
    for (int i = 0, n = weights.size(); i < n; ++i) {
        scaled_weights[i].resize(weights[i].size());
        for (int j = 0, m = weights[i].size(); j < m; ++j) {
            if (weights[i][j] <= min_dist) {
                scaled_weights[i][j] = (cost_t) 0;
                continue;
            }
            if (weights[i][j] - min_dist > max_cost_norm) {
                scaled_weights[i][j] = inf;
                continue;
            }
            const double normalized = scaling_factor
                                    * (weights[i][j] - min_dist);
            if (normalized >= (double) inf) {
                scaled_weights[i][j] = inf;
                continue;
            }
            scaled_weights[i][j] = static_cast<cost_t>(
                do_round ? std::round(normalized) : normalized
            );
        }
    }
    if (verbose > 0) {
        std::cout << "Distances:" << std::endl;
        utils::displayMatrix(weights);
        std::cout << "Scaled Distances:" << std::endl;
        utils::displayMatrix(recastMatrix<double>(scaled_weights));
    }
    return scaled_weights;
}

#endif
