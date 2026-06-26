#!/bin/sh
# =============================================================================
# validate_mcts.sh — Validates MCTS experiment logs against expected values
# =============================================================================
# Usage:
#   ./validate_mcts.sh [PROBLEM_ID] [EXPERIMENT]
#
# Outputs a CSV report to results/validation/report_<EXPERIMENT>_<PROBLEM_ID>.csv
# =============================================================================

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
RUNNER="$PROJECT_DIR/cmake-build-release/flexfringe.exe"
RESULTS_BASE="$PROJECT_DIR/results"
VALIDATION_BASE="$PROJECT_DIR/validation"

PROBLEM_ID=${1:-12}
EXPERIMENT=${2:-"gamma"}
PROBLEM_FILE="$PROJECT_DIR/data/staminadata/${PROBLEM_ID}_training.txt.dat"

POLICIES="uniform weighted-avg weighted-min uniform-merge-first weighted-merge-first"

BETA_MIN=1
BETA_MAX=2
GAMMA_MIN=1
GAMMA_MAX=30

get_reward_function() {
    case "$1" in
        gamma) echo "rollout_steps" ;;
        beta) echo "model_size"  ;;
        *) echo "ERROR: unknown seed_nr '$1'." >&2; exit 1 ;;
    esac
}

REWARD=$(get_reward_function "${EXPERIMENT}")


CSV_FILE="$VALIDATION_BASE/report_${REWARD}_${PROBLEM_ID}.csv"

# ---------------------------------------------------------------------------
# Helper functions
# ---------------------------------------------------------------------------
get_seed_value() {
    case "$1" in
        1) echo 96466 ;;
        2) echo 3546  ;;
        3) echo 15478 ;;
        *) echo "ERROR: unknown seed_nr '$1'." >&2; exit 1 ;;
    esac
}

get_action_policy() {
    case "$1" in
        weighted-avg)         echo "weighted"                   ;;
        weighted-min)         echo "weighted"                   ;;
        uniform)              echo "uniform"                    ;;
        uniform-merge-first)  echo "uniform-merge-split-first"  ;;
        weighted-merge-first) echo "weighted-merge-split-first" ;;
        *) echo "ERROR: unknown policy '$1'." >&2; exit 1 ;;
    esac
}

get_action_sub_policy() {
    case "$1" in
        weighted-avg) echo "avg" ;;
        weighted-min) echo "min" ;;
        *)            echo "min" ;;
    esac
}

get_policy_dir() {
    case "$1" in
        weighted-avg)         echo "weighted/avg"         ;;
        weighted-min)         echo "weighted/min"         ;;
        uniform)              echo "uniform"              ;;
        uniform-merge-first)  echo "uniform-merge-first"  ;;
        weighted-merge-first) echo "weighted-merge-first" ;;
        *) echo "ERROR: unknown policy '$1'." >&2; exit 1 ;;
    esac
}

get_valuation_policy() {
    case "$1" in
        beta)  echo "norm-model-size(minimise)"     ;;
        gamma) echo "norm-rollout-length(minimise)" ;;
        *) echo "ERROR: unknown experiment '$1'." >&2; exit 1 ;;
    esac
}

get_pta_size() {
    case "$1" in
        12) echo 2228 ;;
        16) echo 2013 ;;
        18) echo 2827 ;;
        20) echo 1760 ;;
        *) echo "ERROR: problem $1 not in experiment set." >&2; exit 1 ;;
    esac
}

get_exploration_rate() {
    # C = param_val / (|DFA_0| - 1)
    awk -v p="$1" -v d="$2" 'BEGIN { printf "%.6g\n", p / d }'
}

# # extract_field LOG_FILE FIELD_LABEL
extract_field() {
    head -n 171 "$1" | grep -m 1 "$2" | sed 's/.*| //;s/^[^:]*: //;s/^[[:space:]]*//;s/[[:space:]]*$//'
}

parse_log() {
    eval "$(awk 'NR>171{exit}
        function field(s,    v) { sub(/.*\| /, "", s); sub(/^[^:]*: /, "", s); gsub(/^[[:space:]]+|[[:space:]]+$/, "", s); return s }
        /Using Problem:/                 && !p { print "actual_problem=\x27" field($0) "\x27"; p=1 }
        /Rollout Action Seed:/           && !s { print "actual_seed=\x27"    field($0) "\x27"; s=1 }
        /Rollout Action Policy:/         && !a { print "actual_policy=\x27"  field($0) "\x27"; a=1 }
        /Rollout Weighted Extend Scoring:/ && !w { print "actual_sub=\x27"   field($0) "\x27"; w=1 }
        /Quality Evaluator Policy:/      && !q { print "actual_val=\x27"     field($0) "\x27"; q=1 }
        /LCB1 Constant:/                 && !l { print "actual_lcb1=\x27"    field($0) "\x27"; l=1 }
        p&&s&&a&&w&&q&&l { exit }
    ' "$1")"
}

