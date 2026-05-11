//
// Created by bjorn on 4-5-2026.
//


#include <ranges>
#include <mcts/MCTS.h>
#include <mcts/action/ActionSelectionPolicy.h>
#include <mcts/selection/SelectionSearchPolicy.h>
#include <mcts/selection/node/NodeSelection.h>


MCTS::MCTS(const MCTSConfig &cfg, state_merger *merger) : config(cfg), merger(merger) {
    rolloutActionSelector = createActionPolicy(cfg.ROLLOUT_ACTION_POLICY, cfg);
    expandActionPolicy = createActionPolicy(cfg.EXPANSION_ACTION_POLICY, cfg);

    auto nodeSelectionPolicy = createNodeSelectionPolicy(cfg.NODE_SELECTION_POLICY, cfg);
    expansionRulePolicy = createExpansionRulePolicy(cfg.EXPANSION_RULE_POLICY);
    selectionPolicy = createSelectionSearchPolicy(
        cfg.SELECTION_SEARCH_METHOD,
        nodeSelectionPolicy,
        expansionRulePolicy
    );
    stateEvaluator = createQualityEvaluation(cfg.QUALITY_EVALUATOR_POLICY);

    for (const auto& policy: cfg.CONVERGENCE_POLICIES) {
        convergencePolicy.push_back(createConvergencePolicy(policy, stateEvaluator, cfg));
    }

    auto nodeDataFactory = std::make_unique<NodeDataFactory>(nodeSelectionPolicy);
    nodeFactory = std::make_unique<MCTSNodeFactory>(std::move(nodeDataFactory));

    auto [refs, extendsRefs] = merger->get_refinements();

    root = nodeFactory->createRoot(merger->get_final_apta_size(), refs, extendsRefs);
}

std::shared_ptr<MCTSNode> MCTS::select() const {
    return selectionPolicy->selectNode(merger, root);
}

bool MCTS::isConverged(const std::shared_ptr<MCTSNode> &expandedNode, const refinement_vector &rolloutLog) const {
    for (const auto& policy: convergencePolicy) {
        if (policy->isConverged(merger, expandedNode, rolloutLog)) {
            return true;
        }
    }
    return false;
}

std::shared_ptr<MCTSNode> MCTS::expand(const std::shared_ptr<MCTSNode> &node) const {
    // Meaning the node cannot be expanded
    if (!expansionRulePolicy->isExpandable(node)) return nullptr;

    auto [refs, index] = expandActionPolicy->action(
        node->getUnvisitedRefinements(),
        node->getUnvisitedExtendRefinements()
    );

    if (index == -1) return nullptr;

    const auto ref = refs[index];

    // perform the choosen action
    ref->doref(merger);

    auto [newRefs, newExtendRefs] = merger->get_refinements();
    auto childNode = nodeFactory->create(ref, merger->get_final_apta_size(), newRefs, newExtendRefs, node);

    node->expand(childNode, index);

    return childNode;
}

refinement_vector MCTS::rollout(const std::shared_ptr<MCTSNode> &rolloutNode) const {
    if (rolloutNode->isTerminal()) return {};

    refinement_vector log{};


    auto [refs, extendRefs] = merger->get_refinements();

    int step = 0;

    while (!MCTSNode::isTerminal(refs, extendRefs)) {
        step++;
        if (config.MAX_ROLLOUT_STEPS != -1 && step > config.MAX_ROLLOUT_STEPS) {
            LOG_S(WARNING) << "Max rollout steps reached, stopping rollout at Node: " << rolloutNode->toString();
            break;
        }
        auto [chosenRef, index] = rolloutActionSelector->action(refs, extendRefs);

        // No more actions can be selected
        if (index == -1) break;

        auto ref = chosenRef[index];
        ref->doref(merger);
        log.push_back(ref);

        for (auto delRef: refs) {
            if (delRef != ref) {
                delRef->erase();
            }
        }

        for (auto delRef: extendRefs) {
            if (delRef != ref) {
                delRef->erase();
            }
        }
        auto [newRefs, newExtendRefs] = merger->get_refinements();
        refs = std::move(newRefs);
        extendRefs = std::move(newExtendRefs);
    }

    return log;
}

void MCTS::backPropagation(const std::shared_ptr<MCTSNode> &rolloutNode, const refinement_vector &log) const {
    const double score = stateEvaluator->evaluate(merger);
    rolloutNode->setScore(score);

    for (const auto it: std::views::reverse(log)) {
        it->undo(merger);
    }

    auto node = rolloutNode;
    while (node != nullptr) {
        node->updateContext(score);
        node->undo(merger);

        node = node->getParent();
    }

    for (const auto it: log) {
        it->erase();
    }
}

refinement_vector MCTS::expandLog(const std::shared_ptr<MCTSNode> &node, const refinement_vector &expansionLog) const {
    refinement_vector log{};

    // Get the ascendent log
    std::shared_ptr<MCTSNode> n = node;
    while (n->getParent() != nullptr) {
        log.push_back(n->getRefinement());
        n = node->getParent();
    }

    std::ranges::reverse(log);

    for (auto it = log.begin(); it != log.end(); ++it) {
        (*it)->doref(merger);
    }

    n = node;
    auto it = expansionLog.begin();

    // Procedure to find last child
    while (true) {
        const auto current = *it;
        std::shared_ptr<MCTSNode> selected = nullptr;
        for (const auto& ref: n->getChildren()) {
            if (ref->getRefinement() == current) {
                selected = ref;
                break;
            }
        }

        if (selected == nullptr) {
            break;
        }

        current->doref(merger);
        ++it;
        n = selected;
        log.push_back(current);
    }

    // Expand last node towards the end of the log
    for (; it != log.end(); ++it) {
        (*it)->doref(merger);
        auto [newRefs, newExtendRefs] = merger->get_refinements();
        const auto childNode = nodeFactory->create(n->getRefinement(), merger->get_final_apta_size(), newRefs, newExtendRefs, n);
        n->expand(childNode, -1);
        log.push_back(*it);
    }

    return log;
}

refinement_vector MCTS::undoNode(const std::shared_ptr<MCTSNode> &node, const refinement_vector &expansionLog) const {
    refinement_vector log{};
    for (auto it = expansionLog.rbegin(); it != expansionLog.rend(); ++it) {
        log.push_back(*it);
        (*it)->undo(merger);
    }

    auto n = node;
    while (n != nullptr) {
        if (n->getRefinement() != nullptr) log.push_back(n->getRefinement());
        n->undo(merger);
        n = n->getParent();
    }

    std::ranges::reverse(log);

    return log;
}
