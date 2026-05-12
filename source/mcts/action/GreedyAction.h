//
// Created by bjorn on 6-5-2026.
//

#ifndef FLEXFRINGE_GREEDY_ACTION_SELECTION_H
#define FLEXFRINGE_GREEDY_ACTION_SELECTION_H
#include <functional>
#include <mcts/Refinements.h>
#include <mcts/action/ActionSelectionPolicy.h>

/**
 * @brief Deterministic policy that always selects the best refinement action.
 */
struct GreedyAction : ActionSelectionPolicy {
private:
    const std::function<bool(refinement*, refinement*)> comparator;

public:
    /**
     * @brief Construct the greedy action selection policy.
     * @tparam Comparator The type comparator used to compare refinement actions.
     * @param comparator A comparator function to which returns true when the first refinement parameter is better than
     * the second refinement parameter.
     */
    template<RefinementComparator Comparator>
    explicit GreedyAction(Comparator comparator) : comparator(comparator) {
    }

    explicit GreedyAction() : GreedyAction(score_compare{}) {
    }

    std::tuple<refinement_vector, int> action(const refinement_vector& refinements, const refinement_vector& extendRefs) override;
};

#endif //FLEXFRINGE_GREEDY_ACTION_SELECTION_H
