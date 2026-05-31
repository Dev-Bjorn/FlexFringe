//
// Created by bjorn on 13-5-2026.
//

#include <apta.h>
#include <refinement.h>
#include <mcts/Refinements.h>

bool ref_equal(const refinement* ref1, const refinement* ref2) {
  if (ref1 == ref2) return true;
  if (ref1->type() != ref2->type()) return false;
  if (ref1->red->get_number() != ref2->red->get_number()) return false;
  if (ref1->type() == 3) return true;
  if (ref1->type() == 2) {
    auto mr1 = dynamic_cast<const merge_refinement*>(ref1);
    auto mr2 = dynamic_cast<const merge_refinement*>(ref2);
    return mr1->blue->get_number() == mr2->blue->get_number();
  }
  if (ref1->type() == 1) {
    auto sr1 = dynamic_cast<const split_refinement*>(ref1);
    auto sr2 = dynamic_cast<const split_refinement*>(ref2);
    return sr1->attribute == sr2->attribute && sr1->split_point == sr2->split_point;
  }
  return false;
}

std::string_view to_ref_string(const refinement* ref) {
  if (ref->type() == 3) {
    return "x" + std::to_string(ref->red->get_number());
  };
  if (ref->type() == 2) {
    auto mr1 = dynamic_cast<const merge_refinement*>(ref);
    return "m(" + std::to_string(mr1->blue->get_number()) + ", " + std::to_string(mr1->blue->get_number()) + ")";
  }
  if (ref->type() == 1) {
    return "s(" + std::to_string(ref->red->get_number()) +")";
  }
  return "";
}