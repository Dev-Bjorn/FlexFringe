#include "state_merger.h"
#include "evaluate.h"
#include "gini.h"
#include "parameters.h"

REGISTER_DEF_DATATYPE(gini_data);
REGISTER_DEF_TYPE(gini);

namespace {
    struct binary_weighted_counts {
        double pos = 0.0;
        double neg = 0.0;

        double total() const{
            return pos + neg;
        }
    };

    double tail_weight(tail* t){
        int positions = t->get_length() + 1;
        if(positions <= 0) return 1.0;
        return 1.0 / static_cast<double>(positions);
    }

    /* Returns weighted label counts AND the raw tail count (for evidence
     * gating). The raw count is unaffected by GINI_LAMBDA or tail type,
     * so it never stays zero on a node that actually has tails. */
    struct counted_result {
        binary_weighted_counts counts;
        int raw_tail_count = 0;
    };

    counted_result weighted_total_counts_ex(apta_node* node){
        counted_result result;

        for(tail_iterator it = tail_iterator(node); *it != nullptr; ++it){
            tail* t = *it;
            ++result.raw_tail_count;

            double weight = tail_weight(t) * (t->is_final() ? CURRENT_CONFIG.GINI_LAMBDA : (1.0 - CURRENT_CONFIG.GINI_LAMBDA));

            if(t->get_type() == 1){
                result.counts.pos += weight;
            } else if(t->get_type() == 0){
                result.counts.neg += weight;
            }
        }

        return result;
    }

    binary_weighted_counts weighted_total_counts(apta_node* node){
        return weighted_total_counts_ex(node).counts;
    }

    /* Gini impurity mass: total * (1 - sum_of_squared_probs).
     * Used for split scoring. */
    double binary_gini_mass(const binary_weighted_counts& counts){
        double total = counts.total();
        if(total == 0.0) return 0.0;

        double pos_prob = counts.pos / total;
        double neg_prob = counts.neg / total;
        return total * (1.0 - ((pos_prob * pos_prob) + (neg_prob * neg_prob)));
    }

    /* Hellinger distance between two binary distributions.
     *
     * H(p, q) = (1/sqrt(2)) * sqrt( (sqrt(p+) - sqrt(q+))^2
     *                               + (sqrt(p-) - sqrt(q-))^2 )
     *
     * Returns a value in [0, 1]. Higher means the two nodes have more
     * dissimilar label distributions, so merging them is more costly.
     * Returns 0 when either node is empty (no evidence of divergence).
     */
    double binary_hellinger(const binary_weighted_counts& left,
                            const binary_weighted_counts& right){
        double lt = left.total();
        double rt = right.total();
        if(lt == 0.0 || rt == 0.0) return 0.0;

        double lp = std::sqrt(left.pos  / lt);
        double ln = std::sqrt(left.neg  / lt);
        double rp = std::sqrt(right.pos / rt);
        double rn = std::sqrt(right.neg / rt);

        return (1.0 / std::sqrt(2.0)) * std::sqrt((lp - rp) * (lp - rp)
                                                 + (ln - rn) * (ln - rn));
    }

    /* Minimum raw tails a node must have before the consistency check fires.
     * Using raw tail count rather than weighted label sum means nodes that
     * have only path tails (type != 0/1) or a high GINI_LAMBDA still get
     * checked once they have enough observations. */
    static constexpr int MIN_EVIDENCE_TAILS = 1;
}

/* -----------------------------------------------------------------------
 * SPLIT scoring — Gini impurity
 * --------------------------------------------------------------------- */

void gini::split_update_score_before(state_merger*, apta_node* left, apta_node* right, tail*){
    binary_weighted_counts left_counts  = weighted_total_counts(left);
    binary_weighted_counts right_counts = weighted_total_counts(right);

    if(right_counts.total() == 0.0)
        num_split += left_counts.total();

    split_score += binary_gini_mass(right_counts);
    split_score += binary_gini_mass(left_counts);
}

void gini::split_update_score_after(state_merger*, apta_node* left, apta_node* right, tail*){
    binary_weighted_counts left_counts  = weighted_total_counts(left);
    binary_weighted_counts right_counts = weighted_total_counts(right);

    if(left_counts.total() == 0.0)
        num_split -= right_counts.total();

    split_score -= binary_gini_mass(right_counts);
    split_score -= binary_gini_mass(left_counts);
}

bool gini::split_compute_consistency(state_merger *, apta_node* left, apta_node* right){
    return true;
}

double gini::split_compute_score(state_merger *, apta_node* left, apta_node* right){
    if(num_split == 0) return -1;
    return (split_score / num_split) - (CURRENT_CONFIG.CHECK_PARAMETER);
}

/* -----------------------------------------------------------------------
 * MERGE scoring — Hellinger distance
 *
 * We accumulate:
 *   merge_score += hellinger(left, right) * min(lt, rt)
 *   num_merge   += min(lt, rt)
 *
 * compute_score returns 1 - (merge_score / num_merge), in [0, 1]:
 *   1 = identical distributions (best merge)
 *   0 = maximally divergent distributions (worst merge)
 *  -1 = sentinel when there is no evidence (num_merge == 0)
 *
 * CONSISTENCY — hard rejection via Hellinger threshold
 *
 * A merge is marked inconsistent when both nodes have sufficient evidence
 * (raw tail count >= MIN_EVIDENCE_TAILS) and their Hellinger distance
 * exceeds CHECK_PARAMETER.
 *
 * Evidence is gated on raw tail count, NOT weighted label sum, so that
 * nodes with only path tails (type != 0/1) or high GINI_LAMBDA values
 * are not silently skipped.
 * --------------------------------------------------------------------- */

bool gini::compute_consistency(state_merger *merger, apta_node* left, apta_node* right){
    counted_result lc = weighted_total_counts_ex(left);
    counted_result rc = weighted_total_counts_ex(right);

    if(lc.raw_tail_count < MIN_EVIDENCE_TAILS || rc.raw_tail_count < MIN_EVIDENCE_TAILS) return true;

    if(binary_hellinger(lc.counts, rc.counts) > CURRENT_CONFIG.CHECK_PARAMETER){
        inconsistency_found = true;
        return false;
    }
    return true;
}

void gini::update_score(state_merger *, apta_node* left, apta_node* right){
    binary_weighted_counts left_counts  = weighted_total_counts(left);
    binary_weighted_counts right_counts = weighted_total_counts(right);

    double lt = left_counts.total();
    double rt = right_counts.total();

    /* Skip when either side has no weighted label mass — Hellinger is
     * undefined and nothing useful can be accumulated. */
    if(lt == 0.0 || rt == 0.0) return;

    double weight = std::min(lt, rt);
    double h = binary_hellinger(left_counts, right_counts);

    merge_score += h * weight;
    num_merge   += weight;
}

double gini::compute_score(state_merger *, apta_node*, apta_node*){
    if(num_merge == 0) return -1.0;
    return 1.0 - (merge_score / num_merge);
}

void gini::reset(state_merger *merger){
    inconsistency_found = false;
    num_pos   = 0;
    num_neg   = 0;
    merge_score = 0.0;
    split_score = 0.0;
    num_split   = 0.0;
    num_merge   = 0.0;
}