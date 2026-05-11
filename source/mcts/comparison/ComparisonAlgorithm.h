//
// Created by bjorn on 9-5-2026.
//

#ifndef FLEXFRINGE_COMPARISON_ALGORITHM_H
#define FLEXFRINGE_COMPARISON_ALGORITHM_H
#include <mcts/node/MCTSNode.h>
#include <mcts/propagate/QualityEvaluation.h>


typedef std::vector<std::shared_ptr<MCTSNode>> refinementNodeMap;
typedef std::unordered_map<refinement *, int> refinementSizeMap;

struct AlgorithmResult {
private:
    std::string_view algorithm;
    refinement_vector refs;
    refinementNodeMap refNodes;
    refinementSizeMap refSizes;
    double score;

public:
    /**
     * The algorithm result
     * @param algorithm The used algorithm
     * @param refs The refinements applied to the MCTS tree, in the order from root to leaf
     * @param refNodes The nodes affected by the refinements
     * @param refSizes The size of each node after applying the refinements
     * @param score The score of the algorithm result
     */
    explicit AlgorithmResult(
        const std::string_view algorithm,
        refinement_vector refs,
        refinementNodeMap refNodes,
        refinementSizeMap refSizes,
        const double score
    ) : algorithm(algorithm), refs(std::move(refs)), refNodes(std::move(refNodes)), refSizes(std::move(refSizes)),
        score(score) {
    }

    /**
     * Get the applied refinements in the order from the root to leaf.
     * @return The applied refinements
     */
    refinement_vector getRefs() const { return refs; }

    /**
     * Get the size of the DFA for the given refinement.
     * @param refinement The refinement to get the DFA size for
     * @return The size of the DFA for the refinement
     */
    int getDFASize(refinement *refinement) const;

    /**
     * Get the final quality score.
     * @return The final quality score.
     */
    inline double getScore() const { return score; }

    /**
     * Check whether the given node has a refinement applied.
     * @param node The node to check
     * @return True if the node has a refinement, false otherwise
     */
    bool hasRefinement(const std::shared_ptr<MCTSNode> &node) const;

    /**
     * Get the applied refinements from root to leaf, when the MCTS has only non
     * @return Iterator to the beginning of non-child refinements
     */
    refinement_vector::const_iterator beginNonChildRefinements();

    /**
     * @return Iterator to the end of refinements
     */
    refinement_vector::const_iterator end() const;

    /**
     * Get the leaf node of the MCTS tree, where the refinement has also been applied
     * @return The leaf node of the MCTS tree, where the refinement has also been applied
     */
    std::shared_ptr<MCTSNode> getLastChildDecision();

    inline std::string_view getAlgorithm() const {
        return algorithm;
    }
};

struct ComparisonAlgorithm {
private:
    // --- EVALUATOR ---
    std::shared_ptr<QualityEvaluation> evaluator;

protected:
    // --- CONTEXT ---
    state_merger *merger;

    /**
     * Get the refinements to apply from root to leaf to find the final DFA.
     * @param root The root of the MCTS tree to get refinements for
     * @return A log of refinements to apply from root to leaf
     */
    [[nodiscard]] virtual refinement_vector getRefinements(const std::shared_ptr<MCTSNode> &root) const = 0;

    /**
     * Get child node that contains the refinement
     * @param node The node to search for the child
     * @param refinement The refinement to search for
     * @return The child node containing the refinement, or nullptr if not found
     */
    static std::shared_ptr<MCTSNode> getChild(const std::shared_ptr<MCTSNode> &node, const refinement *refinement);

public:
    virtual ~ComparisonAlgorithm() = default;

    /**
     * Create an algorithm to compare the results of the underlying algorithm against an MCTS run.
     * @param merger The state merger to use
     * @param evaluator The state evaluator to use
     */
    explicit ComparisonAlgorithm(state_merger *merger, std::shared_ptr<QualityEvaluation> evaluator) : merger(merger), evaluator(std::move(evaluator)) {

    };

    /**
     * Run the underlying algorithm on the given MCTS tree, this provides a comparison of the current algorithm.
     * @param root The root of the MCTS tree to run the algorithm on
     * @return The result of the algorithm
     */
    [[nodiscard]] std::shared_ptr<AlgorithmResult> run(const std::shared_ptr<MCTSNode> &root) const;

private:
    /**
     * Undo the refinements applied to the MCTS tree
     * @param refs The refinements to undo, in the order from root to leaf
     * @return A map of the size of each node after applying the refinements
     */
    [[nodiscard]] refinementSizeMap undo(const refinement_vector &refs) const;

    /**
     * Find the nodes that are affected by the refinements
     * @param root The root of the MCTS tree to find the affected nodes for
     * @param refs The refinements to find the affected nodes for, in the order from root to leaf
     * @return A map of nodes that are affected by the refinements
     */
    static refinementNodeMap findRefinementMap(const std::shared_ptr<MCTSNode> &root, const refinement_vector &refs);
};

/**
 * Create the comparison algorithm
 * @param name The name of the algorithm to create
 * @param merger The state merger to use
 * @param evaluator The state evaluator to use
 * @return A unique pointer to the created algorithm
 */
std::unique_ptr<ComparisonAlgorithm> createAlgorithm(std::string_view name, state_merger* merger, std::shared_ptr<QualityEvaluation> evaluator);

#endif //FLEXFRINGE_COMPARISON_ALGORITHM_H
