//
// Created by bjorn on 11-5-2026.
//

#include <mcts/convergence/IterationConvergence.h>

bool IterationConvergence::isConverged(const state_merger * merger, const std::shared_ptr<MCTSNode>& expansion, const refinement_vector& refinements) {
    if (iteration >= maxIteration) {
        return true;
    }

    if (const double x = evaluator->evaluate(merger); evaluator->compare(x, bestScore)) {
        LOG_S(INFO) << "Iteration detected, updating best score and refinements at iteration: " << iteration;
        this->bestScore = x;
        this->bestNode = expansion;
        this->bestRefinements = refinements;
    }

    iteration++;
    return false;
}
