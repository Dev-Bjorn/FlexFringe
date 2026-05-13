//
// Created by bjorn on 4-5-2026.
//

#ifndef FLEXFRINGE_MCTS_NODE_H
#define FLEXFRINGE_MCTS_NODE_H
#include <memory>
#include <refinement.h>
#include <vector>
#include <mcts/AlgorithmTypes.h>
#include <mcts/propagate/data/NodeData.h>


struct MCTSNode {
private:
    // ---------------- CONTEXT --------------------
    int                               id;
    std::shared_ptr<NodeData>         context;
    refinement*                       currentRef = nullptr;
    double                            score      = -1;
    int                               dfaSize;
    std::unordered_set<AlgorithmType> nodeType;
    int                               height;

    // --------------------- ACTIONS ----------------------
    // All possible actions that follow from this node are stored in the node
    // such that the computation does not require recomputation of the possible refinements.
    // The extend refinements perform when the possible refinements are exhausted.
    const refinement_vector refs;
    const refinement_vector extendRefs;
    // The following will only be used
    refinement_vector unvisitedRefs;
    refinement_vector unvisitedExtendRefs;

    const std::shared_ptr<MCTSNode>        parent = nullptr;
    std::vector<std::shared_ptr<MCTSNode>> children;

public:
    // When both vectors are empty, no further expansion is possible
    // thus meaning the search is complete
    static bool isTerminal(const refinement_vector& refs, const refinement_vector& extendRefs);

    explicit MCTSNode(
        int                              id,
        int                              dfaSize,
        int                              height,
        const std::shared_ptr<NodeData>& context,
        refinement*                      ref,
        refinement_vector                possibleRefs,
        refinement_vector                extendRefs,
        std::shared_ptr<MCTSNode>        parent
    );

    [[nodiscard]] inline int                                           getId() const { return id; }
    [[nodiscard]] inline int                                           getDFASize() const { return dfaSize; }
    [[nodiscard]] inline double                                        getScore() const { return score; }
    [[nodiscard]] inline refinement*                                   getRefinement() const { return currentRef; }
    [[nodiscard]] inline std::shared_ptr<NodeData>                     getContext() const { return context; }
    [[nodiscard]] inline std::shared_ptr<MCTSNode>                     getParent() const { return parent; }
    [[nodiscard]] inline const std::vector<std::shared_ptr<MCTSNode>>& getChildren() const { return children; }
    [[nodiscard]] inline const refinement_vector&                      getRefinements() const { return refs; }
    [[nodiscard]] inline const refinement_vector&                      getExtendRefinements() const { return extendRefs; }
    [[nodiscard]] inline refinement_vector                             getUnvisitedRefinements() const { return unvisitedRefs; }
    [[nodiscard]] inline refinement_vector                             getUnvisitedExtendRefinements() const { return unvisitedExtendRefs; };
    [[nodiscard]] inline bool                                          isInAlgorithm(AlgorithmType algorithm) const { return nodeType.contains(algorithm); };
    [[nodiscard]] inline int                                           getAlgorithmTypes() const { return encodeAlgorithmType(nodeType); };
    [[nodiscard]] inline int                                           getHeight() const { return height; };

    // This means all states are colored red
    bool isTerminal() const;

    void expand(std::shared_ptr<MCTSNode> child, int index);

    bool isVisited(refinement* refinement) const;

    // Undo
    void undo(state_merger* merger) const;

    void doRef(state_merger* merger) const;

    void updateContext(double value) const;

    std::string toString() const;

    void setScore(double score);

    void annotate(const AlgorithmType algorithm) {
        nodeType.insert(algorithm);
    }
};


#endif //FLEXFRINGE_MCTS_NODE_H
