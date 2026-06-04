//
// Created by bjorn on 11-5-2026.
//

#ifndef FLEXFRINGE_SCORE_IMPROVEMENT_CONVERGENCE_H
#define FLEXFRINGE_SCORE_IMPROVEMENT_CONVERGENCE_H
#include <mcts/convergence/ConvergencePolicy.h>

/**
 * @brief Convergence policy that terminates when the score does not improve for a certain number of iterations.
 */
struct ScoreImprovementConvergence : ConvergencePolicy {
private:
    std::shared_ptr<QualityEvaluation> evaluator;
    unsigned int                       maxNoImprovementIterations;
    double                             bestScore;
    size_t                             iteration = 0;

public:
    explicit ScoreImprovementConvergence(
        const std::shared_ptr<QualityEvaluation> evaluator,
        const int                                maxNoImprovementIterations
    ) : evaluator(std::move(evaluator)), maxNoImprovementIterations(maxNoImprovementIterations), bestScore(evaluator->limit()) {
    };

    bool isConverged(const std::shared_ptr<MCTSNode>& expansion, const refinement_vector& refinements) override;
};

#endif //FLEXFRINGE_SCORE_IMPROVEMENT_CONVERGENCE_H
