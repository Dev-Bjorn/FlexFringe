//
// Created by bjorn on 4-5-2026.
//

#ifndef FLEXFRINGE_NODE_SELECTION_POLICY_H
#define FLEXFRINGE_NODE_SELECTION_POLICY_H
#include <config/MCTSConfig.h>
#include <mcts/node/MCTSNode.h>

/**
 * @brief The policy that selects from a set of nodes.
 */
struct NodeSelectionPolicy {
    virtual ~NodeSelectionPolicy() = default;

    [[nodiscard]] virtual std::shared_ptr<MCTSNode> select(const std::vector<std::shared_ptr<MCTSNode> > &nodes) const = 0;

    /**
     * Construct the node data required for the selection policy.
     * @return The node data required for the selection policy.
     */
    [[nodiscard]] virtual std::shared_ptr<NodeData> createNodeData() const = 0;
};

/**
 * Create a node selection policy based on the given policy string and configuration.
 * @param policy The policy string.
 * @param config The configuration.
 * @return The created node selection policy.
 */
std::shared_ptr<NodeSelectionPolicy> createNodeSelectionPolicy(std::string_view policy, const MCTSConfig &config);

#endif //FLEXFRINGE_NODE_SELECTION_POLICY_H
