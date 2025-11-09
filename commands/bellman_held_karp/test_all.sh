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
mkdir -p "$RESULTS_DIR/263/tsp/uint8_t"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/263.txt" 1 1e+12 2 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/263.txt" 1 1e+12 2 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/263.txt" 1 1e+12 2 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/263.txt" 1 1e+12 3 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/263.txt" 1 1e+12 16 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/5.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/263.txt" 1 1e+12 25 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/6.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/263.txt" 1 1e+12 90 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/7.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/263.txt" 1 1e+12 147 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/8.txt" &

wait


# RAM: 0.000 GB = 0.001 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/263.txt" 1 1e+12 616 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/9.txt" &

# RAM: 0.000 GB = 0.001 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/263.txt" 1 1e+12 1008 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/10.txt" &

wait


# RAM: 0.000 GB = 0.003 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/263.txt" 1 1e+12 3300 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/11.txt" &

# RAM: 0.000 GB = 0.005 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/263.txt" 1 1e+12 5445 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/12.txt" &

wait


# RAM: 0.000 GB = 0.015 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/263.txt" 1 1e+12 16104 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/13.txt" &

# RAM: 0.000 GB = 0.026 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/263.txt" 1 1e+12 26884 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/14.txt" &

wait


# RAM: 0.000 GB = 0.072 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/263.txt" 1 1e+12 75166 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/15.txt" &

# RAM: 0.000 GB = 0.121 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/263.txt" 1 1e+12 126945 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/16.txt" &

wait


# RAM: 0.000 GB = 0.326 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/263.txt" 1 1e+12 341968 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/17.txt" &

# RAM: 0.001 GB = 0.556 MB
echo "Solving 18 points..."
"$SRC_DIR/main.exe" 18 "$PROBLEMS_DIR/263.txt" 1 1e+12 583508 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/18.txt" &

wait


# RAM: 0.001 GB = 1.459 MB
echo "Solving 19 points..."
"$SRC_DIR/main.exe" 19 "$PROBLEMS_DIR/263.txt" 1 1e+12 1529388 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/19.txt" &

# RAM: 0.002 GB = 2.511 MB
echo "Solving 20 points..."
"$SRC_DIR/main.exe" 20 "$PROBLEMS_DIR/263.txt" 1 1e+12 2632773 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/20.txt" &

wait


# RAM: 0.006 GB = 6.441 MB
echo "Solving 21 points..."
"$SRC_DIR/main.exe" 21 "$PROBLEMS_DIR/263.txt" 1 1e+12 6754120 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/21.txt" &

# RAM: 0.011 GB = 11.172 MB
echo "Solving 22 points..."
"$SRC_DIR/main.exe" 22 "$PROBLEMS_DIR/263.txt" 1 1e+12 11715039 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/22.txt" &

wait


# RAM: 0.028 GB = 28.166 MB
echo "Solving 23 points..."
"$SRC_DIR/main.exe" 23 "$PROBLEMS_DIR/263.txt" 1 1e+12 29534648 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/23.txt" &

# RAM: 0.048 GB = 49.174 MB
echo "Solving 24 points..."
"$SRC_DIR/main.exe" 24 "$PROBLEMS_DIR/263.txt" 1 1e+12 51562159 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint8_t/24.txt" &

wait



echo "solving uint8_t shp"
mkdir -p "$RESULTS_DIR/263/shp/uint8_t"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/263.txt" 1 1e+12 2 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/263.txt" 1 1e+12 2 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/263.txt" 1 1e+12 3 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/263.txt" 1 1e+12 16 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/263.txt" 1 1e+12 25 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/5.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/263.txt" 1 1e+12 90 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/6.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/263.txt" 1 1e+12 147 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/7.txt" &

# RAM: 0.000 GB = 0.001 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/263.txt" 1 1e+12 616 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/8.txt" &

wait


# RAM: 0.000 GB = 0.001 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/263.txt" 1 1e+12 1008 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/9.txt" &

# RAM: 0.000 GB = 0.003 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/263.txt" 1 1e+12 3300 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/10.txt" &

wait


# RAM: 0.000 GB = 0.005 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/263.txt" 1 1e+12 5445 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/11.txt" &

# RAM: 0.000 GB = 0.015 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/263.txt" 1 1e+12 16104 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/12.txt" &

wait


# RAM: 0.000 GB = 0.026 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/263.txt" 1 1e+12 26884 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/13.txt" &

# RAM: 0.000 GB = 0.072 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/263.txt" 1 1e+12 75166 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/14.txt" &

wait


# RAM: 0.000 GB = 0.121 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/263.txt" 1 1e+12 126945 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/15.txt" &

# RAM: 0.000 GB = 0.326 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/263.txt" 1 1e+12 341968 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/16.txt" &

wait


# RAM: 0.001 GB = 0.556 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/263.txt" 1 1e+12 583508 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/17.txt" &

# RAM: 0.001 GB = 1.459 MB
echo "Solving 18 points..."
"$SRC_DIR/main.exe" 18 "$PROBLEMS_DIR/263.txt" 1 1e+12 1529388 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/18.txt" &

wait


# RAM: 0.002 GB = 2.511 MB
echo "Solving 19 points..."
"$SRC_DIR/main.exe" 19 "$PROBLEMS_DIR/263.txt" 1 1e+12 2632773 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/19.txt" &

# RAM: 0.006 GB = 6.441 MB
echo "Solving 20 points..."
"$SRC_DIR/main.exe" 20 "$PROBLEMS_DIR/263.txt" 1 1e+12 6754120 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/20.txt" &

wait


