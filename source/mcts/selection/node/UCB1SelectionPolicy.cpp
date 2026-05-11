//
// Created by bjorn on 4-5-2026.
//

#include <cstdint>
#include <cmath>
#include <memory>
#include <mcts/node/MCTSNode.h>
#include <mcts/propagate/data/LUCB1Data.h>
#include <mcts/selection/node/UCB1SelectionPolicy.h>


double UCB1SelectionPolicy::score(const std::shared_ptr<MCTSNode>& node) const {
    // Means we are in the root
    if (node->getParent() == nullptr) return std::numeric_limits<double>::infinity();

    const auto &ctx = static_cast<LUCB1NodeData&>(*node->getContext());
    // Means no expansion from this node happened.
    if (ctx.getVisits() == 0) return std::numeric_limits<double>::infinity();

    const auto &parentCtx = static_cast<LUCB1NodeData&>(*node->getParent()->getContext());
    return ctx.Q / ctx.getVisits() + C * std::sqrt(std::log(parentCtx.getVisits()) / ctx.getVisits());
}

std::shared_ptr<MCTSNode> UCB1SelectionPolicy::select(const std::vector<std::shared_ptr<MCTSNode>>& nodes) const {
    double bestScore = -std::numeric_limits<double>::infinity();
    std::shared_ptr<MCTSNode> bestNode = nullptr;

    for(const auto& child : nodes) {
        if (const double s = score(child); s > bestScore) {
            bestScore = s;
            bestNode = child;

            if (s == std::numeric_limits<double>::infinity()) break;
        }
    }

    return bestNode;
}

std::shared_ptr<NodeData> UCB1SelectionPolicy::createNodeData() const {
    return std::make_shared<LUCB1NodeData>();
}
