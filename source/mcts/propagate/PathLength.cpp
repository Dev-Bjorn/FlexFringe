//
// Created by bjorn on 20-5-2026.
//

#include <mcts/propagate/PathLength.h>

double PathLengthEvaluator::evaluate(const state_merger*, const std::shared_ptr<MCTSNode>& node, const refinement_vector& log) const {
    return log.size() + node->getHeight();
}

bool PathLengthEvaluator::compare(const double score1, const double score2) const {
    return score1 < score2;
}

