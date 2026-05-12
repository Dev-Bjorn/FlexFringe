//
// Created by bjorn on 6-5-2026.
//

#ifndef FLEXFRINGE_MCTS_NODE_FACTORY_H
#define FLEXFRINGE_MCTS_NODE_FACTORY_H
#include <memory>
#include <mcts/node/MCTSNode.h>
#include <mcts/propagate/data/NodeDataFactory.h>

/**
 * @brief Factory for creating MCTS nodes with associated node data.
 */
struct MCTSNodeFactory {
private:
    std::unique_ptr<NodeDataFactory> contextFactory;
    int                              currentId = 1;

public:
    explicit MCTSNodeFactory(std::unique_ptr<NodeDataFactory> factory) : contextFactory(std::move(factory)) {
    };

    /**
     * Create the root node for the MCTS tree.
     * @param dfaSize The size of the DFA associated with the node.
     * @param possibleRefs A vector of possible refinements for the node.
     * @param extendRefs A vector of extensions for the node.
     * @return A shared pointer to the created root node.
     */
    std::shared_ptr<MCTSNode> createRoot(
        int                      dfaSize,
        const refinement_vector& possibleRefs,
        const refinement_vector& extendRefs
    ) const;

    /**
     * Create a child node for the MCTS tree.
     * @param ref The refinement associated with the node.
     * @param dfaSize The size of the DFA associated with the node.
     * @param possibleRefs A vector of possible refinements for the node.
     * @param extendRefs A vector of extensions for the node.
     * @param parent The parent node of the created node.
     * @return A shared pointer to the created child node.
     */
    std::shared_ptr<MCTSNode> create(
        refinement*               ref,
        int                       dfaSize,
        const refinement_vector&  possibleRefs,
        const refinement_vector&  extendRefs,
        std::shared_ptr<MCTSNode> parent
    );

    std::shared_ptr<MCTSNode> createMCTSNode(refinement* ref, int dfaSize, const refinement_vector& possibleRefs, const refinement_vector& extendRefs, std::shared_ptr<MCTSNode> parent);
};

#endif //FLEXFRINGE_MCTS_NODE_FACTORY_H
