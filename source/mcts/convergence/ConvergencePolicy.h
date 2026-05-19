//
// Created by bjorn on 11-5-2026.
//

#ifndef FLEXFRINGE_CONVERGENCE_POLICY_H
#define FLEXFRINGE_CONVERGENCE_POLICY_H
#include <state_merger.h>
#include <mcts/node/MCTSNode.h>
#include <mcts/propagate/QualityEvaluation.h>

struct ConvergencePolicy {
    virtual ~ConvergencePolicy() = default;

    /**
     * Check whether the MCTS algorithm has converged based on the provided state merger, expansion node, and rollout refinements.
     * @param expansion The expansion node being considered for convergence.
     * @param refinements The vector of refinements applied during the rollout MCTS process.
     * @return True if convergence is detected, false otherwise.
     */
    virtual bool isConverged(const std::shared_ptr<MCTSNode>& expansion, const refinement_vector& refinements) = 0;
};

/**
 * Create a convergence policy based on the specified policy type and configuration.
 * @param policy The type of convergence policy to create.
 * @param qualityEvaluation The quality evaluation strategy for convergence detection.
 * @param config The configuration parameters for the convergence policy.
 * @return A unique pointer to the created convergence policy.
 */
std::unique_ptr<ConvergencePolicy> createConvergencePolicy(const std::string_view policy, std::shared_ptr<QualityEvaluation> qualityEvaluation, const MCTSConfig& config);

#endif //FLEXFRINGE_CONVERGENCE_POLICY_H
