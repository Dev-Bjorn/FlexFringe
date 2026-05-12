//
// Created by bjorn on 11-5-2026.
//

#include <mcts/AlgorithmTypes.h>

int encodeAlgorithmType(std::unordered_set<AlgorithmType> algorithm) {
    int result = 0;
    for (auto type : algorithm) {
        result |= (1 << static_cast<int>(type));
    }
    return result;
}

bool isMCTS(int algorithm) {
    return algorithm & static_cast<int>(AlgorithmType::mcts);
}

bool isComparison(int algorithm) {
    return algorithm & static_cast<int>(AlgorithmType::comparison);
}

bool isFinisher(int algorithm) {
    return algorithm & static_cast<int>(AlgorithmType::finisher);
}