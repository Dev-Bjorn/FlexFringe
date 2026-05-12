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

std::unique_ptr<ActionSelectionPolicy> createActionPolicy(const std::string_view policy, const MCTSConfig& config) {
    const std::unordered_map<std::string, std::function<std::unique_ptr<ActionSelectionPolicy>()>> table = {
        {"first", [&]() -> std::unique_ptr<ActionSelectionPolicy> { return std::make_unique<FirstAction>(); }},
        {"weighted-merge-split-first", [&]() -> std::unique_ptr<ActionSelectionPolicy> { return std::make_unique<WeightedMergeSplitFirst>(config.ROLLOUT_ACTION_SEED); }},
        {"uniform-merge-split-first", [&]() -> std::unique_ptr<ActionSelectionPolicy> { return std::make_unique<UniformMergeSplitFirst>(config.ROLLOUT_ACTION_SEED); }},
        {"uniform", [&]() -> std::unique_ptr<ActionSelectionPolicy> { return std::make_unique<UniformAction>(config.ROLLOUT_ACTION_SEED); }},
        {"greedy", [&]() -> std::unique_ptr<ActionSelectionPolicy> { return std::make_unique<GreedyAction>(); }},
    };
    const auto it = table.find(toLower(policy));
    if (it == table.end()) throw std::invalid_argument("Unknown ActionPolicy: " + std::string(policy));
    return it->second();
}
