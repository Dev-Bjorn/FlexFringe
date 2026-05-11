//
// Created by bjorn on 8-5-2026.
//

#ifndef FLEXFRINGE_SKIP_VISITED_SINGLE_EXTEND_RULE_H
#define FLEXFRINGE_SKIP_VISITED_SINGLE_EXTEND_RULE_H
#include <mcts/expansion/ExpansionRulePolicy.h>

/**
 * @brief Expansion rule that allows a single expand for extend-only nodes and none for merge-split-extend nodes.
 */
struct SingleExtendRule : ExpansionRulePolicy {

    bool isExpandable(const std::shared_ptr<MCTSNode> &node) override;

};
#endif //FLEXFRINGE_SKIP_VISITED_SINGLE_EXTEND_RULE_H
