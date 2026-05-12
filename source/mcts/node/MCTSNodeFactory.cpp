//
// Created by bjorn on 6-5-2026.
//


#include <mcts/node/MCTSNodeFactory.h>

std::shared_ptr<MCTSNode> MCTSNodeFactory::create(refinement* ref, int dfaSize, const refinement_vector& possibleRefs, const refinement_vector& extendRefs, std::shared_ptr<MCTSNode> parent) {
    return std::make_shared<MCTSNode>(currentId++, contextFactory->create(), ref, dfaSize, possibleRefs, extendRefs, parent);
}

std::shared_ptr<MCTSNode> MCTSNodeFactory::createMCTSNode(refinement* ref, int dfaSize, const refinement_vector& possibleRefs, const refinement_vector& extendRefs, std::shared_ptr<MCTSNode> parent) {
    auto mcts_node = std::make_shared<MCTSNode>(currentId++, contextFactory->create(), ref, dfaSize, possibleRefs, extendRefs, parent);
    mcts_node->annotate(AlgorithmType::mcts);
    return mcts_node;
}

std::shared_ptr<MCTSNode> MCTSNodeFactory::createRoot(int dfaSize, const refinement_vector& possibleRefs, const refinement_vector& extendRefs) const {
    return std::make_shared<MCTSNode>(0, contextFactory->create(), nullptr, dfaSize, possibleRefs, extendRefs, nullptr);
}
