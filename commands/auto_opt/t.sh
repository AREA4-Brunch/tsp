#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$SCRIPT_DIR/.."
SRC_DIR="$SCRIPT_DIR/../../src/auto_opt"
PROBLEMS_DIR="$SCRIPT_DIR/../../problems"
RESULTS_DIR="$SCRIPT_DIR/../../results/auto_opt_auto"
HISTORY_DIR="$RESULTS_DIR/histories"



# echo "Compiling..."
# if ! g++ -S --static -std=c++23 -O3 -Wall -Winline -Wextra -o "$SRC_DIR/main.s" "$SRC_DIR/main.cpp"; then
#     echo "Compilation failed."
#     exit 1
# fi

BOOST_INCLUDE=-IC:/boost_1_90_0

echo "Compiling..."
if ! g++ --static -std=c++23 -O3 -Wall -Winline -Wextra "$BOOST_INCLUDE" -o "$SRC_DIR/main_.exe" "$SRC_DIR/main.cpp"; then
    echo "Compilation failed."
    exit 1
fi

echo "Solving problem: 263"

mkdir -p "$RESULTS_DIR/263/tsp"
mkdir -p "$RESULTS_DIR/263/shp"

echo "Solving 3 points..."
"$SRC_DIR/main_.exe" "auto" "5_opt" 3 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/263" 30  500 300 tsp 1 --no-history > "$RESULTS_DIR/263/tsp/3.txt" 2>&1 &
wait


echo "Solving 4 points..."
"$SRC_DIR/main_.exe" "auto" "5_opt" 4 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/263" 30  500 300 tsp 1 --no-history > "$RESULTS_DIR/263/tsp/4.txt" 2>&1 &
wait


echo "Solving 263 points..."
"$SRC_DIR/main_.exe" "auto" "5_opt" 263 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/263" 1  500 300 shp 1 --no-history > "$RESULTS_DIR/263/shp/263_5opt.txt" 2>&1 &
# "$SRC_DIR/main_.exe" "auto" "5_opt" 263 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/263" 5  500 300 shp 1 --no-history > "$RESULTS_DIR/263/shp/263.txt" 2>&1 &
wait


echo "All done."

# # Test results:
# echo "Checking results 263..."
# python "$ROOT_DIR/judge_results.py" auto_opt_auto 263 263.txt 263