# RAM: 0.011 GB = 11.172 MB
echo "Solving 21 points..."
"$SRC_DIR/main.exe" 21 "$PROBLEMS_DIR/263.txt" 1 1e+12 11715039 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/21.txt" &

# RAM: 0.028 GB = 28.166 MB
echo "Solving 22 points..."
"$SRC_DIR/main.exe" 22 "$PROBLEMS_DIR/263.txt" 1 1e+12 29534648 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/22.txt" &

wait


# RAM: 0.048 GB = 49.174 MB
echo "Solving 23 points..."
"$SRC_DIR/main.exe" 23 "$PROBLEMS_DIR/263.txt" 1 1e+12 51562159 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/23.txt" &

# RAM: 0.119 GB = 122.185 MB
echo "Solving 24 points..."
"$SRC_DIR/main.exe" 24 "$PROBLEMS_DIR/263.txt" 1 1e+12 128120304 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint8_t/24.txt" &

wait



echo "solving uint16_t tsp"
mkdir -p "$RESULTS_DIR/263/tsp/uint16_t"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/263.txt" 1 1e+12 3 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/263.txt" 1 1e+12 3 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/263.txt" 1 1e+12 4 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/263.txt" 1 1e+12 6 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/263.txt" 1 1e+12 32 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/5.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/263.txt" 1 1e+12 50 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/6.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/263.txt" 1 1e+12 180 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/7.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/263.txt" 1 1e+12 294 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/8.txt" &

wait


# RAM: 0.000 GB = 0.001 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/263.txt" 1 1e+12 1064 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/9.txt" &

# RAM: 0.000 GB = 0.002 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/263.txt" 1 1e+12 1764 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/10.txt" &

wait


# RAM: 0.000 GB = 0.005 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/263.txt" 1 1e+12 5400 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/11.txt" &

# RAM: 0.000 GB = 0.009 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/263.txt" 1 1e+12 9075 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/12.txt" &

wait


# RAM: 0.000 GB = 0.024 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/263.txt" 1 1e+12 25608 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/13.txt" &

# RAM: 0.000 GB = 0.042 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/263.txt" 1 1e+12 43615 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/14.txt" &

wait


# RAM: 0.000 GB = 0.112 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/263.txt" 1 1e+12 117208 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/15.txt" &

# RAM: 0.000 GB = 0.193 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/263.txt" 1 1e+12 202020 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/16.txt" &

wait


# RAM: 0.000 GB = 0.501 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/263.txt" 1 1e+12 525008 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/17.txt" &

# RAM: 0.001 GB = 0.872 MB
echo "Solving 18 points..."
"$SRC_DIR/main.exe" 18 "$PROBLEMS_DIR/263.txt" 1 1e+12 914124 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/18.txt" &

wait


# RAM: 0.002 GB = 2.210 MB
echo "Solving 19 points..."
"$SRC_DIR/main.exe" 19 "$PROBLEMS_DIR/263.txt" 1 1e+12 2317032 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/19.txt" &

# RAM: 0.004 GB = 3.880 MB
echo "Solving 20 points..."
"$SRC_DIR/main.exe" 20 "$PROBLEMS_DIR/263.txt" 1 1e+12 4068831 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/20.txt" &

wait


# RAM: 0.009 GB = 9.645 MB
echo "Solving 21 points..."
"$SRC_DIR/main.exe" 21 "$PROBLEMS_DIR/263.txt" 1 1e+12 10113320 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/21.txt" &

# RAM: 0.017 GB = 17.059 MB
echo "Solving 22 points..."
"$SRC_DIR/main.exe" 22 "$PROBLEMS_DIR/263.txt" 1 1e+12 17887569 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/22.txt" &

wait


# RAM: 0.041 GB = 41.734 MB
echo "Solving 23 points..."
"$SRC_DIR/main.exe" 23 "$PROBLEMS_DIR/263.txt" 1 1e+12 43760860 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/23.txt" &

# RAM: 0.073 GB = 74.268 MB
echo "Solving 24 points..."
"$SRC_DIR/main.exe" 24 "$PROBLEMS_DIR/263.txt" 1 1e+12 77875677 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint16_t/24.txt" &

wait



echo "solving uint16_t shp"
mkdir -p "$RESULTS_DIR/263/shp/uint16_t"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/263.txt" 1 1e+12 3 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/263.txt" 1 1e+12 4 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/263.txt" 1 1e+12 6 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/263.txt" 1 1e+12 32 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/263.txt" 1 1e+12 50 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/5.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/263.txt" 1 1e+12 180 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/6.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/263.txt" 1 1e+12 294 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/7.txt" &

# RAM: 0.000 GB = 0.001 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/263.txt" 1 1e+12 1064 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/8.txt" &

wait


# RAM: 0.000 GB = 0.002 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/263.txt" 1 1e+12 1764 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/9.txt" &

# RAM: 0.000 GB = 0.005 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/263.txt" 1 1e+12 5400 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/10.txt" &

wait


# RAM: 0.000 GB = 0.009 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/263.txt" 1 1e+12 9075 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/11.txt" &

# RAM: 0.000 GB = 0.024 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/263.txt" 1 1e+12 25608 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/12.txt" &

wait


# RAM: 0.000 GB = 0.042 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/263.txt" 1 1e+12 43615 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/13.txt" &

# RAM: 0.000 GB = 0.112 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/263.txt" 1 1e+12 117208 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/14.txt" &

wait


# RAM: 0.000 GB = 0.193 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/263.txt" 1 1e+12 202020 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/15.txt" &

# RAM: 0.000 GB = 0.501 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/263.txt" 1 1e+12 525008 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/16.txt" &

wait


# RAM: 0.001 GB = 0.872 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/263.txt" 1 1e+12 914124 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/17.txt" &

