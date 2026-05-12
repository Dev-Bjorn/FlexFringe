//
// Created by bjorn on 11-5-2026.
//

#ifndef FLEXFRINGE_ALGORITHMTYPES_H
#define FLEXFRINGE_ALGORITHMTYPES_H
#include <cstdint>
#include <unordered_set>

// These numbers can never change, if new values will be assigned
// assign a unique number, since they are used as identifiers
// in encodings.
enum AlgorithmType : uint8_t {
    mcts       = 1,
    finisher   = 2,
    comparison = 3
};

int encodeAlgorithmType(const std::unordered_set<AlgorithmType>& algorithm);

bool isMCTS(int encoding);

bool isComparison(int encoding);

bool isFinisher(int encoding);


#endif //FLEXFRINGE_ALGORITHMTYPES_H
