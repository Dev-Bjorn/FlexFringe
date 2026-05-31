//
// Created by bjorn on 20-5-2026.
//


#include <mcts/propagate/ExtendAmount.h>

double ExtendAmountEvaluator::evaluate(const state_merger*, const std::shared_ptr<MCTSNode>& r, const refinement_vector& log) const {
    int extends = 0;
    // auto node = r;
    // while (node->getParent() != nullptr) {
    //     if (node->getRefinement()->type() == 3) {
    //         extends++;
    //     }
    //     node = node->getParent();
    // }
    for (const auto& refinement : log) {
        if (refinement->type() == 3) {
            extends++;
        }
    }
    return extends;
}

bool ExtendAmountEvaluator::compare(const double score1, const double score2) const {
    return score1 < score2;
}
