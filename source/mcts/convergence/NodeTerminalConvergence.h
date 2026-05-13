//
// Created by bjorn on 12-5-2026.
//

#ifndef FLEXFRINGE_NODETERMINALCONVERGENCE_H
#define FLEXFRINGE_NODETERMINALCONVERGENCE_H
#include <mcts/convergence/ConvergencePolicy.h>

struct NodeTerminalConvergence : ConvergencePolicy {
    explicit NodeTerminalConvergence()  {
    }

    bool isConverged(const std::shared_ptr<MCTSNode>& expansion, const refinement_vector& refinements) override;
};

#endif //FLEXFRINGE_NODETERMINALCONVERGENCE_H
