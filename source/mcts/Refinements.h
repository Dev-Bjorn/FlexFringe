//
// Created by bjorn on 10-5-2026.
//

#ifndef FLEXFRINGE_REFINEMENTS_HPP
#define FLEXFRINGE_REFINEMENTS_HPP
#include <apta.h>
#include <concepts>
#include <refinement.h>

template<typename Comparator>
concept RefinementComparator = requires(Comparator c, refinement* ref)
{
    { c(ref, ref) } -> std::same_as<bool>;
};

inline bool equal(refinement* ref1, refinement* ref2) {
    if (ref1 == ref2) return true;
    if (ref1->type() != ref2->type()) return false;
    if (ref1->red->get_number() != ref2->red->get_number()) return false;
    if (ref1->type() == 3) return true;
    if (ref1->type() == 2) {
        auto mr1 = dynamic_cast<const merge_refinement*>(ref1);
        auto mr2 = dynamic_cast<const merge_refinement*>(ref2);
        return mr1->blue->get_number() == mr2->blue->get_number();
    }
    if (ref1->type() == 1) {             // split
        auto sr1 = dynamic_cast<const split_refinement*>(ref1);
        auto sr2 = dynamic_cast<const split_refinement*>(ref2);
        return sr1->attribute == sr2->attribute && sr1->split_point == sr2->split_point;
    }
    return false;
}

inline std::string_view to_ref_string(refinement* ref1) {
    if (ref1->type() == 3) {
        return "x" + std::to_string(ref1->red->get_number());
    };
    if (ref1->type() == 2) {
        auto mr1 = dynamic_cast<const merge_refinement*>(ref1);
        return "m(" + std::to_string(mr1->blue->get_number()) + ", " + std::to_string(mr1->blue->get_number()) + ")";
    }
    return "";
}



#endif //FLEXFRINGE_REFINEMENTS_HPP
