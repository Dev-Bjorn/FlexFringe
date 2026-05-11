//
// Created by bjorn on 7-5-2026.
//

#ifndef FLEXFRINGE_DOT_PRINTER_H
#define FLEXFRINGE_DOT_PRINTER_H
#include <memory>
#include <mcts/expansion/ExpansionRulePolicy.h>
#include <mcts/node/MCTSNode.h>
#include <mcts/node/printer/MCTSPrinter.h>

struct DotPrinter : MCTSPrinter {
protected:
    std::shared_ptr<ExpansionRulePolicy> expansionRulePolicy;
    std::queue<std::shared_ptr<MCTSNode>> queue;

    std::string getNodeColor(const NodeInfo &node) const;

    void print_label(const NodeInfo &info) const;

    void print_attributes(const NodeInfo &info);

    void print_node(const NodeInfo &info);

    void print_edge(int from, int to, int visits) const;

    void print_edges(const std::shared_ptr<MCTSNode> &node);

    void print_unvisited(const std::shared_ptr<MCTSNode> &node);

    void rolloutComparison(const std::shared_ptr<MCTSNode> &node);

public:
    explicit DotPrinter(const std::shared_ptr<AlgorithmResult> &result, std::ofstream &output, const MCTSConfig& config) : MCTSPrinter(result, output, config) {
        expansionRulePolicy = createExpansionRulePolicy(config.EXPANSION_RULE_POLICY);
    }

    void print(const std::shared_ptr<MCTSNode> &node) override;
};
#endif //FLEXFRINGE_DOT_PRINTER_H