# RAM: 0.002 GB = 2.210 MB
echo "Solving 18 points..."
"$SRC_DIR/main.exe" 18 "$PROBLEMS_DIR/263.txt" 1 1e+12 2317032 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/18.txt" &

wait


# RAM: 0.004 GB = 3.880 MB
echo "Solving 19 points..."
"$SRC_DIR/main.exe" 19 "$PROBLEMS_DIR/263.txt" 1 1e+12 4068831 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/19.txt" &

# RAM: 0.009 GB = 9.645 MB
echo "Solving 20 points..."
"$SRC_DIR/main.exe" 20 "$PROBLEMS_DIR/263.txt" 1 1e+12 10113320 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/20.txt" &

wait


# RAM: 0.017 GB = 17.059 MB
echo "Solving 21 points..."
"$SRC_DIR/main.exe" 21 "$PROBLEMS_DIR/263.txt" 1 1e+12 17887569 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/21.txt" &

# RAM: 0.041 GB = 41.734 MB
echo "Solving 22 points..."
"$SRC_DIR/main.exe" 22 "$PROBLEMS_DIR/263.txt" 1 1e+12 43760860 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/22.txt" &

wait


# RAM: 0.073 GB = 74.268 MB
echo "Solving 23 points..."
"$SRC_DIR/main.exe" 23 "$PROBLEMS_DIR/263.txt" 1 1e+12 77875677 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/23.txt" &

# RAM: 0.175 GB = 179.317 MB
echo "Solving 24 points..."
"$SRC_DIR/main.exe" 24 "$PROBLEMS_DIR/263.txt" 1 1e+12 188027760 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint16_t/24.txt" &

wait



echo "solving uint32_t tsp"
mkdir -p "$RESULTS_DIR/263/tsp/uint32_t"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/263.txt" 1 1e+04 5 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/263.txt" 1 1e+04 5 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/263.txt" 1 1e+04 8 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/263.txt" 1 1e+04 12 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/263.txt" 1 1e+04 64 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/5.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/263.txt" 1 1e+04 100 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/6.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/263.txt" 1 1e+04 360 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/7.txt" &

# RAM: 0.000 GB = 0.001 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/263.txt" 1 1e+04 588 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/8.txt" &

wait


# RAM: 0.000 GB = 0.002 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/263.txt" 1 1e+04 1960 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/9.txt" &

# RAM: 0.000 GB = 0.003 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/263.txt" 1 1e+04 3276 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/10.txt" &

wait


# RAM: 0.000 GB = 0.009 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/263.txt" 1 1e+04 9600 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/11.txt" &

# RAM: 0.000 GB = 0.016 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/263.txt" 1 1e+04 16335 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/12.txt" &

wait


# RAM: 0.000 GB = 0.043 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/263.txt" 1 1e+04 44616 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/13.txt" &

# RAM: 0.000 GB = 0.074 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/263.txt" 1 1e+04 77077 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/14.txt" &

wait


# RAM: 0.000 GB = 0.192 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/263.txt" 1 1e+04 201292 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/15.txt" &

# RAM: 0.000 GB = 0.336 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/263.txt" 1 1e+04 352170 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/16.txt" &

wait


# RAM: 0.001 GB = 0.850 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/263.txt" 1 1e+04 891088 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/17.txt" &

# RAM: 0.001 GB = 1.502 MB
echo "Solving 18 points..."
"$SRC_DIR/main.exe" 18 "$PROBLEMS_DIR/263.txt" 1 1e+04 1575356 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/18.txt" &

wait


# RAM: 0.004 GB = 3.712 MB
echo "Solving 19 points..."
"$SRC_DIR/main.exe" 19 "$PROBLEMS_DIR/263.txt" 1 1e+04 3892320 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/19.txt" &

# RAM: 0.006 GB = 6.619 MB
echo "Solving 20 points..."
"$SRC_DIR/main.exe" 20 "$PROBLEMS_DIR/263.txt" 1 1e+04 6940947 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/20.txt" &

wait


# RAM: 0.016 GB = 16.052 MB
echo "Solving 21 points..."
"$SRC_DIR/main.exe" 21 "$PROBLEMS_DIR/263.txt" 1 1e+04 16831720 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/21.txt" &

# RAM: 0.028 GB = 28.832 MB
echo "Solving 22 points..."
"$SRC_DIR/main.exe" 22 "$PROBLEMS_DIR/263.txt" 1 1e+04 30232629 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/22.txt" &

wait


# RAM: 0.067 GB = 68.868 MB
echo "Solving 23 points..."
"$SRC_DIR/main.exe" 23 "$PROBLEMS_DIR/263.txt" 1 1e+04 72213284 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/23.txt" &

# RAM: 0.122 GB = 124.457 MB
echo "Solving 24 points..."
"$SRC_DIR/main.exe" 24 "$PROBLEMS_DIR/263.txt" 1 1e+04 130502713 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint32_t/24.txt" &

wait



echo "solving uint32_t shp"
mkdir -p "$RESULTS_DIR/263/shp/uint32_t"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/263.txt" 1 1e+04 5 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/263.txt" 1 1e+04 8 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/263.txt" 1 1e+04 12 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/263.txt" 1 1e+04 64 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/263.txt" 1 1e+04 100 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/5.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/263.txt" 1 1e+04 360 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/6.txt" &

wait


# RAM: 0.000 GB = 0.001 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/263.txt" 1 1e+04 588 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/7.txt" &

# RAM: 0.000 GB = 0.002 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/263.txt" 1 1e+04 1960 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/8.txt" &

wait


