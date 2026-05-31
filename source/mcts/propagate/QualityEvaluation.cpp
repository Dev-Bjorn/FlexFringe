//
// Created by bjorn on 5-5-2026.
//

#include <mcts/StringReader.h>
#include <mcts/Strings.h>
#include <mcts/propagate/ExtendAmount.h>
#include <mcts/propagate/InterpolateSizeAndLength.h>
#include <mcts/propagate/ModelSizeEvaluator.h>
#include <mcts/propagate/PathLength.h>
#include <mcts/propagate/QualityEvaluation.h>
#include <mcts/propagate/RolloutLength.h>

std::unique_ptr<QualityEvaluation> parseEvaluator(StringReader& reader, const MCTSConfig& config);

std::unique_ptr<InterpolateSizeAndLength> parseInterpolate(StringReader& reader, const MCTSConfig& config) {
    reader.expect('(');

    reader.skipWhitespace();
    double factor = std::stod(reader.readUntil({','}));

    reader.expect(',');
    reader.skipWhitespace();
    auto first = parseEvaluator(reader, config);

    reader.expect(',');
    reader.skipWhitespace();
    auto second = parseEvaluator(reader, config);

    reader.skipWhitespace();
    reader.expect(')');

    return std::make_unique<InterpolateSizeAndLength>(factor, std::move(first), std::move(second));
}


std::unique_ptr<QualityEvaluation> parseEvaluator(StringReader& reader, const MCTSConfig& config) {
    auto name = reader.readUntil({'(', ')'});

    if (name == "model-size")     return std::make_unique<ModelSizeEvaluator>();
    if (name == "rollout-length") return std::make_unique<RolloutLengthEvaluator>();
    if (name == "path-length")    return std::make_unique<PathLengthEvaluator>();
    if (name == "extend-amount")  return std::make_unique<ExtendAmountEvaluator>();
    if (name == "interpolate")    return parseInterpolate(reader, config);

    throw std::invalid_argument("Unknown evaluator: " + name);
}

std::unique_ptr<QualityEvaluation> createQualityEvaluation(const std::string_view evaluator, const MCTSConfig& config) {
    StringReader reader(evaluator);
    return parseEvaluator(reader, config);
}


