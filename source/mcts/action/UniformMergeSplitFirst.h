//
// Created by bjorn on 5-5-2026.
//

#ifndef FLEXFRINGE_MERGE_SPLIT_FIRST_ACTION_SELECTION_H
#define FLEXFRINGE_MERGE_SPLIT_FIRST_ACTION_SELECTION_H
#include <random>
#include <refinement.h>
#include <mcts/action/ActionSelectionPolicy.h>

/**
 * @brief Stochastic policy that exhausts all merge/split refinements before considering extends.
 *
 * Selects uniformly at random from merge/split refinements until none remain, only then falling
 * back to uniform extend selection.
 */
struct UniformMergeSplitFirst : ActionSelectionPolicy {
private:
    std::mt19937 rng;

public:
    /**
     * Constructs the policy with a fixed seed.
     * @param seed The seed for the random number generator
     */
    explicit UniformMergeSplitFirst(const uint32_t seed) : rng(seed) {
    }

    std::tuple<refinement_vector, int> action(const refinement_vector& refinements, const refinement_vector& extendRefs) override;
};

#endif //FLEXFRINGE_MERGE_SPLIT_FIRST_ACTION_SELECTION_H
