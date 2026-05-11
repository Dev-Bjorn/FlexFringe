//
// Created by bjorn on 11-5-2026.
//

#ifndef FLEXFRINGE_ITERATION_CONVERGENCE_POLICY_H
#define FLEXFRINGE_ITERATION_CONVERGENCE_POLICY_H
#include <mcts/convergence/ConvergencePolicy.h>

/**
 * @brief Convergence policy that terminates when reaching a fixed number of iterations.
 */
struct IterationConvergence : ConvergencePolicy {
private:
    unsigned int maxIteration;
    unsigned int iteration = 0;

public:
    explicit IterationConvergence(std::shared_ptr<QualityEvaluation> qualityEvaluation, const unsigned int maxIteration) : ConvergencePolicy(std::move(qualityEvaluation)), maxIteration(maxIteration) {
    };

    bool isConverged(const state_merger *merger, const std::shared_ptr<MCTSNode> &expansion, const refinement_vector &refinements) override;
};

#endif //FLEXFRINGE_ITERATION_CONVERGENCE_POLICY_H
