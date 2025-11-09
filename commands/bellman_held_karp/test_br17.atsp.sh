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
mkdir -p "$RESULTS_DIR/br17/tsp/uint8_t"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 2 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 2 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 2 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 9 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 24 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/5.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 80 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/6.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 240 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/7.txt" &

# RAM: 0.000 GB = 0.001 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 595 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/8.txt" &

wait


# RAM: 0.000 GB = 0.001 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 1456 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/9.txt" &

# RAM: 0.000 GB = 0.003 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 3276 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/10.txt" &

wait


# RAM: 0.000 GB = 0.007 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 7440 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/11.txt" &

# RAM: 0.000 GB = 0.015 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 16104 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/12.txt" &

wait


# RAM: 0.000 GB = 0.034 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 35376 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/13.txt" &

# RAM: 0.000 GB = 0.072 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 75218 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/14.txt" &

wait


# RAM: 0.000 GB = 0.155 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 162344 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/15.txt" &

# RAM: 0.000 GB = 0.326 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 341835 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/16.txt" &

wait


# RAM: 0.001 GB = 0.696 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 729696 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint8_t/17.txt" &


echo "solving uint16_t tsp"
mkdir -p "$RESULTS_DIR/br17/tsp/uint16_t"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 3 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 3 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 4 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 18 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 48 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/5.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 130 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/6.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 360 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/7.txt" &

# RAM: 0.000 GB = 0.001 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 840 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/8.txt" &

wait


# RAM: 0.000 GB = 0.002 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 2016 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/9.txt" &

# RAM: 0.000 GB = 0.004 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 4410 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/10.txt" &

wait


# RAM: 0.000 GB = 0.009 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 9960 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/11.txt" &

# RAM: 0.000 GB = 0.020 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 21186 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/12.txt" &

wait


# RAM: 0.000 GB = 0.044 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 46464 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/13.txt" &

# RAM: 0.000 GB = 0.093 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 97526 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/14.txt" &

wait


# RAM: 0.000 GB = 0.201 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 210392 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/15.txt" &

# RAM: 0.000 GB = 0.418 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 438360 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/16.txt" &

wait


# RAM: 0.001 GB = 0.892 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 935616 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint16_t/17.txt" &


echo "solving uint32_t tsp"
mkdir -p "$RESULTS_DIR/br17/tsp/uint32_t"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 5 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 5 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 8 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 36 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 96 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/5.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 230 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/6.txt" &

wait


# RAM: 0.000 GB = 0.001 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 600 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/7.txt" &

# RAM: 0.000 GB = 0.001 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 1330 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/8.txt" &

wait


# RAM: 0.000 GB = 0.003 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 3136 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/9.txt" &

# RAM: 0.000 GB = 0.006 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 6678 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/10.txt" &

wait


# RAM: 0.000 GB = 0.014 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 15000 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/11.txt" &

# RAM: 0.000 GB = 0.030 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 31350 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/12.txt" &

wait


# RAM: 0.000 GB = 0.065 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 68640 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/13.txt" &

# RAM: 0.000 GB = 0.136 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 142142 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/14.txt" &

wait


# RAM: 0.000 GB = 0.292 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 306488 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/15.txt" &

# RAM: 0.001 GB = 0.602 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 631410 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/16.txt" &

wait


# RAM: 0.001 GB = 1.285 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/br17.atsp" 1 1e+04 1347456 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint32_t/17.txt" &


echo "solving uint64_t tsp"
mkdir -p "$RESULTS_DIR/br17/tsp/uint64_t"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 9 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 9 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 16 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 72 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 192 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/5.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 430 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/6.txt" &

wait


# RAM: 0.000 GB = 0.001 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 1080 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/7.txt" &

# RAM: 0.000 GB = 0.002 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 2310 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/8.txt" &

wait


# RAM: 0.000 GB = 0.005 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 5376 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/9.txt" &

# RAM: 0.000 GB = 0.011 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 11214 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/10.txt" &

wait


# RAM: 0.000 GB = 0.024 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 25080 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/11.txt" &

# RAM: 0.000 GB = 0.049 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 51678 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/12.txt" &

wait


# RAM: 0.000 GB = 0.108 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 112992 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/13.txt" &

# RAM: 0.000 GB = 0.221 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 231374 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/14.txt" &

wait


# RAM: 0.000 GB = 0.476 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 498680 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/15.txt" &

# RAM: 0.001 GB = 0.970 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 1017510 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/16.txt" &

wait


# RAM: 0.002 GB = 2.071 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 2171136 0 300 tsp 0 > "$RESULTS_DIR/br17/tsp/uint64_t/17.txt" &


echo "solving float tsp"
mkdir -p "$RESULTS_DIR/br17/tsp/float"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 5 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 5 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 8 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 36 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 96 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/5.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 230 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/6.txt" &

wait


# RAM: 0.000 GB = 0.001 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 600 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/7.txt" &

# RAM: 0.000 GB = 0.001 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 1330 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/8.txt" &

wait


# RAM: 0.000 GB = 0.003 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 3136 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/9.txt" &

# RAM: 0.000 GB = 0.006 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 6678 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/10.txt" &

wait


# RAM: 0.000 GB = 0.014 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 15000 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/11.txt" &

# RAM: 0.000 GB = 0.030 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 31350 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/12.txt" &

wait


# RAM: 0.000 GB = 0.065 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 68640 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/13.txt" &

# RAM: 0.000 GB = 0.136 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 142142 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/14.txt" &

wait


# RAM: 0.000 GB = 0.292 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 306488 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/15.txt" &

# RAM: 0.001 GB = 0.602 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 631410 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/16.txt" &

wait


# RAM: 0.001 GB = 1.285 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 1347456 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/float/17.txt" &


echo "solving double tsp"
mkdir -p "$RESULTS_DIR/br17/tsp/double"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 9 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 9 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 16 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 72 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 192 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/5.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 430 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/6.txt" &

wait


# RAM: 0.000 GB = 0.001 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 1080 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/7.txt" &

# RAM: 0.000 GB = 0.002 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 2310 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/8.txt" &

wait


# RAM: 0.000 GB = 0.005 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 5376 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/9.txt" &

# RAM: 0.000 GB = 0.011 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 11214 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/10.txt" &

wait


# RAM: 0.000 GB = 0.024 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 25080 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/11.txt" &

# RAM: 0.000 GB = 0.049 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 51678 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/12.txt" &

wait


# RAM: 0.000 GB = 0.108 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 112992 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/13.txt" &

# RAM: 0.000 GB = 0.221 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 231374 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/14.txt" &

wait


# RAM: 0.000 GB = 0.476 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 498680 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/15.txt" &

# RAM: 0.001 GB = 0.970 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 1017510 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/16.txt" &

wait


# RAM: 0.002 GB = 2.071 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/br17.atsp" 1 1e+12 2171136 1 300 tsp 0 > "$RESULTS_DIR/br17/tsp/double/17.txt" &


# Test results:
echo "Checking results br17..."
python "$ROOT_DIR/judge_results.py" bellman_held_karp br17 br17.atsp 17
