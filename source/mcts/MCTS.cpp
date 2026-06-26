//
// Created by bjorn on 4-5-2026.
//


#include <ranges>
#include <mcts/MCTS.h>
#include <mcts/Refinements.h>
#include <mcts/action/ActionSelectionPolicy.h>
#include <mcts/comparison/Algorithm.h>
#include <mcts/selection/SelectionSearchPolicy.h>
#include <mcts/selection/node/NodeSelection.h>


MCTS::MCTS(const MCTSConfig& cfg, state_merger* merger) : config(cfg), merger(merger) {
    rolloutActionSelector = createActionPolicy(cfg.ROLLOUT_ACTION_POLICY, cfg, false);
    expandActionPolicy    = createActionPolicy(cfg.EXPANSION_ACTION_POLICY, cfg, true);

    auto nodeSelectionPolicy = createNodeSelectionPolicy(cfg.NODE_SELECTION_POLICY, cfg);
    expansionRulePolicy      = createExpansionRulePolicy(cfg.EXPANSION_RULE_POLICY);
    selectionPolicy          = createSelectionSearchPolicy(
        cfg.SELECTION_SEARCH_METHOD,
        nodeSelectionPolicy,
        expansionRulePolicy
    );
    stateEvaluator     = createQualityEvaluation(cfg.QUALITY_EVALUATOR_POLICY, merger, cfg);
    goalEvaluator      = createQualityEvaluation(cfg.GOAL_EVALUATOR_POLICY, merger, cfg);
    bestScore          = goalEvaluator->limit();
    rolloutDataFactory = std::make_unique<RolloutDataFactory>(cfg);

    for (const auto& policy: cfg.CONVERGENCE_POLICIES) {
        convergencePolicy.push_back(createConvergencePolicy(policy, stateEvaluator, cfg));
    }

    auto nodeDataFactory = std::make_unique<NodeDataFactory>(nodeSelectionPolicy);
    nodeFactory          = std::make_shared<MCTSNodeFactory>(std::move(nodeDataFactory));

    auto [refs, extendsRefs] = merger->get_refinements();

    root = nodeFactory->createRoot(merger->get_final_apta_size(), refs, extendsRefs);
}

std::shared_ptr<MCTSNode> MCTS::select() const {
    return selectionPolicy->selectNode(merger, root);
}

bool MCTS::isConverged(const std::shared_ptr<MCTSNode>& expandedNode, const refinement_vector& rolloutLog) const {
    if (config.FORCE_UNTIL_TERMINAL && rolloutLog.empty()) {
        return false;
    }
    for (const auto& policy: convergencePolicy) {
        if (policy->isConverged(expandedNode, rolloutLog)) {
            return true;
        }
    }
    return false;
}

std::shared_ptr<MCTSNode> MCTS::expand(const std::shared_ptr<MCTSNode>& node) const {
    // Meaning the node cannot be expanded
    if (!expansionRulePolicy->isExpandable(node)) return node;

    auto [refs, index] = expandActionPolicy->action(
        node->getUnvisitedRefinements(),
        node->getUnvisitedExtendRefinements()
    );

    if (index == -1) return nullptr;

    const auto ref = refs[index];

    // perform the choosen action
    ref->doref(merger);

    auto [newRefs, newExtendRefs] = merger->get_refinements();
    auto childNode                = nodeFactory->createMCTSNode(ref, merger->get_final_apta_size(), newRefs, newExtendRefs, node);

    node->expand(childNode, index);

    return childNode;
}


refinement_vector MCTS::rollout(const std::shared_ptr<MCTSNode>& rolloutNode) const {
    if (rolloutNode->isTerminal()) return {};

    refinement_vector log{};

    auto [refs, extendRefs] = merger->get_refinements();

    int step = 0;
    int extends = 0;

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
        if (config.STORE_ROLLOUTS) rolloutNode->addRolloutStep(rolloutDataFactory->create(ref, refs, extendRefs, merger->get_final_apta_size()));
        else if (ref->type() == 3) extends++;

        if (!config.STORE_ROLLOUT_REFINEMENTS) {
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
        }
        auto [newRefs, newExtendRefs] = merger->get_refinements();
        refs                          = std::move(newRefs);
        extendRefs                    = std::move(newExtendRefs);
    }

    if (!config.STORE_ROLLOUTS) {
        rolloutNode->setRolloutInfo(log.size(), extends);
    }

    return log;
}

bool MCTS::backPropagation(double score, const std::shared_ptr<MCTSNode>& rolloutNode, const refinement_vector& log) {
    rolloutNode->setScore(score);
    auto goalScore = goalEvaluator->evaluate(merger, rolloutNode, log);

    for (auto it = log.rbegin(); it != log.rend(); ++it) {
        (*it)->undo(merger);
    }

    // Propagate the result up towards the root
    auto node = rolloutNode;
    while (node != nullptr) {
        node->updateContext(score);
        node->undo(merger);

        node = node->getParent();
    }

    if (goalEvaluator->compare(goalScore, bestScore)) {
        eraseRollout(bestRefinements);
        LOG_S(INFO) << "GOAL: New best score: " << goalScore << " at node: " << rolloutNode->toString();

        auto copy       = log;
        bestScore       = goalScore;
        bestNode        = rolloutNode;
        bestRefinements = copy;
        return true;
    }
    return config.STORE_ROLLOUTS;
}

