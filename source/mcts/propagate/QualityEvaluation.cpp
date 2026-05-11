//
// Created by bjorn on 5-5-2026.
//

#include <mcts/Strings.h>
#include <mcts/propagate/ModelSizeEvaluator.h>
#include <mcts/propagate/QualityEvaluation.h>

std::unique_ptr<QualityEvaluation> createQualityEvaluation(const std::string_view evaluator) {
    static const std::unordered_map<std::string, std::function<std::unique_ptr<QualityEvaluation>()>> table = {
        {"model-size", [&]() { return std::make_unique<ModelSizeEvaluator>(); }},
    };
    const auto it = table.find(toLower(evaluator));
    if (it == table.end())
        throw std::invalid_argument("Unknown State Evaluator: " + std::string(evaluator));
    return it->second();
}
