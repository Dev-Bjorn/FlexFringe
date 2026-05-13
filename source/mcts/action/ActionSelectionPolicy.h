//
// Created by bjorn on 5-5-2026.
//

#ifndef FLEXFRINGE_ACTION_SELECTION_POLICY_H
#define FLEXFRINGE_ACTION_SELECTION_POLICY_H
#include <memory>
#include <refinement.h>
#include <config/MCTSConfig.h>

struct ActionSelectionPolicy {
    virtual ~ActionSelectionPolicy() = default;

    /**
     * Select a single refinement action during MCTS search from the available candidates.
     * @param refinements Merge and split refinement candidates
     * @param extendRefs Extend refinement candidates
     * @return A tuple consisting of the candidate list, and the index in the candidate list, where index == -1 indicates no action can be selected
     */
    virtual std::tuple<refinement_vector, int> action(const refinement_vector& refinements, const refinement_vector& extendRefs) = 0;
};

/**
 * Construct the action selection policy
 * @param policy The name of the action selection policy to create
 * @param config The configuration parameters for the action selection policy
 * @param isExpansion Indicates whether the policy is for expansion or rollout
 * @return A unique pointer to the created action selection policy
 */
std::unique_ptr<ActionSelectionPolicy> createActionPolicy(const std::string_view policy, const MCTSConfig& config, const bool isExpansion);

#endif //FLEXFRINGE_ACTION_SELECTION_POLICY_H
