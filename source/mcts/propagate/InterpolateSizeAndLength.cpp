//
// Created by bjorn on 20-5-2026.
//


#include <mcts/propagate/InterpolateSizeAndLength.h>

double InterpolateSizeAndLength::evaluate(const state_merger* merger, const std::shared_ptr<MCTSNode>& node, const refinement_vector& log) const {
    return (1 - interpolationFactor) * firstEvaluator->evaluate(merger, node, log) + interpolationFactor * secondEvaluator->evaluate(merger, node, log);
}

bool InterpolateSizeAndLength::compare(const double score1, const double score2) const {
    return score1 < score2;
}
