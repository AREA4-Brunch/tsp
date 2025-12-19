"""
python analize_results.py <problem_name> [--markdown|-md] [--no-cache|-nc]

python commands/k_opt/analize_results.py 263 -md
python commands/k_opt/analize_results.py 263
"""

import os
import pickle
from collections import defaultdict
import hashlib
import sys
import re

K = 3
ALGOS = [
    f'{K}_opt_classical',
    f'{K}_opt_best_cut',
    f'{K}_opt_funky',
    f'{K}_opt_rand',
    f'{K}_opt_rand_no_2_opt',
]
MODE = 'shp'
PATH_CWD = os.path.dirname(os.path.realpath(__file__))
PATH_RES = os.path.join(PATH_CWD, f'../../results')
PATH_ANALYSIS = os.path.join(PATH_CWD, f'../../analysis', f'all_{K}_opt')
PATH_CACHED = os.path.join(PATH_CWD, f'../../analysis', 'cached')


def main():
    output_markdown = any(
        arg.lower() in ['--markdown', '-md'] for arg in sys.argv
    )
    no_cache = any(
        arg.lower() in ['--no-cache', '-nc'] for arg in sys.argv
    )
    prob_name = sys.argv[1] if len(sys.argv) > 1 else '263'

    global PATH_ANALYSIS, PATH_CACHED
    PATH_ANALYSIS = os.path.join(PATH_ANALYSIS, prob_name)
    PATH_CACHED = os.path.join(PATH_CACHED, prob_name)
    os.makedirs(PATH_ANALYSIS, exist_ok=True)
    os.makedirs(PATH_CACHED, exist_ok=True)

    cache_fname = shorten_file_name(
        PATH_CACHED + f'{MODE}_merged_{prob_name}' + '+'.join(ALGOS)
    )
    path_cached = os.path.realpath(os.path.join(PATH_CACHED, cache_fname))

    if not no_cache and os.path.isfile(path_cached):
        with open(path_cached, 'rb') as file:
            algo_results = pickle.load(file)
        print(f'\nWARNING: using cached results from:\n{path_cached}\n')
    else:
        algo_results = fetch_algo_results(
            PATH_RES, MODE, prob_name, ALGOS
        )
        with open(path_cached, 'wb') as file:
            pickle.dump(algo_results, file)
        print(f'Cached results to: {path_cached}')

    out_suffix = "_markdown.md" if output_markdown else ".txt"
    path_analysis = os.path.join(
        PATH_ANALYSIS, f'analysis_{MODE}{out_suffix}'
    )

    analyze(
        algo_results,
        path_analysis,
        ALGOS,
        output_markdown=output_markdown,
    )

def fetch_algo_results(path_dir, mode, prob_name, algos):
    """
    Fetch results for all algorithms.
    Returns: dict[num_points][algo_name] = result_dict
    """
    algo_results = defaultdict(dict)
    for algo in algos:
        algo_path = os.path.join(path_dir, algo, prob_name, mode)
        if not os.path.isdir(algo_path):
            print(f'Warning: {algo_path} not found')
            continue
        for root, dirs, filenames in os.walk(algo_path):
            for filename in filenames:
                if not filename.endswith('.txt'):
                    continue

                num_points = int(filename.removesuffix('.txt'))
                file_path = os.path.join(root, filename)

                result = parse_execution_file(file_path)
                if result:
                    algo_results[num_points][algo] = result

    return algo_results

def parse_execution_file(file_path):
    """Parse a single execution result file."""
    total_costs, num_execs = 0, 0
    ttl_exec_times, max_exec_time_ms = 0, 0
    best_cost = float('inf')
    best_time, best_run = 0, 0
    total_time_seconds = 0

    with open(file_path, 'r') as file:
        content = file.read()
        exec_blocks = re.split(r'Execution #\d+/\d+', content)
        for block in exec_blocks[1:]:
            block_result = parse_execution_block(block)
            if block_result:
                cost, exec_ms, ttb_ms = block_result
                total_costs += cost
                num_execs += 1
                ttl_exec_times += exec_ms
                if cost < best_cost:
                    best_time = ttl_exec_times
                    best_cost = cost
                    best_run = num_execs
                max_exec_time_ms = max(max_exec_time_ms, exec_ms)
                total_time_seconds += exec_ms / 1000

    if num_execs <= 0:
        return None

    avg_cost = total_costs / num_execs
    avg_exec_time_ms = ttl_exec_times / num_execs
    return {
        'avg_cost': avg_cost,
        'best_cost': best_cost,
        'time_to_best': format_time_ms(best_time),
        'total_exec_time': format_time(total_time_seconds),
        'avg_exec_time': avg_exec_time_ms,
        'max_exec_time': max_exec_time_ms,
        'runs': num_execs,
        'best_run': best_run,
    }

