#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$SCRIPT_DIR/.."
SRC_DIR="$SCRIPT_DIR/../../src/bellman_held_karp"
PROBLEMS_DIR="$SCRIPT_DIR/../../problems"
RESULTS_DIR="$SCRIPT_DIR/../../results/bellman_held_karp"

echo "Compiling..."
if ! g++ -static -std=c++20 -O3 -Wall -Wextra -o "$SRC_DIR/main.exe" "$SRC_DIR/main.cpp"; then
    echo "Compilation failed."
    exit 1
fi


echo "solving uint8_t tsp"
mkdir -p "$RESULTS_DIR/bayg29/tsp/uint8_t"

# RAM: 2.203 GB = 2256.198 MB
echo "Solving 29 points..."
"$SRC_DIR/main.exe" 29 "$PROBLEMS_DIR/bayg29.tsp" 1 1e+12 2365795488 0 300 tsp 0 > "$RESULTS_DIR/bayg29/tsp/uint8_t/29.txt"


echo "solving uint16_t tsp"
mkdir -p "$RESULTS_DIR/bayg29/tsp/uint16_t"

# RAM: 3.180 GB = 3256.012 MB
echo "Solving 29 points..."
"$SRC_DIR/main.exe" 29 "$PROBLEMS_DIR/bayg29.tsp" 1 1e+12 3414175968 0 300 tsp 0 > "$RESULTS_DIR/bayg29/tsp/uint16_t/29.txt"


echo "solving uint32_t tsp"
mkdir -p "$RESULTS_DIR/bayg29/tsp/uint32_t"

# RAM: 5.132 GB = 5255.639 MB
echo "Solving 29 points..."
"$SRC_DIR/main.exe" 29 "$PROBLEMS_DIR/bayg29.tsp" 1 1e+04 5510936928 0 300 tsp 0 > "$RESULTS_DIR/bayg29/tsp/uint32_t/29.txt"


echo "solving uint64_t tsp"
mkdir -p "$RESULTS_DIR/bayg29/tsp/uint64_t"

# RAM: 9.038 GB = 9254.893 MB
echo "Solving 29 points..."
"$SRC_DIR/main.exe" 29 "$PROBLEMS_DIR/bayg29.tsp" 1 1e+12 9704458848 0 300 tsp 0 > "$RESULTS_DIR/bayg29/tsp/uint64_t/29.txt"


echo "solving float tsp"
mkdir -p "$RESULTS_DIR/bayg29/tsp/float"

# RAM: 5.132 GB = 5255.639 MB
echo "Solving 29 points..."
"$SRC_DIR/main.exe" 29 "$PROBLEMS_DIR/bayg29.tsp" 1 1e+12 5510936928 1 300 tsp 0 > "$RESULTS_DIR/bayg29/tsp/float/29.txt"


echo "solving double tsp"
mkdir -p "$RESULTS_DIR/bayg29/tsp/double"

# RAM: 9.038 GB = 9254.893 MB
echo "Solving 29 points..."
"$SRC_DIR/main.exe" 29 "$PROBLEMS_DIR/bayg29.tsp" 1 1e+12 9704458848 1 300 tsp 0 > "$RESULTS_DIR/bayg29/tsp/double/29.txt"


# Test results:
echo "Checking results bayg29..."
python "$ROOT_DIR/judge_results.py" bellman_held_karp bayg29 bayg29.tsp 29
