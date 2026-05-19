//
// Created by bjorn on 12-5-2026.
//

#include <algorithm>
#include <parameters.h>
#include <ranges>
#include <mcts/action/WeightedAction.h>

double WeightedAction::scoreExtend(const refinement_vector& refinements) const {
    if (CURRENT_CONFIG.EXTEND_SCORE != 0) return CURRENT_CONFIG.EXTEND_SCORE;
    if (this->weightExtendScoring == "min") {
        return std::ranges::min(refinements | std::views::transform([](const refinement* r) { return r->score; }));
    } else if (this->weightExtendScoring == "max") {
        return std::ranges::max(refinements | std::views::transform([](const refinement* r) { return r->score; }));
    } else if (this->weightExtendScoring == "avg") {
        auto scores = refinements | std::views::transform([](const refinement* r) { return r->score; });
        auto sum    = std::accumulate(scores.begin(), scores.end(), 0.0);
        return sum / refinements.size();
    }

    throw std::invalid_argument("Invalid weightExtendScoring value: " + this->weightExtendScoring);
}

std::tuple<refinement_vector, int> WeightedAction::action(const refinement_vector& refinements, const refinement_vector& extendRefs) {
    if (refinements.empty() && extendRefs.empty()) return std::make_tuple(refinement_vector{}, -1);
    refinement_vector combined = refinements;
    combined.insert(combined.end(), extendRefs.begin(), extendRefs.end());
    auto extendScore = scoreExtend(combined);

    auto weights = combined | std::views::transform([&](const refinement* r) {
        if (r->type() == 3) return extendScore;
        return r->score;
    });
    auto sum     = std::accumulate(weights.begin(), weights.end(), 0.0);
    int idx;
    if (sum == 0.0) {
        std::uniform_int_distribution<int> dist(0, combined.size() - 1);
        idx = dist(rng);
    } else {
        std::discrete_distribution<int> dist(weights.begin(), weights.end());
        idx = dist(rng);
    }
    if (idx >= refinements.size()) {
        return {extendRefs, idx - refinements.size()};
    }
    return {refinements, idx};
}
