//
// Created by bjorn on 5-5-2026.
//

#include <mcts/action/UniformMergeSplitFirst.h>

std::tuple<refinement_vector, int> UniformMergeSplitFirst::action(const refinement_vector& refinements, const refinement_vector& extendRefs) {
    if (!refinements.empty()) {
        std::uniform_int_distribution<int> dist(0, refinements.size() - 1);
        int                                index = dist(rng);
        return {refinements, index};
    }

    if (!extendRefs.empty()) {
        std::uniform_int_distribution<int> dist(0, extendRefs.size() - 1);
        int                                index = dist(rng);
        return {extendRefs, index};
    }

    return std::make_tuple(refinement_vector{}, -1);
}
