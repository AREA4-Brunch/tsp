"""
[run_idx] can be str: 'best' or int: idx or -1 or not provided for random run.

python animate_search.py [algo] [problem_name] [problem_file] [mode] [run_idx] [marker_size] [edge_type] [num_points]
python animate_search.py 3_opt_rand 263 263.txt shp 263 -1 5 red
python animate_search.py 3_opt_classical 263 263.txt shp 263 best 5 red
"""


import pandas as pd
import matplotlib.pyplot as plt
import os
import random
import numpy as np
import sys
from analize_results import parse_execution_file


ITERS_TO_EXAMINE = None  # <=> means plot all iterations
ITERS_TO_EXAMINE = [1, 200, 500, -201, -1]  # 1-indexed


def main():
    args = sys.argv[1:]
    algo = args[0] if len(args) > 0 else '3_opt_rand'
    prob_name = args[1] if len(args) > 1 else '263'
    prob_file = args[2] if len(args) > 2 else f'{prob_name}.txt'
    mode = args[3] if len(args) > 3 else 'tsp'  # 'tsp' or 'shp'
    num_points = int(args[4]) if len(args) > 4 else float('inf')
    run_idx = args[5] if len(args) > 5 else -1  # -1 means random/last run
    marker_size = int(args[6]) if len(args) > 6 else None
    edge_type = args[7] if len(args) > 7 else 'red'

    path_cwd = os.path.dirname(os.path.realpath(__file__))
    run_idx, run = find_and_load_run_data(
        path_cwd, algo, prob_name, mode, num_points, run_idx
    )

    path_problem = os.path.join(path_cwd, f'../../problems/{prob_file}')
    print(f"Loading points from: {path_problem}")
    points = load_points(path_problem, num_points)

    path_animations_dir = os.path.join(
        path_cwd,
        f'../../analysis/{algo}/{prob_name}/{mode}/animations/run_{run_idx}/',
    )
    config = {
        'algo': algo,
        'marker_size': marker_size,
        'edge_type': edge_type,
        'iters_to_examine': ITERS_TO_EXAMINE,
        'animations_dir': path_animations_dir,
        'mode': mode,
        'run_idx': run_idx,
    }

    process_run(run, config, points)
    print(f'\nDone processing run #{run_idx}')

def load_points(filepath, num_points=float('inf')):
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
            raise Exception(f'Problem file does not contain points coo.')
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

    return np.array(points)

