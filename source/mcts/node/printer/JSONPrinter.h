//
// Created by bjorn on 7-5-2026.
//

#ifndef FLEXFRINGE_JSON_PRINTER_HPP
#define FLEXFRINGE_JSON_PRINTER_HPP
#include <iosfwd>
#include <mcts/comparison/ComparisonAlgorithm.h>
#include <mcts/node/printer/MCTSPrinter.h>

class JSONPrinter : MCTSPrinter {
public:
    // printUnvisited: include unvisited refinements as leaf nodes in the tree
    explicit JSONPrinter(const std::shared_ptr<AlgorithmResult> &result, std::ofstream &output, const MCTSConfig& config)
        : MCTSPrinter(result, output, config) {
    }

    void print(const std::shared_ptr<MCTSNode> &root) override;

protected:
    std::vector<std::tuple<int, int, int>> edges;
    std::vector<NodeInfo> nodes;

    static std::string ind(int depth);

    void print_attributes(std::unordered_map<std::string, std::string> attributeMap, int depth);

    void print_node(const NodeInfo &info, int depth);

    void print_edge(const std::tuple<int, int, int> &t, int depth);

    void add_unvisited(const std::shared_ptr<MCTSNode> &node);

    void add_comparison_rollout(const std::shared_ptr<MCTSNode> &node);

    void print_info(int depth);
};


#endif //FLEXFRINGE_JSON_PRINTER_HPP
