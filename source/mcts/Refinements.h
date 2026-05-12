//
// Created by bjorn on 10-5-2026.
//

#ifndef FLEXFRINGE_REFINEMENTS_HPP
#define FLEXFRINGE_REFINEMENTS_HPP
#include <concepts>
#include <refinement.h>

template<typename Comparator>
concept RefinementComparator = requires(Comparator c, refinement* ref)
{
    { c(ref, ref) } -> std::same_as<bool>;
};


#endif //FLEXFRINGE_REFINEMENTS_HPP
