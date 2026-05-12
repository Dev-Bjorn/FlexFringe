//
// Created by bjorn on 4-5-2026.
//


#include <functional>
#include <unordered_map>
#include <mcts/Strings.h>
#include <mcts/selection/node/NodeSelection.h>
#include <mcts/selection/node/LCB1SelectionPolicy.h>
#include <mcts/selection/node/UCB1SelectionPolicy.h>

std::shared_ptr<NodeSelectionPolicy> createNodeSelectionPolicy(const std::string_view policy, const MCTSConfig& config) {
    const std::unordered_map<std::string, std::function<std::unique_ptr<NodeSelectionPolicy>()>> table = {
        {"lcb1", [&]() { return std::make_unique<LCB1SelectionPolicy>(config.LCB1_CONSTANT); }},
        {"ucb1", [&]() { return std::make_unique<UCB1SelectionPolicy>(config.UCB1_CONSTANT); }},
    };
    const auto it = table.find(toLower(policy));
    if (it == table.end()) throw std::invalid_argument("Unknown State Evaluator: " + std::string(policy));
    return it->second();
}