def parse_execution_block(block):
    """
    Parse a single execution block.
    Returns: (cost, exec_time_ms, time_to_best_ms) or None
    """
    best_match = re.search(
        r'Best found total distance for \d+ points:\s*([\d.]+)', 
        block
    )
    if not best_match:
        return None
    best_cost = float(best_match.group(1))
    time_match = re.search(
        r'Execution time:\s*([\d.]+)\s*ms', 
        block
    )
    exec_ms = float(time_match.group(1)) if time_match else 0
    ttb_ms = exec_ms  # default to total time
    return (best_cost, exec_ms, ttb_ms)

def analyze(
    algo_results_all_sizes, path_analysis, algos, output_markdown=False
):
    """Write analysis report."""
    algo_results_all_sizes = dict(
        sorted(
            algo_results_all_sizes.items(),
            key=lambda x: int(x[0])
        )
    )

    with open(path_analysis, 'w') as file:
        file.write('## 3-opt Variants Comparison\n\n')

        for num_points, algo_results in algo_results_all_sizes.items():
            file.write(f'\nProblem Size: {num_points} points\n\n')
            write_results_table(
                file, algo_results, algos, output_markdown
            )

    print(f'Analysis saved to: {path_analysis}')

def write_results_table(file, algo_results, algos, output_markdown):
    """Write results table in markdown or text format."""
    if output_markdown:
        write_markdown_table(file, algo_results, algos)
    else:
        write_text_table(file, algo_results, algos)

def write_markdown_table(file, algo_results, algos):
    """Write markdown formatted table."""
    header = (
        '| Variant | Avg Cost | Best Cost | Time to Best | '
        'Total Time | Avg Run Time [ms] | Max Run Time [ms] | No. Runs |'
    )
    separator = (
        '|---------|----------|-----------|--------------|'
        '------------|--------------|---------------|------|'
    )

    file.write(header + '\n' + separator + '\n')

    for algo in algos:
        if algo not in algo_results:
            continue

        r = algo_results[algo]
        row = (
            f'| {algo} | {r["avg_cost"]:.3f} | '
            f'{r["best_cost"]:.3f} | {r["time_to_best"]} | '
            f'{r["total_exec_time"]} | {r["avg_exec_time"]:.3f} | '
            f'{r["max_exec_time"]:.3f} | {r["runs"]} |'
        )
        file.write(row + '\n')

def write_text_table(file, algo_results, algos):
    """Write text formatted table."""
    header = (
        'Variant | Avg Cost | Best Cost | Time to Best | '
        'Total Time | Avg Time [ms] | Max Time [ms] | Runs'
    )
    separator = '-' * 130

    file.write(header + '\n' + separator + '\n')

    for algo in algos:
        if algo not in algo_results:
            continue

        r = algo_results[algo]
        row = (
            f'{algo:25} | {r["avg_cost"]:8.3f} | '
            f'{r["best_cost"]:9.3f} | {r["time_to_best"]:12} | '
            f'{r["total_exec_time"]:10} | {r["avg_exec_time"]:12.3f} | '
            f'{r["max_exec_time"]:13.3f} | {r["runs"]:4}'
        )
        file.write(row + '\n')

def format_time(seconds):
    """Convert seconds to human readable format."""
    if seconds < 60:
        return f'{seconds:.3f} s'
    elif seconds < 3600:
        mins = int(seconds // 60)
        secs = seconds % 60
        return f'{mins}:{secs:05.2f} m'
    else:
        hours = int(seconds // 3600)
        mins = int((seconds % 3600) // 60)
        secs = seconds % 60
        return f'{hours}:{mins:02d}:{secs:05.2f} h'

def format_time_ms(milliseconds):
    """Convert milliseconds to human readable format."""
    return format_time(milliseconds / 1000)

def shorten_file_name(file_name):
    """Shorten long filenames using hash."""
    max_chars = 20
    if len(file_name) <= max_chars:
        return file_name
    hashed = hashlib.sha256(file_name.encode()).digest()
    return hashed[2 : 2 + max_chars // 2].hex()


if __name__ == '__main__':
    main()
