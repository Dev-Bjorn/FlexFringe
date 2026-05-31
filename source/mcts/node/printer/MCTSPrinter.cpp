//
// Created by bjorn on 6-5-2026.
//


#include <mcts/node/printer/MCTSPrinter.h>

std::string MCTSPrinter::getRefinementName(const refinement* ref) {
    if (ref->type() == 3) return "Extend";
    if (ref->type() == 2) return "Merge";
    if (ref->type() == 1) return "Split";
    throw std::runtime_error("Unknown refinement type");
}

std::string MCTSPrinter::involvedRefinementNodes(const refinement* refinement) {
    if (const auto mr = dynamic_cast<const merge_refinement*>(refinement); mr != nullptr) {
        return std::to_string(mr->red->get_number()) + ", " + std::to_string(mr->blue->get_number());
    }
    if (const auto er = dynamic_cast<const extend_refinement*>(refinement); er != nullptr) {
        return std::to_string(er->red->get_number());
    }
    if (const auto sr = dynamic_cast<const split_refinement*>(refinement); sr != nullptr) {
        return std::to_string(sr->red->get_number());
    }
    return "Unknown";
}

int MCTSPrinter::visits(const refinement* ref) {
    if (const auto mr = dynamic_cast<const merge_refinement*>(ref); mr != nullptr) {
        return mr->blue->get_size();
    }
    if (const auto er = dynamic_cast<const extend_refinement*>(ref); er != nullptr) {
        return er->red->get_size();
    }
    if (const auto sr = dynamic_cast<const split_refinement*>(ref); sr != nullptr) {
        return sr->red->get_size();
    }
    return ref->red->get_size();
}
