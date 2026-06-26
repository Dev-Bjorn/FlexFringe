//
// Created by bjorn on 9-5-2026.
//

#include <functional>
#include <mcts/StringReader.h>
#include <mcts/Strings.h>
#include <mcts/selection/BFSSearch.h>
#include <mcts/selection/ChildVSParentSearch.h>
#include <mcts/selection/ProgressiveWideningSearch.h>
#include <mcts/selection/SelectionSearchPolicy.h>

std::unique_ptr<SelectionSearchPolicy> parseSelectionPolicy(
    StringReader&                        reader,
    std::shared_ptr<NodeSelectionPolicy> selection_policy,
    std::shared_ptr<ExpansionRulePolicy> expansion_rule_policy
) {
    reader.skipWhitespace();
    auto name = toLower(reader.readUntil({'(', ')', ':', ' '}));
    reader.skipWhitespace();

    if (name == "bfs") {
        return std::make_unique<BFSSearch>(std::move(selection_policy), std::move(expansion_rule_policy));
    }

    if (name == "best-child-vs-parent") {
        return std::make_unique<ChildVSParentSearch>(std::move(selection_policy), std::move(expansion_rule_policy));
    }

    if (name == "progressive-widening") {
        reader.expect('(');
        reader.skipWhitespace();
        double C = std::stod(reader.readUntil({' ', ':'}));
        reader.skipWhitespace();
        reader.expect(':');
        reader.skipWhitespace();
        double alpha = std::stod(reader.readUntil({' ', ')'}));
        reader.skipWhitespace();
        reader.expect(')');
        return std::make_unique<ProgressiveWideningSearch>(std::move(selection_policy), std::move(expansion_rule_policy), C, alpha);
    }

    throw std::invalid_argument("Unknown Selection Policy: " + name);
}

std::unique_ptr<SelectionSearchPolicy> createSelectionSearchPolicy(
    const std::string_view               policy,
    std::shared_ptr<NodeSelectionPolicy> selection_policy,
    std::shared_ptr<ExpansionRulePolicy> expansion_rule_policy
) {
    StringReader reader(policy);
    return parseSelectionPolicy(reader, std::move(selection_policy), std::move(expansion_rule_policy));
}