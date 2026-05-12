//
// Created by bjorn on 11-5-2026.
//

#include <mcts/convergence/ScoreImprovementConvergence.h>

bool ScoreImprovementConvergence::isConverged(const state_merger* merger, const std::shared_ptr<MCTSNode>& expansion, const refinement_vector& refinements) {
    iteration++;

    if (const double currentScore = evaluator->evaluate(merger); evaluator->compare(currentScore, bestScore)) {
        LOG_S(INFO) << "Score improvement detected, updating best score and refinements after " << iteration << " iterations.";
        bestScore       = currentScore;
        bestRefinements = refinements;
        bestNode        = expansion;
        iteration       = 0;
    }
    return iteration >= maxNoImprovementIterations;
}
