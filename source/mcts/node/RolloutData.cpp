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