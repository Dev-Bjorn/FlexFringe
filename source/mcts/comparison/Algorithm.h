//
// Created by bjorn on 9-5-2026.
//

#ifndef FLEXFRINGE_COMPARISON_ALGORITHM_H
#define FLEXFRINGE_COMPARISON_ALGORITHM_H
#include <mcts/AlgorithmTypes.h>
#include <mcts/node/MCTSNode.h>
#include <mcts/node/MCTSNodeFactory.h>
#include <mcts/propagate/QualityEvaluation.h>


typedef std::vector<std::shared_ptr<MCTSNode>> algorithmLog;


struct Algorithm {
private:
    AlgorithmType algorithmType = AlgorithmType::comparison;

protected:
    state_merger*                      merger;
    std::shared_ptr<MCTSNodeFactory>   factory;
    std::shared_ptr<QualityEvaluation> evaluator;


    /**
     * Get the refinements to apply from root to leaf to find the final DFA.
     * @param root The root of the MCTS tree to get refinements for
     */
    virtual refinement_vector updateRefinements(const std::shared_ptr<MCTSNode>& root) const = 0;

    /**
     * Get child node that contains the refinement
     * @param node The node to search for the child
     * @param refinement The refinement to search for
     * @return The child node containing the refinement, or nullptr if not found
     */
    static std::shared_ptr<MCTSNode> getChild(const std::shared_ptr<MCTSNode>& node, const refinement* refinement);

    /**
     * Annotate a node that it is part of the algorithmic solution.
     * @param node The node to annotate
     */
    void annotateNode(const std::shared_ptr<MCTSNode>& node) const;

public:
    virtual ~Algorithm() = default;

    /**
     * Create an algorithm to compare the results of the underlying algorithm against an MCTS run.
     * @param merger The state merger to use
     * @param evaluator The state evaluator to use
     * @param factory The MCTS node factory to use
     */
    explicit Algorithm(state_merger* merger, std::shared_ptr<QualityEvaluation> evaluator, std::shared_ptr<MCTSNodeFactory> factory) : merger(merger), factory(std::move(factory)), evaluator(std::move(evaluator)) {
    };

    /**
     * Run the underlying algorithm on the given MCTS tree, this provides a comparison of the current algorithm.
     * @param root The root of the MCTS tree to run the algorithm on
     * @param algorithmType The type of algorithm used
     *
     */
    refinement_vector run(const std::shared_ptr<MCTSNode>& root, AlgorithmType algorithmType);

private:
    /**
     * Undo the refinements applied to the MCTS tree
     * @param refs The refinements to undo, in the order from root to leaf
     */
    void undo(const refinement_vector& refs) const;
};

/**
 * Create the algorithm
 * @param name The name of the algorithm to create
 * @param merger The state merger to use
 * @param evaluator The state evaluator to use
 * @param factory The MCTS node factory to use
 * @return A unique pointer to the created algorithm
 */
std::unique_ptr<Algorithm> createAlgorithm(std::string_view name, state_merger* merger, std::shared_ptr<QualityEvaluation> evaluator, std::shared_ptr<MCTSNodeFactory> factory);

#endif //FLEXFRINGE_COMPARISON_ALGORITHM_H