void MCTS::eraseRollout(const refinement_vector& log) {
    for (const auto it: log) {
        it->erase();
    }
}

refinement_vector MCTS::finishExpansion(const std::shared_ptr<MCTSNode>& lastChild, refinement_vector& log) const {
    if (!config.USE_FINISHER) return log;
    LOG_S(INFO) << "Preparing finisher algorithm";
    auto comparison_algorithm = createAlgorithm(config.FINISH_ALGORITHM, merger, stateEvaluator, this->nodeFactory);
    LOG_S(INFO) << "Starting finisher algorithm";
    auto finishLog = comparison_algorithm->run(lastChild, AlgorithmType::finisher);
    LOG_S(INFO) << "Finisher algorithm finished";
    auto combined = log;
    combined.insert(combined.end(), finishLog.begin(), finishLog.end());
    return combined;
}

refinement_vector MCTS::expandBestLog() const {
    if (bestNode == nullptr) return {};

    refinement_vector log{};

    // Get the ascendant log
    std::shared_ptr<MCTSNode> n = bestNode;
    while (n->getParent() != nullptr) {
        log.push_back(n->getRefinement());
        n = n->getParent();
    }

    std::ranges::reverse(log);

    for (auto it = log.begin(); it != log.end(); ++it) {
        (*it)->doref(merger);
    }

    n       = bestNode;
    auto it = bestRefinements.begin();

    // Find last overlapping child with the log.
    while (it != bestRefinements.end()) {
        const auto                current  = *it;
        std::shared_ptr<MCTSNode> selected = nullptr;
        for (const auto& ref: n->getChildren()) {
            if (ref_equal(current, ref->getRefinement())) {
                selected = ref;
                break;
            }
        }

        if (selected == nullptr) break;

        current->doref(merger);
        log.push_back(current);
        n = selected;
        ++it;
    }

    // Expand last node towards the end of the log
    std::vector<std::shared_ptr<MCTSNode>> childNodes;
    auto                                   beginIt = it;

    for (; it != bestRefinements.end(); ++it) {
        log.push_back(*it);
        (*it)->doref(merger);
        auto       [newRefs, newExtendRefs] = merger->get_refinements();
        const auto childNode                = nodeFactory->createMCTSNode(*it, merger->get_final_apta_size(), newRefs, newExtendRefs, n);
        n->expand(childNode, -1);
        auto futureSteps = rolloutDataFactory->createRange(std::next(it), bestRefinements.end(), merger);
        n->addRolloutSteps(std::move(futureSteps));
        n = childNode;
        childNodes.push_back(childNode);
    }

    for (size_t i = 0; i < childNodes.size(); ++i) {
        auto   refIt = beginIt + i;
        double score = this->stateEvaluator->evaluate(merger, childNodes[i], {refIt, bestRefinements.end() - 1});
        childNodes[i]->setScore(score);
        childNodes[i]->updateContext(score);
        childNodes[i]->annotate(AlgorithmType::best_node);
    }

    auto finishedLog = finishExpansion(n, log);

    // Bring state merger back to original state
    for (auto iterator = log.rbegin(); iterator != log.rend(); ++iterator) {
        (*iterator)->undo(merger);
    }

    return finishedLog;
}

refinement_vector MCTS::selectNode() {
    auto node = select();

    while (node != nullptr) {
        LOG_S(INFO) << "Selected node: " << node->toString() << " with APTA size: " << merger->get_final_apta_size();
        // AUTO expand when only one unvisited refinement or extend refinement is available
        if (config.AUTO_EXPAND_ONE_CHILD && (node->getRefinements().size() + node->getExtendRefinements().size()) == 1) {
            LOG_S(INFO) << "Auto expanding node: " << node->toString();

            // The auto expansion always preserves the same score, since parent
            // must always go through this node, the assumption can be made, that
            // the current node is also a path to that scoring node.
            auto parentScore = node->getScore();
            node             = expand(node);
            if (node == nullptr) break;
            backPropagation(parentScore, node, refinement_vector{});
            node = select();
            continue;
        }
        auto rolloutNode = expand(node);

        if (rolloutNode == nullptr) break;

        const auto log = rollout(rolloutNode);
        LOG_S(INFO) << "Current Expansion: " << rolloutNode->toString() << " after " << log.size() << " rollout steps has APTA size: " << merger->get_final_apta_size();
        const auto score = stateEvaluator->evaluate(merger, rolloutNode, log);

        // Placed before backpropagation
        if (!backPropagation(score, rolloutNode, log)) MCTS::eraseRollout(log);
        if (isConverged(rolloutNode, log)) {
            LOG_S(INFO) << "Convergence detected at node: " << rolloutNode->toString();
            break;
        };

        node = select();
    }

    LOG_S(INFO) << "Finished selecting node";
    LOG_S(INFO) << "Best node: " << bestNode->toString() << " with best score " << bestScore;
    return expandBestLog();
}
