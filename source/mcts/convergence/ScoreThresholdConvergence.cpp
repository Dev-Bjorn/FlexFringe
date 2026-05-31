//
// Created by bjorn on 11-5-2026.
//

#include <mcts/convergence/ScoreThresholdConvergence.h>

bool ScoreThresholdConvergence::isConverged(const std::shared_ptr<MCTSNode>& expansion, const refinement_vector&) {
    if (expansion->getScore() >= threshold) {
        LOG_S(INFO) << "Score threshold reached";
        return true;
    }

    return false;
}
