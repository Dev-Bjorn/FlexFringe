#!/bin/sh

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
RUNNER="$PROJECT_DIR/cmake-build-release/flexfringe.exe"
RESULTS_BASE="$PROJECT_DIR/results/research"
RUN=${1:-1}
PARAMS=${2:-1}
PROBLEM=${3:-"$PROJECT_DIR/data/staminadata/16_training.txt.dat"}

# Find next available run number
echo "Starting run $RUN..."

"$RUNNER" \
    --mode predict \
    --ini "$PROJECT_DIR/ini/edsm.ini" \
    --predicttype 1 \
    --aptafile "$RESULTS_BASE/$RUN/mcts.mcts.final.json" \
    --logpath "$RESULTS_BASE/$RUN/predict.log.txt" \
    "$PROBLEM"
#        mcts "$PARAMS" \

echo "Run $RUN complete."
