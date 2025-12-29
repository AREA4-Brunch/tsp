#!/bin/bash

# set limit on seolving all 263 points to something unachivable in given
# time frame for real mode, deafult to 30 for demo mode
mode=${1:-demo}
if [ "$mode" = "real" ]; then
    iterations=1000000
else
    iterations=30
fi

do_compile=${2-}

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$SCRIPT_DIR/.."
SRC_DIR="$SCRIPT_DIR/../../src/auto_opt"
PROBLEMS_DIR="$SCRIPT_DIR/../../problems"
RESULTS_DIR="$SCRIPT_DIR/../../results/auto_opt_funky"
HISTORY_DIR="$RESULTS_DIR/histories"

BOOST_INCLUDE=-IC:/boost_1_90_0
if [ "$do_compile" = "--no-compile" ]; then
    echo "Skipping compilation..."
else
    echo "Compiling..."
    if ! g++ --static -std=c++23 -O3 -Wall -Wextra "$BOOST_INCLUDE" -o "$SRC_DIR/main.exe" "$SRC_DIR/main.cpp"; then
        echo "Compilation failed."
        exit 1
    fi
fi

echo "Solving problem: 263"

echo "solving tsp"
mkdir -p "$RESULTS_DIR/263/tsp"
echo "Solving 1 points..."
"$SRC_DIR/main.exe" "funky" 4 1 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/1" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/1.txt" &
echo "Solving 2 points..."
"$SRC_DIR/main.exe" "funky" 4 2 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/2" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/2.txt" &

wait

echo "Solving 3 points..."
"$SRC_DIR/main.exe" "funky" 4 3 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/3" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/3.txt" &
echo "Solving 4 points..."
"$SRC_DIR/main.exe" "funky" 4 4 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/4" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/4.txt" &

wait

echo "Solving 5 points..."
"$SRC_DIR/main.exe" "funky" 4 5 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/5" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/5.txt" &
echo "Solving 6 points..."
"$SRC_DIR/main.exe" "funky" 4 6 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/6" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/6.txt" &

wait

echo "Solving 7 points..."
"$SRC_DIR/main.exe" "funky" 4 7 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/7" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/7.txt" &
echo "Solving 8 points..."
"$SRC_DIR/main.exe" "funky" 4 8 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/8" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/8.txt" &

wait

echo "Solving 9 points..."
"$SRC_DIR/main.exe" "funky" 4 9 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/9" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/9.txt" &
echo "Solving 10 points..."
"$SRC_DIR/main.exe" "funky" 4 10 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/10" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/10.txt" &

wait

echo "Solving 11 points..."
"$SRC_DIR/main.exe" "funky" 4 11 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/11" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/11.txt" &
echo "Solving 12 points..."
"$SRC_DIR/main.exe" "funky" 4 12 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/12" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/12.txt" &

wait

echo "Solving 13 points..."
"$SRC_DIR/main.exe" "funky" 4 13 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/13" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/13.txt" &
echo "Solving 14 points..."
"$SRC_DIR/main.exe" "funky" 4 14 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/14" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/14.txt" &

wait

echo "Solving 15 points..."
"$SRC_DIR/main.exe" "funky" 4 15 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/15" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/15.txt" &
echo "Solving 16 points..."
"$SRC_DIR/main.exe" "funky" 4 16 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/16" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/16.txt" &

wait

echo "Solving 17 points..."
"$SRC_DIR/main.exe" "funky" 4 17 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/17" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/17.txt" &
echo "Solving 18 points..."
"$SRC_DIR/main.exe" "funky" 4 18 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/18" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/18.txt" &

wait

echo "Solving 19 points..."
"$SRC_DIR/main.exe" "funky" 4 19 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/19" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/19.txt" &
echo "Solving 20 points..."
"$SRC_DIR/main.exe" "funky" 4 20 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/20" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/20.txt" &

wait

