//
// Created by bjorn on 12-5-2026.
//

#ifndef FLEXFRINGE_WEIGHTED_ACTION_H
#define FLEXFRINGE_WEIGHTED_ACTION_H
#include <random>
#include <mcts/action/ActionSelectionPolicy.h>

/**
 * @brief Stochastic policy that selects refinements fully weighted.
 */
struct WeightedAction : ActionSelectionPolicy {
private:
    std::mt19937 rng;
    std::string weightExtendScoring = "uniform";

    double scoreExtend(const refinement_vector& refinements) const;
public:
    /**
     * Constructs the policy with a fixed seed.
     * @param seed The seed for the random number generator
     * @param weightExtendScoring The scoring method used for extends when no static scoring is available
     */
    explicit WeightedAction(const uint32_t seed, const std::string_view weightExtendScoring) : rng(seed), weightExtendScoring(weightExtendScoring) {
    }

    std::tuple<refinement_vector, int> action(const refinement_vector& refinements, const refinement_vector& extendRefs) override;
};

#endif //FLEXFRINGE_WEIGHTED_ACTION_H
