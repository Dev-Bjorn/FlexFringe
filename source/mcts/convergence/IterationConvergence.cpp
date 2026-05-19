//
// Created by bjorn on 11-5-2026.
//

#include <mcts/convergence/IterationConvergence.h>

bool IterationConvergence::isConverged(const std::shared_ptr<MCTSNode>&, const refinement_vector&) {
    if (++iteration >= maxIteration) {
        LOG_S(INFO) << "Reached the maximum number of iterations";
        return true;
    }

    return false;
}
