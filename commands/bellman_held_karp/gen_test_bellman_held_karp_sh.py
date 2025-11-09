import math

PARALLEL_CAPACITY = None  # do not parallelize
PARALLEL_CAPACITY = 2

MIN_N = 1
MAX_N = 24
NUM_RERUNS = 1
SINGLE_TEST_TIMEOUT_SEC = 5 * 60
PROBLEM_NAME = '263'
PROBLEM_FNAME = f'{PROBLEM_NAME}.txt'
IS_PROBLEM_IN_PTS_FORMAT = 1
MODES = ['tsp', 'shp']
IS_SYMMETRIC = True
PARALLEL_CAPACITY = 2

MIN_N = 1
MAX_N = 17
NUM_RERUNS = 1
PROBLEM_NAME = 'br17'
PROBLEM_FNAME = f'{PROBLEM_NAME}.atsp'
IS_PROBLEM_IN_PTS_FORMAT = 0
MODES = ['tsp']
IS_SYMMETRIC = False
PARALLEL_CAPACITY = 2

MIN_N = 29
MAX_N = 29
NUM_RERUNS = 1
PROBLEM_NAME = 'bayg29'
PROBLEM_FNAME = f'{PROBLEM_NAME}.tsp'
IS_PROBLEM_IN_PTS_FORMAT = 0
MODES = ['tsp']
IS_SYMMETRIC = True
PARALLEL_CAPACITY = None

MIN_N = 1
MAX_N = 24
NUM_RERUNS = 10000
SINGLE_TEST_TIMEOUT_SEC = 5 * 60
MODES = ['tsp', 'shp']
PROBLEM_NAME = '263'
PROBLEM_FNAME = f'{PROBLEM_NAME}.txt'
IS_PROBLEM_IN_PTS_FORMAT = 1
IS_SYMMETRIC = True
PARALLEL_CAPACITY = 2

MIN_N = 25
NUM_RERUNS = 10000
SINGLE_TEST_TIMEOUT_SEC = 5 * 60
PROBLEM_NAME = '263'
PROBLEM_FNAME = f'{PROBLEM_NAME}.txt'
IS_PROBLEM_IN_PTS_FORMAT = 1
MAX_N = 29
# MODES = ['shp']
MAX_N = 30
MODES = ['tsp']
IS_SYMMETRIC = True
PARALLEL_CAPACITY = None


def main():
    print(get_script_header())
    for cost_dtype in ['uint8_t', 'uint16_t', 'uint32_t', 'uint64_t', 'float', 'double']:
        for mode in MODES:
            print(f'\n\necho "solving {cost_dtype} {mode}"')
            vertex_type_size = 1  # in bytes, always int8_t
            precision = 1e12 if 'int32_t' not in cost_dtype else 1e4
            print_sh_commands(MIN_N, MAX_N, SINGLE_TEST_TIMEOUT_SEC,
                              precision, NUM_RERUNS, IS_SYMMETRIC,
                              cost_dtype, vertex_type_size, mode,
                              IS_PROBLEM_IN_PTS_FORMAT, PARALLEL_CAPACITY)
    print(f'\n\n# Test results:')
    print(f'echo "Checking results {PROBLEM_NAME}..."')
    print(f'python "$ROOT_DIR/judge_results.py" bellman_held_karp'
          f' {PROBLEM_NAME} {PROBLEM_FNAME} {MAX_N}')


def print_sh_commands(min_n, max_n, timeout_secs, precision, num_reruns, is_symmetric,
                      cost_t: str, tv=1, mode='shp', is_dist_pts_fmt=1, parallel_cap=None):
    print(f'mkdir -p "$RESULTS_DIR/{PROBLEM_NAME}/{mode}/{cost_t}"')
    do_not_prefer_int = 1 if 'int' not in cost_t else 0
    for n in range(min_n, max_n + 1):
        # provide min memory to enforce given dtype
        mem = space(n, mode == 'tsp', is_symmetric, dtype_size(cost_t), tv)
        cmd = f'"$SRC_DIR/main.exe" {n} "$PROBLEMS_DIR/{PROBLEM_FNAME}" {num_reruns} ' \
            + f'{precision:.0e} {mem} {do_not_prefer_int} {timeout_secs} {mode} ' \
            + f'{is_dist_pts_fmt} > "$RESULTS_DIR/{PROBLEM_NAME}/{mode}/{cost_t}/{n}.txt"' \
            + (' &' if parallel_cap is not None else '')
        print(f'\n# RAM: {mem / 2**30:.3f} GB = {mem / 2**20:.3f} MB')
        print(f'echo "Solving {n} points..."')
        print(cmd)
        if parallel_cap is not None and (n - min_n + 1) % parallel_cap == 0:
            print('\nwait\n')
    if parallel_cap is not None and (max_n - min_n + 1) % parallel_cap != 0:
        print('\nwait\n')

def space(n, cycle: bool, is_symmetric: bool, tc=2, tv=1):
    if cycle:
        n -= 1
    if n <= 1:
        return 1 * tc + 1 * tv
    next_k = n // 2
    next_cost = (next_k) *  math.comb(n, next_k)
    if n <= 2:
        prev_k = 0
    else:
        prev_k = n // 2 + (-1 if is_symmetric else 1)
    prev_cost = prev_k *  math.comb(n, prev_k)
    cost = next_cost + prev_cost
    max_k = n // 2 if is_symmetric else n - 1
    prev = sum([ math.comb(n, k - 1) * (n - (k - 1))
                 for k in range(3, max_k) ])
    return cost * tc + prev * tv

def dtype_size(dtype: str) -> int:
    if dtype == 'uint8_t':
        return 1
    if dtype == 'uint16_t':
        return 2
    elif dtype == 'uint32_t':
        return 4
    elif dtype == 'uint64_t':
        return 8
    elif dtype == 'float':
        return 4
    elif dtype == 'double':
        return 8
    else:
        raise ValueError(f'Unsupported dtype: {dtype}')

def get_script_header():
    return r"""
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
    """.strip()



if __name__ == "__main__":
    main()
