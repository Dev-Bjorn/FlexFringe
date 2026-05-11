//
// Created by bjorn on 8-5-2026.
//

#ifndef FLEXFRINGE_CONFIG_H
#define FLEXFRINGE_CONFIG_H
#include <string>

struct HeuristicConfig {
    std::string CONFIG_NAME;
    std::string HEURISTIC_NAME;
    std::string DATA_NAME;
    double RANDOMIZE_SCORES = 0.0;

    bool DEPTH_FIRST = false;

    bool USE_SINKS = false;
    int SINK_COUNT = 10;
    bool CONVERT_SINK_STATES = false;
    bool SINK_TYPE = false;

    bool EXTEND_ANY_RED = false;
    bool EXTEND_SINKS = false;

    bool MERGE_MOST_VISITED = false;
    bool MERGE_BLUE_BLUE = false;
    bool MERGE_RED_RED = false;
    int MERGE_LOCAL = -1;
    int MERGE_LOCAL_COLLECTOR_COUNT = -1;
    bool MERGE_ROOT = true;
    bool MERGE_WHEN_TESTING = true;
    bool MERGE_DATA = true;
    bool MERGE_SINKS = false;
    bool MERGE_SINKS_WITH_CORE = false;
    bool MERGE_IDENTICAL_SINKS = false;

    bool RED_FIXED = false;
    bool ALL_FIXED = false;

    int MARKOVIAN_MODEL = 0;

    // HEURISTIC SPECIFIC
    bool FINAL_PROBABILITIES = false;
    bool USE_LOWER_BOUND = false;
    float LOWER_BOUND = 0.0;
    float EXTEND_SCORE = 0.0;
    int STATE_COUNT = 25;
    int SYMBOL_COUNT = 10;
    float CORRECTION = 1.0;
    float CORRECTION_SEEN = 0.0;
    float CORRECTION_UNSEEN = 0.0;
    float CORRECTION_PER_SEEN = 0.0;
    float CHECK_PARAMETER = 0.05;
    bool TYPE_DISTRIBUTIONS = false;
    bool SYMBOL_DISTRIBUTIONS = true;
    bool TYPE_CONSISTENT = true;

    // the count-min-sketches
    int NROWS_SKETCHES = 0;
    int NCOLUMNS_SKETCHES = 0;
    int DISTANCE_METRIC_SKETCHES = 1;
    int RANDOM_INITIALIZATION_SKETCHES = 0;
    int NSTEPS_SKETCHES = 2;

    // SIZE_BOUNDS
    int DFA_SIZE_BOUND = -1;
    int APTA_SIZE_BOUND = -1;

    // CHECKS
    bool PERFORM_SYMBOL_CHECK = false;
    bool PERFORM_DEPTH_CHECK = false;
    int DEPTH_CHECK_MAX_DEPTH = -1;
    bool PERFORM_MERGE_CHECK = true;

    // KTAIL config
    int KTAIL = -1;
    int IDENTICAL_KTAIL = -1;
    int KSTATE = -1;

    // NOT ASSIGNED IN CLI
    bool STAR_FREE = false;

    void log();
};

HeuristicConfig getConfiguration(const std::string &config_name);


#endif //FLEXFRINGE_CONFIG_H
