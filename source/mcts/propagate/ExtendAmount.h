//
// Created by bjorn on 20-5-2026.
//

#ifndef FLEXFRINGE_EXTEND_AMOUNT_H
#define FLEXFRINGE_EXTEND_AMOUNT_H
#include <mcts/node/MCTSNode.h>
#include <mcts/propagate/QualityEvaluation.h>

struct ExtendAmountEvaluator : QualityEvaluation {
    double evaluate(const state_merger* merger, const std::shared_ptr<MCTSNode>& rolloutNode, const refinement_vector& log) const;

    bool compare(double score1, double score2) const;
};

#endif //FLEXFRINGE_EXTEND_AMOUNT_H
