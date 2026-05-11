//
// Created by bjorn on 6-5-2026.
//

#ifndef FLEXFRINGE_NODE_DATA_FACTORY_HPP
#define FLEXFRINGE_NODE_DATA_FACTORY_HPP
#include <memory>
#include <mcts/propagate/data/NodeData.h>
#include <mcts/selection/node/NodeSelection.h>

/**
 * @brief Factory for creating node data instances.
 */
struct NodeDataFactory {
private:
    std::shared_ptr<NodeSelectionPolicy> policy;

public:
    explicit NodeDataFactory(std::shared_ptr<NodeSelectionPolicy> policy) : policy(std::move(policy)) {
    }

    /**
     * Construct the node data.
     */
    std::shared_ptr<NodeData> create() const;
};


#endif //FLEXFRINGE_NODE_DATA_FACTORY_HPP