def process_run(run, config, points):
    """Process a single run and generate visualizations."""
    animations_dir = config['animations_dir']
    os.makedirs(animations_dir, exist_ok=True)
    print(f'\nStoring processed run animation to:\n{animations_dir}\n')

    iters_to_examine = config['iters_to_examine']
    use_combined_plot = iters_to_examine is not None
    if use_combined_plot:
        fig, axes = plt.subplots((2 * len(iters_to_examine) + 1) // 2, 2, figsize=(16, 16))
        axes = axes.flatten()
        iter_examine = 0

    prev_iter, inner_iter_idx = -1, -1
    num_iters = len(run)
    for row_id, path_desc in run.iterrows():
        path_desc = path_desc.values
        for idx in range(2):  # before and after cut
            i, j, k, iter_val = path_desc[-4], path_desc[-3], path_desc[-2], path_desc[-1]
            if i == -1:
                if idx == 1: continue;
                path = list(path_desc[:-4])  # include dummy node
            elif idx == 1:
                path = list(path_desc[:-4])  # include dummy node

            if config['algo'] == '3_opt_funky':
                if prev_iter != iter_val:
                    prev_iter = iter_val
                    inner_iter_idx = 1
                elif idx == 0:
                    inner_iter_idx += 1
                inner_idx = f'.{inner_iter_idx}'
            else:
                inner_idx = ''

            x, y = points[:, 0], points[:, 1]
            dummy_pt_idx = path.index(len(path) - 1)

            if i != -1:  # not init iter
                if idx == 0:
                    v1, v2, v3 = path[i], path[j], path[k]
                    v4, v5, v6 = (
                        path[i + 1],
                        path[j + 1],
                        path[(k + 1) % len(path)],
                    )
                else:
                    indices = []
                    for v in [v1, v2, v3, v4, v5, v6]:
                        indices.append(path.index(v))
                    indices = sorted(indices)
                    v1 = path[indices[1]]
                    v4 = path[indices[2]]
                    v2 = path[indices[3]]
                    v5 = path[indices[4]]
                    v3 = path[indices[5]]
                    v6 = path[indices[0]]

                if i + 1 == j or j + 1 == k or (k + 1) % len(path) == i:
                    print(f'Examining points: {(v1, v2, v3)}')
                    print(f'Examining points i, j, k: {(i, j, k)}')
                    try:
                        print(f'positions: {"".join([
                            f"\n{(x[v], y[v])}" for v in [v1, v4, v2, v5, v3, v6]])}')
                    except IndexError as e:
                        print(f'positions not logged, one vi = {len(path) - 1}')

            path_no_dummy = path[1 + dummy_pt_idx:] + path[:dummy_pt_idx]
            should_plot = (
                iters_to_examine is None
                or iter_val in iters_to_examine
                or (iter_val - num_iters) in iters_to_examine
            )

            if should_plot:
                if use_combined_plot:
                    ax = axes[iter_examine]
                    iter_examine += 1
                else:
                    fig, ax = plt.subplots(figsize=(8, 8))

                ax.plot(
                    [x[vertex] for vertex in path_no_dummy],
                    [y[vertex] for vertex in path_no_dummy],
                    marker='o', linestyle='-', color='b', alpha=0.4
                )

                if i != -1 or idx == 1:
                    def plot_single_point(v, color='k', marker='o', label=None):
                        if v != len(path_no_dummy):
                            ax.plot(
                                [x[v]], [y[v]],
                                marker=marker, linestyle='None', color=color, alpha=0.9, label=label,
                                markersize=config['marker_size']
                            )
                        # else v is dummy point

                    if config['edge_type'] == 'red':
                        edge_colors = ['r'] * 3
                        point_colors = ['r'] * 3
                        point_markers = ['o', 'o', 'o']
                    else:  # yellow
                        edge_colors = ["#EBA206", '#EBA206', '#EBA206']
                        point_colors = ['#EBA206', '#EBA206', '#EBA206']
                        point_markers = ['s', 's', 's']

                    if v1 != len(path_no_dummy) and v4 != len(path_no_dummy):
                        ax.plot(
                            [x[v1], x[v4]], [y[v1], y[v4]],
                            marker=point_markers[0], linestyle='-', color=edge_colors[0], alpha=0.9, label='cut 1',
                            markersize=config['marker_size']
                        )
                    else:
                        print(f'Warning: dummy point is v1 or v4')
                        plot_single_point(v1, color=edge_colors[0], marker=point_markers[0])
                        plot_single_point(v4, color=edge_colors[0], marker=point_markers[0])

                    if v2 != len(path_no_dummy) and v5 != len(path_no_dummy):
                        ax.plot(
                            [x[v2], x[v5]], [y[v2], y[v5]],
                            marker=point_markers[1], linestyle='-', color=edge_colors[1], alpha=0.9, label='cut 2',
                            markersize=config['marker_size']
                        )
                    else:
                        print(f'Warning: dummy point is v2 or v5')
                        plot_single_point(v2, color=edge_colors[1], marker=point_markers[1])
                        plot_single_point(v5, color=edge_colors[1], marker=point_markers[1])

                    if v3 != len(path_no_dummy) and v6 != len(path_no_dummy):
                        ax.plot(
                            [x[v3], x[v6]], [y[v3], y[v6]],
                            marker=point_markers[2], linestyle='-', color=edge_colors[2], alpha=0.9, label='cut 3',
                            markersize=config['marker_size']
                        )
                    else:
                        print(f'Warning: dummy point is v3 or v6')
                        plot_single_point(v3, color=edge_colors[2], marker=point_markers[2])
                        plot_single_point(v6, color=edge_colors[2], marker=point_markers[2])

                    plot_single_point(v1, color=point_colors[0], marker=point_markers[0], label='v1')
                    plot_single_point(v2, color=point_colors[1], marker=point_markers[1], label='v2')
                    plot_single_point(v3, color=point_colors[2], marker=point_markers[2], label='v3')

                if i == -1:
                    change_txt = ''
                elif idx == 0:
                    change_txt = ' before cut'
                else:
                    change_txt = ' after cut'

                ax.set_xlabel('x')
                ax.set_ylabel('y')
                ax.set_title(f'Path of run {config['run_idx']} at iter {iter_val}{change_txt}')

                if not use_combined_plot:
                    plt.tight_layout()
                    fname = os.path.join(animations_dir, f'{iter_val}{inner_idx}.{idx}.png')
                    plt.savefig(fname)
                    plt.close(fig)

    if use_combined_plot:
        plt.tight_layout()
        fname = os.path.join(animations_dir, f'selected_iters.png')
        plt.savefig(fname)
        plt.close(fig)


