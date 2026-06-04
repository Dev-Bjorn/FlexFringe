//
// Created by bjorn on 20-5-2026.
//


#include <mcts/propagate/RolloutLength.h>

double RolloutLengthEvaluator::evaluate(const state_merger*, const std::shared_ptr<MCTSNode>&, const refinement_vector& log) const {
    return log.size();
}

double NormalisedRolloutLengthEvaluator::evaluate(const state_merger*, const std::shared_ptr<MCTSNode>&, const refinement_vector& log) const {
    return normalise(log.size());
}

double NormalisedRolloutLengthEvaluator::normalise(const double value) const {
    return (value - 1) / (initialPTASize - 1);
}
