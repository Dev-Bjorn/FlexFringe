//
// Created by bjorn on 6-5-2026.
//

#include <limits>
#include <mcts/action/GreedyAction.h>

std::tuple<refinement_vector, int> GreedyAction::action(const refinement_vector& refinements, const refinement_vector& extendRefs) {
    if (refinements.empty() && extendRefs.empty()) return {refinement_vector{}, -1};
    if (refinements.empty()) return {extendRefs, 0};

    size_t bestIndex = 0;

    for (size_t i = 1; i < refinements.size(); ++i) {
        if (auto ref = refinements.at(i); comparator(ref, refinements.at(bestIndex))) {
            bestIndex = i;
        }
    }

    return {refinements, bestIndex};
}
