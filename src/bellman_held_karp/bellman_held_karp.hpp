#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <vector>
#include <functional>
#include <span>

namespace detail {

std::vector<std::vector<unsigned long long>> binomialsMatr(
    const int max_n,
    int max_k
) {
    using ull = unsigned long long;
    if (max_k < 0) max_k = max_n;
    std::vector<std::vector<ull>> coefs(
        max_n + 1, std::vector<ull>(max_k + 1, 0)
    );
    for (int n = 0; n <= max_n; ++n) {
        coefs[n][0] = 1ULL;
        for (int k = 1; k <= std::min(n, max_k); ++k) {
            coefs[n][k] = coefs[n-1][k-1] + coefs[n-1][k];
        }
    }
    return coefs;
}

unsigned long long comb(const int n, int k) {
    if (k > n) return 0ULL;
    if (k * 2 > n) k = n - k;
    if (k == 0) return 1ULL;

    unsigned long long result = static_cast<unsigned long long>(n);
    for (int i = 2; i <= k; ++i) {
        result *= (n - i + 1);
        result /= i;
    }
    return result;
}

std::vector<unsigned long long> prevVertexStarts(
    const int n,
    const int max_k,
    const std::vector<std::vector<unsigned long long>> &binomials
) {
    std::vector<unsigned long long> prev_starts(max_k, 0ULL);
    for (int k = 3; k < max_k; ++k) {
        prev_starts[k] = prev_starts[k - 1]
                       + binomials[n][k - 1] * (n - (k - 1));
    }
    return prev_starts;
}

/// @param solution - changes to the found path iff find_path=true
/// @return min cost
template<
    typename T,
    bool is_symmetric,
    bool is_n_odd,
    bool has_no_neg_weights=true,
    bool find_path=true,
    typename vertex_t=uint8_t,
    typename set_t=uint64_t
>
T bellmanHeldKarp(
    std::vector<vertex_t> &solution,
    const std::vector<std::vector<T>> &weights,
    const bool end_in_starting_point,
    T best_cost = std::numeric_limits<T>::max()
) {
    using ull = unsigned long long;
    if (weights.size() == 0) {
        solution = {};
        return (T) 0;
    }
    if (weights.size() == 1) {
        if (end_in_starting_point) solution = { 0, 0 };
        else solution = { 0 };
        return end_in_starting_point ? weights[0][0] : (T) 0;
    }
    if (weights.size() == 2 && end_in_starting_point) {
        solution = { 0, 1, 0 };
        return weights[0][1] + weights[1][0];
    }

    constexpr T inf = std::numeric_limits<T>::max();
    const int n = end_in_starting_point ? weights.size() - 1
                                        : weights.size();
    if (static_cast<int> (std::numeric_limits<vertex_t>::max()) < n - 1) {
        throw std::runtime_error(
            "Cannot handle num of vertices V=" + std::to_string(n)
          + " with vertex_t's max value, required at least max value of "
          + std::to_string(n - 1)
        );
    }
    if (static_cast<int> (sizeof(set_t) * 8) < n) {
        throw std::runtime_error(
            "Cannot handle num of vertices V=" +  std::to_string(n)
          + "with set_t's num of bits, required at least "
          + std::to_string(n) + " bits."
        );
    }

    constexpr auto add_to_set = [] (const set_t set,
                                    const vertex_t v) -> set_t {
        return set | (static_cast<set_t>(1) << v);
    };
    constexpr auto remove_from_set = [] (const set_t set,
                                         const vertex_t v) -> set_t {
        return set ^ (static_cast<set_t>(1) << v);
    };
    const set_t all_vertices = sizeof(set_t) * 8 == n
                             ? ~(static_cast<set_t>(0))
                             : (static_cast<set_t>(1) << (n)) - 1;

    const auto get_other_half = [&add_to_set, all_vertices] (
        const set_t set, const vertex_t vertex
    ) -> set_t {
        return add_to_set(~set, vertex) & all_vertices;
    };

    // utilize cache since src is changing within fixed dst in nested loop
    const auto get_weight = [&weights] (
        const vertex_t s, const vertex_t d
    ) {
        if constexpr (is_symmetric) return weights[d][s];
        return weights[s][d];
    };

    constexpr auto store_sum_iflt = [] (const T x, const T y, T &c) {
        if constexpr (std::is_same_v<T, uint64_t>) {
            if (x < c && y < c - x) {  // faster for uint64_t only
                c = x + y;
                return true;
            }
        } else {
            const double sum = static_cast<double>(x)
                             + static_cast<double>(y);
            if (sum < static_cast<double>(c)) {
                c = static_cast<T>(sum);
                return true;
            }
        }
        return false;
    };

    const auto find_best_ending = [&store_sum_iflt, &get_weight] (
        const set_t set,
        const T * prev_cost_iter,
        const vertex_t dst,
        T &cost
    ) -> vertex_t {
        vertex_t best_prev = (vertex_t) 0;
        for (set_t src_bits = set;
             src_bits;
             src_bits &= src_bits - 1, ++prev_cost_iter
        ) {
            const vertex_t src = (vertex_t) __builtin_ctzll(src_bits);
            const T weight = get_weight(src, dst);
            if (store_sum_iflt(*prev_cost_iter, weight, cost)) {
                best_prev = src;
            }
        }
        return best_prev;
    };

    const int max_card = is_symmetric ? std::max(1, n / 2) : n - 1;
    const int big_cost_card = n / 2;
    const int small_cost_card = n <= 2 ? 0 : n / 2 + (is_symmetric ? -1 : 1);
    const std::vector<std::vector<ull>> bin_coef
        = detail::binomialsMatr(n, max_card);
    std::vector<T> costs_big(bin_coef[n][big_cost_card] * big_cost_card);
    std::vector<T> costs_small(bin_coef[n][small_cost_card] * small_cost_card);
    // prev_starts[cardinality_without_ending] = costs_prev segment start
    std::vector<ull> prev_starts;
    std::vector<vertex_t> best_previous_vertices;
    if constexpr (find_path) {
        prev_starts = detail::prevVertexStarts(n, max_card, bin_coef);
        best_previous_vertices.resize(prev_starts.back());
    }

    const auto get_cost_start = [&bin_coef] (const set_t set) -> ull {
        ull prev_rank = 0ULL;
        int v_idx = 0;
        for (set_t src_bits = set; src_bits; src_bits &= src_bits - 1) {
            prev_rank += bin_coef[__builtin_ctzll(src_bits)][++v_idx];
        }
        return prev_rank * v_idx;  // rank * cardinality
    };

    bool is_next_big = big_cost_card & 1;
    auto &first_row_costs = is_next_big ? costs_big : costs_small;
    for (int dst = 0; dst < n; ++dst) {
        first_row_costs[dst] = end_in_starting_point
                             ? weights[n][dst] : (T) 0;
    }

    set_t best_set = (set_t) 0;
    vertex_t best_left_end = (vertex_t) 0;
    vertex_t best_left_prev = (vertex_t) 0;
    vertex_t best_right_prev = (vertex_t) 0;
    vertex_t *best_previous_vertex = best_previous_vertices.data();
    for (int cardinality = 1; cardinality < max_card; ++cardinality) {
        is_next_big = !is_next_big;
        const bool do_store_prev =  cardinality > 1
                                 && cardinality < max_card - 1;

        T * const __restrict costs_next = is_next_big
                            ? costs_big.data() : costs_small.data();
        const T * const __restrict costs_prev = is_next_big
                            ? costs_small.data() : costs_big.data();
        const T * cost_prev = costs_prev;

        ull set_idx = bin_coef[n][cardinality];
        for (set_t set = (static_cast<set_t>(1) << cardinality) - 1;
             set_idx;
             --set_idx, cost_prev += cardinality
        ) {
            for (set_t dst_bits = ~set & all_vertices;
                 dst_bits;
                 dst_bits &= dst_bits - 1
            ) {
                const vertex_t dst = (vertex_t) __builtin_ctzll(dst_bits);
                const set_t added_dst_to_set = add_to_set(set, dst);
                ull next_rank = 0ULL;
                T left_best_cost = inf;
                vertex_t left_prev = (vertex_t) 0;
                int v_idx = 0;
                bool did_not_add = true;
                for (set_t src_bits = set;
                     src_bits;
                     src_bits &= src_bits - 1, ++cost_prev
                ) {
                    const vertex_t src = (vertex_t) __builtin_ctzll(src_bits);
                    const T weight = get_weight(src, dst);
                    const bool is_lt = store_sum_iflt(*cost_prev, weight,
                                                      left_best_cost);
                    if constexpr (find_path) {
                        if (is_lt) left_prev = src;
                    }
                    if (did_not_add && dst < src) {
                        next_rank += bin_coef[dst][++v_idx];
                        did_not_add = false;
                    }
                    next_rank += bin_coef[src][++v_idx];
                }
                if (did_not_add) next_rank += bin_coef[dst][++v_idx];
                cost_prev -= cardinality;

                const int next_dst_rank = __builtin_popcountll(
                    set & ((added_dst_to_set ^ set) - 1)
                );
                *(costs_next + next_rank * (cardinality + 1) + next_dst_rank)
                        = left_best_cost;
                if constexpr (find_path) {
                    if (do_store_prev) *(best_previous_vertex++) = left_prev;
                }
            }

            // Gosper's hack:
            const set_t c = set & -set;
            const set_t r = set + c;
            set = (((r ^ set) >> 2) / c) | r;
        }
    }

    for (int cardinality = max_card; cardinality <= max_card; ++cardinality) {
        is_next_big = !is_next_big;
        const T * const __restrict costs_prev = is_next_big
                                   ? costs_small.data() : costs_big.data();
        const T * const __restrict costs_prev_end
            = 1ULL + &(is_next_big ? costs_small.back() : costs_big.back());
        const T * cost_prev = costs_prev;

        ull set_idx = bin_coef[n][cardinality];
        if constexpr (is_symmetric && !is_n_odd) {
            set_idx /= 2;
        }
        for (set_t set = (static_cast<set_t>(1) << cardinality) - 1;
             set_idx;
             --set_idx, cost_prev += cardinality
        ) {
            for (set_t dst_bits = ~set & all_vertices;
                 dst_bits;
                 dst_bits &= dst_bits - 1
            ) {
                const vertex_t dst = (vertex_t) __builtin_ctzll(dst_bits);
                const set_t added_dst_to_set = add_to_set(set, dst);
                T left_best_cost;
                if constexpr (has_no_neg_weights) left_best_cost = best_cost;
                else                              left_best_cost = inf;
                vertex_t left_prev = (vertex_t) 0;
                for (set_t src_bits = set;
                     src_bits;
                     src_bits &= src_bits - 1, ++cost_prev
                ) {
                    const vertex_t src = (vertex_t) __builtin_ctzll(src_bits);
                    const T weight = get_weight(src, dst);
                    const bool is_better = store_sum_iflt(*cost_prev, weight,
                                                          left_best_cost);
                    if constexpr (find_path) {
                        if (is_better) left_prev = src;
                    }
                }
                cost_prev -= cardinality;

                if constexpr (is_symmetric) {
                    if constexpr (has_no_neg_weights) {
                        if (left_best_cost >= best_cost) continue;
                    }

                    const set_t right = get_other_half(added_dst_to_set, dst);
                    T other_half_cost = best_cost;
                    vertex_t right_prev = (vertex_t) 0;
                    if constexpr (is_n_odd) {  // compute dst connect second half
                        const set_t right_no_middle = remove_from_set(right, dst);
                        const T * right_cost_prev = costs_prev
                                                  + get_cost_start(right_no_middle);
                        if constexpr (has_no_neg_weights) {
                            other_half_cost -= left_best_cost;
                            if constexpr (std::is_floating_point_v<T>) {
                                other_half_cost += 1.;  // avoid precision err
                            }
                        }
                        right_prev = find_best_ending(
                            right_no_middle, right_cost_prev,
                            dst, other_half_cost
                        );
                    } else {  // n is even, already computed solution
                        const int prev_dst_rank
                            = __builtin_popcountll(right & ((1ULL << dst) - 1));
                        const ull prev_rank = cost_prev - costs_prev + cardinality;
                        other_half_cost = *(costs_prev_end - prev_rank + prev_dst_rank);
                    }

                    const bool is_new_best = store_sum_iflt(
                        other_half_cost, left_best_cost, best_cost
                    );
                    if constexpr (find_path) {
                        if (is_new_best) {
                            best_set = added_dst_to_set;
                            best_left_end = dst;
                            best_left_prev = left_prev;
                            if constexpr (is_n_odd) best_right_prev = right_prev;
                        }
                    }
                } else {  // asymmetric
                    const bool is_new_best = store_sum_iflt(
                        weights[dst][n], left_best_cost, best_cost
                    );
                    if constexpr (find_path) {
                        if (is_new_best) {
                            best_left_end = dst;
                            best_left_prev = left_prev;
                        }
                    }
                }
            }

            // Gosper's hack:
            const set_t c = set & -set;
            const set_t r = set + c;
            set = (((r ^ set) >> 2) / c) | r;
        }
    }

    if constexpr (!find_path) {
        return best_cost;
    }

    const auto get_set_ordinal = [&bin_coef] (const set_t set) -> ull {
        ull prev_rank = 0ULL;
        int v_idx = 0;
        for (set_t src_bits = set; src_bits; src_bits &= src_bits - 1) {
            prev_rank += bin_coef[__builtin_ctzll(src_bits)][++v_idx];
        }
        return prev_rank;
    };

    const T * const costs_prev = is_next_big ? costs_big.data()
                                             : costs_small.data();
    const auto get_prev = [&] (
        const int card_with_ending,
        const set_t with_ending,
        const set_t without_ending
    ) -> vertex_t {
        const ull segment_start = prev_starts[card_with_ending - 1];
        const ull set_rank = get_set_ordinal(without_ending)
                           * (n - (card_with_ending - 1));
        const int ending_rank = __builtin_popcountll(
            (~without_ending) & ((with_ending ^ without_ending) - 1)
        );
        return best_previous_vertices
            [segment_start + set_rank + ending_rank];
    };

    solution.resize(end_in_starting_point ? n + 2 : n);
    std::span<vertex_t> path(solution.data() + end_in_starting_point, n);
    const auto reconstruct_half = [&] (set_t half, int path_idx, const int dir) {
        if (path_idx < 0 || path_idx >= n) return;
        const T * const cost_prev = costs_prev + get_cost_start(half);
        T cost = inf;
        path[path_idx] = find_best_ending(
            half, cost_prev,
            path[path_idx - dir], cost
        );
        int cardinality = __builtin_popcountll(half);
        for (path_idx += dir;
             cardinality >= 3;
             path_idx += dir, --cardinality
        ) {
            const vertex_t cur_end = path[path_idx - dir];
            const set_t next_half = remove_from_set(half, cur_end);
            path[path_idx] = get_prev(cardinality, half, next_half);
            half = next_half;
        }
        if (path_idx >= 0 && path_idx < n) {
            half = remove_from_set(half, path[path_idx - dir]);
            path[path_idx] = (vertex_t) __builtin_ctzll(half);
        }
    };

    set_t left = is_symmetric ? best_set : all_vertices;
    set_t right = ~left & all_vertices;

    // reconstruct left half
    path[max_card] = best_left_end;
    path[max_card - 1] = best_left_prev;
    const int path_left_idx = max_card - 2;
    if (path_left_idx >= 0) {
        left = remove_from_set(left, best_left_end);
        left = remove_from_set(left, best_left_prev);
        reconstruct_half(left, path_left_idx, -1);
    }

    // reconstruct right half
    if constexpr (is_symmetric) {
        int path_right_idx = max_card + 1;
        if constexpr (is_n_odd) {
            path[path_right_idx++] = best_right_prev;
            right = remove_from_set(right, best_right_prev);
        }
        reconstruct_half(right, path_right_idx, 1);
    }

    if (end_in_starting_point) {
        solution[0] = solution.back() = n;
    }
    return best_cost;
}

template<typename T, typename vertex_t, typename set_t>
struct BHKDispatcher {
    template<bool is_symmetric, bool is_n_odd,
             bool has_no_neg_weights, bool find_path>
    static T call(
        std::vector<vertex_t> &solution,
        const std::vector<std::vector<T>> &weights,
        const bool end_in_starting_point,
        T best_cost
    ) {
        return bellmanHeldKarp<
            T, is_symmetric, is_n_odd, has_no_neg_weights,
            find_path, vertex_t, set_t
        >(solution, weights, end_in_starting_point, best_cost);
    }
};

}  // detail namesspace


