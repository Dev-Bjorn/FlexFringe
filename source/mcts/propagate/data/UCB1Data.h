//
// Created by bjorn on 5-5-2026.
//

#ifndef FLEXFRINGE_UCB1_CONTEXT_H
#define FLEXFRINGE_UCB1_CONTEXT_H
#include <limits>
#include <mcts/propagate/data/NodeData.h>


/**
 * The node data for the upper confidence bound selection policy
 */
struct UCB1NodeData : NodeData {
private:
    double s;
public:
    void update(double value) override;

    [[nodiscard]] double sum() const { return s; };

};

#endif //FLEXFRINGE_UCB1_CONTEXT_H
