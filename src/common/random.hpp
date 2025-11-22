#ifndef PERM_HPP
#define PERM_HPP

#include <random>
#include <iostream>
#include <utility>

namespace random {

unsigned int genRandomSeed() {
    static std::random_device rd;
    unsigned int seed = rd();
    return seed;
}

/// @param seed - if 0 then function desides a seed
std::mt19937 initPSRNG(unsigned int seed=0U, const int verbose=0) {
    if (seed == 0) seed = genRandomSeed();
    if (verbose > 0) std::cout << "Seed: " << seed << std::endl;
    std::mt19937 mt(seed);
    return mt;
}

template<typename T>
void permuteRandomly(std::vector<T>& perm, std::mt19937& psrng) {
    for (int i = perm.size() - 1; i > 0; --i) {
        std::uniform_int_distribution<std::mt19937::result_type>
            rand_idx(0, i);
        const int j = rand_idx(psrng);
        if (i != j) std::swap(perm[i], perm[j]);
    }
}

}  // namespace random


#endif
