//
// Created by bjorn on 11-5-2026.
//


#include <memory>
#include <mcts/Strings.h>
#include <mcts/convergence/ConvergencePolicy.h>
#include <mcts/convergence/IterationConvergence.h>
#include <mcts/convergence/NodeTerminalConvergence.h>
#include <mcts/convergence/ScoreImprovementConvergence.h>
#include <mcts/convergence/ScoreThresholdConvergence.h>

std::unique_ptr<ConvergencePolicy> createConvergencePolicy(const std::string_view policy, std::shared_ptr<QualityEvaluation> qualityEvaluation, const MCTSConfig& config) {
    const std::unordered_map<std::string, std::function<std::unique_ptr<ConvergencePolicy>(std::shared_ptr<QualityEvaluation>)>> table = {
        {"iterations", [&](std::shared_ptr<QualityEvaluation>) { return std::make_unique<IterationConvergence>(config.MAX_ITERATIONS); }},
        {"score-threshold", [&](std::shared_ptr<QualityEvaluation>) { return std::make_unique<ScoreThresholdConvergence>(config.SCORE_THRESHOLD); }},
        {"score-improvement", [&](std::shared_ptr<QualityEvaluation> eval) { return std::make_unique<ScoreImprovementConvergence>(std::move(eval), config.MAX_NO_IMPROVEMENT); }},
        {"node-termination", [&](std::shared_ptr<QualityEvaluation>) { return std::make_unique<NodeTerminalConvergence>(); }}
    };

    const auto it = table.find(toLower(policy));
    if (it == table.end()) throw std::invalid_argument("Unknown ActionPolicy: " + std::string(policy));
    return it->second(std::move(qualityEvaluation));
}
