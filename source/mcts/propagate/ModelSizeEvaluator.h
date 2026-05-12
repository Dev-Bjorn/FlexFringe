//
// Created by bjorn on 6-5-2026.
//

#ifndef FLEXFRINGE_MODEL_SIZE_EVALUATOR_H
#define FLEXFRINGE_MODEL_SIZE_EVALUATOR_H
#include <mcts/propagate/QualityEvaluation.h>

struct ModelSizeEvaluator : QualityEvaluation {
    double evaluate(const state_merger* merger) const override;

    bool compare(double score1, double score2) const override;
};

#endif //FLEXFRINGE_MODEL_SIZE_EVALUATOR_H
