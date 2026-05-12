//
// Created by bjorn on 6-5-2026.
//

#ifndef FLEXFRINGE_NODE_DATA_H
#define FLEXFRINGE_NODE_DATA_H

/**
 * @brief The node data class represents the data associated with a node in the MCTS tree.
 */
struct NodeData {
protected:
    int visits = 0;

public:
    virtual ~NodeData() = default;

    /**
     * Update the node data with a new value and increment the visit count.
     *
     * The implementation method is responsible for updating the node data with a new value
     * and the number of visits
     *
     * @param value The new value to update the node data with.
     */
    virtual void update(double value) = 0;

    [[nodiscard]] inline int getVisits() const { return visits; };
};


#endif //FLEXFRINGE_NODE_DATA_H
