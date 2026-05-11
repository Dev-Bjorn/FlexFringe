//
// Created by bjorn on 8-5-2026.
//


#include <mcts/Strings.h>
#include <mcts/expansion/FullRule.h>
#include <mcts/expansion/SingleExtendRule.h>

std::shared_ptr<ExpansionRulePolicy> createExpansionRulePolicy(const std::string_view policy) {
    static const std::unordered_map<std::string, std::function<std::unique_ptr<ExpansionRulePolicy>()>> table = {
        {"full", [&]() { return std::make_unique<FullRule>(); }},
        {"single_extend", [&]() { return std::make_unique<SingleExtendRule>(); }}
    };
    const auto it = table.find(toLower(policy));
    if (it == table.end())
        throw std::invalid_argument("Unknown ActionPolicy: " + std::string(policy));
    return it->second();
}
