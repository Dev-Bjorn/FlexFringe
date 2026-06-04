//
// Created by bjorn on 5-5-2026.
//

#include <csv.hpp>
#include <mcts/StringReader.h>
#include <mcts/Strings.h>
#include <mcts/propagate/Interpolate.h>
#include <mcts/propagate/ModelSizeEvaluator.h>
#include <mcts/propagate/QualityEvaluation.h>
#include <mcts/propagate/RolloutLength.h>

GoalPtr createGoal(const std::string_view goal) {
    const std::unordered_map<std::string, std::function<GoalPtr()>> table = {
        {"minimise", [&]() { return std::make_unique<Minimise>(); }},
        {"maximise", [&]() { return std::make_unique<Maximise>(); }},
    };

    const auto it = table.find(toLower(goal));
    if (it == table.end()) throw std::invalid_argument("Unknown ActionPolicy: " + std::string(goal));
    return it->second();
}

std::unique_ptr<QualityEvaluation> parseEvaluator(StringReader& reader, const state_merger* merger, const MCTSConfig& config);

std::unique_ptr<Interpolate> parseInterpolate(StringReader& reader, const state_merger* merger, const MCTSConfig& config) {
    reader.expect('(');

    reader.skipWhitespace();
    auto goalName = reader.readUntil({':', ' '});
    reader.skipWhitespace();
    auto goal = createGoal(goalName);

    reader.expect(':');

    reader.skipWhitespace();
    double factor = std::stod(reader.readUntil({':', ' '}));
    reader.skipWhitespace();

    reader.expect(':');
    auto first = parseEvaluator(reader, merger, config);

    reader.expect(':');
    auto second = parseEvaluator(reader, merger, config);
    reader.expect(')');

    return std::make_unique<Interpolate>(factor, std::move(first), std::move(second), goal);
}

GoalPtr readGoal(StringReader& reader) {
    reader.expect('(');

    reader.skipWhitespace();
    auto goalName = reader.readUntil({')', ' '});
    reader.skipWhitespace();

    reader.expect(')');
    return createGoal(goalName);
}


std::unique_ptr<QualityEvaluation> parseEvaluator(StringReader& reader, const state_merger* merger, const MCTSConfig& config) {
    reader.skipWhitespace();
    auto name = reader.readUntil({'(', ')', ':', ' '});
    reader.skipWhitespace();
    name = toLower(name);

    if (name == "model-size")     return std::make_unique<ModelSizeEvaluator>(readGoal(reader));
    if (name == "norm-model-size")     return std::make_unique<NormalisedModelSizeEvaluator>(merger, readGoal(reader));
    if (name == "rollout-length") return std::make_unique<RolloutLengthEvaluator>(readGoal(reader));
    if (name == "norm-rollout-length") return std::make_unique<NormalisedRolloutLengthEvaluator>(merger, readGoal(reader));
    if (name == "interpolate")    return parseInterpolate(reader, merger, config);

    throw std::invalid_argument("Unknown evaluator: " + name);
}

std::unique_ptr<QualityEvaluation> createQualityEvaluation(const std::string_view evaluator, const state_merger* merger, const MCTSConfig& config) {
    StringReader reader(evaluator);
    return parseEvaluator(reader, merger, config);
}


