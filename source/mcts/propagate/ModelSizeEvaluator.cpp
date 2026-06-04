//
// Created by bjorn on 6-5-2026.
//


#include <mcts/propagate/ModelSizeEvaluator.h>

double ModelSizeEvaluator::evaluate(const state_merger* merger, const std::shared_ptr<MCTSNode>&, const refinement_vector&) const {
    return merger->get_final_apta_size();
}


double NormalisedModelSizeEvaluator::evaluate(const state_merger* merger, const std::shared_ptr<MCTSNode>&, const refinement_vector&) const {
    return normalise(merger->get_final_apta_size());
}

double NormalisedModelSizeEvaluator::normalise(double value) const {
    return (value - 1) / (initialPTASize - 1);
}

