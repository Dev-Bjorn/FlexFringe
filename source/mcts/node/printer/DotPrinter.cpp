//
// Created by bjorn on 7-5-2026.
//


#include <mcts/node/printer/DotPrinter.h>

std::string DotPrinter::getNodeColor(const std::shared_ptr<MCTSNode> &node) const {
    if (node->getId() < 0) return "violet";
    if (node->isInAlgorithm(AlgorithmType::comparison)) return "lightcoral";
    if (node->isTerminal()) return "tomato";
    if (!expansionRulePolicy->isExpandable(node)) return "forestgreen";
    if (node->getUnvisitedRefinements().empty() && !node->getRefinements().empty()) return "gold";
    return "ghostwhite";
}

void DotPrinter::print_label(const std::shared_ptr<MCTSNode> &info) const {
    if (info->getId() >= 0) output << "ID: " << info->getId() << "\\n";
    output << "Refinement: " << getRefinementName(info->getRefinement());
    output << "\\nRef-Score: " << info->getRefinement()->score;
    output << "\\nVisits: " << visits(info->getRefinement());
    if (info->getScore() != -1) {
        output << "\\nScore: " << info->getScore() << "";
    }
}

void DotPrinter::print_attributes(const std::shared_ptr<MCTSNode> &info) {
    output << "label=\"";
    print_label(info);
    output << "\"";
    output << ", shape=oval";
    output << ", style=\"filled";
    if (info->getId() < 0) output << ", dashed";
    output << "\"";
    output << ", fillcolor=\"" << getNodeColor(info) << "\"";
}

void DotPrinter::print_node(const std::shared_ptr<MCTSNode>& info) {
    output << "\t" << info->getId() << " [";
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
        auto child = std::make_shared<MCTSNode>(unvisited--, nullptr, ref, -1, refinement_vector{}, refinement_vector{}, node);
        print_node(child);
        print_edge(node->getId(), child->getId(), 0);
    }

    for (auto ref: node->getUnvisitedExtendRefinements()) {
        auto child = std::make_shared<MCTSNode>(unvisited--, nullptr, ref, -1, refinement_vector{}, refinement_vector{}, node);
        print_node(child);
        print_edge(node->getId(), child->getId(), 0);
    }
}

void DotPrinter::print(const std::shared_ptr<MCTSNode> &node) {
    output << "digraph DFA {\n";
    output << "\t" << node->getId() << " [label=\"root\" shape=box];\n";
    print_edges(node);

    while (!queue.empty()) {
        auto expandNode = queue.front();
        queue.pop();

        print_node(expandNode);
        print_edges(expandNode);
        print_unvisited(expandNode);
    }

    output << "}\n";
}
