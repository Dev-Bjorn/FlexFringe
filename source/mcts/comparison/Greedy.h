//
// Created by bjorn on 9-5-2026.
//

#ifndef FLEXFRINGE_GREEDY_H
#define FLEXFRINGE_GREEDY_H
#include <mcts/Refinements.h>
#include <mcts/comparison/ComparisonAlgorithm.h>
#include <mcts/propagate/QualityEvaluation.h>

struct GreedyAlgorithm : ComparisonAlgorithm {
private:
    const std::function<bool(refinement*, refinement*)> comparator;

    refinement *getRefinement(const refinement_vector &refs, const refinement_vector &extendRefs) const;

protected:
    [[nodiscard]] refinement_vector getRefinements(const std::shared_ptr<MCTSNode> &root) const override;

public:
    template <RefinementComparator Comparator>
    explicit GreedyAlgorithm(Comparator comparator, state_merger* merger, std::shared_ptr<QualityEvaluation> evaluator) : ComparisonAlgorithm(merger, std::move(evaluator)), comparator(comparator) {}

    explicit GreedyAlgorithm(state_merger* merger, std::shared_ptr<QualityEvaluation> evaluator) : GreedyAlgorithm(score_compare{}, merger, std::move(evaluator)) {}
};
#endif //FLEXFRINGE_GREEDY_H