# RAM: 0.000 GB = 0.003 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/263.txt" 1 1e+04 3276 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/9.txt" &

# RAM: 0.000 GB = 0.009 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/263.txt" 1 1e+04 9600 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/10.txt" &

wait


# RAM: 0.000 GB = 0.016 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/263.txt" 1 1e+04 16335 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/11.txt" &

# RAM: 0.000 GB = 0.043 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/263.txt" 1 1e+04 44616 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/12.txt" &

wait


# RAM: 0.000 GB = 0.074 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/263.txt" 1 1e+04 77077 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/13.txt" &

# RAM: 0.000 GB = 0.192 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/263.txt" 1 1e+04 201292 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/14.txt" &

wait


# RAM: 0.000 GB = 0.336 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/263.txt" 1 1e+04 352170 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/15.txt" &

# RAM: 0.001 GB = 0.850 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/263.txt" 1 1e+04 891088 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/16.txt" &

wait


# RAM: 0.001 GB = 1.502 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/263.txt" 1 1e+04 1575356 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/17.txt" &

# RAM: 0.004 GB = 3.712 MB
echo "Solving 18 points..."
"$SRC_DIR/main.exe" 18 "$PROBLEMS_DIR/263.txt" 1 1e+04 3892320 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/18.txt" &

wait


# RAM: 0.006 GB = 6.619 MB
echo "Solving 19 points..."
"$SRC_DIR/main.exe" 19 "$PROBLEMS_DIR/263.txt" 1 1e+04 6940947 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/19.txt" &

# RAM: 0.016 GB = 16.052 MB
echo "Solving 20 points..."
"$SRC_DIR/main.exe" 20 "$PROBLEMS_DIR/263.txt" 1 1e+04 16831720 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/20.txt" &

wait


# RAM: 0.028 GB = 28.832 MB
echo "Solving 21 points..."
"$SRC_DIR/main.exe" 21 "$PROBLEMS_DIR/263.txt" 1 1e+04 30232629 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/21.txt" &

# RAM: 0.067 GB = 68.868 MB
echo "Solving 22 points..."
"$SRC_DIR/main.exe" 22 "$PROBLEMS_DIR/263.txt" 1 1e+04 72213284 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/22.txt" &

wait


# RAM: 0.122 GB = 124.457 MB
echo "Solving 23 points..."
"$SRC_DIR/main.exe" 23 "$PROBLEMS_DIR/263.txt" 1 1e+04 130502713 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/23.txt" &

# RAM: 0.287 GB = 293.582 MB
echo "Solving 24 points..."
"$SRC_DIR/main.exe" 24 "$PROBLEMS_DIR/263.txt" 1 1e+04 307842672 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint32_t/24.txt" &

wait



echo "solving uint64_t tsp"
mkdir -p "$RESULTS_DIR/263/tsp/uint64_t"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/263.txt" 1 1e+12 9 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/263.txt" 1 1e+12 9 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/263.txt" 1 1e+12 16 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/263.txt" 1 1e+12 24 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/263.txt" 1 1e+12 128 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/5.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/263.txt" 1 1e+12 200 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/6.txt" &

wait


# RAM: 0.000 GB = 0.001 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/263.txt" 1 1e+12 720 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/7.txt" &

# RAM: 0.000 GB = 0.001 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/263.txt" 1 1e+12 1176 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/8.txt" &

wait


# RAM: 0.000 GB = 0.004 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/263.txt" 1 1e+12 3752 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/9.txt" &

# RAM: 0.000 GB = 0.006 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/263.txt" 1 1e+12 6300 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/10.txt" &

wait


# RAM: 0.000 GB = 0.017 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/263.txt" 1 1e+12 18000 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/11.txt" &

# RAM: 0.000 GB = 0.029 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/263.txt" 1 1e+12 30855 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/12.txt" &

wait


# RAM: 0.000 GB = 0.079 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/263.txt" 1 1e+12 82632 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/13.txt" &

# RAM: 0.000 GB = 0.137 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/263.txt" 1 1e+12 144001 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/14.txt" &

wait


# RAM: 0.000 GB = 0.352 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/263.txt" 1 1e+12 369460 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/15.txt" &

# RAM: 0.001 GB = 0.622 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/263.txt" 1 1e+12 652470 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/16.txt" &

wait


# RAM: 0.002 GB = 1.548 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/263.txt" 1 1e+12 1623248 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/17.txt" &

# RAM: 0.003 GB = 2.764 MB
echo "Solving 18 points..."
"$SRC_DIR/main.exe" 18 "$PROBLEMS_DIR/263.txt" 1 1e+12 2897820 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/18.txt" &

wait


# RAM: 0.007 GB = 6.717 MB
echo "Solving 19 points..."
"$SRC_DIR/main.exe" 19 "$PROBLEMS_DIR/263.txt" 1 1e+12 7042896 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/19.txt" &

# RAM: 0.012 GB = 12.098 MB
echo "Solving 20 points..."
"$SRC_DIR/main.exe" 20 "$PROBLEMS_DIR/263.txt" 1 1e+12 12685179 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/20.txt" &

wait


# RAM: 0.028 GB = 28.866 MB
echo "Solving 21 points..."
"$SRC_DIR/main.exe" 21 "$PROBLEMS_DIR/263.txt" 1 1e+12 30268520 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/21.txt" &

# RAM: 0.051 GB = 52.378 MB
echo "Solving 22 points..."
"$SRC_DIR/main.exe" 22 "$PROBLEMS_DIR/263.txt" 1 1e+12 54922749 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/22.txt" &

wait


# RAM: 0.120 GB = 123.137 MB
echo "Solving 23 points..."
"$SRC_DIR/main.exe" 23 "$PROBLEMS_DIR/263.txt" 1 1e+12 129118132 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/23.txt" &

