//
// Created by bjorn on 7-5-2026.
//


#include <mcts/node/printer/DotPrinter.h>

std::string DotPrinter::getNodeColor(const NodeInfo &node) const {
    if (node.isGreedyNode) return "lightcoral";
    if (node.node == nullptr) return "violet";
    if (node.node->isTerminal()) return "tomato";
    if (!expansionRulePolicy->isExpandable(node.node)) return "forestgreen";
    if (node.node->getUnvisitedRefinements().empty() && !node.node->getRefinements().empty()) return "gold";
    return "ghostwhite";
}

void DotPrinter::print_label(const NodeInfo &info) const {
    if (info.id >= 0) output << "ID: " << info.id << "\\n";
    output << "Refinement: " << getRefinementName(info.ref);
    output << "\\nRef-Score: " << info.ref->score;
    output << "\\nVisits: " << visits(info.ref);
    if (info.score != -1) {
        output << "\\nScore: " << info.score << "";
    }
}

void DotPrinter::print_attributes(const NodeInfo &info) {
    output << "label=\"";
    print_label(info);
    output << "\"";
    output << ", shape=oval";
    output << ", style=\"filled";
    if (info.id < 0) output << ", dashed";
    output << "\"";
    output << ", fillcolor=\"" << getNodeColor(info) << "\"";
}

void DotPrinter::print_node(const NodeInfo &info) {
    output << "\t" << info.id << " [";
    print_attributes(info);
    output << "];\n";
}

void DotPrinter::print_edge(const int from, const int to, const int visits) const {
    output << "\t\t" << from << " -> " << to;
    output << " [";

    std::vector<std::string> attrs;

    if (visits > 0) {
        attrs.push_back("penwidth=\"" + std::to_string(log(1 + visits)) + "\"");
    }
    if (to < 0) attrs.push_back("style=dashed");

    for (size_t i = 0; i < attrs.size(); i++) {
        if (i > 0) output << ", ";
        output << attrs[i];
    }

    output << "];\n";
}

void DotPrinter::print_edges(const std::shared_ptr<MCTSNode> &node) {
    for (auto &child: node->getChildren()) {
        print_edge(node->getId(), child->getId(), child->getContext()->getVisits());
        queue.push(child);
    }
}

void DotPrinter::print_unvisited(const std::shared_ptr<MCTSNode> &node) {
    if (!config.PRINT_UNVISITED) return;

    if (node->getRefinements().size() + node->getExtendRefinements().size() - node->getUnvisitedExtendRefinements().
    size() - node->getUnvisitedRefinements().size() == 0) return;

    for (auto ref: node->getUnvisitedRefinements()) {
        print_node({unvisited, -1, ref, -1, false, true, false, nullptr});
        print_edge(node->getId(), unvisited, 0);
        --unvisited;
    }

    for (auto ref: node->getUnvisitedExtendRefinements()) {
        print_node({unvisited, -1, ref, -1, false, true, false, nullptr});
        print_edge(node->getId(), unvisited, 0);
        --unvisited;
    }
}

void DotPrinter::rolloutComparison(const std::shared_ptr<MCTSNode> &node) {
    if (node->isTerminal()) return;

    print_edge(node->getId(), unvisited, 0);

    for (auto it = comparisonResult->beginNonChildRefinements(); it != comparisonResult->end() - 1; ++it) {
        auto ref = *it;
        print_node(NodeInfo{unvisited, -1, ref,  comparisonResult->getDFASize(ref), true, false, false, nullptr});
        print_edge(unvisited, unvisited - 1, 0);
        --unvisited;
    }

    auto ref = *(comparisonResult->end() - 1);
    print_node(NodeInfo{unvisited, comparisonResult->getScore(), ref, comparisonResult->getDFASize(ref), true, false, false, nullptr});
    --unvisited;
}
void DotPrinter::print(const std::shared_ptr<MCTSNode> &node) {
    output << "digraph DFA {\n";
    output << "\t" << node->getId() << " [label=\"root\" shape=box];\n";
    print_edges(node);

    while (!queue.empty()) {
        auto expandNode = queue.front();
        queue.pop();

        print_node(NodeInfo{
            expandNode->getId(),
            expandNode->getScore(),
            expandNode->getRefinement(),
            expandNode->getDFASize(),
            comparisonResult->hasRefinement(expandNode),
            expansionRulePolicy->isExpandable(expandNode),
            false,
            expandNode
        });
        print_edges(expandNode);
        print_unvisited(expandNode);

        if (comparisonResult->getLastChildDecision() == expandNode) rolloutComparison(expandNode);
    }

    output << "}\n";
}
