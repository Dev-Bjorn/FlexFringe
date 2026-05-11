//
// Created by bjorn on 11-5-2026.
//

#ifndef FLEXFRINGE_SCORE_THRESHOLD_CONVERGENCE_H
#define FLEXFRINGE_SCORE_THRESHOLD_CONVERGENCE_H
#include <mcts/convergence/ConvergencePolicy.h>

/**
 * @brief Convergence policy that terminates when the score reaches a certain threshold.
 */
struct ScoreThresholdConvergence : ConvergencePolicy {
private:
    double threshold;

public:
    explicit ScoreThresholdConvergence(std::shared_ptr<QualityEvaluation> evaluator, const double threshold) : ConvergencePolicy(std::move(evaluator)), threshold(threshold) {
    }

    bool isConverged(const state_merger *merger, const std::shared_ptr<MCTSNode> &expansion, const refinement_vector &refinements) override;
};

#endif //FLEXFRINGE_SCORE_THRESHOLD_CONVERGENCE_H
