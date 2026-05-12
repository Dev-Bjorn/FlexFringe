//
// Created by bjorn on 9-5-2026.
//

#ifndef FLEXFRINGE_GREEDY_H
#define FLEXFRINGE_GREEDY_H
#include <mcts/Refinements.h>
#include <mcts/comparison/Algorithm.h>
#include <mcts/propagate/QualityEvaluation.h>

struct GreedyAlgorithm : Algorithm {
private:
    const std::function<bool(refinement*, refinement*)> comparator;

    refinement* getRefinement(const std::shared_ptr<MCTSNode>& node) const;

protected:
    refinement_vector updateRefinements(const std::shared_ptr<MCTSNode>& root) const override;

public:
    template<RefinementComparator Comparator>
    explicit GreedyAlgorithm(Comparator comparator, state_merger* merger, std::shared_ptr<QualityEvaluation> evaluator, std::shared_ptr<MCTSNodeFactory> factory) : Algorithm(merger, std::move(evaluator), std::move(factory)), comparator(comparator) {
    }

    explicit GreedyAlgorithm(state_merger* merger, std::shared_ptr<QualityEvaluation> evaluator, std::shared_ptr<MCTSNodeFactory> factory) : GreedyAlgorithm(score_compare{}, merger, std::move(evaluator), std::move(factory)) {
    }
};
#endif //FLEXFRINGE_GREEDY_H
