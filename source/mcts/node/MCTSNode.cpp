//
// Created by bjorn on 4-5-2026.
//

#include <mcts/MCTS.h>
#include <mcts/node/MCTSNode.h>

MCTSNode::MCTSNode(
    const int id,
    const std::shared_ptr<NodeData> &context,
    refinement *ref,
    const int dfaSize,
    refinement_vector possibleRefs,
    refinement_vector extendRefs,
    std::shared_ptr<MCTSNode> parent
) : id(id),
    context(context),
    currentRef(ref),
    dfaSize(dfaSize),
    refs(std::move(possibleRefs)),
    extendRefs(std::move(extendRefs)),
    unvisitedRefs(this->refs),
    unvisitedExtendRefs(this->extendRefs),
    parent(std::move(parent)) {
    // Reserve locations for children and visited nodes
    children.reserve(possibleRefs.size() + extendRefs.size());
}

bool MCTSNode::isTerminal(const refinement_vector &refs, const refinement_vector &extendRefs) {
    return refs.empty() && extendRefs.empty();
}

bool MCTSNode::isTerminal() const {
    return isTerminal(refs, extendRefs);
}

bool MCTSNode::isVisited(refinement *refinement) const {
    if (refinement->type() == 3) {
        return std::ranges::find(unvisitedExtendRefs, refinement) != unvisitedExtendRefs.end();
    } else {
        return std::ranges::find(unvisitedRefs, refinement) != unvisitedRefs.end();
    }
}

bool equal(refinement* ref1, refinement* ref2) {
    if (ref1 == ref2) return true;
    if (ref1->type() != ref2->type()) return false;
    if (ref1->red->get_number() != ref2->red->get_number()) return false;
    if (ref1->type() == 3) return true;
    if (ref1->type() == 2) {
        auto mr1 = dynamic_cast<const merge_refinement*>(ref1);
        auto mr2 = dynamic_cast<const merge_refinement*>(ref2);
        return mr1->blue->get_number() == mr2->blue->get_number();
    }

}

size_t findIndex(const refinement_vector &refs, const int index, refinement *refinement) {
    if (index < 0 || static_cast<std::size_t>(index) >= refs.size()) {
        for (size_t i = 0; i < refs.size(); ++i) {
            const auto &ref = refs.at(i);
            if (equal(ref, refinement)) {
                return i;
            }
        }
        return refs.size();
    }
    return index;
}

void MCTSNode::expand(std::shared_ptr<MCTSNode> child, const int index) {
    auto &mutRef = child->currentRef->type() == 3 ? unvisitedExtendRefs : unvisitedRefs;
    const size_t idx = findIndex(mutRef, index, child->currentRef);
    if (idx >= mutRef.size()) {
        throw std::out_of_range("non-existing refinement index");
    }

    if (idx == mutRef.size() - 1) mutRef[idx] = std::move(mutRef.back());
    mutRef.pop_back();

    children.push_back(std::move(child));
}


void MCTSNode::undo(state_merger *merger) const {
    if (currentRef == nullptr) return;
    currentRef->undo(merger);
}

void MCTSNode::doRef(state_merger *merger) const {
    if (currentRef == nullptr) return;
    currentRef->doref(merger);
}

void MCTSNode::updateContext(double value) const {
    context->update(value);
}

std::string MCTSNode::toString() const {
    return "Node " + std::to_string(id);
}

void MCTSNode::setScore(double newScore) {
    if (score != -1) return;
    this->score = newScore;
}
