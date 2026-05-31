//
// Created by bjorn on 9-5-2026.
//

#include <printer.h>
#include <mcts/Strings.h>
#include <mcts/comparison/Algorithm.h>
#include <mcts/comparison/Greedy.h>

std::string_view getName(const Algorithm& algorithm) {
    if (auto _ = dynamic_cast<const GreedyAlgorithm*>(&algorithm)) {
        return "greedy";
    }
    throw std::invalid_argument("Unknown Algorithm");
}

void Algorithm::annotateNode(const std::shared_ptr<MCTSNode>& node) const {
    node->annotate(algorithmType);
}

refinement_vector Algorithm::run(
    const std::shared_ptr<MCTSNode>& root,
    const AlgorithmType              at
) {
    this->algorithmType    = at;
    return updateRefinements(root);
}

std::shared_ptr<MCTSNode> Algorithm::getChild(
    const std::shared_ptr<MCTSNode>& node,
    const refinement*                refinement
) {
    if (node == nullptr) return nullptr;

    for (auto mcts_node: node->getChildren()) {
        if (ref_equal(mcts_node->getRefinement(), refinement)) return mcts_node;
    }
    return nullptr;
}

std::unique_ptr<Algorithm> createAlgorithm(
    const std::string_view             name,
    state_merger*                      merger,
    std::shared_ptr<QualityEvaluation> evaluator,
    std::shared_ptr<MCTSNodeFactory>   factory
) {
    const std::unordered_map<std::string,
                std::function<std::unique_ptr<Algorithm>(
                    std::shared_ptr<QualityEvaluation>,
                    std::shared_ptr<MCTSNodeFactory>
                )>>
            table = {
                {
                    "greedy",
                    [&](
                std::shared_ptr<QualityEvaluation> eval,
                std::shared_ptr<MCTSNodeFactory>   f
            ) {
                        return std::make_unique<GreedyAlgorithm>(
                            merger,
                            std::move(eval),
                            std::move(f)
                        );
                    }
                },
            };
    const auto it = table.find(toLower(name));
    if (it == table.end()) throw std::invalid_argument("Unknown ActionPolicy: " + std::string(name));
    return it->second(std::move(evaluator), std::move(factory));
}
