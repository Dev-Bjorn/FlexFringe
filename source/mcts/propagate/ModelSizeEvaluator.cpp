//
// Created by bjorn on 6-5-2026.
//


#include <mcts/propagate/ModelSizeEvaluator.h>

double ModelSizeEvaluator::evaluate(const state_merger* merger) const {
    return merger->get_final_apta_size();
}

bool ModelSizeEvaluator::compare(const double score1, const double score2) const {
    return score1 < score2;
}
