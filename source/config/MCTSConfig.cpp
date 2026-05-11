//
// Created by bjorn on 9-5-2026.
//

#include <loguru.hpp>
#include <parameters.h>
#include <ranges>
#include <stringutil.h>
#include <config/MCTSConfig.h>

void MCTSConfig::log() const {
    LOG_S(INFO) << "MCTS Config";
    LOG_S(INFO) << "-------------------------------------";
    LOG_S(INFO) << "MCTS Heuristic: " << MCTS_HEURISTIC_NAME;
    LOG_S(INFO) << "Comparison Heuristic: " << COMPARISON_HEURISTIC_NAME;
    LOG_S(INFO) << "Comparison Algorithm: " << COMPARISON_ALGORITHM;
    LOG_S(INFO) << "Convergence Policies: " << strutil::join(CONVERGENCE_POLICIES, ", ");
    LOG_S(INFO) << "Max Iterations: " << MAX_ITERATIONS;
    LOG_S(INFO) << "Score Threshold: " << SCORE_THRESHOLD;
    LOG_S(INFO) << "Max No Improvement Iterations: " << MAX_NO_IMPROVEMENT;
    LOG_S(INFO) << "Print Unvisited: " << PRINT_UNVISITED;
    LOG_S(INFO) << "Print JSON Line Separator: " << PRINT_JSON_LINE_SEP_BETWEEN_ATTR;
    LOG_S(INFO) << "Expansion Action Policy: " << EXPANSION_ACTION_POLICY;
    LOG_S(INFO) << "Expansion Rule Policy: " << EXPANSION_RULE_POLICY;
    LOG_S(INFO) << "Expansion Action Seed: " << EXPANSION_ACTION_SEED;
    LOG_S(INFO) << "Auto Expand One Child: " << AUTO_EXPAND_ONE_CHILD;
    LOG_S(INFO) << "Rollout Action Policy: " << ROLLOUT_ACTION_POLICY;
    LOG_S(INFO) << "Rollout Action Seed: " << ROLLOUT_ACTION_SEED;
    LOG_S(INFO) << "Rollout Cut Off: " << MAX_ROLLOUT_STEPS;
    LOG_S(INFO) << "Quality Evaluator Policy: " << QUALITY_EVALUATOR_POLICY;
    LOG_S(INFO) << "Selection Node Policy: " << NODE_SELECTION_POLICY;
    LOG_S(INFO) << "Selection Search Method: " << SELECTION_SEARCH_METHOD;
    LOG_S(INFO) << "UCB1 Constant: " << UCB1_CONSTANT;
    LOG_S(INFO) << "LCB1 Constant: " << LCB1_CONSTANT;
    LOG_S(INFO) << "-------------------------------------";
}
