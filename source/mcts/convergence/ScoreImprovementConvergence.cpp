//
// Created by bjorn on 11-5-2026.
//

#include <mcts/convergence/ScoreImprovementConvergence.h>

bool ScoreImprovementConvergence::isConverged(const std::shared_ptr<MCTSNode>& expansion, const refinement_vector& refinements) {
    iteration++;

    if (evaluator->compare(expansion->getScore(), bestScore)) {
        LOG_S(INFO) << "Score improvement detected, refinements after " << iteration << " iterations.";
        bestScore       = expansion->getScore();
        iteration       = 0;
    }
    if (iteration >= maxNoImprovementIterations) {
        LOG_S(INFO) << "Max number of improvement iterations reached, refinements after " << iteration << " iterations.";
        return true;
    }
    return false;
}
