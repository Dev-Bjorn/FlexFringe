//
// Created by bjorn on 11-5-2026.
//

#ifndef FLEXFRINGE_SCORE_IMPROVEMENT_CONVERGENCE_H
#define FLEXFRINGE_SCORE_IMPROVEMENT_CONVERGENCE_H
#include <mcts/convergence/ConvergencePolicy.h>

/**
 * @brief Convergence policy that terminates when the score does not improve for a certain number of iterations.
 */
struct ScoreImprovementConvergence : ConvergencePolicy {
private:
    int maxNoImprovementIterations;
    int iteration = 0;

public:
    explicit ScoreImprovementConvergence(std::shared_ptr<QualityEvaluation> qualityEvaluation, const int maxNoImprovementIterations) : ConvergencePolicy(std::move(qualityEvaluation)), maxNoImprovementIterations(maxNoImprovementIterations) {
    };

    bool isConverged(const state_merger *merger, const std::shared_ptr<MCTSNode> &expansion, const refinement_vector &refinements) override;
};

#endif //FLEXFRINGE_SCORE_IMPROVEMENT_CONVERGENCE_H
