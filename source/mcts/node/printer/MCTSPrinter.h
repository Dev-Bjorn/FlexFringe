//
// Created by bjorn on 6-5-2026.
//

#ifndef FLEXFRINGE_MCTS_PRINTER_H
#define FLEXFRINGE_MCTS_PRINTER_H
#include <memory>
#include <mcts/node/MCTSNode.h>
#include <mcts/comparison/Algorithm.h>
#include <mcts/expansion/ExpansionRulePolicy.h>

struct MCTSPrinter {
protected:
    int                                  unvisited = -1;
    const MCTSConfig                     config;
    std::ofstream&                       output;
    std::shared_ptr<ExpansionRulePolicy> expansionRulePolicy;

    static std::string getRefinementName(refinement* ref);

    static std::string involvedRefinementNodes(refinement* refinement);

    static int visits(refinement* ref);

public:
    virtual ~MCTSPrinter() = default;

    virtual void print(const std::shared_ptr<MCTSNode>& node) = 0;

    explicit MCTSPrinter(
        std::ofstream&    output,
        const MCTSConfig& config
    ) : config(config), output(output) {
        expansionRulePolicy = createExpansionRulePolicy(config.EXPANSION_RULE_POLICY);
    }
};

#endif //FLEXFRINGE_MCTS_PRINTER_H