# RAM: 0.220 GB = 224.835 MB
echo "Solving 24 points..."
"$SRC_DIR/main.exe" 24 "$PROBLEMS_DIR/263.txt" 1 1e+12 235756785 0 300 tsp 1 > "$RESULTS_DIR/263/tsp/uint64_t/24.txt" &

wait



echo "solving uint64_t shp"
mkdir -p "$RESULTS_DIR/263/shp/uint64_t"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/263.txt" 1 1e+12 9 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/263.txt" 1 1e+12 16 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/263.txt" 1 1e+12 24 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/263.txt" 1 1e+12 128 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/263.txt" 1 1e+12 200 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/5.txt" &

# RAM: 0.000 GB = 0.001 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/263.txt" 1 1e+12 720 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/6.txt" &

wait


# RAM: 0.000 GB = 0.001 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/263.txt" 1 1e+12 1176 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/7.txt" &

# RAM: 0.000 GB = 0.004 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/263.txt" 1 1e+12 3752 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/8.txt" &

wait


# RAM: 0.000 GB = 0.006 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/263.txt" 1 1e+12 6300 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/9.txt" &

# RAM: 0.000 GB = 0.017 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/263.txt" 1 1e+12 18000 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/10.txt" &

wait


# RAM: 0.000 GB = 0.029 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/263.txt" 1 1e+12 30855 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/11.txt" &

# RAM: 0.000 GB = 0.079 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/263.txt" 1 1e+12 82632 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/12.txt" &

wait


# RAM: 0.000 GB = 0.137 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/263.txt" 1 1e+12 144001 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/13.txt" &

# RAM: 0.000 GB = 0.352 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/263.txt" 1 1e+12 369460 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/14.txt" &

wait


# RAM: 0.001 GB = 0.622 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/263.txt" 1 1e+12 652470 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/15.txt" &

# RAM: 0.002 GB = 1.548 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/263.txt" 1 1e+12 1623248 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/16.txt" &

wait


# RAM: 0.003 GB = 2.764 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/263.txt" 1 1e+12 2897820 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/17.txt" &

# RAM: 0.007 GB = 6.717 MB
echo "Solving 18 points..."
"$SRC_DIR/main.exe" 18 "$PROBLEMS_DIR/263.txt" 1 1e+12 7042896 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/18.txt" &

wait


# RAM: 0.012 GB = 12.098 MB
echo "Solving 19 points..."
"$SRC_DIR/main.exe" 19 "$PROBLEMS_DIR/263.txt" 1 1e+12 12685179 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/19.txt" &

# RAM: 0.028 GB = 28.866 MB
echo "Solving 20 points..."
"$SRC_DIR/main.exe" 20 "$PROBLEMS_DIR/263.txt" 1 1e+12 30268520 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/20.txt" &

wait


# RAM: 0.051 GB = 52.378 MB
echo "Solving 21 points..."
"$SRC_DIR/main.exe" 21 "$PROBLEMS_DIR/263.txt" 1 1e+12 54922749 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/21.txt" &

# RAM: 0.120 GB = 123.137 MB
echo "Solving 22 points..."
"$SRC_DIR/main.exe" 22 "$PROBLEMS_DIR/263.txt" 1 1e+12 129118132 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/22.txt" &

wait


# RAM: 0.220 GB = 224.835 MB
echo "Solving 23 points..."
"$SRC_DIR/main.exe" 23 "$PROBLEMS_DIR/263.txt" 1 1e+12 235756785 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/23.txt" &

# RAM: 0.510 GB = 522.110 MB
echo "Solving 24 points..."
"$SRC_DIR/main.exe" 24 "$PROBLEMS_DIR/263.txt" 1 1e+12 547472496 0 300 shp 1 > "$RESULTS_DIR/263/shp/uint64_t/24.txt" &

wait



echo "solving float tsp"
mkdir -p "$RESULTS_DIR/263/tsp/float"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/263.txt" 1 1e+12 5 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/263.txt" 1 1e+12 5 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/263.txt" 1 1e+12 8 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/263.txt" 1 1e+12 12 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/263.txt" 1 1e+12 64 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/5.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/263.txt" 1 1e+12 100 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/6.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/263.txt" 1 1e+12 360 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/7.txt" &

# RAM: 0.000 GB = 0.001 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/263.txt" 1 1e+12 588 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/8.txt" &

wait


# RAM: 0.000 GB = 0.002 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/263.txt" 1 1e+12 1960 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/9.txt" &

# RAM: 0.000 GB = 0.003 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/263.txt" 1 1e+12 3276 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/10.txt" &

wait


# RAM: 0.000 GB = 0.009 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/263.txt" 1 1e+12 9600 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/11.txt" &

# RAM: 0.000 GB = 0.016 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/263.txt" 1 1e+12 16335 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/12.txt" &

wait


# RAM: 0.000 GB = 0.043 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/263.txt" 1 1e+12 44616 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/13.txt" &

# RAM: 0.000 GB = 0.074 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/263.txt" 1 1e+12 77077 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/14.txt" &

wait


# RAM: 0.000 GB = 0.192 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/263.txt" 1 1e+12 201292 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/15.txt" &

# RAM: 0.000 GB = 0.336 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/263.txt" 1 1e+12 352170 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/16.txt" &

wait


# RAM: 0.001 GB = 0.850 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/263.txt" 1 1e+12 891088 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/17.txt" &

# RAM: 0.001 GB = 1.502 MB
echo "Solving 18 points..."
"$SRC_DIR/main.exe" 18 "$PROBLEMS_DIR/263.txt" 1 1e+12 1575356 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/18.txt" &

