//
// Created by bjorn on 20-5-2026.
//

#ifndef FLEXFRINGE_ROLLOUTDATA_H
#define FLEXFRINGE_ROLLOUTDATA_H
#include <refinement.h>
#include <state_merger.h>
#include <config/MCTSConfig.h>

struct RolloutData {
    refinement* ref;
    const int   dfaSize;
    const int   refsSize, extendRefsSize;

    const std::vector<refinement*> refs;
    const std::vector<refinement*> extendRefs;
};

struct RolloutDataFactory {
    const MCTSConfig config;

    explicit RolloutDataFactory(const MCTSConfig& config) : config(config) {
    }

    RolloutData create(refinement* ref, const std::vector<refinement*>& refs, const std::vector<refinement*>& extendRefs, const int dfaSize) const;

    std::vector<RolloutData> createRange(
        refinement_vector::const_iterator begin,
        refinement_vector::const_iterator end,
        state_merger*                     merger
    ) const;
};

#endif //FLEXFRINGE_ROLLOUTDATA_H
