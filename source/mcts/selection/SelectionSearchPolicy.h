//
// Created by bjorn on 9-5-2026.
//

#ifndef FLEXFRINGE_SEARCH_SELECTION_POLICY_HPP
#define FLEXFRINGE_SEARCH_SELECTION_POLICY_HPP
#include <memory>
#include <mcts/expansion/ExpansionRulePolicy.h>
#include <mcts/node/MCTSNode.h>
#include <mcts/selection/node/NodeSelection.h>

/**
 * @brief The search method used for selecting nodes for expansion in the MCTS algorithm.
 */
struct SelectionSearchPolicy {
protected:
    std::shared_ptr<NodeSelectionPolicy> selectionPolicy;
    std::shared_ptr<ExpansionRulePolicy> expansionRulePolicy;

public:
    explicit SelectionSearchPolicy(
        std::shared_ptr<NodeSelectionPolicy> selectionPolicy,
        std::shared_ptr<ExpansionRulePolicy> expansionRulePolicy
    ) : selectionPolicy(std::move(selectionPolicy)), expansionRulePolicy(std::move(expansionRulePolicy)) {
    }

    virtual ~SelectionSearchPolicy() = default;

    /**
     * Select a node for expansion based on the selection policy.
     * @param merger The state merger to use for merging states
     * @param root The root node of the search tree
     * @return The selected node for expansion
     */
    virtual std::shared_ptr<MCTSNode> selectNode(state_merger* merger, const std::shared_ptr<MCTSNode>& root) const = 0;
};

/**
 * Create the selection search policy.
 * @param policy The policy to use for selection
 * @param selection_policy The node selection policy
 * @param expansion_rule_policy The expansion rule policy
 * @return The created selection search policy
 */
std::unique_ptr<SelectionSearchPolicy> createSelectionSearchPolicy(
    std::string_view                     policy,
    std::shared_ptr<NodeSelectionPolicy> selection_policy,
    std::shared_ptr<ExpansionRulePolicy> expansion_rule_policy
);
#endif //FLEXFRINGE_SEARCH_SELECTION_POLICY_HPP
