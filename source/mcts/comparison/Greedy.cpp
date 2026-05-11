//
// Created by bjorn on 9-5-2026.
//

#include "Greedy.h"

refinement *GreedyAlgorithm::getRefinement(const refinement_vector &refs, const refinement_vector &extendRefs) const {
    // Base Case
    if (refs.empty() && extendRefs.empty()) return nullptr;
    if (refs.empty()) {
        return extendRefs[0];
    }

    refinement *bestRef = nullptr;

    for (const auto ref: refs) {
        if (bestRef == nullptr || comparator(ref, bestRef)) {
            bestRef = ref;
        }
    }

    return bestRef;
}

refinement_vector GreedyAlgorithm::getRefinements(const std::shared_ptr<MCTSNode> &root) const {
    auto refs = root->getRefinements();
    auto extendRefs = root->getExtendRefinements();

    refinement_vector greedyPicks{};

    auto node = root;

    LOG_S(INFO) << "Greedy picking refinements";

    while (!MCTSNode::isTerminal(refs, extendRefs)) {
        auto ref = getRefinement(refs, extendRefs);

        if (ref == nullptr) break;

        LOG_S(INFO) << ref->to_short_string();

        ref->doref(merger);

        if (auto child = getChild(node, ref); node != nullptr && child != nullptr) {
            refs = child->getRefinements();
            extendRefs = child->getExtendRefinements();
            node = child;
        } else {
            auto [newRefs, newExtendRefs] = merger->get_refinements();
            refs = newRefs;
            extendRefs = newExtendRefs;
            node = nullptr;
        }

        greedyPicks.push_back(ref);
    }

    LOG_S(INFO) << "Greedy picking done";
    return greedyPicks;
}
