//
// Created by bjorn on 8-5-2026.
//

#ifndef FLEXFRINGE_FULL_EXPANSION_RULE_H
#define FLEXFRINGE_FULL_EXPANSION_RULE_H
#include <mcts/expansion/ExpansionRulePolicy.h>

/**
 * @brief Full expansion rule returns true, when a merge, split, or extend refinement is not visited.
 */
struct FullRule : ExpansionRulePolicy {

    bool isExpandable(const std::shared_ptr<MCTSNode> &node) override;

};
#endif //FLEXFRINGE_FULL_EXPANSION_RULE_H
