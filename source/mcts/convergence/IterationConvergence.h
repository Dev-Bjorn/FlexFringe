//
// Created by bjorn on 11-5-2026.
//

#ifndef FLEXFRINGE_ITERATION_CONVERGENCE_POLICY_H
#define FLEXFRINGE_ITERATION_CONVERGENCE_POLICY_H
#include <mcts/convergence/ConvergencePolicy.h>

/**
 * @brief Convergence policy that terminates when reaching a fixed number of iterations.
 */
struct IterationConvergence : ConvergencePolicy {
private:
    unsigned int maxIteration;

public:
    explicit IterationConvergence(const unsigned int maxIteration) : maxIteration(maxIteration) {
    };

    bool isConverged(const std::shared_ptr<MCTSNode>& expansion, const refinement_vector& refinements) override;
};

#endif //FLEXFRINGE_ITERATION_CONVERGENCE_POLICY_H
