//
// Created by bjorn on 9-5-2026.
//

#ifndef FLEXFRINGE_BFS_SEARCH_SELECTION_H
#define FLEXFRINGE_BFS_SEARCH_SELECTION_H
#include <mcts/selection/SelectionSearchPolicy.h>

struct BFSSearch : SelectionSearchPolicy {
    explicit BFSSearch(
        std::shared_ptr<NodeSelectionPolicy> selectionPolicy,
        std::shared_ptr<ExpansionRulePolicy> expansionRulePolicy
    ) : SelectionSearchPolicy(std::move(selectionPolicy), std::move(expansionRulePolicy)) {
    }

    std::shared_ptr<MCTSNode> selectNode(state_merger* merger, const std::shared_ptr<MCTSNode>& root) const override;
};
#endif //FLEXFRINGE_BFS_SEARCH_SELECTION_H
