//
// Created by bjorn on 8-5-2026.
//

#include <mcts/expansion/SingleExtendRule.h>

bool SingleExtendRule::isExpandable(const std::shared_ptr<MCTSNode>& node) {
    if (node->getRefinements().empty() && node->getExtendRefinements().empty()) return false;
    if (!node->getRefinements().empty()) return node->getUnvisitedRefinements().size() > 0;
    if (node->getChildren().size() == 0) return true;
    return false;
}
