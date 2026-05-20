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
    MCTSConfig                       config;
    state_merger*                    merger;
    std::shared_ptr<MCTSNode>        root;
    std::shared_ptr<MCTSNodeFactory> nodeFactory;

    int                       bestScore = std::numeric_limits<int>::max();
    std::shared_ptr<MCTSNode> bestNode;
    refinement_vector         bestRefinements;

    // Selection Fields
    std::unique_ptr<SelectionSearchPolicy> selectionPolicy;

    // Rollout Fields
    std::unique_ptr<ActionSelectionPolicy> rolloutActionSelector;
    std::unique_ptr<RolloutDataFactory> rolloutDataFactory;

    // Expansion Fields
    std::unique_ptr<ActionSelectionPolicy> expandActionPolicy;
    std::shared_ptr<ExpansionRulePolicy>   expansionRulePolicy;

    // BackPropagation Fields
    std::shared_ptr<QualityEvaluation> stateEvaluator;

    // Convergence Fields
    std::vector<std::shared_ptr<ConvergencePolicy>> convergencePolicy;

    static void eraseRollout(const refinement_vector& log);

    bool isConverged(const std::shared_ptr<MCTSNode>& expandedNode, const refinement_vector& rolloutLog) const;

    [[nodiscard]] refinement_vector finishExpansion(const std::shared_ptr<MCTSNode>& lastChild, refinement_vector& log) const;

    [[nodiscard]] refinement_vector expandBestLog() const;

public:
    explicit MCTS(const MCTSConfig& cfg, state_merger* merger);

    [[nodiscard]] std::shared_ptr<MCTSNode> getRoot() const {
        return root;
    }

    [[nodiscard]] std::shared_ptr<MCTSNodeFactory> getNodeFactory() const {
        return nodeFactory;
    }

    [[nodiscard]] MCTSConfig getConfig() const {
        return config;
    }

    [[nodiscard]] state_merger* getMerger() const {
        return merger;
    }

    [[nodiscard]] std::shared_ptr<MCTSNode> select() const;

    [[nodiscard]] std::shared_ptr<MCTSNode> expand(const std::shared_ptr<MCTSNode>& node) const;

    [[nodiscard]] refinement_vector rollout(const std::shared_ptr<MCTSNode>& rolloutNode) const;

    bool backPropagation(double score, const std::shared_ptr<MCTSNode>& rolloutNode, const refinement_vector& log);

    refinement_vector selectNode();
};

#endif //FLEXFRINGE_MCTS_HPP
