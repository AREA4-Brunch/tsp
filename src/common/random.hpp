#ifndef PERM_HPP
#define PERM_HPP

#include <iostream>
#include <utility>
#include <vector>
#include <random>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>

namespace random {

unsigned int genRandomSeed() {
    static std::random_device rd;
    unsigned int seed = rd();
    return seed;
}

/// @param seed - if 0 then function desides a seed
boost::random::mt19937 initPSRNG(unsigned int seed=0U, const int verbose=0) {
    if (seed == 0) seed = genRandomSeed();
    if (verbose > 0) std::cout << "Seed: " << seed << std::endl;
    boost::random::mt19937 mt(seed);
    return mt;
}

template<typename T>
void permuteRandomly(
    std::vector<T> &perm,
    boost::random::mt19937 &psrng,
    auto &&swap = [] (T &x, T &y) { std::swap(x, y); }
) {
    for (int i = perm.size() - 1; i > 0; --i) {
        boost::random::uniform_int_distribution<
            boost::random::mt19937::result_type
        > rand_idx(0, i);
        const int j = rand_idx(psrng);
        if (i != j) swap(perm[i], perm[j]);
    }
}

}  // namespace random


#endif