echo "Solving 21 points..."
"$SRC_DIR/main.exe" "funky" 4 21 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/21" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/21.txt" &
echo "Solving 22 points..."
"$SRC_DIR/main.exe" "funky" 4 22 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/22" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/22.txt" &

wait

echo "Solving 23 points..."
"$SRC_DIR/main.exe" "funky" 4 23 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/23" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/23.txt" &
echo "Solving 24 points..."
"$SRC_DIR/main.exe" "funky" 4 24 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/24" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/24.txt" &

wait

echo "Solving 25 points..."
"$SRC_DIR/main.exe" "funky" 4 25 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/25" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/25.txt" &
echo "Solving 26 points..."
"$SRC_DIR/main.exe" "funky" 4 26 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/26" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/26.txt" &

wait

echo "Solving 27 points..."
"$SRC_DIR/main.exe" "funky" 4 27 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/27" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/27.txt" &
echo "Solving 28 points..."
"$SRC_DIR/main.exe" "funky" 4 28 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/28" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/28.txt" &

wait

echo "Solving 29 points..."
"$SRC_DIR/main.exe" "funky" 4 29 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/29" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/29.txt" &
echo "Solving 30 points..."
"$SRC_DIR/main.exe" "funky" 4 30 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/30" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/30.txt" &

wait

echo "Solving 31 points..."
"$SRC_DIR/main.exe" "funky" 4 31 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/31" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/31.txt" &
echo "Solving 32 points..."
"$SRC_DIR/main.exe" "funky" 4 32 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/32" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/32.txt" &

wait

echo "Solving 33 points..."
"$SRC_DIR/main.exe" "funky" 4 33 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/33" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/33.txt" &

wait

echo "Solving 263 points..."
"$SRC_DIR/main.exe" "funky" 4 263 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/tsp/263" 30  500 300 1000 tsp 1 > "$RESULTS_DIR/263/tsp/263.txt" &

wait


echo "solving shp"
mkdir -p "$RESULTS_DIR/263/shp"
echo "Solving 1 points..."
"$SRC_DIR/main.exe" "funky" 4 1 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/1" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/1.txt" &
echo "Solving 2 points..."
"$SRC_DIR/main.exe" "funky" 4 2 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/2" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/2.txt" &

wait

echo "Solving 3 points..."
"$SRC_DIR/main.exe" "funky" 4 3 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/3" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/3.txt" &
echo "Solving 4 points..."
"$SRC_DIR/main.exe" "funky" 4 4 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/4" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/4.txt" &

wait

echo "Solving 5 points..."
"$SRC_DIR/main.exe" "funky" 4 5 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/5" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/5.txt" &
echo "Solving 6 points..."
"$SRC_DIR/main.exe" "funky" 4 6 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/6" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/6.txt" &

wait

echo "Solving 7 points..."
"$SRC_DIR/main.exe" "funky" 4 7 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/7" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/7.txt" &
echo "Solving 8 points..."
"$SRC_DIR/main.exe" "funky" 4 8 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/8" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/8.txt" &

wait

echo "Solving 9 points..."
"$SRC_DIR/main.exe" "funky" 4 9 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/9" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/9.txt" &
echo "Solving 10 points..."
"$SRC_DIR/main.exe" "funky" 4 10 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/10" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/10.txt" &

wait

echo "Solving 11 points..."
"$SRC_DIR/main.exe" "funky" 4 11 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/11" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/11.txt" &
echo "Solving 12 points..."
"$SRC_DIR/main.exe" "funky" 4 12 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/12" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/12.txt" &

wait

echo "Solving 13 points..."
"$SRC_DIR/main.exe" "funky" 4 13 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/13" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/13.txt" &
echo "Solving 14 points..."
"$SRC_DIR/main.exe" "funky" 4 14 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/14" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/14.txt" &

wait

