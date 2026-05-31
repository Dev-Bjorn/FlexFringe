//
// Created by bjorn on 8-5-2026.
//

#ifndef FLEXFRINGE_MCTS_CONFIG_H
#define FLEXFRINGE_MCTS_CONFIG_H

#include <complex>
#include <string>
#include <vector>

struct MCTSConfig {
     // The heuristic used within MCTS algorithm
     std::string MCTS_HEURISTIC_NAME;
     // The heuristic within the comparison MCTS algorithm
     std::string COMPARISON_HEURISTIC_NAME;
     // The algorithm used within the comparison MCTS algorithm
     std::string COMPARISON_ALGORITHM = "greedy";

     // The convergence policies for MCTS, these are used to determine when to stop the MCTS algorithm.
     // If any of the provided convergence policies are met, the MCTS algorithm will stop.
     // MCTS will always stop the algorithm when the selection policy selects a terminal node.
     //
     // The available convergence policies are: "iteration", "score-threshold", "score-improvement", "node-termination"
     std::vector<std::string> CONVERGENCE_POLICIES = {"node-termination"};
     // The maximum number of iterations for MCTS, only works when CONVERGENCE_POLICIES contains "iteration"
     int MAX_ITERATIONS = 1000;
     // The threshold score for MCTS, only works when CONVERGENCE_POLICIES contains "score-threshold"
     double SCORE_THRESHOLD = 0.0;
     // The maximum number of iterations without improvement for MCTS, only works when CONVERGENCE_POLICIES contains "score-improvement"
     int MAX_NO_IMPROVEMENT = 100;
     // Whether to finish using greedy policy when the number of iterations reaches MAX_ITERATIONS, when the
     // convergence policy is hit and the DFA is not in a terminal state
     bool USE_FINISHER = true;
     // The algorithm to use when finishing the algorithm, only works when USE_FINISHER is true
     std::string FINISH_ALGORITHM = "greedy";
     // Whether to force MCTS to continue until the DFA reaches a terminal state, regardless of convergence
     bool FORCE_UNTIL_TERMINAL = false;

     // whether to print the unexpanded nodes
     bool PRINT_UNVISITED = false;
     // whether to print JSON line separator between attributes
     bool PRINT_JSON_LINE_SEP_BETWEEN_ATTR = false;

     // Expansion Action Policies: first, weighted-merge-split-first, uniform-merge-split-first, uniform, greedy, weighted
     std::string EXPANSION_ACTION_POLICY = "first";
     // Expansion Rule Policies: full, only_merge
     std::string EXPANSION_RULE_POLICY = "full";
     // Expansion sampling seed
     int EXPANSION_ACTION_SEED = 42L;
     // Do not perform a rollout when it only has a single child.
     bool AUTO_EXPAND_ONE_CHILD = true;
     // WEIGHTED ACTION POLICY SETTINGS
     // Weight extend Scoring is used when the scoring of an extend is equal to 0
     // min: every extend score is equal to the min scoring of the split and merge refinements
     // max: every extend score is equal to the max scoring of the split and merge refinements
     // avg: every extend score is equal to the average scoring of the split and merge refinements
     std::string EXPANSION_WEIGHTED_EXTEND_SCORING = "avg";

     // Rollout Action Policies: first, weighted-merge-split-first, uniform-merge-split-first, uniform, greedy, weighted
     std::string ROLLOUT_ACTION_POLICY = "uniform";
     // The seed for the rollout policy, when seed
     // equal to -1, there is no seed
     int ROLLOUT_ACTION_SEED = 42L;
     // The cut off value when the rollout has too many steps
     int MAX_ROLLOUT_STEPS = 1000;
     // WEIGHTED ACTION POLICY SETTINGS
     // Weight extend Scoring is used when the scoring of an extend is equal to 0
     // min: every extend score is equal to the min scoring of the split and merge refinements
     // max: every extend score is equal to the max scoring of the split and merge refinements
     // avg: every extend score is equal to the average scoring of the split and merge refinements
     // none: every extend score is equal to 0 when extend scoring is equal to 0
     std::string ROLLOUT_WEIGHTED_EXTEND_SCORING = "avg";
     // Whether to store the rollout information of an expanded node
     bool STORE_ROLLOUTS = false;
     // Whether to store the all refinements during the rollout as well, when false stores the proportions
     bool STORE_ROLLOUT_REFINEMENTS = false;

     // Quality Policies: model-size
     std::string QUALITY_EVALUATOR_POLICY = "model-size";
     // Goal Policies: the goal of the algorithm.
     std::string GOAL_EVALUATOR_POLICY = "model-size";

     // Selection Policies: lcb1
     std::string NODE_SELECTION_POLICY = "lcb1";
     // Selection Search Method: bfs, best-child-vs-parent
     std::string SELECTION_SEARCH_METHOD = "bfs";

     // The LCB1 constant parameter
     double LCB1_CONSTANT = std::sqrt(2.0);
     // The UCB1 constant parameter
     double UCB1_CONSTANT = std::sqrt(2.0);

     void log() const;
};

#endif //FLEXFRINGE_MCTS_CONFIG_H