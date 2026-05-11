//
// Created by bjorn on 6-5-2026.
//

#include <mcts/action/FirstAction.h>

FirstAction::FirstAction() = default;

std::tuple<refinement_vector, int> FirstAction::action(const refinement_vector& refinements, const refinement_vector& extendRefs) {
    if (!refinements.empty()) {
        return {refinements, 0};
    }
    if (!extendRefs.empty()) {
        return {extendRefs, 0};
    }
    return std::make_tuple(refinement_vector{}, -1);
}