wait


# RAM: 0.004 GB = 3.712 MB
echo "Solving 19 points..."
"$SRC_DIR/main.exe" 19 "$PROBLEMS_DIR/263.txt" 1 1e+12 3892320 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/19.txt" &

# RAM: 0.006 GB = 6.619 MB
echo "Solving 20 points..."
"$SRC_DIR/main.exe" 20 "$PROBLEMS_DIR/263.txt" 1 1e+12 6940947 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/20.txt" &

wait


# RAM: 0.016 GB = 16.052 MB
echo "Solving 21 points..."
"$SRC_DIR/main.exe" 21 "$PROBLEMS_DIR/263.txt" 1 1e+12 16831720 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/21.txt" &

# RAM: 0.028 GB = 28.832 MB
echo "Solving 22 points..."
"$SRC_DIR/main.exe" 22 "$PROBLEMS_DIR/263.txt" 1 1e+12 30232629 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/22.txt" &

wait


# RAM: 0.067 GB = 68.868 MB
echo "Solving 23 points..."
"$SRC_DIR/main.exe" 23 "$PROBLEMS_DIR/263.txt" 1 1e+12 72213284 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/23.txt" &

# RAM: 0.122 GB = 124.457 MB
echo "Solving 24 points..."
"$SRC_DIR/main.exe" 24 "$PROBLEMS_DIR/263.txt" 1 1e+12 130502713 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/float/24.txt" &

wait



echo "solving float shp"
mkdir -p "$RESULTS_DIR/263/shp/float"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/263.txt" 1 1e+12 5 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/263.txt" 1 1e+12 8 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/263.txt" 1 1e+12 12 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/263.txt" 1 1e+12 64 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/263.txt" 1 1e+12 100 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/5.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/263.txt" 1 1e+12 360 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/6.txt" &

wait


# RAM: 0.000 GB = 0.001 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/263.txt" 1 1e+12 588 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/7.txt" &

# RAM: 0.000 GB = 0.002 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/263.txt" 1 1e+12 1960 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/8.txt" &

wait


# RAM: 0.000 GB = 0.003 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/263.txt" 1 1e+12 3276 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/9.txt" &

# RAM: 0.000 GB = 0.009 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/263.txt" 1 1e+12 9600 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/10.txt" &

wait


# RAM: 0.000 GB = 0.016 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/263.txt" 1 1e+12 16335 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/11.txt" &

# RAM: 0.000 GB = 0.043 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/263.txt" 1 1e+12 44616 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/12.txt" &

wait


# RAM: 0.000 GB = 0.074 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/263.txt" 1 1e+12 77077 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/13.txt" &

# RAM: 0.000 GB = 0.192 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/263.txt" 1 1e+12 201292 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/14.txt" &

wait


# RAM: 0.000 GB = 0.336 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/263.txt" 1 1e+12 352170 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/15.txt" &

# RAM: 0.001 GB = 0.850 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/263.txt" 1 1e+12 891088 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/16.txt" &

wait


# RAM: 0.001 GB = 1.502 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/263.txt" 1 1e+12 1575356 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/17.txt" &

# RAM: 0.004 GB = 3.712 MB
echo "Solving 18 points..."
"$SRC_DIR/main.exe" 18 "$PROBLEMS_DIR/263.txt" 1 1e+12 3892320 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/18.txt" &

wait


# RAM: 0.006 GB = 6.619 MB
echo "Solving 19 points..."
"$SRC_DIR/main.exe" 19 "$PROBLEMS_DIR/263.txt" 1 1e+12 6940947 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/19.txt" &

# RAM: 0.016 GB = 16.052 MB
echo "Solving 20 points..."
"$SRC_DIR/main.exe" 20 "$PROBLEMS_DIR/263.txt" 1 1e+12 16831720 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/20.txt" &

wait


# RAM: 0.028 GB = 28.832 MB
echo "Solving 21 points..."
"$SRC_DIR/main.exe" 21 "$PROBLEMS_DIR/263.txt" 1 1e+12 30232629 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/21.txt" &

# RAM: 0.067 GB = 68.868 MB
echo "Solving 22 points..."
"$SRC_DIR/main.exe" 22 "$PROBLEMS_DIR/263.txt" 1 1e+12 72213284 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/22.txt" &

wait


# RAM: 0.122 GB = 124.457 MB
echo "Solving 23 points..."
"$SRC_DIR/main.exe" 23 "$PROBLEMS_DIR/263.txt" 1 1e+12 130502713 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/23.txt" &

# RAM: 0.287 GB = 293.582 MB
echo "Solving 24 points..."
"$SRC_DIR/main.exe" 24 "$PROBLEMS_DIR/263.txt" 1 1e+12 307842672 1 300 shp 1 > "$RESULTS_DIR/263/shp/float/24.txt" &

wait



echo "solving double tsp"
mkdir -p "$RESULTS_DIR/263/tsp/double"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/263.txt" 1 1e+12 9 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/263.txt" 1 1e+12 9 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/263.txt" 1 1e+12 16 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/263.txt" 1 1e+12 24 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/263.txt" 1 1e+12 128 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/5.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/263.txt" 1 1e+12 200 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/6.txt" &

wait


# RAM: 0.000 GB = 0.001 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/263.txt" 1 1e+12 720 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/7.txt" &

# RAM: 0.000 GB = 0.001 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/263.txt" 1 1e+12 1176 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/8.txt" &

wait


# RAM: 0.000 GB = 0.004 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/263.txt" 1 1e+12 3752 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/9.txt" &

