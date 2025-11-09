#ifndef PROBLEM_LOADER_HPP
#define PROBLEM_LOADER_HPP

#include <vector>
#include <complex>
#include <fstream>
#include "utils.hpp"

namespace detail {

template<typename T>
std::vector<std::complex<T>> loadPoints(
    std::istream &in,
    const std::string &point_format_str,
    const int num_points
) {
    std::vector<std::complex<T>> points;
    points.reserve(num_points);

    T x = (T) 0, y = (T) 0;
    for (int i = 0; i < num_points; ++i) {
        std::string line;
        if (!std::getline(in, line)) break;
        sscanf(line.c_str(), point_format_str.c_str(), &x, &y);
        points.push_back({ x, y });
    }

    return points;
}

template<typename distance_t, typename TIterator>
std::vector<std::vector<distance_t>> calcDistances(
    const TIterator points_start,
    const TIterator points_end,
    distance_t &min_dist,  // not including the 0s diagonal
    distance_t &max_dist  // not including the 0s diagonal
) {
    const int num_points = std::distance(points_start, points_end);
    std::vector<std::vector<distance_t>> distances(
        num_points,
        std::vector<distance_t> (num_points)
    );
    TIterator row = points_start;
    for (int i = 0, n = num_points; i < n; ++i, ++row) {
        distances[i][i] = (distance_t) 0;

        TIterator col = points_start + i + 1;
        for (int j = i + 1, m = num_points; j < m; ++j, ++col) {
            const distance_t cur_distance = (distance_t) std::abs(*row - *col);
            distances[j][i] = distances[i][j] = cur_distance;
            min_dist = std::min(min_dist, cur_distance);
            max_dist = std::max(max_dist, cur_distance);
        }
    }
    return distances;
}

template<typename distance_t>
std::vector<std::vector<distance_t>> loadWeightsMatrix(
    std::istream &in,
    const int num_points,
    distance_t &min_dist,  // not including the 0s diagonal
    distance_t &max_dist,  // not including the 0s diagonal
    bool &is_symmetric
) {
    min_dist = std::numeric_limits<distance_t>::max();
    max_dist = std::numeric_limits<distance_t>::min();
    std::string line;
    const std::string weights_section = "EDGE_WEIGHT_SECTION";
    std::string edge_weight_format = "FULL_MATRIX";

    in.clear();
    in.seekg(0);
    int dimension = 0;
    while (std::getline(in, line)) {
        if (line.find("EDGE_WEIGHT_FORMAT") != std::string::npos) {
            if (line.find("UPPER_ROW") != std::string::npos) {
                edge_weight_format = "UPPER_ROW";
            } else if (line.find("LOWER_ROW") != std::string::npos) {
                edge_weight_format = "LOWER_ROW";
            }
        } else if (line.find("DIMENSION") != std::string::npos) {
            dimension = std::atoi(
                line.substr(line.find_first_of("0123456789")).c_str()
            );
        }
        if (line.find(weights_section) != std::string::npos) {
            break;
        }
    }

    const bool is_sym = (edge_weight_format != "FULL_MATRIX");

    // determine iteration bounds based on format
    const auto get_bounds = [&] (int i) -> std::pair<int, int> {
        if (edge_weight_format == "UPPER_ROW") return { i + 1, dimension };
        if (edge_weight_format == "LOWER_ROW") return { 0, i };
        return { 0, dimension };  // FULL_MATRIX
    };

    std::vector<std::vector<distance_t>> distances(
        dimension,
        std::vector<distance_t>(dimension)
    );
    is_symmetric = true;
    for (int i = 0; i < dimension; ++i) {
        const auto [j_start, j_end] = get_bounds(i);
        for (int j = j_start; j < j_end; ++j) {
            distance_t value;
            in >> value;
            distances[i][j] = value;
            if (is_sym) {
                distances[j][i] = value;
            } else if (is_symmetric && j > i) {
                is_symmetric = (value == distances[j][i]);
            }
            if (i != j) {  // exclude diagonal
                min_dist = std::min(min_dist, value);
                max_dist = std::max(max_dist, value);
            }
        }
    }
    is_symmetric = is_symmetric || is_sym;
    if (dimension != num_points) {
        distances.resize(num_points);
        for (auto &row : distances) {
            row.resize(num_points);
        }
        if (!is_symmetric) {
            is_symmetric = true;
            for (int i = 0; i < num_points; ++i) {
                for (int j = i + 1; j < num_points; ++j) {
                    if (distances[i][j] != distances[j][i]) {
                        is_symmetric = false;
                        break;
                    }
                }
            }
        }
    }
    return distances;
}

} // detail namespace


template<typename point_t, typename distance_t>
std::vector<std::vector<distance_t>> loadDistances(
    const std::string &path_in_file,
    const std::string &point_format_str,
    const int num_points,
    distance_t &min_dist,
    distance_t &max_dist,
    bool &is_symmetric,
    const bool should_compute_distances = true
) {
    std::ifstream in_file(path_in_file);
    if (!in_file.is_open()) {
        std::cout << "Failed to open input file: "
                  << path_in_file << std::endl;
        throw std::runtime_error("Failed to open input file.");
    }
    if (should_compute_distances) {
        const std::vector<std::complex<point_t>> points
            = detail::loadPoints<point_t>(
                in_file,
                point_format_str,
                num_points
            );
        in_file.close();
        is_symmetric = true;
        return detail::calcDistances(
            points.begin(), points.end(), min_dist, max_dist
        );
    }
    return detail::loadWeightsMatrix<distance_t>(
        in_file, num_points, min_dist, max_dist,
        is_symmetric
    );
}

#endif
