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
    for (const auto &[key, value]: attributeMap) {
        if (config.PRINT_JSON_LINE_SEP_BETWEEN_ATTR) {
            output << sep << ind(depth) << "\"" << key << "\": " << value;
            sep = ",\n";
        } else {
            output << sep << "\"" << key << "\": " << value;
            sep = ", ";
        }
    }
}

void JSONPrinter::print_node(const NodeInfo &info, const int depth) {
    std::unordered_map<std::string, std::string> attributeMap = {
        {"id", std::to_string(info.id)},
        {"score", std::to_string(info.score)},
        {"dfaSize", std::to_string(info.dfaSize)},
        {"refinement", "\"" + getRefinementName(info.ref) + "\""},
        {"involvedNodes", "[" + involvedRefinementNodes(info.ref) + "]"},
        {"refVisits", std::to_string(visits(info.ref))},
        {"refScore", std::to_string(info.ref->score)},
        {"isGreedyNode", std::to_string(info.isGreedyNode)},
        {"isVisited", std::to_string(!info.isUnvisited)},
        {"isTerminal", std::to_string(info.isTerminal())},
        {"isExpandable", std::to_string(info.isExpandable())},
        {"hasOnlyVisitedMerges", std::to_string(info.hasMergeRefinements() && !info.hasUnexpandedMerges())},
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

void JSONPrinter::print_edge(const std::tuple<int, int, int> &t, const int depth) {
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

void JSONPrinter::add_unvisited(const std::shared_ptr<MCTSNode> &node) {
    if (!config.PRINT_UNVISITED) return;

    if (node->getRefinements().size() + node->getExtendRefinements().size() - node->getUnvisitedExtendRefinements().
        size() - node->getUnvisitedRefinements().size() == 0)
        return;

    for (auto ref: node->getUnvisitedRefinements()) {
        nodes.push_back(NodeInfo{unvisited, -1, ref, -1, false, true, false, nullptr});
        edges.push_back({node->getId(), unvisited, 0});
        --unvisited;
    }

    for (auto ref: node->getUnvisitedExtendRefinements()) {
        nodes.push_back({unvisited, -1, ref, -1, false, true, false, nullptr});
        edges.push_back({node->getId(), unvisited, 0});
        --unvisited;
    }
}


void JSONPrinter::add_comparison_rollout(const std::shared_ptr<MCTSNode> &node) {
    if (node->isTerminal()) return;

    edges.push_back({node->getId(), unvisited, 0});

    for (auto it = comparisonResult->beginNonChildRefinements(); it != comparisonResult->end() - 1; ++it) {
        auto ref = *it;
        nodes.push_back(NodeInfo{unvisited, -1, ref, comparisonResult->getDFASize(ref), true, false, false, nullptr});
        edges.push_back({unvisited, unvisited - 1, 1});
        --unvisited;
    }

    auto ref = *(comparisonResult->end() - 1);
    nodes.push_back(NodeInfo{
        unvisited, comparisonResult->getScore(), ref, comparisonResult->getDFASize(ref), true, false, false, nullptr
    });
    --unvisited;
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

void JSONPrinter::print(const std::shared_ptr<MCTSNode> &root) {
    std::queue<std::shared_ptr<MCTSNode> > queue;
    std::shared_ptr<ExpansionRulePolicy> expansionRulePolicy = createExpansionRulePolicy(config.EXPANSION_RULE_POLICY);

    for (auto child: root->getChildren()) {
        queue.push(child);
        edges.push_back({0, child->getId(), child->getContext()->getVisits()});
    }

    while (!queue.empty()) {
        auto expandNode = queue.front();
        queue.pop();
        NodeInfo info{
            expandNode->getId(),
            expandNode->getScore(),
            expandNode->getRefinement(),
            expandNode->getDFASize(),
            comparisonResult->hasRefinement(expandNode),
            false,
            expansionRulePolicy->isExpandable(expandNode),
            expandNode
        };
        nodes.push_back(info);
        add_unvisited(expandNode);

        if (comparisonResult->getLastChildDecision() == expandNode) {
            add_comparison_rollout(expandNode);
        }

        for (auto child: expandNode->getChildren()) {
            queue.push(child);
            edges.push_back({expandNode->getId(), child->getId(), child->getContext()->getVisits()});
        }
    }

    output << "{\n";
    print_info(1);
    output << ind(1) << "\"nodes\":[\n";
    output << ind(2) << "{\"id\": 0}";

    for (auto &node: nodes) {
        output << ",\n";
        print_node(node, 2);
    }

    output << ind(1) << "],\n";
    output << ind(1) << "\"edges\":[\n";
    auto sep = "";
    for (auto &edge: edges) {
        output << sep;
        print_edge(edge, 2);
        sep = ",\n";
    }

    output << "\n" << ind(1) << "]\n";
    output << "}\n";
}