# RAM: 0.000 GB = 0.006 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/263.txt" 1 1e+12 6300 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/10.txt" &

wait


# RAM: 0.000 GB = 0.017 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/263.txt" 1 1e+12 18000 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/11.txt" &

# RAM: 0.000 GB = 0.029 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/263.txt" 1 1e+12 30855 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/12.txt" &

wait


# RAM: 0.000 GB = 0.079 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/263.txt" 1 1e+12 82632 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/13.txt" &

# RAM: 0.000 GB = 0.137 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/263.txt" 1 1e+12 144001 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/14.txt" &

wait


# RAM: 0.000 GB = 0.352 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/263.txt" 1 1e+12 369460 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/15.txt" &

# RAM: 0.001 GB = 0.622 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/263.txt" 1 1e+12 652470 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/16.txt" &

wait


# RAM: 0.002 GB = 1.548 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/263.txt" 1 1e+12 1623248 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/17.txt" &

# RAM: 0.003 GB = 2.764 MB
echo "Solving 18 points..."
"$SRC_DIR/main.exe" 18 "$PROBLEMS_DIR/263.txt" 1 1e+12 2897820 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/18.txt" &

wait


# RAM: 0.007 GB = 6.717 MB
echo "Solving 19 points..."
"$SRC_DIR/main.exe" 19 "$PROBLEMS_DIR/263.txt" 1 1e+12 7042896 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/19.txt" &

# RAM: 0.012 GB = 12.098 MB
echo "Solving 20 points..."
"$SRC_DIR/main.exe" 20 "$PROBLEMS_DIR/263.txt" 1 1e+12 12685179 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/20.txt" &

wait


# RAM: 0.028 GB = 28.866 MB
echo "Solving 21 points..."
"$SRC_DIR/main.exe" 21 "$PROBLEMS_DIR/263.txt" 1 1e+12 30268520 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/21.txt" &

# RAM: 0.051 GB = 52.378 MB
echo "Solving 22 points..."
"$SRC_DIR/main.exe" 22 "$PROBLEMS_DIR/263.txt" 1 1e+12 54922749 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/22.txt" &

wait


# RAM: 0.120 GB = 123.137 MB
echo "Solving 23 points..."
"$SRC_DIR/main.exe" 23 "$PROBLEMS_DIR/263.txt" 1 1e+12 129118132 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/23.txt" &

# RAM: 0.220 GB = 224.835 MB
echo "Solving 24 points..."
"$SRC_DIR/main.exe" 24 "$PROBLEMS_DIR/263.txt" 1 1e+12 235756785 1 300 tsp 1 > "$RESULTS_DIR/263/tsp/double/24.txt" &

wait



echo "solving double shp"
mkdir -p "$RESULTS_DIR/263/shp/double"

# RAM: 0.000 GB = 0.000 MB
echo "Solving 1 points..."
"$SRC_DIR/main.exe" 1 "$PROBLEMS_DIR/263.txt" 1 1e+12 9 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/1.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 2 points..."
"$SRC_DIR/main.exe" 2 "$PROBLEMS_DIR/263.txt" 1 1e+12 16 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/2.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 3 points..."
"$SRC_DIR/main.exe" 3 "$PROBLEMS_DIR/263.txt" 1 1e+12 24 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/3.txt" &

# RAM: 0.000 GB = 0.000 MB
echo "Solving 4 points..."
"$SRC_DIR/main.exe" 4 "$PROBLEMS_DIR/263.txt" 1 1e+12 128 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/4.txt" &

wait


# RAM: 0.000 GB = 0.000 MB
echo "Solving 5 points..."
"$SRC_DIR/main.exe" 5 "$PROBLEMS_DIR/263.txt" 1 1e+12 200 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/5.txt" &

# RAM: 0.000 GB = 0.001 MB
echo "Solving 6 points..."
"$SRC_DIR/main.exe" 6 "$PROBLEMS_DIR/263.txt" 1 1e+12 720 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/6.txt" &

wait


# RAM: 0.000 GB = 0.001 MB
echo "Solving 7 points..."
"$SRC_DIR/main.exe" 7 "$PROBLEMS_DIR/263.txt" 1 1e+12 1176 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/7.txt" &

# RAM: 0.000 GB = 0.004 MB
echo "Solving 8 points..."
"$SRC_DIR/main.exe" 8 "$PROBLEMS_DIR/263.txt" 1 1e+12 3752 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/8.txt" &

wait


# RAM: 0.000 GB = 0.006 MB
echo "Solving 9 points..."
"$SRC_DIR/main.exe" 9 "$PROBLEMS_DIR/263.txt" 1 1e+12 6300 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/9.txt" &

# RAM: 0.000 GB = 0.017 MB
echo "Solving 10 points..."
"$SRC_DIR/main.exe" 10 "$PROBLEMS_DIR/263.txt" 1 1e+12 18000 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/10.txt" &

wait


# RAM: 0.000 GB = 0.029 MB
echo "Solving 11 points..."
"$SRC_DIR/main.exe" 11 "$PROBLEMS_DIR/263.txt" 1 1e+12 30855 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/11.txt" &

# RAM: 0.000 GB = 0.079 MB
echo "Solving 12 points..."
"$SRC_DIR/main.exe" 12 "$PROBLEMS_DIR/263.txt" 1 1e+12 82632 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/12.txt" &

wait


# RAM: 0.000 GB = 0.137 MB
echo "Solving 13 points..."
"$SRC_DIR/main.exe" 13 "$PROBLEMS_DIR/263.txt" 1 1e+12 144001 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/13.txt" &

# RAM: 0.000 GB = 0.352 MB
echo "Solving 14 points..."
"$SRC_DIR/main.exe" 14 "$PROBLEMS_DIR/263.txt" 1 1e+12 369460 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/14.txt" &

