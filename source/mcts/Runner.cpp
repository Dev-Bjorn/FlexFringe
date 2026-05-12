//
// Created by bjorn on 7-5-2026.
//

#include <printer.h>
#include <state_merger.h>
#include <input/inputdatalocator.h>
#include <mcts/MCTS.h>
#include <mcts/Random.h>
#include <mcts/node/MCTSNode.h>
#include <mcts/node/printer/DotPrinter.h>
#include <mcts/node/printer/JSONPrinter.h>

// The MCTS Hot loop
refinement_vector selectNode(const MCTS& mcts) {
    auto node = mcts.select();

    while (!node->isTerminal()) {
        LOG_S(INFO) << "Selected node: " << node->toString() << " with APTA size: " << mcts.getMerger()->get_final_apta_size();
        // AUTO expand when only one unvisited refinement or extend refinement is available
        if (mcts.getConfig().AUTO_EXPAND_ONE_CHILD && (node->getRefinements().size() + node->getExtendRefinements().size()) == 1) {
            LOG_S(INFO) << "Auto expanding node: " << node->toString();
            node = mcts.expand(node);
            continue;
        }
        auto rolloutNode = mcts.expand(node);

        if (rolloutNode == nullptr) {
            return mcts.undoNode(node);
        }
        const auto log = mcts.rollout(rolloutNode);
        LOG_S(INFO) << "Current Expansion: " << rolloutNode->toString() << " after rollout has size: " << mcts.getMerger()->get_final_apta_size();

        const bool converged = !mcts.getConfig().FORCE_UNTIL_TERMINAL && mcts.isConverged(rolloutNode, log);

        mcts.backPropagation(rolloutNode, log);

        if (converged) {
            return mcts.expandLog(rolloutNode, log);
        } else {
            MCTS::eraseRollout(log);
        }

        node = mcts.select();
    }
    LOG_S(INFO) << "Final node: " << node->toString() << " with APTA size: " << mcts.getMerger()->get_final_apta_size();
    LOG_S(INFO) << "Final node score: " << node->getScore();

    return mcts.undoNode(node);
}

refinement_vector getComparisonResult(state_merger* merger, const MCTS& mcts) {
    std::unique_ptr<QualityEvaluation> evaluator = createQualityEvaluation(mcts.getConfig().QUALITY_EVALUATOR_POLICY);
    const std::unique_ptr<Algorithm>   algorithm = createAlgorithm(mcts.getConfig().COMPARISON_ALGORITHM, merger, std::move(evaluator), mcts.getNodeFactory());
    LOG_S(INFO) << "Comparison Algorithm Prepared";
    LOG_S(INFO) << "Starting Comparison Algorithm";
    return algorithm->run(mcts.getRoot(), AlgorithmType::comparison);
}


void applyRefinements(state_merger* merger, const refinement_vector& log) {
    for (const auto it: log) {
        it->doref(merger);
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

void eraseAllRefs(const std::shared_ptr<MCTSNode>& node) {
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

    auto greedy_file = OUTPUT_FILE + ".greedy";
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

    const auto mcts        = MCTS(config, mcts_state_merger);
    const auto refinements = selectNode(mcts);
    LOG_S(INFO) << "MCTS Complete, starting comparison algorithm";
    CURRENT_CONFIG = greedyConfig;

    LOG_S(INFO) << "Preparing Comparison Algorithm";
    const auto result = getComparisonResult(greedy_state_merger, mcts);
    LOG_S(INFO) << "Comparison Algorithm Complete";

    print_current_automaton(greedy_state_merger, greedy_file, ".final");

    CURRENT_CONFIG = mctsConfig;

    applyRefinements(mcts_state_merger, refinements);
    print_current_automaton(mcts_state_merger, mcts_file, ".final");

    printTree(mcts);

    LOG_S(INFO) << "MCTS run finished";
    std::cout << "MCTS finished" << std::endl;

    LOG_S(INFO) << "Start Erasing Refinements";
    eraseAllRefs(mcts.getRoot());
    LOG_S(INFO) << "Complete Erasing Refinements";
}
