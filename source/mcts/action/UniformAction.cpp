//
// Created by bjorn on 10-5-2026.
//


#include <random>
#include <refinement.h>
#include <mcts/action/UniformAction.h>

std::tuple<refinement_vector, int> UniformAction::action(const refinement_vector &refinements, const refinement_vector &extendRefs) {
    if (refinements.empty() && extendRefs.empty()) return std::make_tuple(refinement_vector{}, -1);
    std::uniform_int_distribution<int> dist(0, refinements.size() + extendRefs.size() - 1);
    size_t index = dist(rng);
    if (index >= refinements.size()) {
        return {extendRefs, index - refinements.size()};
    }
    return {refinements, index};
}
