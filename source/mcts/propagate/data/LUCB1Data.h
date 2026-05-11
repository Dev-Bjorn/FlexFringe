//
// Created by bjorn on 5-5-2026.
//

#ifndef FLEXFRINGE_LUCB1_CONTEXT_H
#define FLEXFRINGE_LUCB1_CONTEXT_H
#include <mcts/propagate/data/NodeData.h>


/**
 * The node data for the upper and lower confidence bound selection policy
 */
struct LUCB1NodeData : NodeData {
    double Q = 0.0;

    void update(double value) override;

};

#endif //FLEXFRINGE_LUCB1_CONTEXT_H
