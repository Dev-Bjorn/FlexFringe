//
// Created by bjorn on 9-5-2026.
//

#include <mcts/Strings.h>
#include <mcts/comparison/ComparisonAlgorithm.h>
#include <mcts/comparison/Greedy.h>

std::string_view getName(const ComparisonAlgorithm &algorithm) {
    if (auto _ = dynamic_cast<const GreedyAlgorithm*>(&algorithm)) {
        return "greedy";
    }
    throw std::invalid_argument("Unknown Algorithm");
}


bool AlgorithmResult::hasRefinement(const std::shared_ptr<MCTSNode> &node) const {
    for (const auto &refNode : refNodes) {
        if (refNode == node) {
            return true;
        }
    }
    return false;
}

int AlgorithmResult::getDFASize(refinement *refinement) const {
    return refSizes.at(refinement);
}

refinement_vector::const_iterator AlgorithmResult::beginNonChildRefinements() {
    return refs.begin() + refNodes.size();
}

refinement_vector::const_iterator AlgorithmResult::end() const {
    return refs.end();
}

std::shared_ptr<MCTSNode> AlgorithmResult::getLastChildDecision() {
    return refNodes.back();
}

refinementSizeMap ComparisonAlgorithm::undo(const refinement_vector &refs) const {
    refinementSizeMap size_map;
    for (auto it = refs.rbegin(); it != refs.rend(); ++it) {
        size_map[*it] = merger->get_final_apta_size();
        (*it)->undo(merger);
    }
    return size_map;
}

refinementNodeMap ComparisonAlgorithm::findRefinementMap(const std::shared_ptr<MCTSNode>& root, const refinement_vector& refs) {
    refinementNodeMap refNodes{};

    std::shared_ptr<MCTSNode> node = root;

    for (auto it = refs.begin(); it != refs.end(); ++it) {
        const auto ref = *it;
        bool found = false;

        for (auto child: node->getChildren()) {
            if (child->getRefinement() == ref) {
                refNodes.push_back(child);
                found = true;
                break;
            }
        }

        if (!found) break;
        node = refNodes.back();
    }

    return refNodes;
}

std::shared_ptr<AlgorithmResult> ComparisonAlgorithm::run(const std::shared_ptr<MCTSNode> &root) const {
    auto refinementLog = getRefinements(root);
    auto refNodes = findRefinementMap(root, refinementLog);
    auto score = evaluator->evaluate(merger);
    auto size_map = undo(refinementLog);

    return std::make_shared<AlgorithmResult>(getName(*this), refinementLog, refNodes, size_map, score);
}

std::shared_ptr<MCTSNode> ComparisonAlgorithm::getChild(const std::shared_ptr<MCTSNode> &node, const refinement *refinement) {
    if (node == nullptr) return nullptr;

    for (auto mcts_node: node->getChildren()) {
        if (mcts_node->getRefinement() == refinement) return mcts_node;
    }
    return nullptr;
}

std::unique_ptr<ComparisonAlgorithm> createAlgorithm(const std::string_view name, state_merger* merger, std::shared_ptr<QualityEvaluation> evaluator) {
    static const std::unordered_map<std::string, std::function<std::unique_ptr<ComparisonAlgorithm>(std::shared_ptr<QualityEvaluation>)>> table = {
        {"greedy", [&](std::shared_ptr<QualityEvaluation> eval) { return std::make_unique<GreedyAlgorithm>(merger, std::move(eval)); }},
    };
    const auto it = table.find(toLower(name));
    if (it == table.end())
        throw std::invalid_argument("Unknown ActionPolicy: " + std::string(name));
    return it->second(std::move(evaluator));
}