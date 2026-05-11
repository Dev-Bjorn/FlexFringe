//
// Created by bjorn on 6-5-2026.
//

#ifndef FLEXFRINGE_MCTS_PRINTER_H
#define FLEXFRINGE_MCTS_PRINTER_H
#include <memory>
#include <mcts/node/MCTSNode.h>
#include <mcts/comparison/ComparisonAlgorithm.h>

struct NodeInfo {
    int id;
    double score;
    refinement *ref;

    bool isGreedyNode;
    bool isUnvisited;
    bool expandable;
    int dfaSize;
    std::shared_ptr<MCTSNode> node;

    NodeInfo(
        const int id,
        const double score,
        refinement *ref,
        const int dfaSize,
        const bool isGreedyNode,
        const bool isUnvisited,
        const bool expandable,
        const std::shared_ptr<MCTSNode> &node
    ) : id(id),
        score(score),
        ref(ref),
        isGreedyNode(isGreedyNode),
        isUnvisited(isUnvisited),
        expandable(expandable),
        dfaSize(dfaSize),
        node(node) {
    }

    [[nodiscard]] bool isTerminal() const {
        if (node == nullptr) return false;
        return node->isTerminal();
    }

    [[nodiscard]] bool isExpandable() const {
        return expandable;
    }

    [[nodiscard]] bool hasUnexpandedMerges() const {
        if (node == nullptr) return false;
        return !node->getUnvisitedRefinements().empty();
    }

    [[nodiscard]] bool hasMergeRefinements() const {
        if (node == nullptr) return false;
        return !node->getRefinements().empty();
    }

    [[nodiscard]] std::vector<std::shared_ptr<MCTSNode> > getChildren() const {
        if (node == nullptr) return {};
        return node->getChildren();
    }

    [[nodiscard]] int getVisits() const {
        if (node == nullptr) return 1;
        return node->getContext()->getVisits();
    }
};

struct MCTSPrinter {
protected:
    int unvisited = -1;
    const MCTSConfig config;
    std::shared_ptr<AlgorithmResult> comparisonResult;
    std::ofstream &output;

    static std::string getRefinementName(refinement *ref);

    static std::string involvedRefinementNodes(refinement *refinement);

    static int visits(refinement *ref);

public:
    virtual ~MCTSPrinter() = default;

    virtual void print(const std::shared_ptr<MCTSNode> &node) = 0;

    explicit MCTSPrinter(
        std::shared_ptr<AlgorithmResult> picks,
        std::ofstream &output,
        const MCTSConfig &config
    ) : config(config), comparisonResult(std::move(picks)), output(output) {
    }
};

#endif //FLEXFRINGE_MCTS_PRINTER_H
