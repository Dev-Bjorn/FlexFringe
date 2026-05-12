//
// Created by bjorn on 6-5-2026.
//


#include <mcts/node/printer/MCTSPrinter.h>

std::string MCTSPrinter::getRefinementName(refinement* ref) {
    if (ref->type() == 3) return "Extend";
    return "Merge";
}

std::string MCTSPrinter::involvedRefinementNodes(refinement* refinement) {
    if (const auto mr = dynamic_cast<merge_refinement*>(refinement); mr != nullptr) {
        return std::to_string(mr->red->get_number()) + ", " + std::to_string(mr->blue->get_number());
    }
    if (const auto er = dynamic_cast<extend_refinement*>(refinement); er != nullptr) {
        return std::to_string(er->red->get_number());
    }
    return "Unknown";
}

int MCTSPrinter::visits(refinement* ref) {
    if (const auto mr = dynamic_cast<merge_refinement*>(ref); mr != nullptr) {
        return mr->blue->get_size();
    }
    if (const auto er = dynamic_cast<extend_refinement*>(ref); er != nullptr) {
        return er->red->get_size();
    }
    return ref->red->get_size();
}
