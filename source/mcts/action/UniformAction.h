//
// Created by bjorn on 10-5-2026.
//

#ifndef FLEXFRINGE_UNIFORM_ACTION_SELECTION_H
#define FLEXFRINGE_UNIFORM_ACTION_SELECTION_H
#include <random>
#include <refinement.h>
#include <mcts/action/ActionSelectionPolicy.h>

/**
 * @brief Stochastic policy that selects refinements fully uniformly.
 */
struct UniformAction : ActionSelectionPolicy {
private:
    std::mt19937 rng;

public:
    /**
     * Constructs the policy with a fixed seed.
     * @param seed The seed for the random number generator
     */
    explicit UniformAction(const uint32_t seed) : rng(seed) {
    }

    std::tuple<refinement_vector, int> action(const refinement_vector& refinements, const refinement_vector& extendRefs) override;
};

#endif //FLEXFRINGE_UNIFORM_ACTION_SELECTION_H
