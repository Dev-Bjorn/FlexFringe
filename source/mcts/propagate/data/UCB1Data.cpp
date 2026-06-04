//
// Created by bjorn on 5-5-2026.
//


#include <mcts/propagate/data/UCB1Data.h>

void UCB1NodeData::update(const double value) {
    visits++;
    s += value;
}


