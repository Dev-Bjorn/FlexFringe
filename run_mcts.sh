#!/bin/sh

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
RUNNER="$PROJECT_DIR/cmake-build-release/flexfringe.exe"
RESULTS_BASE="$PROJECT_DIR/results/mcts"
RUNS=${1:-1}
PARAMS=${2:-1}
PROBLEM=${3:-"$PROJECT_DIR/data/test/ctu-13_train.csv"}

# Find next available run number
RUN=1
while [ -d "$RESULTS_BASE/$RUN" ]; do
    RUN=$((RUN + 1))
done

i=0
while [ $i -lt $RUNS ]; do
    CURRENT=$((RUN + i))
    mkdir -p "$RESULTS_BASE/$CURRENT"
    echo "Starting run $CURRENT..."

    "$RUNNER" \
        --ini "$PROJECT_DIR/ini/gini.ini" \
        --outputfile "$RESULTS_BASE/$CURRENT/mcts" \
        --logpath "$RESULTS_BASE/$CURRENT/log.txt" \
        "$PROBLEM"
#        mcts "$PARAMS" \

    echo "Run $CURRENT complete."
    i=$((i + 1))
done