def find_and_load_run_data(path_cwd, algo, prob_name, mode, num_points, run_idx):
    """
    Find and load run data from appropriate csv file based on run_idx.
    Returns (run_idx, runs, path_file_paths) where run_idx may be updated if it was -1.
    """
    if run_idx == 'best':
        result_fpath = os.path.join(
            path_cwd,
            f'../../results/{algo}/{prob_name}/{mode}/{num_points}.txt'
        )
        exec_desc = parse_execution_file(result_fpath)
        run_idx = exec_desc['best_run']
    else:
        run_idx = int(run_idx)

    results_dir = os.path.join(
        path_cwd,
        f'../../results/{algo}/histories/{prob_name}/{mode}/{num_points}'
    )
    # find all directories matching pattern runs_X_Y
    run_ranges = []
    try:
        for item in os.listdir(results_dir):
            if item.startswith('runs_'):
                csv_file = os.path.join(results_dir, item, '0.paths.csv')
                if os.path.isfile(csv_file):
                    start, end = map(int, item.split('_')[1:])
                    run_ranges.append((start, end, csv_file))
    except FileNotFoundError:
        raise FileNotFoundError(f"Results directory not found: {results_dir}")

    if not run_ranges:
        raise FileNotFoundError(f"No csv files found in {results_dir}")

    orig_run_idx = run_idx
    if run_idx == -1:  # choose random run batch
        all_runs = []
        for start, end, _ in run_ranges:
            all_runs.extend(range(start, end + 1))
        run_idx = random.choice(all_runs)

    # find which csv contains this run_idx
    path_file_paths = None
    for start, end, csv_file in run_ranges:
        if start <= run_idx <= end:
            path_file_paths = csv_file
            break

    if path_file_paths is None:
        raise ValueError(f"Run index {run_idx} not found in any available csv")

    print(f"Loading paths from: {path_file_paths}")
    paths_reruns = pd.read_csv(path_file_paths, header=None)
    last_column_index = paths_reruns.columns[-1]
    zero_indices = paths_reruns.index[paths_reruns[last_column_index] == 0].tolist()
    if orig_run_idx == -1:
        run_idx = random.randint(0, len(zero_indices) - 1)
    else:
        run_idx -= 1  # convert to 0-indexed
    if run_idx < len(zero_indices) - 1:
        print(f' run idx: {run_idx}, zero indices: {zero_indices}')
        run = paths_reruns.iloc[zero_indices[run_idx] : zero_indices[run_idx + 1]]
    else:  # run is last run
        run = paths_reruns.iloc[zero_indices[-1]:]
    run_idx += 1  # convert to 1-indexed
    print(f"Processing run #{run_idx}")
    return run_idx, run


if __name__ == '__main__':
    main()
