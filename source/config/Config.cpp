//
// Created by bjorn on 9-5-2026.
//

#include <loguru.hpp>
#include <parameters.h>
#include <config/Config.h>

void HeuristicConfig::log() {
    LOG_S(INFO) << "Heuristic Config";
    LOG_S(INFO) << "-------------------------------------";
    LOG_S(INFO) << "Configuration Name: " << CONFIG_NAME;
    LOG_S(INFO) << "Heuristic Name: " << HEURISTIC_NAME;
    LOG_S(INFO) << "Data Name: " << DATA_NAME;
    LOG_S(INFO) << "Randomize Scores: " << RANDOMIZE_SCORES;
    LOG_S(INFO) << "Depth First: " << DEPTH_FIRST;
    LOG_S(INFO) << "Use Sinks: " << USE_SINKS;
    LOG_S(INFO) << "Sink Count: " << SINK_COUNT;
    LOG_S(INFO) << "Convert Sink States: " << CONVERT_SINK_STATES;
    LOG_S(INFO) << "Sink Type: " << SINK_TYPE;
    LOG_S(INFO) << "Extend Any Red: " << EXTEND_ANY_RED;
    LOG_S(INFO) << "Extend Sinks: " << EXTEND_SINKS;
    LOG_S(INFO) << "Merge Most Visited: " << MERGE_MOST_VISITED;
    LOG_S(INFO) << "Merge Blue Blue: " << MERGE_BLUE_BLUE;
    LOG_S(INFO) << "Merge Red Red: " << MERGE_RED_RED;
    LOG_S(INFO) << "Merge Local: " << MERGE_LOCAL;
    LOG_S(INFO) << "Merge Local Collector Count: " << MERGE_LOCAL_COLLECTOR_COUNT;
    LOG_S(INFO) << "Merge Root: " << MERGE_ROOT;
    LOG_S(INFO) << "Merge When Testing: " << MERGE_WHEN_TESTING;
    LOG_S(INFO) << "Merge Data: " << MERGE_DATA;
    LOG_S(INFO) << "Merge Sinks: " << MERGE_SINKS;
    LOG_S(INFO) << "Merge Sinks With Core: " << MERGE_SINKS_WITH_CORE;
    LOG_S(INFO) << "Merge Identical Sinks: " << MERGE_IDENTICAL_SINKS;
    LOG_S(INFO) << "Red Fixed: " << RED_FIXED;
    LOG_S(INFO) << "All Fixed: " << ALL_FIXED;
    LOG_S(INFO) << "Markovian Model: " << MARKOVIAN_MODEL;
    LOG_S(INFO) << "Final Probabilities: " << FINAL_PROBABILITIES;
    LOG_S(INFO) << "Use Lower Bound: " << USE_LOWER_BOUND;
    LOG_S(INFO) << "Lowerbound: " << LOWER_BOUND;
    LOG_S(INFO) << "Extend Score: " << EXTEND_SCORE;
    LOG_S(INFO) << "State Count: " << STATE_COUNT;
    LOG_S(INFO) << "Symbol Count: " << SYMBOL_COUNT;
    LOG_S(INFO) << "Correction: " << CORRECTION;
    LOG_S(INFO) << "Correction Seen: " << CORRECTION_SEEN;
    LOG_S(INFO) << "Correction Unseen: " << CORRECTION_UNSEEN;
    LOG_S(INFO) << "Correction Per Seen: " << CORRECTION_PER_SEEN;
    LOG_S(INFO) << "Check Parameter: " << CHECK_PARAMETER;
    LOG_S(INFO) << "Type Distributions: " << TYPE_DISTRIBUTIONS;
    LOG_S(INFO) << "GINI Lambda" << GINI_LAMBDA;
    LOG_S(INFO) << "Symbol Distributions: " << SYMBOL_DISTRIBUTIONS;
    LOG_S(INFO) << "Type Consistent: " << TYPE_CONSISTENT;
    LOG_S(INFO) << "Nrows Sketches: " << NROWS_SKETCHES;
    LOG_S(INFO) << "Ncolumns Sketches: " << NCOLUMNS_SKETCHES;
    LOG_S(INFO) << "Distance Metric Sketches: " << DISTANCE_METRIC_SKETCHES;
    LOG_S(INFO) << "Random Initialization Sketches: " << RANDOM_INITIALIZATION_SKETCHES;
    LOG_S(INFO) << "Nsteps Sketches: " << NSTEPS_SKETCHES;
    LOG_S(INFO) << "DFA Size Bound: " << DFA_SIZE_BOUND;
    LOG_S(INFO) << "APTA Size Bound: " << APTA_SIZE_BOUND;
    LOG_S(INFO) << "Perform Symbol Check: " << PERFORM_SYMBOL_CHECK;
    LOG_S(INFO) << "Perform Depth Check: " << PERFORM_DEPTH_CHECK;
    LOG_S(INFO) << "Depth Check Max Depth: " << DEPTH_CHECK_MAX_DEPTH;
    LOG_S(INFO) << "Perform Merge Check: " << PERFORM_MERGE_CHECK;
    LOG_S(INFO) << "KTAIL: " << KTAIL;
    LOG_S(INFO) << "IDENTICAL_KTAIL: " << IDENTICAL_KTAIL;
    LOG_S(INFO) << "KSTATE: " << KSTATE;
    LOG_S(INFO) << "STAR FREE: " << STAR_FREE;
    LOG_S(INFO) << "-------------------------------------";
}

HeuristicConfig getConfiguration(const std::string &config_name) {
    for (const auto &config: HEURISTIC_CONFIGS) {
        if (config.CONFIG_NAME == config_name) {
            return config;
        }
    }
    throw std::runtime_error("Configuration not found: " + config_name);
}
