//
// Created by bjorn on 6-5-2026.
//

#ifndef FLEXFRINGE_FIRST_ACTION_SELECTION_H
#define FLEXFRINGE_FIRST_ACTION_SELECTION_H
#include <mcts/action/ActionSelectionPolicy.h>

/**
 * @brief Deterministic policy that always selects the first available candidate refinement action.
 *
 * Checks merge/split refinements first, then extend refinements.
 */
struct FirstAction : ActionSelectionPolicy {
public:
    explicit FirstAction();
    std::tuple<refinement_vector, int> action(const refinement_vector& refinements, const refinement_vector& extendRefs) override;
};

#endif //FLEXFRINGE_FIRST_ACTION_SELECTION_H
