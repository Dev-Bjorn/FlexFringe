//
// Created by bjorn on 4-5-2026.
//

#ifndef FLEXFRINGE_MCTS_HPP
#define FLEXFRINGE_MCTS_HPP
#include <mcts/action/ActionSelectionPolicy.h>
#include <mcts/convergence/ConvergencePolicy.h>
#include <mcts/expansion/ExpansionRulePolicy.h>
#include <mcts/node/MCTSNodeFactory.h>
#include <mcts/propagate/QualityEvaluation.h>
#include <mcts/selection/SelectionSearchPolicy.h>

struct MCTS {
private:
    MCTSConfig config;
    state_merger *merger;
    std::shared_ptr<MCTSNode> root;
    std::unique_ptr<MCTSNodeFactory> nodeFactory;

    // Selection Fields
    std::unique_ptr<SelectionSearchPolicy> selectionPolicy;

    // Rollout Fields
    std::unique_ptr<ActionSelectionPolicy> rolloutActionSelector;

    // Expansion Fields
    std::unique_ptr<ActionSelectionPolicy> expandActionPolicy;
    std::shared_ptr<ExpansionRulePolicy> expansionRulePolicy;

    // BackPropagation Fields
    std::shared_ptr<QualityEvaluation> stateEvaluator;

    // Convergence Fields
    std::vector<std::shared_ptr<ConvergencePolicy>> convergencePolicy;

public:
    explicit MCTS(const MCTSConfig &cfg, state_merger *merger);

    std::shared_ptr<MCTSNode> getRoot() const {
        return root;
    }

    MCTSConfig getConfig() const {
        return config;
    }

    state_merger* getMerger() const {
        return merger;
    }

    bool isConverged(const std::shared_ptr<MCTSNode>& expandedNode, const refinement_vector& rolloutLog) const;

    std::shared_ptr<MCTSNode> select() const;

    std::shared_ptr<MCTSNode> expand(const std::shared_ptr<MCTSNode> &node) const;

    refinement_vector rollout(const std::shared_ptr<MCTSNode> &rolloutNode) const;

    void backPropagation(const std::shared_ptr<MCTSNode> &rolloutNode, const refinement_vector &log) const;

    refinement_vector expandLog(const std::shared_ptr<MCTSNode> &node, const refinement_vector &expansionLog) const;

    refinement_vector undoNode(const std::shared_ptr<MCTSNode> &node, const refinement_vector &expansionLog = {}) const;
};

#endif //FLEXFRINGE_MCTS_HPP
