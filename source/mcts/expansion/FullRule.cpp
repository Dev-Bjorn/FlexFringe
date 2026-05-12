//
// Created by bjorn on 8-5-2026.
//

#include "FullRule.h"

bool FullRule::isExpandable(const std::shared_ptr<MCTSNode>& node) {
    return (node->getUnvisitedExtendRefinements().size() + node->getUnvisitedRefinements().size()) > 0;
}
