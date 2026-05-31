//
// Created by bjorn on 20-5-2026.
//


#include <mcts/propagate/RolloutLength.h>

double RolloutLengthEvaluator::evaluate(const state_merger* merger, const std::shared_ptr<MCTSNode>& node, const refinement_vector& log) const {
    return log.size() + merger->get_final_apta_size();
}

bool RolloutLengthEvaluator::compare(const double score1, const double score2) const {
    return score1 < score2;
}
