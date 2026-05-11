//
// Created by bjorn on 6-5-2026.
//

#ifndef FLEXFRINGE_WEIGHTED_MERGE_SPLIT_FIRST_ACTION_SELECTION_H
#define FLEXFRINGE_WEIGHTED_MERGE_SPLIT_FIRST_ACTION_SELECTION_H
#include <random>
#include <mcts/action/ActionSelectionPolicy.h>

/**
 * @brief Stochastic policy that selects refinements proportionally to their score.
 *
 * Selects proportionally to the refinement score at random from merge/split refinements until none remain, only then falling
 * back to uniform extend selection.
 */
struct WeightedMergeSplitFirst : ActionSelectionPolicy {
private:
    std::mt19937 rng;

public:
    /**
     * Constructs the policy with a fixed seed.
     * @param seed The seed for the random number generator
     */
    explicit WeightedMergeSplitFirst(const uint32_t seed) : rng(seed) {
    }

    std::tuple<refinement_vector, int> action(const refinement_vector &refinements, const refinement_vector &extendRefs) override;
};


#endif //FLEXFRINGE_WEIGHTED_MERGE_SPLIT_FIRST_ACTION_SELECTION_H
