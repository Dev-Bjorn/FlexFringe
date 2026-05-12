//
// Created by bjorn on 11-5-2026.
//

#include <mcts/convergence/ScoreThresholdConvergence.h>

bool ScoreThresholdConvergence::isConverged(const state_merger* merger, const std::shared_ptr<MCTSNode>& expansion, const refinement_vector& refinements) {
    iteration++;
    const double currentScore = evaluator->evaluate(merger);
    if (currentScore >= threshold) return true;

    if (evaluator->compare(currentScore, bestScore)) {
        LOG_S(INFO) << "Score threshold detected, updating best score and refinements at iteration: " << iteration;
        bestScore       = currentScore;
        bestNode        = expansion;
        bestRefinements = refinements;
    }

    return false;
}
