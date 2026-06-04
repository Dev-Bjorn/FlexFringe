//
// Created by bjorn on 20-5-2026.
//

#ifndef FLEXFRINGE_ROLLOUTLENGTH_H
#define FLEXFRINGE_ROLLOUTLENGTH_H
#include <mcts/node/MCTSNode.h>
#include <mcts/propagate/QualityEvaluation.h>

struct RolloutLengthEvaluator : QualityEvaluation {
    explicit RolloutLengthEvaluator(GoalPtr goal) : QualityEvaluation(std::move(goal)) {
    }

    double evaluate(const state_merger* merger, const std::shared_ptr<MCTSNode>& rolloutNode, const refinement_vector& log) const override;

};

struct NormalisedRolloutLengthEvaluator : QualityEvaluation {
private:
    double initialPTASize;

    double normalise(double value) const;
public:
    explicit NormalisedRolloutLengthEvaluator(const state_merger* initialMerger, GoalPtr goal) : QualityEvaluation(std::move(goal)), initialPTASize(initialMerger->get_final_apta_size()) {
    }

    double evaluate(const state_merger* merger, const std::shared_ptr<MCTSNode>& rolloutNode, const refinement_vector& log) const override;

};


#endif //FLEXFRINGE_ROLLOUTLENGTH_H
