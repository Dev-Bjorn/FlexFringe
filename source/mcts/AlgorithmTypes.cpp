//
// Created by bjorn on 11-5-2026.
//

#include <mcts/AlgorithmTypes.h>

int encodeAlgorithmType(const std::unordered_set<AlgorithmType>& algorithm) {
    int result = 0;
    for (const auto type: algorithm) {
        result |= (1 << static_cast<int>(type));
    }
    return result;
}

bool isMCTS(const int encoding) {
    return encoding & static_cast<int>(AlgorithmType::mcts);
}

bool isComparison(const int encoding) {
    return encoding & static_cast<int>(AlgorithmType::comparison);
}

bool isFinisher(const int encoding) {
    return encoding & static_cast<int>(AlgorithmType::finisher);
}
