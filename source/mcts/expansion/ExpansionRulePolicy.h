//
// Created by bjorn on 8-5-2026.
//

#ifndef FLEXFRINGE_EXPANSION_POLICY_H
#define FLEXFRINGE_EXPANSION_POLICY_H
#include <memory>
#include <mcts/node/MCTSNode.h>

/**
 * @brief Checks whether a node is expandable.
 */
struct ExpansionRulePolicy {

    virtual ~ExpansionRulePolicy() = default;

    /**
     * Check whether the provided node is expandable.
     * @param node The node to check for expandability.
     * @return True if the node is expandable, false otherwise.
     */
    virtual bool isExpandable(const std::shared_ptr<MCTSNode> &node) = 0;

};

/**
 * Create an expansion rule policy based on the provided policy string.
 * @param policy The policy string specifying the type of expansion rule.
 * @return A shared pointer to the created expansion rule policy.
 */
std::shared_ptr<ExpansionRulePolicy> createExpansionRulePolicy(std::string_view policy);

#endif //FLEXFRINGE_EXPANSION_POLICY_H
