//
// Created by bjorn on 20-5-2026.
//


#include <mcts/propagate/Interpolate.h>

double Interpolate::evaluate(const state_merger* merger, const std::shared_ptr<MCTSNode>& node, const refinement_vector& log) const {
    return (interpolationFactor) * firstEvaluator->evaluate(merger, node, log) + (1 - interpolationFactor) * secondEvaluator->evaluate(merger, node, log);
}


