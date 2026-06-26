#!/bin/sh
# =============================================================================
# run_mcts.sh — MCTS experiment runner for DFA inference (FlexFringe)
# =============================================================================
# Usage:
#   ./run_mcts.sh [RUNS] [PROBLEM_ID]
#
# RUNS      : number of experiments to run in this invocation (default: 1).
# PROBLEM_ID: StaMiNa dataset number (default: 12).
#   The script resolves the data file automatically as:
#     <PROJECT_DIR>/data/staminadata/<PROBLEM_ID>_training.txt.dat
#   Override the resolved path by setting PROBLEM_FILE directly.
#
# Each invocation picks up the next unfinished experiment(s)
# (policy → seed → param_val) and runs them, then exits.
# When all experiments are done it prints a completion message and exits 0.
# =============================================================================

to_win_path() {
    case "$(uname -s)" in
        Linux*)
            if echo "$1" | grep -q "^/mnt/[a-z]/"; then
                drive=$(echo "$1" | sed 's|^/mnt/\([a-z]\)/.*|\1|' | tr '[:lower:]' '[:upper:]')
                rest=$(echo "$1" | sed 's|^/mnt/[a-z]/||')
                echo "${drive}:/${rest}"
            else
                echo "$1"
            fi
            ;;
        *) echo "$1" ;;
    esac
}

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
RUNNER="$PROJECT_DIR/cmake-build-release2/flexfringe.exe"
RESULTS_BASE="$PROJECT_DIR/tests2"
RUNS=${1:-1}

# ---------------------------------------------------------------------------
# Experiment configuration
# ---------------------------------------------------------------------------
EXPERIMENT="beta"
PROBLEM_ID=${2:-18}
PROBLEM_FILE="${PROBLEM_FILE:-"$PROJECT_DIR/data/staminadata/${PROBLEM_ID}_training.txt.dat"}"

# Policies (iterated in order)
POLICIES="weighted-merge-first uniform weighted-avg weighted-min uniform-merge-first"

# Beta  : norm-model-size reward metric.     Range: [1, |DFA_0|] per problem.
BETA_MIN=1
BETA_MAX=30

# Gamma : norm-rollout-length reward metric. Range: [1, |DFA_0|] per problem.
GAMMA_MIN=1
GAMMA_MAX=30

# ---------------------------------------------------------------------------
# get_seed_value SEED_NR
# ---------------------------------------------------------------------------
get_seed_value() {
    case "$1" in
        1) echo 96466 ;;
        2) echo 3546  ;;
        3) echo 15478 ;;
        *)
            echo "ERROR: unknown seed_nr '$1', must be 1, 2, or 3." >&2
            exit 1
            ;;
    esac
}

# ---------------------------------------------------------------------------
# get_action_policy POLICY
# ---------------------------------------------------------------------------
get_action_policy() {
    case "$1" in
        weighted-avg)         echo "weighted"                   ;;
        weighted-min)         echo "weighted"                   ;;
        uniform)              echo "uniform"                    ;;
        uniform-merge-first)  echo "uniform-merge-split-first"  ;;
        weighted-merge-first) echo "weighted-merge-split-first" ;;
        *)
            echo "ERROR: unknown policy '$1'." >&2
            exit 1
            ;;
    esac
}

# ---------------------------------------------------------------------------
# get_action_sub_policy POLICY
# ---------------------------------------------------------------------------
get_action_sub_policy() {
    case "$1" in
        weighted-avg) echo "avg" ;;
        weighted-min) echo "min" ;;
        *)            echo "min" ;;
    esac
}

# ---------------------------------------------------------------------------
# get_policy_dir POLICY
# ---------------------------------------------------------------------------
get_policy_dir() {
    case "$1" in
        weighted-avg)         echo "weighted/avg"         ;;
        weighted-min)         echo "weighted/min"         ;;
        uniform)              echo "uniform"              ;;
        uniform-merge-first)  echo "uniform-merge-first"  ;;
        weighted-merge-first) echo "weighted-merge-first" ;;
        *)
            echo "ERROR: unknown policy '$1'." >&2
            exit 1
            ;;
    esac
}

# ---------------------------------------------------------------------------
# get_valuation_policy EXPERIMENT
# ---------------------------------------------------------------------------
get_valuation_policy() {
    case "$1" in
        beta)  echo "norm-model-size(minimise)"     ;;
        gamma) echo "norm-rollout-length(minimise)" ;;
        *)
            echo "ERROR: unknown experiment '$1', must be 'beta' or 'gamma'." >&2
            exit 1
            ;;
    esac
}

# ---------------------------------------------------------------------------
# get_pta_size PROBLEM_ID
#   Returns |DFA_0| (PTA size) from Appendix G, Table 28.
# ---------------------------------------------------------------------------
get_pta_size() {
    case "$1" in
        12) echo 2228 ;;
        16) echo 2013 ;;
        18) echo 2827 ;;
        20) echo 1760 ;;
        *)
            echo "ERROR: problem $1 is not in the experiment set (12, 16, 18, 20)." >&2
            exit 1
            ;;
    esac
}

# ---------------------------------------------------------------------------
# get_exploration_rate PARAM_VAL DFA0_MINUS1
#   C = param_val / (|DFA_0| - 1)
# ---------------------------------------------------------------------------
get_exploration_rate() {
    param_val="$1"
    dfa0_minus1="$2"
    awk -v p="$param_val" -v d="$dfa0_minus1" \
        'BEGIN { printf "%.10g\n", p / d }'
}


