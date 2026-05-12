//
// Created by bjorn on 6-5-2026.
//


#include <parameters.h>
#include <ranges>
#include <mcts/Random.h>
#include <mcts/action/WeightedMergeSplitFirst.h>

std::tuple<refinement_vector, int> WeightedMergeSplitFirst::action(const refinement_vector& refinements, const refinement_vector& extendRefs) {
    if (!refinements.empty()) {
        auto weights = refinements | std::views::transform([](const refinement* r) { return r->score; });
        auto sum     = std::accumulate(weights.begin(), weights.end(), 0.0);
        if (sum == 0.0) {
            std::uniform_int_distribution<int> dist(0, refinements.size() - 1);
            return {refinements, dist(rng)};
        } else {
            std::discrete_distribution<int> dist(weights.begin(), weights.end());
            return {refinements, dist(rng)};
        }
    }
    if (!extendRefs.empty()) {
        std::uniform_int_distribution<int> dist(0, extendRefs.size() - 1);
        return {extendRefs, dist(rng)};
    }
    return std::make_tuple(refinement_vector{}, -1);
}
