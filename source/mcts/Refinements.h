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

bool ref_equal(const refinement* ref1, const refinement* ref2);

std::string_view to_ref_string(const refinement* ref1);



#endif //FLEXFRINGE_REFINEMENTS_HPP
