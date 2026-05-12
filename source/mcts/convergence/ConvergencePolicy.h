//
// Created by bjorn on 11-5-2026.
//

#ifndef FLEXFRINGE_CONVERGENCE_POLICY_H
#define FLEXFRINGE_CONVERGENCE_POLICY_H
#include <state_merger.h>
#include <mcts/node/MCTSNode.h>
#include <mcts/propagate/QualityEvaluation.h>

struct ConvergencePolicy {
protected:
    std::shared_ptr<QualityEvaluation> evaluator;
    size_t                             iteration = 0;
    int                                bestScore = std::numeric_limits<int>::max();
    std::shared_ptr<MCTSNode>          bestNode;
    refinement_vector                  bestRefinements;

public:
    /**
     * Create a convergence policy with the specified quality evaluation strategy.
     * @param evaluator The quality evaluation strategy for convergence detection.
     */
    explicit ConvergencePolicy(std::shared_ptr<QualityEvaluation> evaluator) : evaluator(std::move(evaluator)) {
    }

    virtual ~ConvergencePolicy() = default;

    /**
     * Check whether the MCTS algorithm has converged based on the provided state merger, expansion node, and rollout refinements.
     * @param merger The state merger used for merging states.
     * @param expansion The expansion node being considered for convergence.
     * @param refinements The vector of refinements applied during the rollout MCTS process.
     * @return True if convergence is detected, false otherwise.
     */
    virtual bool isConverged(const state_merger* merger, const std::shared_ptr<MCTSNode>& expansion, const refinement_vector& refinements) = 0;

    /**
     * The best expansion node.
     */
    [[nodiscard]] inline std::shared_ptr<MCTSNode> getBestNode() const { return bestNode; };

    /**
     * The best rollout refinement log in the order from the best node to the leaf.
     */
    [[nodiscard]] inline const refinement_vector& getBestRefinement() const { return bestRefinements; };
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
