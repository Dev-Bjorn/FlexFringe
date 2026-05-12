//
// Created by bjorn on 9-5-2026.
//

#include "Greedy.h"

refinement *GreedyAlgorithm::getRefinement(const std::shared_ptr<MCTSNode>& node) const {
    // Base Case
    if (node->isTerminal()) return nullptr;
    if (node->getRefinements().empty()) {
        return node->getExtendRefinements()[0];
    }

    refinement *bestRef = nullptr;

    for (const auto ref: node->getRefinements()) {
        if (bestRef == nullptr || comparator(ref, bestRef)) {
            bestRef = ref;
        }
    }

    return bestRef;
}

refinement_vector GreedyAlgorithm::updateRefinements(const std::shared_ptr<MCTSNode> &root) const {
    auto refs = root->getRefinements();
    auto extendRefs = root->getExtendRefinements();

    refinement_vector greedyPicks{};

    auto node = root;
    annotateNode(node);

    LOG_S(INFO) << "Greedy picking refinements";

    while (!MCTSNode::isTerminal(refs, extendRefs)) {
        auto ref = getRefinement(node);

        if (ref == nullptr) break;

        LOG_S(INFO) << ref->to_short_string();

        ref->doref(merger);

        if (auto child = getChild(node, ref); node != nullptr && child != nullptr) {
            node = child;
        } else {
            auto [newRefs, newExtendRefs] = merger->get_refinements();
            const auto newChild = factory->create(ref, merger->get_final_apta_size(), newRefs, newExtendRefs, node);
            node->expand(newChild, -1);
            node = newChild;
        }

        annotateNode(node);
        greedyPicks.push_back(ref);
    }

    node->setScore(evaluator->evaluate(merger));

    LOG_S(INFO) << "Greedy picking done";
    return greedyPicks;
}
