//
// Created by bjorn on 8-6-2026.
//

#ifndef FLEXFRINGE_K_BFSSEARCH_H
#define FLEXFRINGE_K_BFSSEARCH_H
#include <mcts/selection/SelectionSearchPolicy.h>

struct ProgressiveWideningSearch : SelectionSearchPolicy {
private:
    double C;
    double alpha;

public:
    explicit ProgressiveWideningSearch(
        std::shared_ptr<NodeSelectionPolicy> selectionPolicy,
        std::shared_ptr<ExpansionRulePolicy> expansionRulePolicy,
        double C,
        double alpha
    ) : SelectionSearchPolicy(std::move(selectionPolicy), std::move(expansionRulePolicy)), C(C), alpha(alpha) {
    };

    [[nodiscard]] bool isSelected(const std::shared_ptr<MCTSNode>& node) const;

    std::shared_ptr<MCTSNode> selectNode(state_merger* merger, const std::shared_ptr<MCTSNode>& root) const override;
};


#endif //FLEXFRINGE_K_BFSSEARCH_H
