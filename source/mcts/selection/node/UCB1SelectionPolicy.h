//
// Created by bjorn on 4-5-2026.
//

#ifndef FLEXFRINGE_UCB1_SELECTION_HPP
#define FLEXFRINGE_UCB1_SELECTION_HPP
#include <mcts/selection/node/NodeSelection.h>

/**
 * @brief Selects a node using the upperbound confidence bound (UCB1) policy.
 *
 * Use this policy for when maximization is required.
 */
struct UCB1SelectionPolicy : NodeSelectionPolicy {
private:
    double C;
public:
    explicit UCB1SelectionPolicy(const double UCB_constant) : C(UCB_constant) {};
    [[nodiscard]] double score(const std::shared_ptr<MCTSNode> &node) const;
    [[nodiscard]] std::shared_ptr<MCTSNode> select(const std::vector<std::shared_ptr<MCTSNode>>& nodes) const override;
    [[nodiscard]] std::shared_ptr<NodeData> createNodeData() const override;
};


#endif //FLEXFRINGE_UCB1_SELECTION_HPP
