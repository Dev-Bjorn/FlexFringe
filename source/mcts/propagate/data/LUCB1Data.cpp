//
// Created by bjorn on 5-5-2026.
//


#include <mcts/propagate/data/LUCB1Data.h>

void LUCB1NodeData::update(const double value) {
    visits++;
    Q += value;
}

