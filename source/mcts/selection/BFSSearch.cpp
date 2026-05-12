//
// Created by bjorn on 9-5-2026.
//

#include <mcts/selection/BFSSearch.h>

std::shared_ptr<MCTSNode> BFSSearch::selectNode(state_merger* merger, const std::shared_ptr<MCTSNode>& root) const {
    std::shared_ptr<MCTSNode> node = root;

    while (!expansionRulePolicy->isExpandable(node) && !node->isTerminal()) {
        auto children = node->getChildren();
        if (children.empty()) break;

        auto childNode = selectionPolicy->select(children);

        // Means the node cannot be expanded
        if (childNode == nullptr) break;

        childNode->doRef(merger);
        node = childNode;
    }

    return node;
}
