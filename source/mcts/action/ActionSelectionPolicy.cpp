//
// Created by bjorn on 6-5-2026.
//

#include <algorithm>
#include <functional>
#include <memory>
#include <mcts/Strings.h>
#include <mcts/action/ActionSelectionPolicy.h>
#include <mcts/action/FirstAction.h>
#include <mcts/action/GreedyAction.h>
#include <mcts/action/WeightedMergeSplitFirst.h>
#include <mcts/action/UniformMergeSplitFirst.h>
#include <mcts/action/UniformAction.h>
#include <mcts/action/WeightedAction.h>

std::unique_ptr<ActionSelectionPolicy> createActionPolicy(const std::string_view policy, const MCTSConfig& config, const bool isExpansion) {
    const std::unordered_map<std::string, std::function<std::unique_ptr<ActionSelectionPolicy>()>> table = {
        {"first", [&]() -> std::unique_ptr<ActionSelectionPolicy> { return std::make_unique<FirstAction>(); }},
        {"weighted-merge-split-first", [&]() -> std::unique_ptr<ActionSelectionPolicy> { return std::make_unique<WeightedMergeSplitFirst>(isExpansion ? config.EXPANSION_ACTION_SEED : config.ROLLOUT_ACTION_SEED); }},
        {"uniform-merge-split-first", [&]() -> std::unique_ptr<ActionSelectionPolicy> { return std::make_unique<UniformMergeSplitFirst>(isExpansion ? config.EXPANSION_ACTION_SEED : config.ROLLOUT_ACTION_SEED); }},
        {"uniform", [&]() -> std::unique_ptr<ActionSelectionPolicy> { return std::make_unique<UniformAction>(isExpansion ? config.EXPANSION_ACTION_SEED : config.ROLLOUT_ACTION_SEED); }},
        {"weighted", [&]() -> std::unique_ptr<ActionSelectionPolicy> { return std::make_unique<WeightedAction>(isExpansion ? config.EXPANSION_ACTION_SEED : config.ROLLOUT_ACTION_SEED, isExpansion ? config.EXPANSION_WEIGHTED_EXTEND_SCORING : config.ROLLOUT_WEIGHTED_EXTEND_SCORING); }},
        {"greedy", [&]() -> std::unique_ptr<ActionSelectionPolicy> { return std::make_unique<GreedyAction>(); }},
    };
    const auto it = table.find(toLower(policy));
    if (it == table.end()) throw std::invalid_argument("Unknown ActionPolicy: " + std::string(policy));
    return it->second();
}
