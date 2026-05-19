//
// Created by bjorn on 7-5-2026.
//

#ifndef FLEXFRINGE_JSON_PRINTER_HPP
#define FLEXFRINGE_JSON_PRINTER_HPP
#include <iosfwd>
#include <mcts/comparison/Algorithm.h>
#include <mcts/node/printer/MCTSPrinter.h>

class JSONPrinter : MCTSPrinter {
public:
    // printUnvisited: include unvisited refinements as leaf nodes in the tree
    explicit JSONPrinter(std::ofstream& output, const MCTSConfig& config) : MCTSPrinter(output, config) {
    }

    void print(const std::shared_ptr<MCTSNode>& root) override;

protected:
    std::vector<std::tuple<int, int, int>> edges;
    std::vector<std::shared_ptr<MCTSNode>> nodes;

    static std::string ind(int depth);

    void print_attributes(std::ostream &output,
                     std::unordered_map<std::string, std::string> attributeMap,
                     int depth);
    void print_ref(std::ostream &output, refinement *data, int depth);
    std::string to_refs_string(const refinement_vector &data, int depth);
    void print_rollout_node(std::ostream &output, const RolloutData &data,
                            int depth);
    std::string to_rollout_string(const std::vector<RolloutData> &rollouts,
                                  int depth);

    void print_node(const std::shared_ptr<MCTSNode>& node, int depth);

    void print_edge(const std::tuple<int, int, int>& t, int depth);

    void add_unvisited(const std::shared_ptr<MCTSNode>& node);

    void print_info(int depth);
};


#endif //FLEXFRINGE_JSON_PRINTER_HPP
