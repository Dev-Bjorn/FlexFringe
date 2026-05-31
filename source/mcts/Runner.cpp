//
// Created by bjorn on 7-5-2026.
//

#include <printer.h>
#include <ranges>
#include <state_merger.h>
#include <input/inputdatalocator.h>
#include <mcts/MCTS.h>
#include <mcts/Random.h>
#include <mcts/node/MCTSNode.h>
#include <mcts/node/printer/DotPrinter.h>
#include <mcts/node/printer/JSONPrinter.h>


// The MCTS Hot loop
refinement_vector getComparisonResult(state_merger* merger, const MCTS& mcts) {
    std::unique_ptr<QualityEvaluation> evaluator = createQualityEvaluation(mcts.getConfig().QUALITY_EVALUATOR_POLICY, mcts.getConfig());
    const std::unique_ptr<Algorithm>   algorithm = createAlgorithm(mcts.getConfig().COMPARISON_ALGORITHM, merger, std::move(evaluator), mcts.getNodeFactory());
    LOG_S(INFO) << "Comparison Algorithm Prepared";
    LOG_S(INFO) << "Starting Comparison Algorithm";
    return algorithm->run(mcts.getRoot(), comparison);
}


void applyRefinements(state_merger* merger, const refinement_vector& log) {
    for (const auto it: log) {
        it->doref(merger);
    }
}

void undoRefinements(state_merger* merger, const refinement_vector& log) {
    for (const auto it: std::views::reverse(log)) {
        it->undo(merger);
    }
}

void printTree(const MCTS& mcts) {
    std::cout << "Start tree printing to " << OUTPUT_FILE << ".mcts_tree.dot" << std::endl;

    std::ofstream output((OUTPUT_FILE + ".mcts_tree.dot").c_str());
    DotPrinter    printer(output, mcts.getConfig());
    printer.print(mcts.getRoot());
    output.close();

    std::ofstream outputJson((OUTPUT_FILE + ".mcts_tree.json").c_str());
    JSONPrinter   jsonPrinter(outputJson, mcts.getConfig());
    jsonPrinter.print(mcts.getRoot());
    outputJson.close();

    std::cout << "MCTS tree printed to " << OUTPUT_FILE << ".mcts_tree.dot" << std::endl;
}

void eraseAllRefs(const MCTSConfig& cfg, const std::shared_ptr<MCTSNode>& node) {
    std::queue<std::shared_ptr<MCTSNode>> queue;
    queue.push(node);
    while (!queue.empty()) {
        auto n = queue.front();
        queue.pop();

        for (auto refinement: n->getRefinements()) {
            refinement->erase();
        }

        for (auto refinement: n->getExtendRefinements()) {
            refinement->erase();
        }

        if (cfg.STORE_ROLLOUTS) {
            for (const auto& rollout : node->getRollout()) {
                if (!cfg.STORE_ROLLOUT_REFINEMENTS) {
                    rollout.ref->erase();
                    continue;
                }
                for (auto ref : rollout.refs) {
                    ref->erase();
                }

                for (auto extend_ref : rollout.extendRefs) {
                    extend_ref->erase();
                }
            }
        }
        for (const auto& child: n->getChildren()) {
            queue.push(child);
        }
    }

}


void runMCTS(std::unordered_map<std::string, std::tuple<state_merger*, evaluation_function*>>& evals) {
    LOG_S(INFO) << "Using Problem: " << INPUT_FILE;

    LOG_S(INFO) << "Preparing MCTS";
    MCTSConfig config = MCTS_CONFIG;

    auto mctsConfig = CURRENT_CONFIG = getConfiguration(config.MCTS_HEURISTIC_NAME);
    mctsConfig.log();

    auto greedyConfig = getConfiguration(config.COMPARISON_HEURISTIC_NAME);
    greedyConfig.log();

    auto [greedy_state_merger, _] = evals.at(config.COMPARISON_HEURISTIC_NAME);
    auto [mcts_state_merger, __]  = evals.at(config.MCTS_HEURISTIC_NAME);

    LOG_S(INFO) << "MCTS Heuristic: " << mctsConfig.HEURISTIC_NAME;
    LOG_S(INFO) << "Greedy Heuristic: " << greedyConfig.HEURISTIC_NAME;

    auto greedy_file = OUTPUT_FILE + "." + config.COMPARISON_ALGORITHM;
    auto mcts_file   = OUTPUT_FILE + ".mcts";

    config.EXPANSION_ACTION_SEED = resolveSeed(config.EXPANSION_ACTION_SEED);
    LOG_S(INFO) << "Expansion seed: " << config.EXPANSION_ACTION_SEED;

    config.ROLLOUT_ACTION_SEED = resolveSeed(config.ROLLOUT_ACTION_SEED);
    LOG_S(INFO) << "Rollout seed: " << config.ROLLOUT_ACTION_SEED;

    LOG_S(INFO) << "Output files: " << greedy_file << " " << mcts_file;

    print_current_automaton(greedy_state_merger, greedy_file, ".init");
    print_current_automaton(greedy_state_merger, mcts_file, ".init");

    config.log();

    LOG_S(INFO) << "End of Preparation, Starting MCTS run";

    auto       mcts        = MCTS(config, mcts_state_merger);
    const auto refinements = mcts.selectNode();
    LOG_S(INFO) << "MCTS Complete, starting comparison algorithm";
    CURRENT_CONFIG = greedyConfig;

    LOG_S(INFO) << "Preparing Comparison Algorithm";
    const auto result = getComparisonResult(greedy_state_merger, mcts);
    LOG_S(INFO) << "Comparison Algorithm Complete";

    print_current_automaton(greedy_state_merger, greedy_file, ".final");
    undoRefinements(greedy_state_merger, result);

    CURRENT_CONFIG = mctsConfig;

    applyRefinements(mcts_state_merger, refinements);
    print_current_automaton(mcts_state_merger, mcts_file, ".final");

    printTree(mcts);

    LOG_S(INFO) << "MCTS run finished";
    std::cout << "MCTS finished" << std::endl;

    LOG_S(INFO) << "Start Erasing Refinements";
    eraseAllRefs(config, mcts.getRoot());
    LOG_S(INFO) << "Complete Erasing Refinements";
}