/// @return (num mem locations for path, num mem locs for costs)
std::pair<uint64_t, uint64_t> calcSpaceNeeded(
    int n,
    const bool search_cycle,
    const bool is_symmetric,
    const bool cost_only=true
) {
    if (search_cycle) n--;
    if (n <= 1) return { 1ULL, 1ULL };
    const int max_card = is_symmetric ? std::max(1, n / 2) : n - 1;
    const auto binomials = detail::binomialsMatr(n, max_card);
    const int big_cost_card = n / 2;
    const int small_cost_card = n <= 2 ? 0 : n / 2 + (is_symmetric ? -1 : 1);
    const uint64_t costs = binomials[n][small_cost_card] * small_cost_card
                         + binomials[n][big_cost_card] * big_cost_card;
    if (cost_only) return { 0ULL, costs };
    const uint64_t path
        = detail::prevVertexStarts(n, max_card, binomials).back();
    return { path, costs };
}

/// @return (min_cost, vertices making up the path)
template<typename T, typename vertex_t=uint8_t, typename set_t=uint64_t>
T bellmanHeldKarp(
    std::vector<vertex_t> &solution,
    const std::vector<std::vector<T>> &weights,
    const bool end_in_starting_point,
    const bool is_symmetric,
    T best_cost = std::numeric_limits<T>::max(),
    bool has_no_neg_weights=true,
    bool find_path=true
) {
    const int n = end_in_starting_point ? weights.size() - 1
                                        : weights.size();
    const bool is_n_odd = n & 1;

    using Dispatcher = detail::BHKDispatcher<T, vertex_t, set_t>;
    #define BHK_CALL(sym, odd, noneg, path) \
        if ( is_symmetric == sym && is_n_odd == odd \
          && has_no_neg_weights == noneg && find_path == path) { \
            return Dispatcher::template call<sym, odd, noneg, path>( \
                solution, weights, end_in_starting_point, best_cost ); \
        }

    BHK_CALL(true, true, true, true);
    BHK_CALL(true, true, true, false);
    BHK_CALL(true, true, false, true);
    BHK_CALL(true, true, false, false);
    BHK_CALL(true, false, true, true);
    BHK_CALL(true, false, true, false);
    BHK_CALL(true, false, false, true);
    BHK_CALL(true, false, false, false);
    BHK_CALL(false, true, true, true);
    BHK_CALL(false, true, true, false);
    BHK_CALL(false, true, false, true);
    BHK_CALL(false, true, false, false);
    BHK_CALL(false, false, true, true);
    BHK_CALL(false, false, true, false);
    BHK_CALL(false, false, false, true);
    BHK_CALL(false, false, false, false);

    #undef BHK_CALL

    return (T) 0;  // should never happen
}


#endif
