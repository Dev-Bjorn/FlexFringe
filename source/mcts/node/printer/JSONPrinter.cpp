//
// Created by bjorn on 7-5-2026.
//


#include <mcts/expansion/ExpansionRulePolicy.h>
#include <mcts/node/printer/JSONPrinter.h>

std::string JSONPrinter::ind(const int depth) {
    return std::string(depth * 4, ' ');
}

void JSONPrinter::print_attributes(std::unordered_map<std::string, std::string> attributeMap, const int depth) {
    auto sep = "";
    for (const auto& [key, value]: attributeMap) {
        if (config.PRINT_JSON_LINE_SEP_BETWEEN_ATTR) {
            output << sep << ind(depth) << "\"" << key << "\": " << value;
            sep = ",\n";
        } else {
            output << sep << "\"" << key << "\": " << value;
            sep = ", ";
        }
    }
}

void JSONPrinter::print_node(const std::shared_ptr<MCTSNode>& node, const int depth) {
    std::unordered_map<std::string, std::string> attributeMap = {
        {"id", std::to_string(node->getId())},
        {"score", std::to_string(node->getScore())},
        {"dfaSize", std::to_string(node->getDFASize())},
        {"refinement", "\"" + getRefinementName(node->getRefinement()) + "\""},
        {"involvedNodes", "[" + involvedRefinementNodes(node->getRefinement()) + "]"},
        {"refVisits", std::to_string(visits(node->getRefinement()))},
        {"refScore", std::to_string(node->getRefinement()->score)},
        {"nodeTypes", std::to_string(node->getAlgorithmTypes())},
        {"isVisited", std::to_string(node->getId() >= 0)},
        {"isTerminal", std::to_string(node->isTerminal())},
        {"isExpandable", std::to_string(expansionRulePolicy->isExpandable(node))},
        {"exhaustedMerges", std::to_string(node->getUnvisitedRefinements().empty() && !node->getRefinements().empty())},
    };

    if (config.PRINT_JSON_LINE_SEP_BETWEEN_ATTR) {
        output << ind(depth) << "{\n";
        print_attributes(attributeMap, depth + 1);
        output << ind(depth) << "}\n";
    } else {
        output << ind(depth) << "{";
        print_attributes(attributeMap, depth);
        output << "}";
    }
}

void JSONPrinter::print_edge(const std::tuple<int, int, int>& t, const int depth) {
    std::unordered_map<std::string, std::string> attributeMap = {
        {"from", std::to_string(std::get<0>(t))},
        {"to", std::to_string(std::get<1>(t))},
        {"visits", std::to_string(std::get<2>(t))},
    };

    if (config.PRINT_JSON_LINE_SEP_BETWEEN_ATTR) {
        output << ind(depth) << "{\n";
        print_attributes(attributeMap, depth + 1);
        output << ind(depth) << "}\n";
    } else {
        output << ind(depth) << "{";
        print_attributes(attributeMap, depth);
        output << "}";
    }
}

void JSONPrinter::add_unvisited(const std::shared_ptr<MCTSNode>& node) {
    if (!config.PRINT_UNVISITED) return;

    if (node->getRefinements().size() + node->getExtendRefinements().size() - node->getUnvisitedExtendRefinements().
        size() - node->getUnvisitedRefinements().size() == 0)
        return;

    for (auto ref: node->getUnvisitedRefinements()) {
        auto child = std::make_shared<MCTSNode>(unvisited--, nullptr, ref, -1, refinement_vector{}, refinement_vector{}, node);
        nodes.push_back(child);
        edges.push_back({node->getId(), child->getId(), 0});
    }

    for (auto ref: node->getUnvisitedExtendRefinements()) {
        auto child = std::make_shared<MCTSNode>(unvisited--, nullptr, ref, -1, refinement_vector{}, refinement_vector{}, node);
        nodes.push_back(child);
        edges.push_back({node->getId(), child->getId(), 0});
    }
}


void JSONPrinter::print_info(int depth) {
    std::unordered_map<std::string, std::string> attributeMap = {
        {"mctsHeuristic", "\"" + config.MCTS_HEURISTIC_NAME + "\""},
        {"comparisonHeuristic", "\"" + config.COMPARISON_HEURISTIC_NAME + "\""},
        {"comparisonAlgorithm", "\"" + config.COMPARISON_ALGORITHM + "\""},
        {"expansionSeed", std::to_string(config.EXPANSION_ACTION_SEED)},
        {"expansionActionPolicy", "\"" + config.EXPANSION_ACTION_POLICY + "\""},
        {"expansionRulePolicy", "\"" + config.EXPANSION_RULE_POLICY + "\""},
        {"rolloutActionPolicy", "\"" + config.ROLLOUT_ACTION_POLICY + "\""},
        {"rolloutSeed", std::to_string(config.ROLLOUT_ACTION_SEED)},
        {"maxRolloutSteps", std::to_string(config.MAX_ROLLOUT_STEPS)},
        {"nodeSelectionPolicy", "\"" + config.NODE_SELECTION_POLICY + "\""},
        {"selectionSearchPolicy", "\"" + config.SELECTION_SEARCH_METHOD + "\""},
        {"qualityEvaluatorPolicy", "\"" + config.QUALITY_EVALUATOR_POLICY + "\""},
    };

    if (config.PRINT_JSON_LINE_SEP_BETWEEN_ATTR) {
        output << ind(depth) << "\"info\": {\n";
        print_attributes(attributeMap, depth + 1);
        output << ind(depth) << "},\n";
    } else {
        output << ind(depth) << "\"info\": {";
        print_attributes(attributeMap, depth);
        output << "},\n";
    }
}

void JSONPrinter::print(const std::shared_ptr<MCTSNode>& root) {
    std::queue<std::shared_ptr<MCTSNode>> queue;

    for (auto child: root->getChildren()) {
        queue.push(child);
        edges.push_back({0, child->getId(), child->getContext()->getVisits()});
    }

    while (!queue.empty()) {
        auto expandNode = queue.front();
        queue.pop();
        nodes.push_back(expandNode);
        add_unvisited(expandNode);

        for (auto child: expandNode->getChildren()) {
            queue.push(child);
            edges.push_back({expandNode->getId(), child->getId(), child->getContext()->getVisits()});
        }
    }

    output << "{\n";
    print_info(1);
    output << ind(1) << "\"nodes\":[\n";
    output << ind(2) << "{\"id\": 0}";

    for (auto& node: nodes) {
        output << ",\n";
        print_node(node, 2);
    }

    output << ind(1) << "],\n";
    output << ind(1) << "\"edges\":[\n";
    auto sep = "";
    for (auto& edge: edges) {
        output << sep;
        print_edge(edge, 2);
        sep = ",\n";
    }

    output << "\n" << ind(1) << "]\n";
    output << "}\n";
}