wait


# RAM: 0.001 GB = 0.622 MB
echo "Solving 15 points..."
"$SRC_DIR/main.exe" 15 "$PROBLEMS_DIR/263.txt" 1 1e+12 652470 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/15.txt" &

# RAM: 0.002 GB = 1.548 MB
echo "Solving 16 points..."
"$SRC_DIR/main.exe" 16 "$PROBLEMS_DIR/263.txt" 1 1e+12 1623248 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/16.txt" &

wait


# RAM: 0.003 GB = 2.764 MB
echo "Solving 17 points..."
"$SRC_DIR/main.exe" 17 "$PROBLEMS_DIR/263.txt" 1 1e+12 2897820 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/17.txt" &

# RAM: 0.007 GB = 6.717 MB
echo "Solving 18 points..."
"$SRC_DIR/main.exe" 18 "$PROBLEMS_DIR/263.txt" 1 1e+12 7042896 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/18.txt" &

wait


# RAM: 0.012 GB = 12.098 MB
echo "Solving 19 points..."
"$SRC_DIR/main.exe" 19 "$PROBLEMS_DIR/263.txt" 1 1e+12 12685179 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/19.txt" &

# RAM: 0.028 GB = 28.866 MB
echo "Solving 20 points..."
"$SRC_DIR/main.exe" 20 "$PROBLEMS_DIR/263.txt" 1 1e+12 30268520 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/20.txt" &

wait


# RAM: 0.051 GB = 52.378 MB
echo "Solving 21 points..."
"$SRC_DIR/main.exe" 21 "$PROBLEMS_DIR/263.txt" 1 1e+12 54922749 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/21.txt" &

# RAM: 0.120 GB = 123.137 MB
echo "Solving 22 points..."
"$SRC_DIR/main.exe" 22 "$PROBLEMS_DIR/263.txt" 1 1e+12 129118132 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/22.txt" &

wait


# RAM: 0.220 GB = 224.835 MB
echo "Solving 23 points..."
"$SRC_DIR/main.exe" 23 "$PROBLEMS_DIR/263.txt" 1 1e+12 235756785 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/23.txt" &

# RAM: 0.510 GB = 522.110 MB
echo "Solving 24 points..."
"$SRC_DIR/main.exe" 24 "$PROBLEMS_DIR/263.txt" 1 1e+12 547472496 1 300 shp 1 > "$RESULTS_DIR/263/shp/double/24.txt" &

wait

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

echo "solving uint8_t tsp"
mkdir -p "$RESULTS_DIR/bayg29/tsp/uint8_t"

# RAM: 2.203 GB = 2256.198 MB
echo "Solving 29 points..."
"$SRC_DIR/main.exe" 29 "$PROBLEMS_DIR/bayg29.tsp" 1 1e+12 2365795488 0 300 tsp 0 > "$RESULTS_DIR/bayg29/tsp/uint8_t/29.txt" &


echo "solving uint16_t tsp"
mkdir -p "$RESULTS_DIR/bayg29/tsp/uint16_t"

# RAM: 3.180 GB = 3256.012 MB
echo "Solving 29 points..."
"$SRC_DIR/main.exe" 29 "$PROBLEMS_DIR/bayg29.tsp" 1 1e+12 3414175968 0 300 tsp 0 > "$RESULTS_DIR/bayg29/tsp/uint16_t/29.txt" &


echo "solving uint32_t tsp"
mkdir -p "$RESULTS_DIR/bayg29/tsp/uint32_t"

# RAM: 5.132 GB = 5255.639 MB
echo "Solving 29 points..."
"$SRC_DIR/main.exe" 29 "$PROBLEMS_DIR/bayg29.tsp" 1 1e+04 5510936928 0 300 tsp 0 > "$RESULTS_DIR/bayg29/tsp/uint32_t/29.txt" &


echo "solving uint64_t tsp"
mkdir -p "$RESULTS_DIR/bayg29/tsp/uint64_t"

# RAM: 9.038 GB = 9254.893 MB
echo "Solving 29 points..."
"$SRC_DIR/main.exe" 29 "$PROBLEMS_DIR/bayg29.tsp" 1 1e+12 9704458848 0 300 tsp 0 > "$RESULTS_DIR/bayg29/tsp/uint64_t/29.txt" &


echo "solving float tsp"
mkdir -p "$RESULTS_DIR/bayg29/tsp/float"

# RAM: 5.132 GB = 5255.639 MB
echo "Solving 29 points..."
"$SRC_DIR/main.exe" 29 "$PROBLEMS_DIR/bayg29.tsp" 1 1e+12 5510936928 1 300 tsp 0 > "$RESULTS_DIR/bayg29/tsp/float/29.txt" &


echo "solving double tsp"
mkdir -p "$RESULTS_DIR/bayg29/tsp/double"

# RAM: 9.038 GB = 9254.893 MB
echo "Solving 29 points..."
"$SRC_DIR/main.exe" 29 "$PROBLEMS_DIR/bayg29.tsp" 1 1e+12 9704458848 1 300 tsp 0 > "$RESULTS_DIR/bayg29/tsp/double/29.txt" &


# Test results:
echo "Checking results bayg29..."
python "$ROOT_DIR/judge_results.py" bellman_held_karp bayg29 bayg29.tsp 29


# Test results:
echo "Checking results br17..."
python "$ROOT_DIR/judge_results.py" bellman_held_karp br17 br17.atsp 17


# Test results:
echo "Checking results 263..."
python "$ROOT_DIR/judge_results.py" bellman_held_karp 263 263.txt 24
