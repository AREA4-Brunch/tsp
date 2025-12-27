#!/bin/bash

# 'demo' or 'real'
mode=${1:-demo}
do_compile=${2-}

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CMDS_DIR="$SCRIPT_DIR/"
SRC_DIR="$SCRIPT_DIR/../../src/k_opt"

BOOST_INCLUDE=-IC:/boost_1_90_0

if [ "$do_compile" = "--no-compile" ]; then
    echo "Skipping compilation..."
else
    echo "Compiling..."
    if ! g++ --static -std=c++23 -O3 -Wall -Winline -Wextra "$BOOST_INCLUDE" -o "$SRC_DIR/main.exe" "$SRC_DIR/main.cpp"; then
        echo "Compilation failed."
        exit 1
    fi
fi

echo "Running all scripts..."

"$CMDS_DIR/run_best_cut.sh" $mode --no-compile &

"$CMDS_DIR/run_classical.sh" $mode --no-compile &

"$CMDS_DIR/run_funky.sh" $mode --no-compile &

"$CMDS_DIR/run_rand_no_2_opt.sh" $mode --no-compile &

"$CMDS_DIR/run_rand.sh" $mode --no-compile &

wait

echo "Analizing and plotting results..."

python "$CMDS_DIR/analize_results.py" 263 -md &

python "$CMDS_DIR/plot_history.py" 263 263 shp 0 -1 1 -ll &

wait

python "$CMDS_DIR/plot_history.py" 263 263 shp 0 -1 1 2 -ll &

python "$CMDS_DIR/plot_history.py" 263 263 shp 0 -1 1 100 -ll &

wait

echo "All done."
