//
// Created by bjorn on 9-5-2026.
//

#ifndef FLEXFRINGE_CHILD_VS_PARENT_SEARCH_SELECTION_POLICY_HPP
#define FLEXFRINGE_CHILD_VS_PARENT_SEARCH_SELECTION_POLICY_HPP
#include <mcts/selection/SelectionSearchPolicy.h>

/**
 * @brief ChildVSParentSearch is a selection policy that selects a child node if it has a better value than its parent.
 */
struct ChildVSParentSearch : SelectionSearchPolicy {
    explicit ChildVSParentSearch(
        std::shared_ptr<NodeSelectionPolicy> selectionPolicy,
        std::shared_ptr<ExpansionRulePolicy> expansionRulePolicy
    ) : SelectionSearchPolicy(std::move(selectionPolicy), std::move(expansionRulePolicy)) {
    };

    std::shared_ptr<MCTSNode> selectNode(state_merger *merger, const std::shared_ptr<MCTSNode>& root) const override;
};

#endif //FLEXFRINGE_CHILD_VS_PARENT_SEARCH_SELECTION_POLICY_HPP
