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
    std::queue<std::shared_ptr<MCTSNode>> queue;

    std::string getNodeColor(const std::shared_ptr<MCTSNode>& node) const;

    void print_label(const std::shared_ptr<MCTSNode>& info) const;

    void print_attributes(const std::shared_ptr<MCTSNode>& info);

    void print_node(const std::shared_ptr<MCTSNode>& info);

    void print_edge(int from, int to, int visits) const;

    void print_edges(const std::shared_ptr<MCTSNode>& node);

    void print_unvisited(const std::shared_ptr<MCTSNode>& node);

public:
    explicit DotPrinter(std::ofstream& output, const MCTSConfig& config) : MCTSPrinter(output, config) {
    }

    void print(const std::shared_ptr<MCTSNode>& node) override;
};
#endif //FLEXFRINGE_DOT_PRINTER_H