# ---------------------------------------------------------------------------
# get_next_experiment
#   Walks policies → seeds → param_vals in order and atomically claims the
#   first unclaimed slot via mkdir. Prints: "POLICY SEED_NR PARAM_VAL"
#   or nothing if all are done.
# ---------------------------------------------------------------------------
get_next_experiment() {
    if [ "$EXPERIMENT" = "beta" ]; then
        p_min=$BETA_MIN
        p_max=$BETA_MAX
    else
        p_min=$GAMMA_MIN
        p_max=$GAMMA_MAX
    fi

    for policy in $POLICIES; do
        policy_dir=$(get_policy_dir "$policy")
        for seed_nr in 1 2 3; do
            parent_dir="$RESULTS_BASE/rollout2_${PROBLEM_ID}/${EXPERIMENT}/${policy_dir}/seed${seed_nr}"
            mkdir -p "$parent_dir"
            v=$p_min
            while [ "$v" -le "$p_max" ]; do
                slot="$parent_dir/${v}"
                if mkdir "$slot" 2>/dev/null; then
                    echo "$policy $seed_nr $v $slot"
                    return
                fi
                v=$((v + 1))
            done
        done
    done
    echo ""
}

# ---------------------------------------------------------------------------
# run_experiment POLICY SEED_NR PARAM_VAL
# ---------------------------------------------------------------------------
run_experiment() {
    policy="$1"
    seed_nr="$2"
    param_val="$3"
    out_dir="$4"

    seed_val=$(get_seed_value "$seed_nr")
    lcb1_c=$(get_exploration_rate "$param_val" "$dfa0_minus1")
    action_policy=$(get_action_policy "$policy")
    action_sub_policy=$(get_action_sub_policy "$policy")

    win_out_dir=$(to_win_path "$out_dir")
    win_ini=$(to_win_path "$PROJECT_DIR/ini/mcts2.ini")
    win_problem_file=$(to_win_path "$PROBLEM_FILE")

    echo ""
    echo "  ┌─ policy=${policy}  seed${seed_nr}(${seed_val})  ${EXPERIMENT}=${param_val}"
    echo "  │  C = ${param_val} / (|DFA_0|=${pta_size} - 1) = ${lcb1_c}"
    echo "  │  valuation-policy = ${valuation_policy}"
    echo "  │  output → ${out_dir}"

    "$RUNNER" \
        --ini                             "$win_ini" \
        --outputfile                      "$win_out_dir/mcts" \
        --logpath                         "$win_out_dir/log.txt" \
        "$win_problem_file" \
        mcts \
        --lcb1-constant                   "$lcb1_c" \
        --rollout-action-seed             "$seed_val" \
        --rollout-action-policy           "$action_policy" \
        --rollout-weighted-extend-scoring "$action_sub_policy"

    status=$?
    if [ $status -eq 0 ]; then
        echo "  └─ DONE (exit 0)"
    else
        echo "  └─ FAILED (exit ${status})"
    fi
    return $status
}

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
valuation_policy=$(get_valuation_policy "$EXPERIMENT")
pta_size=$(get_pta_size "$PROBLEM_ID")
dfa0_minus1=$((pta_size - 1))

echo "============================================================"
echo "  MCTS Experiment Runner"
echo "  Experiment       : ${EXPERIMENT}"
echo "  Valuation policy : ${valuation_policy}"
echo "  Problem          : StaMiNa ${PROBLEM_ID}  (${PROBLEM_FILE})"
echo "  Seeds            : [1:96466  2:3546  3:15478]"
echo "  Policies         : ${POLICIES}"
echo "  Runs             : ${RUNS}"
echo "============================================================"

i=0
failed_runs=""
all_done=0

while [ "$i" -lt "$RUNS" ]; do
    next=$(get_next_experiment)

    if [ -z "$next" ]; then
        all_done=1
        break
    fi

    policy=$(echo "$next"    | cut -d' ' -f1)
    seed_nr=$(echo "$next"   | cut -d' ' -f2)
    param_val=$(echo "$next" | cut -d' ' -f3)
    out_dir=$(echo "$next"   | cut -d' ' -f4-)

    echo ""
    echo "  Run $((i + 1))/${RUNS}: policy=${policy}  seed${seed_nr}  ${EXPERIMENT}=${param_val}"

    run_experiment "$policy" "$seed_nr" "$param_val" "$out_dir"
    if [ $? -ne 0 ]; then
        failed_runs="${failed_runs}    policy=${policy}  seed${seed_nr}  ${EXPERIMENT}=${param_val}\n"
    fi

    echo "  Run $((i + 1))/${RUNS} complete."

    i=$((i + 1))
done

echo ""
echo "============================================================"
if [ "$all_done" -eq 1 ]; then
    echo "  All experiments completed or claimed."
else
    echo "  Stopped after ${i}/${RUNS} runs."
fi
if [ -n "$failed_runs" ]; then
    echo ""
    echo "  Failed runs (delete directory to retry):"
    printf "%b" "$failed_runs"
fi
echo "  Results in : ${RESULTS_BASE}/rollout2_${PROBLEM_ID}/${EXPERIMENT}/"
echo "============================================================"