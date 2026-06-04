//
// Created by bjorn on 26-5-2026.
//

#ifndef FLEXFRINGE_INTERPOLATESIZEANDLENGTH_H
#define FLEXFRINGE_INTERPOLATESIZEANDLENGTH_H
#include <mcts/node/MCTSNode.h>
#include <mcts/propagate/QualityEvaluation.h>

struct Interpolate : QualityEvaluation {
private:
    double                             interpolationFactor;
    std::shared_ptr<QualityEvaluation> firstEvaluator;
    std::shared_ptr<QualityEvaluation> secondEvaluator;

public:
    explicit Interpolate(double interpolationFactor, const std::shared_ptr<QualityEvaluation>& firstEvaluator, const std::shared_ptr<QualityEvaluation>& secondEvaluator, GoalPtr goal) : QualityEvaluation(std::move(goal)), interpolationFactor(interpolationFactor), firstEvaluator(firstEvaluator), secondEvaluator(secondEvaluator) {
        assert(interpolationFactor >= 0.0 && interpolationFactor <= 1.0);
    };

    double evaluate(const state_merger* merger, const std::shared_ptr<MCTSNode>& rolloutNode, const refinement_vector& log) const override;
};

#endif //FLEXFRINGE_INTERPOLATESIZEANDLENGTH_H
