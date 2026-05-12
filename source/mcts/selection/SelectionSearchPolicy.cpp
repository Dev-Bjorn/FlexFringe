//
// Created by bjorn on 9-5-2026.
//

#include <functional>
#include <mcts/Strings.h>
#include <mcts/selection/BFSSearch.h>
#include <mcts/selection/ChildVSParentSearch.h>
#include <mcts/selection/SelectionSearchPolicy.h>

typedef std::function<std::unique_ptr<SelectionSearchPolicy>(
    std::shared_ptr<NodeSelectionPolicy>,
    std::shared_ptr<ExpansionRulePolicy>
)> SelectionPolicyFactory;

std::unique_ptr<SelectionSearchPolicy> createSelectionSearchPolicy(
    const std::string_view policy,
    std::shared_ptr<NodeSelectionPolicy> selection_policy,
    std::shared_ptr<ExpansionRulePolicy> expansion_rule_policy
) {
    const std::unordered_map<std::string, SelectionPolicyFactory> table = {
        {
            "bfs",
            [](std::shared_ptr<NodeSelectionPolicy> node, std::shared_ptr<ExpansionRulePolicy> rule) {
                return std::make_unique<BFSSearch>(std::move(node), std::move(rule));
            }
        },
        {
            "best-child-vs-parent",
            [](std::shared_ptr<NodeSelectionPolicy> node, std::shared_ptr<ExpansionRulePolicy> rule) {
                return std::make_unique<ChildVSParentSearch>(std::move(node), std::move(rule));
            }
        }
    };
    const auto it = table.find(toLower(policy));
    if (it == table.end()) throw std::invalid_argument("Unknown State Evaluator: " + std::string(policy));
    return it->second(std::move(selection_policy), std::move(expansion_rule_policy));
}
