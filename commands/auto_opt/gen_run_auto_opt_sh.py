PARALLEL_CAPACITY = None  # do not parallelize
PARALLEL_CAPACITY = 2

NO_HISTORY = '--no-history'
NO_HISTORY = ''

ALGO = 'auto_opt_funky'
SELECT_ALGO = 'funky'
CUT_ALGO = 'auto_opt'
MIN_N = 1
MAX_N = 33  # inclusive
NUM_RERUNS = 100
RUNS_PER_HISTORY = 500
SINGLE_TEST_TIMEOUT_SEC = 5 * 60
PROBLEM_NAME = '263'
PROBLEM_FNAME = f'{PROBLEM_NAME}.txt'
IS_PROBLEM_IN_PTS_FORMAT = 1
MODES = ['tsp', 'shp']
PARALLEL_CAPACITY = 2

ALGO = 'auto_opt_funky'
SELECT_ALGO = 'funky'
CUT_ALGO = 'auto_opt'
MIN_N = 263
MAX_N = 263  # inclusive
NUM_RERUNS = 10
RUNS_PER_HISTORY = 500
SINGLE_TEST_TIMEOUT_SEC = 5 * 60
PROBLEM_NAME = '263'
PROBLEM_FNAME = f'{PROBLEM_NAME}.txt'
IS_PROBLEM_IN_PTS_FORMAT = 1
MODES = ['tsp', 'shp']
PARALLEL_CAPACITY = 2

ALGO = 'auto_opt_funky'
SELECT_ALGO = 'funky'
CUT_ALGO = 'auto_opt'
MIN_N = 29
MAX_N = 29  # inclusive
NUM_RERUNS = 100
RUNS_PER_HISTORY = 500
SINGLE_TEST_TIMEOUT_SEC = 5 * 60
PROBLEM_NAME = 'bayg29'
PROBLEM_FNAME = f'{PROBLEM_NAME}.tsp'
IS_PROBLEM_IN_PTS_FORMAT = 0
MODES = ['tsp', 'shp']
PARALLEL_CAPACITY = 2



def main():
    print(get_script_header(ALGO))
    print(f'\n\necho "Solving problem: {PROBLEM_NAME}"')
    for mode in MODES:
        print(f'\n\necho "solving {mode}"')
        print_sh_commands(MIN_N, MAX_N, SELECT_ALGO, CUT_ALGO,
                          SINGLE_TEST_TIMEOUT_SEC, NUM_RERUNS,
                          RUNS_PER_HISTORY, mode,
                          IS_PROBLEM_IN_PTS_FORMAT, NO_HISTORY,
                          PARALLEL_CAPACITY)
    print(f'\n\n# Test results:')
    print(f'echo "Checking results {PROBLEM_NAME}..."')
    print(f'python "$ROOT_DIR/judge_results.py" {ALGO}'
          f' {PROBLEM_NAME} {PROBLEM_FNAME} {MAX_N}')


def print_sh_commands(min_n, max_n, select, cut, timeout_secs, num_reruns, runs_per_history,
                      mode='shp', is_dist_pts_fmt=1, no_history='', parallel_cap=None):
    print(f'mkdir -p "$RESULTS_DIR/{PROBLEM_NAME}/{mode}"')
    if no_history:
        no_history = f' {no_history}'
    for n in range(min_n, max_n + 1):
        cmd = f'"$SRC_DIR/main.exe" "{select}" "{cut}" {n} "$PROBLEMS_DIR/{PROBLEM_FNAME}"' \
              f' "$HISTORY_DIR/{PROBLEM_NAME}/{mode}/{n}" {num_reruns} ' \
              f' {runs_per_history} {timeout_secs} {mode} {is_dist_pts_fmt}{no_history}' \
              f' > "$RESULTS_DIR/{PROBLEM_NAME}/{mode}/{n}.txt" &'
        
        print(f'echo "Solving {n} points..."')
        print(cmd)
        if parallel_cap is not None and (n - min_n + 1) % parallel_cap == 0:
            print('\nwait\n')
    if parallel_cap is not None and (max_n - min_n + 1) % parallel_cap != 0:
        print('\nwait\n')

def get_script_header(algo):
    cmd = r"""
#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$SCRIPT_DIR/.."
SRC_DIR="$SCRIPT_DIR/../../src/auto_opt"
PROBLEMS_DIR="$SCRIPT_DIR/../../problems"
RESULTS_DIR="$SCRIPT_DIR/../../results/auto_opt_funky"
HISTORY_DIR="$RESULTS_DIR/histories"

echo "Compiling..."
if ! g++ --static -std=c++20 -O3 -Wall -Wextra -o "$SRC_DIR/main.exe" "$SRC_DIR/main.cpp"; then
    echo "Compilation failed."
    exit 1
fi
    """.strip()

    cmd = cmd.replace('auto_opt_funky', algo)
    return cmd



if __name__ == "__main__":
    main()
