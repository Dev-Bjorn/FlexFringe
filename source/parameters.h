#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__

#include <string>
#include <unordered_set>
#include <vector>
#include <config/Config.h>
#include <config/MCTSConfig.h>

double random_double();

extern HeuristicConfig CURRENT_CONFIG;
// Recommend to ONLY use in MAIN and Runner main function, since the rest is propagated and touches no global variables.
extern MCTSConfig MCTS_CONFIG;
extern std::vector<HeuristicConfig> HEURISTIC_CONFIGS;
extern std::unordered_set<std::string> ACTIVE_HEURISTICS;

// General Parameters
extern std::string INPUT_FILE;
extern std::string OUTPUT_FILE;
extern std::string OUTPUT_TYPE;
extern std::string LOG_PATH;
extern std::string OPERATION_MODE;
extern std::string APTA_FILE;
extern std::string APTA_FILE2;
extern std::string COMMAND_LINE;

extern bool DEBUGGING;
extern bool ADD_TAILS;
extern int PARENT_SIZE_THRESHOLD;

// TRACES SETTINGS
extern bool REVERSE_TRACES;
extern bool STORE_ACCESS_STRINGS;

// SLIDING WINDOW SETTINGS
extern bool SLIDING_WINDOW;
extern int SLIDING_WINDOW_SIZE;
extern int SLIDING_WINDOW_STRIDE;
extern bool SLIDING_WINDOW_TYPE;
extern bool SLIDING_WINDOW_ADD_SHORTER;

// SAT SOLVER SETTINGS
extern std::string SAT_SOLVER;
extern int OFFSET;
extern int EXTRA_STATES;  // Unused
extern bool TARGET_REJECTING;
extern bool SYMMETRY_BREAKING;
extern bool FORCING;
extern bool SAT_RUN_GREEDY;
extern bool MERGE_SINKS_PRESOLVE;

// SEARCHER SETTINGS
extern bool SEARCH_SINKS;
extern bool SEARCH_DEEP;
extern bool SEARCH_LOCAL;
extern bool SEARCH_GLOBAL;
extern bool SEARCH_PARTIAL;
extern double ALIGN_SKIP_PENALTY;
extern double ALIGN_DISTANCE_PENALTY;

// PREDICT SETTINGS
extern bool PREDICT_RESET;
extern bool PREDICT_REMAIN;
extern bool PREDICT_ALIGN;
extern bool PREDICT_MINIMUM;
extern bool PREDICT_TYPE;
extern bool PREDICT_TYPE_PATH;
extern bool PREDICT_SYMBOL;
extern bool PREDICT_TRACE;
extern bool PREDICT_DATA;

// DIFFERENCING SETTINGS
extern int DIFF_SIZE;
extern int DIFF_MAX_LENGTH;
extern double DIFF_MIN;


// PRINTING DFA
extern bool PRINT_WHITE;
extern bool PRINT_BLUE;
extern bool PRINT_RED;
extern bool OUTPUT_SINKS;

// Unused
extern bool RED_BLUE_THRESHOLD;
extern std::string EVALUATION_PARAMETERS;
extern int ENSEMBLE_RUNS;

#endif