echo "Solving 15 points..."
"$SRC_DIR/main.exe" "funky" 4 15 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/15" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/15.txt" &
echo "Solving 16 points..."
"$SRC_DIR/main.exe" "funky" 4 16 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/16" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/16.txt" &

wait

echo "Solving 17 points..."
"$SRC_DIR/main.exe" "funky" 4 17 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/17" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/17.txt" &
echo "Solving 18 points..."
"$SRC_DIR/main.exe" "funky" 4 18 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/18" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/18.txt" &

wait

echo "Solving 19 points..."
"$SRC_DIR/main.exe" "funky" 4 19 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/19" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/19.txt" &
echo "Solving 20 points..."
"$SRC_DIR/main.exe" "funky" 4 20 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/20" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/20.txt" &

wait

echo "Solving 21 points..."
"$SRC_DIR/main.exe" "funky" 4 21 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/21" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/21.txt" &
echo "Solving 22 points..."
"$SRC_DIR/main.exe" "funky" 4 22 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/22" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/22.txt" &

wait

echo "Solving 23 points..."
"$SRC_DIR/main.exe" "funky" 4 23 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/23" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/23.txt" &
echo "Solving 24 points..."
"$SRC_DIR/main.exe" "funky" 4 24 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/24" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/24.txt" &

wait

echo "Solving 25 points..."
"$SRC_DIR/main.exe" "funky" 4 25 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/25" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/25.txt" &
echo "Solving 26 points..."
"$SRC_DIR/main.exe" "funky" 4 26 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/26" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/26.txt" &

wait

echo "Solving 27 points..."
"$SRC_DIR/main.exe" "funky" 4 27 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/27" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/27.txt" &
echo "Solving 28 points..."
"$SRC_DIR/main.exe" "funky" 4 28 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/28" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/28.txt" &

wait

echo "Solving 29 points..."
"$SRC_DIR/main.exe" "funky" 4 29 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/29" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/29.txt" &
echo "Solving 30 points..."
"$SRC_DIR/main.exe" "funky" 4 30 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/30" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/30.txt" &

wait

echo "Solving 31 points..."
"$SRC_DIR/main.exe" "funky" 4 31 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/31" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/31.txt" &
echo "Solving 32 points..."
"$SRC_DIR/main.exe" "funky" 4 32 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/32" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/32.txt" &

wait

echo "Solving 33 points..."
"$SRC_DIR/main.exe" "funky" 4 33 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/33" 30  500 300 1000 shp 1 > "$RESULTS_DIR/263/shp/33.txt" &

wait

echo "Solving 263 points..."
"$SRC_DIR/main.exe" "funky" 4 263 "$PROBLEMS_DIR/263.txt" "$HISTORY_DIR/263/shp/263" $iterations  500 5700 5000 shp 1 > "$RESULTS_DIR/263/shp/263.txt" &

wait


echo "Solving problem: bayg29"


echo "solving tsp"
mkdir -p "$RESULTS_DIR/bayg29/tsp"
echo "Solving 29 points..."
"$SRC_DIR/main.exe" "funky" 4 29 "$PROBLEMS_DIR/bayg29.tsp" "$HISTORY_DIR/bayg29/tsp/29" 30  500 300 1000 tsp 0 > "$RESULTS_DIR/bayg29/tsp/29.txt" &

wait



echo "solving shp"
mkdir -p "$RESULTS_DIR/bayg29/shp"
echo "Solving 29 points..."
"$SRC_DIR/main.exe" "funky" 4 29 "$PROBLEMS_DIR/bayg29.tsp" "$HISTORY_DIR/bayg29/shp/29" 30  500 300 1000 shp 0 > "$RESULTS_DIR/bayg29/shp/29.txt" &

wait


# Test results:
echo "Checking results 263..."
python "$ROOT_DIR/judge_results.py" auto_opt_funky 263 263.txt 263


# Test results:
echo "Checking results bayg29..."
python "$ROOT_DIR/judge_results.py" auto_opt_funky bayg29 bayg29.tsp 29
