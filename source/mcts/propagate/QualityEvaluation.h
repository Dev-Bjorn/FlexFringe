//
// Created by bjorn on 5-5-2026.
//

#ifndef FLEXFRINGE_QUALITY_STATE_EVALUATION_H
#define FLEXFRINGE_QUALITY_STATE_EVALUATION_H
#include <state_merger.h>

/**
 * @brief Evaluates the quality of the state merger.
 */
struct QualityEvaluation {
    virtual ~QualityEvaluation() = default;

    /**
     * Get the score of the current state of the state merger
     * @param merger The state merger to evaluate
     * @return The score of the current state of the state merger
     */
    virtual double evaluate(const state_merger* merger) const = 0;

    /**
     * Compare two scores evaluated from this policy
     * @param score1 The first score to compare
     * @param score2 The second score to compare
     * @return True if score1 is better than score2, false otherwise
     */
    virtual bool compare(double score1, double score2) const = 0;
};

/**
 * Create the quality evaluation
 * @param evaluator The name of the quality evaluation to create
 * @return A unique pointer to the created quality evaluation
 */
std::unique_ptr<QualityEvaluation> createQualityEvaluation(std::string_view evaluator);

#endif //FLEXFRINGE_QUALITY_STATE_EVALUATION_H
