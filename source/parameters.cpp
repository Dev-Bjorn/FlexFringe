#include "parameters.h"
#include <random>
#include <unordered_set>

std::uniform_real_distribution<double> unif(0.0, 1.0);
std::default_random_engine re;
double random_double(){ return unif(re); }

HeuristicConfig CURRENT_CONFIG = {};
MCTSConfig MCTS_CONFIG = {};
std::vector<HeuristicConfig> HEURISTIC_CONFIGS = {};
std::unordered_set<std::string> ACTIVE_HEURISTICS = {};

std::string HEURISTIC_NAME = "alergia";
std::string EVALUATION_PARAMETERS = "";
std::string INPUT_FILE = "test.dat";
std::string OUTPUT_FILE = "";
std::string OUTPUT_TYPE = "both";
std::string LOG_PATH = "flexfringe.log";

std::string OPERATION_MODE = "greedy";

std::string SAT_SOLVER = "";
std::string APTA_FILE = "";
std::string APTA_FILE2 = "";

std::string COMMAND_LINE = "";

bool DEBUGGING = false;
bool ADD_TAILS = true;
bool RED_BLUE_THRESHOLD = false;
int ENSEMBLE_RUNS = 1;
int PARENT_SIZE_THRESHOLD = -1;
bool REVERSE_TRACES = false;
bool SLIDING_WINDOW = false;
int SLIDING_WINDOW_SIZE = 20;
int SLIDING_WINDOW_STRIDE = 5;
bool SLIDING_WINDOW_TYPE = false;
bool SLIDING_WINDOW_ADD_SHORTER = true;
bool STORE_ACCESS_STRINGS = true;

bool MERGE_SINKS_PRESOLVE = 0;
bool SEARCH_SINKS = false;

int OFFSET = 1;
int EXTRA_STATES = 0;
bool TARGET_REJECTING = 0;
bool SYMMETRY_BREAKING = 0;
bool FORCING = 0;

bool PRINT_WHITE = false;
bool PRINT_BLUE = false;
bool PRINT_RED = true;
bool OUTPUT_SINKS = false;

bool SEARCH_DEEP = false;
bool SEARCH_LOCAL = false;
bool SEARCH_GLOBAL = false;
bool SEARCH_PARTIAL = false;

bool PREDICT_RESET = false;
bool PREDICT_REMAIN = false;
bool PREDICT_ALIGN = false;
bool PREDICT_MINIMUM = false;
bool PREDICT_TYPE = false;
bool PREDICT_TYPE_PATH = false;
bool PREDICT_SYMBOL = false;
bool PREDICT_TRACE = true;
bool PREDICT_DATA = false;

double ALIGN_SKIP_PENALTY = 1.0;
double ALIGN_DISTANCE_PENALTY = 0.1;

int DIFF_SIZE = 1000;
int DIFF_MAX_LENGTH = 50;
double DIFF_MIN = -100.0;

bool SAT_RUN_GREEDY = false;