# ---------------------------------------------------------------------------
# check_consistency OUT_DIR
#   Runs predict once, returns "correct/total", "missing", or "no_result".
# ---------------------------------------------------------------------------
check_consistency() {
    out_dir="$1"
    val_dir="$2"
    aptafile="$out_dir/mcts.mcts.final.json"

    if [ ! -f "$aptafile" ]; then
        echo "missing"
        return
    fi

    mkdir -p "$val_dir"
    val_aptafile="$val_dir/mcts.mcts.final.json"
    result_file="$val_dir/mcts.mcts.final.json.result"

    cp "$aptafile" "$val_aptafile"

    "$RUNNER" \
        --mode        predict \
        --ini         "$PROJECT_DIR/ini/edsm.ini" \
        --predicttype 1 \
        --aptafile    "$val_aptafile" \
        --logpath     "$val_dir/predict.log.txt" \
        "$PROBLEM_FILE" \
        > /dev/null 2>&1

    if [ ! -f "$result_file" ]; then
        echo "no_result"
        return
    fi

    # Single awk pass: NF-relative indexing handles variable field counts
    awk -F';' '
        NR == 1 { next }
        {
            actual    = $(NF-3); gsub(/^[[:space:]]+|[[:space:]]+$/, "", actual)
            predicted = $(NF-1); gsub(/^[[:space:]]+|[[:space:]]+$/, "", predicted)
            total++
            if (actual == predicted) correct++
        }
        END {
            if (total == 0) { print "no_data"; exit }
            printf "%d/%d", correct, total
        }
    ' "$result_file"
}

# ---------------------------------------------------------------------------
# validate_experiment OUT_DIR EXP_PROBLEM EXP_SEED EXP_ACTION_POLICY
#                     EXP_SUB_POLICY EXP_VALUATION_POLICY EXP_LCB1_C
#                     POLICY SEED_NR PARAM_VAL
# ---------------------------------------------------------------------------
validate_experiment() {
    out_dir="$1"
    exp_problem="$2"
    exp_seed="$3"
    exp_action_policy="$4"
    exp_sub_policy="$5"
    exp_valuation_policy="$6"
    exp_lcb1_c="$7"
    policy="$8"
    seed_nr="$9"
    param_val="${10}"
    log="$out_dir/log.txt"
    val_dir="$VALIDATION_BASE/stamina_${PROBLEM_ID}/${REWARD}/${policy}/seed${seed_nr}/${param_val}"

    # --- Read log once ---
    parse_log "$log"

    # --- Check fields ---
    cmp() { [ "$1" = "$2" ] && echo "OK" || echo "FAIL"; }
    exp_problem_base=$(basename "$exp_problem")
    actual_problem_base=$(basename "$actual_problem")
    ok_problem=$(cmp "$actual_problem_base" "$exp_problem_base")
    ok_seed=$(cmp    "$actual_seed"         "$exp_seed")
    ok_policy=$(cmp  "$actual_policy"       "$exp_action_policy")
    ok_sub=$(cmp     "$actual_sub"          "$exp_sub_policy")
    ok_val=$(cmp     "$actual_val"          "$exp_valuation_policy")

    # LCB1: compare at 10 significant digits to match spreadsheet precision
    [ "$exp_lcb1_c" = "$actual_lcb1" ] && ok_lcb1="OK" || ok_lcb1="FAIL"

    # Check last line of log for clean exit and extract elapsed seconds
    last_two=$(tail -2 "$log")
    last_line=$(printf '%s\n' "$last_two" | tail -1)
    case "$last_line" in
        *"loguru.cpp"*"atexit"*)
            ok_atexit="OK"
            elapsed_s=$(printf '%s' "$last_line" | sed 's/.*(\([^)]*\)s).*/\1/')
            last_expansion=""
            ;;
        *)
            ok_atexit="FAIL"
            elapsed_s=$(printf '%s' "$last_line" | sed 's/.*(\([^)]*\)s).*/\1/')
            last_expansion=$(printf '%s\n' "$last_two" | grep -m 1 "Current Expansion:" | sed 's/.*Current Expansion: Node \([0-9]*\).*/\1/')
            ;;
    esac

    [ -f "$out_dir/mcts.mcts_tree.json"  ] && ok_tree="OK"  || ok_tree="FAIL"
    [ -f "$out_dir/mcts.mcts.final.json" ] && ok_final="OK" || ok_final="FAIL"

    # --- Consistency ---
    consistency=$(check_consistency "$out_dir" "$val_dir")
    case "$consistency" in
        missing|no_result|no_data) ok_consistency="FAIL" ;;
        *)
            c="${consistency%%/*}"; t="${consistency##*/}"
            [ "$c" = "$t" ] && ok_consistency="OK" || ok_consistency="FAIL"
            ;;
    esac

    # --- Overall ---
    if [ "$ok_problem" = "OK" ] && [ "$ok_seed"        = "OK" ] && \
       [ "$ok_policy"  = "OK" ] && [ "$ok_sub"         = "OK" ] && \
       [ "$ok_val"     = "OK" ] && [ "$ok_lcb1"        = "OK" ] && \
       [ "$ok_atexit"  = "OK" ] && \
       [ "$ok_tree"    = "OK" ] && [ "$ok_final"        = "OK" ] && \
       [ "$ok_consistency" = "OK" ]; then
        overall="PASS"
    else
        overall="FAIL"
    fi

    # --- Write CSV row ---
    echo "${policy},${seed_nr},${param_val},${exp_lcb1_c},${ok_problem}(${actual_problem_base}),${ok_seed},${ok_policy},${ok_sub},${ok_val},${ok_lcb1},${ok_atexit},${ok_tree},${ok_final},${consistency},${elapsed_s},${last_expansion},${overall}" >> "$CSV_FILE"

    # --- Console: only print failures ---
    if [ "$overall" = "FAIL" ]; then
        echo "  FAIL  policy=${policy}  seed${seed_nr}  ${EXPERIMENT}=${param_val}"
        [ "$ok_problem" = "FAIL" ] && echo "        problem          : expected '${exp_problem_base}' got '${actual_problem_base}'"
        [ "$ok_seed"    = "FAIL" ] && echo "        seed             : expected '${exp_seed}' got '${actual_seed}'"
        [ "$ok_policy"  = "FAIL" ] && echo "        action_policy    : expected '${exp_action_policy}' got '${actual_policy}'"
        [ "$ok_sub"     = "FAIL" ] && echo "        sub_policy       : expected '${exp_sub_policy}' got '${actual_sub}'"
        [ "$ok_val"     = "FAIL" ] && echo "        valuation_policy : expected '${exp_valuation_policy}' got '${actual_val}'"
        [ "$ok_lcb1"    = "FAIL" ] && echo "        lcb1_constant    : expected '${exp_lcb1_c}' got '${actual_lcb1}'"
        [ "$ok_atexit"  = "FAIL" ] && echo "        atexit           : log did not end with atexit"
        [ "$ok_tree"    = "FAIL" ] && echo "        mcts_tree.json   : missing"
        [ "$ok_final"   = "FAIL" ] && echo "        mcts.final.json  : missing"
        [ "$ok_consistency" = "FAIL" ] && echo "        consistency      : ${consistency} (not 100%)"
        return 1
    fi
    return 0
}

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
if [ "$EXPERIMENT" = "beta" ]; then
    p_min=$BETA_MIN; p_max=$BETA_MAX
