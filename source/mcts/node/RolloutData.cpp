//
// Created by bjorn on 20-5-2026.
//

#include "RolloutData.h"

RolloutData RolloutDataFactory::create(refinement* ref, const std::vector<refinement*>& refs, const std::vector<refinement*>& extendRefs, const int dfaSize) const {
    if (config.STORE_ROLLOUT_REFINEMENTS) {
        return RolloutData(ref, dfaSize, refs.size(), extendRefs.size(), refs, extendRefs);
    }
    return RolloutData(ref, dfaSize, refs.size(), extendRefs.size(), {}, {});
}

std::vector<RolloutData> RolloutDataFactory::createRange(
    refinement_vector::const_iterator begin,
    refinement_vector::const_iterator end,
    state_merger*                     merger
) const {
    std::vector<RolloutData> steps;
    for (auto it = begin; it != end; ++it) {
        auto [refs, extendRefs] = merger->get_refinements();
        steps.push_back(create(*it, refs, extendRefs, merger->get_final_apta_size()));
        (*it)->doref(merger);
    }
    for (auto it = end; it != begin;) {
        (*--it)->undo(merger);
    }
    return steps;
}