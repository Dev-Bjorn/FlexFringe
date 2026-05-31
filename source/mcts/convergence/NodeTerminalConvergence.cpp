//
// Created by bjorn on 12-5-2026.
//

#include <mcts/convergence/NodeTerminalConvergence.h>

bool NodeTerminalConvergence::isConverged(const std::shared_ptr<MCTSNode>& expansion, const refinement_vector&) {
    if (expansion->isTerminal()) {
        LOG_S(INFO) << "Node is terminal";
        return true;
    }
    return false;
}
