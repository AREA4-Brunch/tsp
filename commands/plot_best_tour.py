"""
    python plot_best_tour.py <problem_name> <problem_file> <n_min> <n_max> <mode> <algo> [variant]
    e.g:
    python plot_best_tour.py 263 263.txt 30 263 tsp 3_opt_rand best_found
    python plot_best_tour.py 263 263.txt 20 30 shp bellman_held_karp double
"""

import os
import sys
import re
import matplotlib.pyplot as plt
from collections import defaultdict


def main():
    args = sys.argv[1:]
    prob_name = args[0] if len(args) > 0 else '263'
    prob_file = args[1] if len(args) > 1 else f'{prob_name}.txt'
    n_min = int(args[2]) if len(args) > 2 else 0
    n_max = int(args[3]) if len(args) > 3 else float('inf')
    mode = args[4] if len(args) > 4 else None
    algo = args[5] if len(args) > 5 else None
    variant = args[6] if len(args) > 6 else None

    # if somethign is not provided genrate for all possibilites
    if algo is None:
        algos = [ 'bellman_held_karp' ]
        for k in range(3, 3 + 1):
            algos.append(f'{k}_opt_classical')
            algos.append(f'{k}_opt_best_cut')
            algos.append(f'{k}_opt_funky')
            algos.append(f'{k}_opt_rand')
            algos.append(f'{k}_opt_rand_no_2_opt')
    else:
        algos = [ algo ]

    variants = [
        variant if variant is not None else (
            'double' if algo == 'bellman_held_karp' else 'best_found'
        )
        for algo in algos
    ]
    modes = [ mode ] if mode is not None else [ 'tsp', 'shp' ]

    for mode in modes:
        for algo, variant in zip(algos, variants):
            plot_best_tours(prob_name, prob_file, n_min, n_max,
                            mode, algo, variant)


def plot_best_tours(prob_name, prob_file, n_min, n_max, mode, algo, variant):
    cwd = os.path.realpath(os.path.dirname(__file__))
    points = load_points(os.path.join(cwd, '..', 'problems', prob_file), n_max)
    if algo == 'bellman_held_karp':
        results_dir = os.path.join(cwd, '..', 'results',
                                   algo, prob_name, mode, variant)
    else:
        results_dir = os.path.join(cwd, '..', 'results',
                                   algo, prob_name, mode)
    solutions = find_best_results(results_dir, n_min, n_max)
    if algo == 'bellman_held_karp':
        analysis_dir = os.path.join(cwd, '..', 'analysis',
                                   algo, prob_name, mode)
    else:
        analysis_dir = os.path.join(cwd, '..', 'analysis',
                                   algo, prob_name, mode, variant)
    os.makedirs(analysis_dir, exist_ok=True)
    print(f'Plotting to dir:\n{analysis_dir}')
    for n, (cost, path) in solutions.items():
        if n_min <= n <= n_max and path and cost is not None:
            plot_tour(points[:n], path, n, cost, mode,
                      analysis_dir, algo, prob_name)
            print(f"Plotted tour for N={n}, cost={cost}")

def load_points(filepath, num_points):
    """Load coordinates from problem file."""
    points = []
    try:
        with open(filepath, "r") as f:
            lines = f.readlines()
        is_edge_fmt_section = any('EDGE_WEIGHT_FORMAT' in line for line in lines)
        is_node_coo_section = (not is_edge_fmt_section
            and any('NODE_COORD_SECTION' in line for line in lines))
        is_pts_fmt = not is_edge_fmt_section and not is_node_coo_section
        if not is_pts_fmt and not is_node_coo_section:
            raise Exception(f'Proble file does not contain points coo.')
        for line in lines:
            parts = line.strip().split()
            if is_node_coo_section:
                parts = parts[1:]  # Skip the index part
            if len(parts) >= 2:
                try:
                    x, y = float(parts[0]), float(parts[1])
                    points.append((x, y))
                    if len(points) == num_points:
                        break
                except ValueError:
                    continue
    except FileNotFoundError as e:
        raise e
    return points

def find_best_results(results_dir, min_n, max_n):
    """Find best solution for each N in the results directory."""
    results = defaultdict(lambda: (float('inf'), None))
    pattern = re.compile(r"total distance for (\d+) points: ([\d\.Ee+-]+)")
    path_pattern = re.compile(r"Point #(\d+)")

    if not os.path.exists(results_dir):
        print(f"Warning: Results directory not found: {results_dir}")
        return results

    for dirpath, _, filenames in os.walk(results_dir):
        for fname in filenames:
            if not fname.endswith('.txt'):
                continue
            fpath = os.path.join(dirpath, fname)
            short_fpath = fpath.replace(results_dir, '')
            try:
                n = int(short_fpath[short_fpath.rfind('\\') + 1
                                    : short_fpath.rfind('.txt')])
            except (ValueError, IndexError):
                continue
            if not (min_n <= n <= max_n):
                continue
            path, best_cost = [], float('inf')
            try:
                did_add = False
                with open(fpath, "r") as f:
                    for line in f:
                        m = pattern.search(line)
                        if m:
                            cost = float(m.group(2))
                            if cost < best_cost:
                                best_cost = cost
                                did_add = True
                                path = []
                            else:
                                did_add = False
                        elif did_add:
                            m = path_pattern.match(line.strip())
                            if m:
                                path.append(int(m.group(1)))
                if best_cost < results[n][0]:
                    results[n] = (best_cost, path if path else None)
            except Exception as e:
                print(f"Error reading {fpath}: {e}")

    return results

def plot_tour(points, path, n, cost, mode, output_dir, algo, prob_name):
    """Plot a single tour and save to file."""
    fig, ax = plt.subplots(figsize=(10, 10))
    if not points or not path:
        return
    plt.plot(
        [ points[vertex][0] for vertex in path ],
        [ points[vertex][1] for vertex in path ],
        marker='o', linestyle='-', color='b', alpha=0.4
    )
    title = f'{algo} - {prob_name} - {mode.upper()} - n={n}, cost={cost:.2f}'
    ax.set_title(title)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    filename = f'{mode}_{n:03d}.png'
    filepath = os.path.join(output_dir, filename)
    plt.savefig(filepath, dpi=150, bbox_inches='tight')
    plt.close()


if __name__ == "__main__":
    main()
