//
// Created by bjorn on 5-5-2026.
//

#include <random>
#include <mcts/Random.h>

unsigned int resolveSeed(const int seed) {
    if (seed == -1) return std::random_device{}();
    if (seed < 0) return -seed;
    return seed;
}
