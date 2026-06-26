//
// Created by bjorn on 8-6-2026.
//

#include "ProgressiveWideningSearch.h"

bool ProgressiveWideningSearch::isSelected(const std::shared_ptr<MCTSNode>& node) const {
    auto visited = static_cast<double>(node->getChildren().size());
    if (visited == 0) return true;
    auto allowed = std::max(1.0, std::pow(node->getContext()->getVisits(), alpha));
    return visited < C * allowed;
}

std::shared_ptr<MCTSNode> ProgressiveWideningSearch::selectNode(state_merger* merger, const std::shared_ptr<MCTSNode>& root) const {
    std::shared_ptr<MCTSNode> node = root;

    while (!node->isTerminal()) {
        if (expansionRulePolicy->isExpandable(node) && isSelected(node)) break;

        auto children = node->getChildren();
        if (children.empty()) break;

        auto childNode = selectionPolicy->select(children);
        if (childNode == nullptr) break;

        childNode->doRef(merger);
        node = childNode;
    }

    return node;
}
