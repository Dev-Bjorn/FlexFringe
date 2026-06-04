//
// Created by bjorn on 6-5-2026.
//

#ifndef FLEXFRINGE_MODEL_SIZE_EVALUATOR_H
#define FLEXFRINGE_MODEL_SIZE_EVALUATOR_H
#include <mcts/node/MCTSNode.h>
#include <mcts/propagate/QualityEvaluation.h>

struct ModelSizeEvaluator : QualityEvaluation {
    explicit ModelSizeEvaluator(GoalPtr goal) : QualityEvaluation(std::move(goal)) {
    }

    double evaluate(const state_merger* merger, const std::shared_ptr<MCTSNode>& rolloutNode, const refinement_vector& log) const override;


};

struct NormalisedModelSizeEvaluator : QualityEvaluation {
private:
    double initialPTASize;

    double normalise(double value) const;
public:
    explicit NormalisedModelSizeEvaluator(const state_merger* initialMerger, GoalPtr goal) : QualityEvaluation(std::move(goal)), initialPTASize(initialMerger->get_final_apta_size()) {
    }

    double evaluate(const state_merger* merger, const std::shared_ptr<MCTSNode>& rolloutNode, const refinement_vector& log) const override;

};

#endif //FLEXFRINGE_MODEL_SIZE_EVALUATOR_H