else
    p_min=$GAMMA_MIN; p_max=$GAMMA_MAX
fi

valuation_policy=$(get_valuation_policy "$EXPERIMENT")

pta_size=$(get_pta_size "$PROBLEM_ID")
dfa0_minus1=$((pta_size - 1))

mkdir -p "$VALIDATION_BASE"
echo "policy,seed_nr,param_val,lcb1_c,ok_problem,ok_seed,ok_action_policy,ok_sub_policy,ok_valuation_policy,ok_lcb1,ok_atexit,ok_tree,ok_final,consistency,elapsed_s,last_expansion,overall" > "$CSV_FILE"

echo "============================================================"
echo "  MCTS Log Validator"
echo "  Experiment       : ${EXPERIMENT}"
echo "  Valuation policy : ${valuation_policy}"
echo "  Problem          : StaMiNa ${PROBLEM_ID}"
echo "  CSV output       : ${CSV_FILE}"
echo "============================================================"

total=0; passed=0; failed=0; missing=0

for policy in $POLICIES; do
    policy_dir=$(get_policy_dir "$policy")
    action_policy=$(get_action_policy "$policy")
    action_sub_policy=$(get_action_sub_policy "$policy")

    for seed_nr in 1 2 3; do
        seed_val=$(get_seed_value "$seed_nr")

        v=$p_min
        while [ "$v" -le "$p_max" ]; do
            out_dir="$RESULTS_BASE/rollout2_${PROBLEM_ID}/${EXPERIMENT}/${policy_dir}/seed${seed_nr}/${v}"
            lcb1_c=$(get_exploration_rate "$v" "$dfa0_minus1")
            total=$((total + 1))

            if [ ! -f "$out_dir/log.txt" ]; then
                echo "${policy},${seed_nr},${v},${lcb1_c},MISSING,MISSING,MISSING,MISSING,MISSING,MISSING,MISSING,MISSING,MISSING,MISSING,MISSING,MISSING" >> "$CSV_FILE"
                echo "  MISSING  policy=${policy}  seed${seed_nr}  ${EXPERIMENT}=${v}"
                missing=$((missing + 1))
            else
                validate_experiment \
                    "$out_dir" "$PROBLEM_FILE" "$seed_val" \
                    "$action_policy" "$action_sub_policy" \
                    "$valuation_policy" "$lcb1_c" \
                    "$policy" "$seed_nr" "$v"
                if [ $? -eq 0 ]; then
                    passed=$((passed + 1))
                else
                    failed=$((failed + 1))
                fi
            fi

            v=$((v + 1))
        done
    done
done

echo ""
echo "============================================================"
echo "  Validation complete."
echo "  Total   : ${total}"
echo "  Passed  : ${passed}"
echo "  Failed  : ${failed}"
echo "  Missing : ${missing}"
echo "  Report  : ${CSV_FILE}"
echo "============================================================"