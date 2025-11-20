#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$SCRIPT_DIR/.."
SRC_DIR="$SCRIPT_DIR/../../src/3_opt"
PROBLEMS_DIR="$SCRIPT_DIR/../../problems"
RESULTS_DIR="$SCRIPT_DIR/../../results/3_opt_rand"
HISTORY_DIR="$RESULTS_DIR/histories"

echo "Compiling..."
if ! g++ --static -std=c++20 -O3 -Wall -Wextra -o "$SRC_DIR/main.exe" "$SRC_DIR/main.cpp"; then
    echo "Compilation failed."
    exit 1
fi

echo "Solving problem: 263"

echo "solving tsp"
mkdir -p "$RESULTS_DIR/263/tsp"
echo "Solving 1 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 1 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/1" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/1.txt" &
echo "Solving 2 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 2 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/2" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/2.txt" &

wait

echo "Solving 3 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 3 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/3" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/3.txt" &
echo "Solving 4 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 4 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/4" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/4.txt" &

wait

echo "Solving 5 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 5 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/5" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/5.txt" &
echo "Solving 6 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 6 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/6" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/6.txt" &

wait

echo "Solving 7 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 7 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/7" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/7.txt" &
echo "Solving 8 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 8 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/8" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/8.txt" &

wait

echo "Solving 9 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 9 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/9" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/9.txt" &
echo "Solving 10 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 10 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/10" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/10.txt" &

wait

echo "Solving 11 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 11 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/11" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/11.txt" &
echo "Solving 12 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 12 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/12" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/12.txt" &

wait

echo "Solving 13 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 13 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/13" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/13.txt" &
echo "Solving 14 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 14 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/14" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/14.txt" &

wait

echo "Solving 15 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 15 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/15" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/15.txt" &
echo "Solving 16 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 16 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/16" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/16.txt" &

wait

echo "Solving 17 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 17 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/17" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/17.txt" &
echo "Solving 18 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 18 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/18" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/18.txt" &

wait

echo "Solving 19 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 19 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/19" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/19.txt" &
echo "Solving 20 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 20 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/20" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/20.txt" &

wait

echo "Solving 21 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 21 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/21" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/21.txt" &
echo "Solving 22 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 22 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/22" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/22.txt" &

wait

echo "Solving 23 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 23 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/23" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/23.txt" &
echo "Solving 24 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 24 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/24" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/24.txt" &

wait

echo "Solving 25 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 25 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/25" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/25.txt" &
echo "Solving 26 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 26 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/26" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/26.txt" &

wait

echo "Solving 27 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 27 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/27" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/27.txt" &
echo "Solving 28 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 28 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/28" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/28.txt" &

wait

echo "Solving 29 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 29 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/29" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/29.txt" &
echo "Solving 30 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 30 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/30" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/30.txt" &

wait

echo "Solving 31 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 31 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/31" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/31.txt" &
echo "Solving 32 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 32 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/32" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/32.txt" &

wait

echo "Solving 33 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 33 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/33" 100  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/33.txt" &

wait

echo "Solving 263 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 263 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/263" 1  500 300 tsp 1 > "$RESULTS_DIR/263/tsp/263.txt" &

wait


echo "solving shp"
mkdir -p "$RESULTS_DIR/263/shp"
echo "Solving 1 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 1 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/1" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/1.txt" &
echo "Solving 2 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 2 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/2" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/2.txt" &

wait

echo "Solving 3 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 3 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/3" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/3.txt" &
echo "Solving 4 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 4 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/4" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/4.txt" &

wait

echo "Solving 5 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 5 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/5" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/5.txt" &
echo "Solving 6 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 6 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/6" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/6.txt" &

wait

echo "Solving 7 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 7 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/7" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/7.txt" &
echo "Solving 8 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 8 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/8" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/8.txt" &

wait

echo "Solving 9 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 9 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/9" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/9.txt" &
echo "Solving 10 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 10 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/10" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/10.txt" &

wait

echo "Solving 11 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 11 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/11" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/11.txt" &
echo "Solving 12 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 12 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/12" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/12.txt" &

wait

echo "Solving 13 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 13 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/13" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/13.txt" &
echo "Solving 14 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 14 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/14" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/14.txt" &

wait

echo "Solving 15 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 15 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/15" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/15.txt" &
echo "Solving 16 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 16 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/16" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/16.txt" &

wait

echo "Solving 17 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 17 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/17" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/17.txt" &
echo "Solving 18 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 18 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/18" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/18.txt" &

wait

echo "Solving 19 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 19 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/19" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/19.txt" &
echo "Solving 20 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 20 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/20" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/20.txt" &

wait

echo "Solving 21 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 21 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/21" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/21.txt" &
echo "Solving 22 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 22 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/22" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/22.txt" &

wait

echo "Solving 23 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 23 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/23" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/23.txt" &
echo "Solving 24 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 24 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/24" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/24.txt" &

wait

echo "Solving 25 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 25 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/25" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/25.txt" &
echo "Solving 26 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 26 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/26" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/26.txt" &

wait

echo "Solving 27 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 27 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/27" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/27.txt" &
echo "Solving 28 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 28 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/28" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/28.txt" &

wait

echo "Solving 29 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 29 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/29" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/29.txt" &
echo "Solving 30 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 30 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/30" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/30.txt" &

wait

echo "Solving 31 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 31 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/31" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/31.txt" &
echo "Solving 32 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 32 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/32" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/32.txt" &

wait

echo "Solving 33 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 33 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/33" 100  500 300 shp 1 > "$RESULTS_DIR/263/shp/33.txt" &

wait

echo "Solving 263 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 263 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/263" 1  500 300 shp 1 > "$RESULTS_DIR/263/shp/263.txt" &

wait



echo "Solving problem: bayg29"


echo "solving tsp"
mkdir -p "$RESULTS_DIR/bayg29/tsp"
echo "Solving 29 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 29 "$PROBLEMS_DIR/bayg29.tsp" "$HISTORY_DIR/bayg29/tsp/29" 100  500 300 tsp 0 > "$RESULTS_DIR/bayg29/tsp/29.txt" &

wait



echo "solving shp"
mkdir -p "$RESULTS_DIR/bayg29/shp"
echo "Solving 29 points..."
"$SRC_DIR/main.exe" "3_opt_rand" 29 "$PROBLEMS_DIR/bayg29.tsp" "$HISTORY_DIR/bayg29/shp/29" 100  500 300 shp 0 > "$RESULTS_DIR/bayg29/shp/29.txt" &

wait


# Test results:
echo "Checking results 263..."
python "$ROOT_DIR/judge_results.py" 3_opt_rand 263 263.txt 263


# Test results:
echo "Checking results bayg29..."
python "$ROOT_DIR/judge_results.py" 3_opt_rand bayg29 bayg29.tsp 29
