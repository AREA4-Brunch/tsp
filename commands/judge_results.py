import os
import re
import sys
import math


def main():
    args = sys.argv[1:]
    res_dir = args[0] if len(args) > 0 else 'bellman_held_karp'
    prob_name = args[1] if len(args) > 1 else '263'
    prob_file = args[2] if len(args) > 2 else f'{prob_name}.txt'
    num_points = int(args[3]) if len(args) > 3 else 263
    cwd = os.path.realpath(os.path.dirname(__file__))
    results_dir = os.path.join(cwd, '..', 'results', res_dir, prob_name)
    results = find_bellman_results(results_dir, num_points)
    weights = load_weights_from_file(
        os.path.join(cwd, '..', 'problems', prob_file),
        num_points,
    )
    compare_solutions(results, *get_solutions(prob_name), weights, fpath_filter=None)
    # find_small_perm_sols(weights)


def find_bellman_results(results_dir, max_n):
    results = []
    pattern = re.compile(r"Optimal total distance for (\d+) points: ([\d\.Ee+-]+)")
    path_pattern = re.compile(r"Point #(\d+)")
    for dirpath, _, filenames in os.walk(results_dir):
        for fname in filenames:
            fpath = os.path.join(dirpath, fname)
            short_fpath = fpath.replace(results_dir, '')
            n = int(short_fpath[ short_fpath.rfind('\\') + 1
                              : short_fpath.rfind('.txt')])
            if n > max_n:
                continue
            path, value = [], None
            try:
                did_add = False
                lines = []
                with open(fpath, "r") as f:
                    for line in f:
                        m = pattern.search(line)
                        if not did_add and m:
                            value = float(m.group(2))
                            did_add = True
                        elif did_add:
                            lines.append(line)
                        m2 = path_pattern.match(line.strip())
                        if m2:
                            path.append(int(m2.group(1)))
                        if line.strip().startswith('Execution time:'):
                            break
                results.append((fpath, short_fpath, n, value, path))
            except Exception as e:
                print(f"Could not read {fpath}: {e}")
                results.append((fpath, short_fpath, n, value, path))
    # sort by problem mode and num points:
    results = sorted(results, key=lambda x: (x[1][:x[1].rfind('\\')], x[2]))
    return results

def compare_solutions(results, solutions_costs,
                      solutions_paths, weights, fpath_filter=None):
    num_unknown, num_ok, total = 0, 0, 0
    for fpath, short_fpath, n, value, actual_path in results:
        if fpath_filter and (fpath_filter not in short_fpath):
            continue
        total += 1
        mode = 'shp' if ('shp\\' in fpath) else 'tsp'
        if isinstance(solutions_costs[mode], list):
            if len(solutions_costs[mode]) <= n:
                expected = None
            else:
                expected = solutions_costs[mode][n]
            if len(solutions_paths[mode]) <= n:
                expected_paths = []
            else:
                expected_paths = solutions_paths[mode][n]
        else:
            expected = solutions_costs[mode].get(n, None)
            expected_paths = solutions_paths[mode].get(n, [])
        status, ok = check_solution(value, expected, actual_path,
                                    expected_paths, mode, weights)
        if expected is None:
            print(f"{short_fpath}: No expected value for {n} points")
            num_unknown += 1
        else:
            if ok:
                num_ok += 1
            print(f"{short_fpath}: {n:2d} points: {value} [{status}]")
    wrong = total - num_ok - num_unknown
    print(f'CORRECT: {num_ok}/{total}, WRONG: {wrong}, UNKNOWN: {num_unknown}')

def check_solution(found_cost, expected, actual_path,
                   expected_path_variants, mode, weights):
    if expected is None:
        return "NO_EXPECTED", False
    do_match = found_cost is not None and abs(found_cost - expected) < 1e-6
    if do_match:
        return "OK", True
    if found_cost is None or actual_path is None:
        return "NO_PATH", False
    if expected_path_variants and set(actual_path) != set(expected_path_variants[0]):
        print(f'\nGot: {set(actual_path)}')
        print(f'Expected: {set(expected_path_variants[0])}')
        return "DIFF_PATH_NODES", False
    path_match = False
    if expected_path_variants and actual_path:
        if mode == 'shp':
            for ep in expected_path_variants:
                if actual_path == ep:
                    path_match = True
                    break
        elif mode == 'tsp':
            for ep in expected_path_variants:
                ep = ep[:-1]
                ep = ep[ep.index(0) : ] + ep[ : ep.index(0)]
                ap = actual_path[:-1]
                ap = ap[ap.index(0) : ] + ap[ : ap.index(0)]
                if ap == ep:
                    path_match = True
                    break
    if path_match:
        return "OK", True

    actual_cost = 0
    for i in range(1, len(actual_path)):
        src = actual_path[i - 1]
        dst = actual_path[i]
        actual_cost += weights[src][dst]

    if abs(actual_cost - found_cost) > 1e-6:
        print(f'WARNING: Computed cost does not match found cost: '
              f'{found_cost} vs {actual_cost} (computed)')

    if expected_path_variants:
        actual_expected_cost = 0
        for i in range(1, len(expected_path_variants[0])):
            src = expected_path_variants[0][i - 1]
            dst = expected_path_variants[0][i]
            actual_expected_cost += weights[src][dst]

        if abs(actual_expected_cost - expected) > 1e-6:
            print(f'WARNING: Expected solution does not have expected cost.')
            print(f'expected cost: {expected} vs {actual_expected_cost}'
                f' (actual exp) vs {actual_cost} (found cost)')
            if actual_cost <= actual_expected_cost:
                print(f'But actual cost {actual_cost} is the solution.')

    if actual_cost <= expected:
        return f"UNEXPECTED_BUT_BETTER ({actual_cost} vs {expected})", True

    n = max(actual_path) + 1
    print(f'n={n}: Expected paths variants:\n'
          f'{'\n'.join([ str(p) for p in expected_path_variants ])}')
    print(f'Found path:\n{actual_path}')
    return f"DIFF ({actual_cost:.6f} vs {expected:.6f})", False

def load_weights_from_file(filepath, num_points):
    def parse_points(lines):
        points = []
        for line in lines:
            parts = line.strip().split()
            if len(parts) == 2:
                try:
                    x, y = float(parts[0]), float(parts[1])
                    points.append((x, y))
                except ValueError:
                    continue
            if len(points) == num_points:
                break
        return points

    def euclidean(p1, p2):
        return math.hypot(p1[0] - p2[0], p1[1] - p2[1])

    with open(filepath, "r") as f:
        lines = f.readlines()

    edge_weight_format = "FULL_MATRIX"
    weights_section = None
    dimension = None
    for idx, line in enumerate(lines):
        if "DIMENSION:" in line:
            dimension = int(line.split(':')[1].strip())
        if "EDGE_WEIGHT_FORMAT" in line:
            if "UPPER_ROW" in line:
                edge_weight_format = "UPPER_ROW"
            elif "LOWER_ROW" in line:
                edge_weight_format = "LOWER_ROW"
        if "EDGE_WEIGHT_SECTION" in line:
            weights_section = idx + 1
            break

    if weights_section is not None:
        distances = [
            [ 0.0 for _ in range(dimension) ]
            for _ in range(dimension)
        ]
        all_weights = []
        for line in lines[weights_section:]:
            for part in line.strip().split():
                try:
                    all_weights.append(float(part))
                except ValueError:
                    continue
        weights_iter = iter(all_weights)
        lines = lines[weights_section : ]
        for i in range(dimension):
            j_start, j_end = 0, dimension
            if edge_weight_format == "UPPER_ROW":
                j_start = i + 1
            elif edge_weight_format == "LOWER_ROW":
                j_end = i
            for j in range(j_start, j_end):
                value = next(weights_iter)
                distances[i][j] = value
                if edge_weight_format != "FULL_MATRIX":
                    distances[j][i] = value
        return [ d[:num_points] for d in distances[:num_points] ]

    distances = [
        [ 0.0 for _ in range(num_points) ]
        for _ in range(num_points)
    ]
    points = parse_points(lines)
    if len(points) < num_points:
        raise ValueError("Not enough points in file")
    for i in range(num_points):
        for j in range(num_points):
            d = euclidean(points[i], points[j])
            distances[i][j] = d
    return distances

def get_solutions(prob_name):
    if prob_name == '263':
        return get_solutions_costs_263(), get_solutions_paths_263()
    if prob_name == 'br17':
        return get_solutions_costs_br17(), get_solutions_paths_br17()
    if prob_name == 'bayg29':
        return get_solutions_costs_bayg29(), get_solutions_paths_bayg29()
    raise ValueError(f'Unsupported problem name: {prob_name}')

def get_solutions_costs_263():
    return {
        'shp': [
            None,
            0.0,
            5.08,
            247.619678,
            247.662769,
            247.682243,
            247.731099,
            253.022736,
            255.434987,
            257.877602,
            260.350624,
            262.854082,
            265.387991,
            267.952354,
            270.54716,
            273.172386,
            275.827996,
            278.513939,
            281.230153,
            283.976562,
            286.75308,
            289.559605,
            312.056873,
            312.276011,
            312.526421,
            312.814854,
            313.149981,
            313.543046,
            314.008745,
            314.566388,
            315.24136,
            316.06678,
            317.084946,
            318.347583,
        ],
        'tsp': [
            None,
            0.0,
            10.16,
            495.219133,
            495.260441,
            495.279085,
            495.325794,
            496.283754,
            498.696005,
            501.13862,
            503.611642,
            506.1151,
            508.649009,
            511.213372,
            513.808178,
            516.433405,
            519.089014,
            521.774957,
            524.491171,
            527.23758,
            530.014098,
            532.81504,
            534.366066,
            536.768442,
            539.203521,
            541.671355,
            544.171981,
            546.705418,
            549.271669,
            551.870722,
            554.502546,
            None,
            None,
            None,
        ],
    }

def get_solutions_paths_263():
    solutions = {
        'shp': [
            None,
            [
                [ 0 ],
                [ 0 ],
            ],
            [
                [ 0, 1 ],
                [ 1, 0 ],
            ],
            [
                [ 1, 0, 2 ],
                [ 2, 0, 1 ],
            ],
            [
                [ 1, 0, 3, 2 ],
                [ 2, 3, 0, 1 ],
            ],
            [
                [ 1, 0, 4, 3, 2 ],
                [ 2, 3, 4, 0, 1 ],
            ],
            [
                [ 2, 3, 4, 5, 0, 1 ],
                [ 1, 0, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 0, 1 ],
                [ 1, 0, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 0, 1 ],
                [ 1, 0, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 0, 1 ],
                [ 1, 0, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 0, 1 ],
                [ 1, 0, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 1 ],
                [ 1, 0, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 1 ],
                [ 1, 0, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 0, 1 ],
                [ 1, 0, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0, 1 ],
                [ 1, 0, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 0, 1 ],
                [ 1, 0, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1 ],
                [ 1, 0, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0, 1 ],
                [ 1, 0, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 1 ],
                [ 1, 0, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 0, 1 ],
                [ 1, 0, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 0, 1 ],
                [ 1, 0, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 0, 1 ],
                [ 1, 0, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 0, 1 ],
                [ 1, 0, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 21, 0, 1 ],
                [ 1, 0, 21, 22, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 23, 22, 21, 0, 1 ],
                [ 1, 0, 21, 22, 23, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 24, 23, 22, 21, 0, 1 ],
                [ 1, 0, 21, 22, 23, 24, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 25, 24, 23, 22, 21, 0, 1 ],
                [ 1, 0, 21, 22, 23, 24, 25, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 26, 25, 24, 23, 22, 21, 0, 1 ],
                [ 1, 0, 21, 22, 23, 24, 25, 26, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 27, 26, 25, 24, 23, 22, 21, 0, 1 ],
                [ 1, 0, 21, 22, 23, 24, 25, 26, 27, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 28, 27, 26, 25, 24, 23, 22, 21, 0, 1 ],
                [ 1, 0, 21, 22, 23, 24, 25, 26, 27, 28, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 ],
            ],
            [
                [ 2, 1, 22, 23, 24, 25, 26, 27, 28, 29, 30, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 22, 23, 24, 25, 26, 27, 28, 29, 30, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 22, 23, 24, 25, 26, 27, 28, 29, 30, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 22, 23, 24, 25, 26, 27, 28, 29, 30, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 22, 23, 24, 25, 26, 27, 28, 29, 30, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 22, 23, 24, 25, 26, 27, 28, 29, 30, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3 ],
                [ 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 30, 29, 28, 27, 26, 25, 24, 23, 22, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 30, 29, 28, 27, 26, 25, 24, 23, 22, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 30, 29, 28, 27, 26, 25, 24, 23, 22, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 30, 29, 28, 27, 26, 25, 24, 23, 22, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 30, 29, 28, 27, 26, 25, 24, 23, 22, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 30, 29, 28, 27, 26, 25, 24, 23, 22, 1, 2 ],
            ],
            [
                [ 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 1, 2 ],
                [ 2, 1, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3 ],
            ],
            [
                [ 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 1, 2 ],
                [ 2, 1, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3 ],
            ],
            [
                [ 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 1, 2 ],
                [ 2, 1, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3 ],
            ],
        ],
        'tsp': [
            None,
            [
                [ 0, 0 ],
                [ 0, 0 ],
            ],
            [
                [ 0, 1, 0 ],
                [ 0, 1, 0 ],
            ],
            [
                [ 2, 0, 1, 2 ],
                [ 2, 1, 0, 2 ],
            ],
            [
                [ 3, 1, 0, 2, 3 ],
                [ 3, 2, 0, 1, 3 ],
            ],
            [
                [ 4, 1, 0, 2, 3, 4 ],
                [ 4, 3, 2, 0, 1, 4 ],
            ],
            [
                [ 5, 1, 0, 2, 3, 4, 5 ],
                [ 5, 4, 3, 2, 0, 1, 5 ],
            ],
            [
                [ 6, 2, 3, 4, 5, 1, 0, 6 ],
                [ 6, 0, 1, 5, 4, 3, 2, 6 ],
            ],
            [
                [ 7, 0, 1, 5, 4, 3, 2, 6, 7 ],
                [ 7, 6, 2, 3, 4, 5, 1, 0, 7 ],
            ],
            [
                [ 8, 0, 1, 5, 4, 3, 2, 6, 7, 8 ],
                [ 8, 7, 6, 2, 3, 4, 5, 1, 0, 8 ],
            ],
            [
                [ 9, 0, 1, 5, 4, 3, 2, 6, 7, 8, 9 ],
                [ 9, 8, 7, 6, 2, 3, 4, 5, 1, 0, 9 ],
            ],
            [
                [ 10, 0, 1, 5, 4, 3, 2, 6, 7, 8, 9, 10 ],
                [ 10, 9, 8, 7, 6, 2, 3, 4, 5, 1, 0, 10 ],
            ],
            [
                [ 11, 0, 1, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11 ],
                [ 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 1, 0, 11 ],
            ],
            [
                [ 12, 0, 1, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12 ],
                [ 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 1, 0, 12 ],
            ],
            [
                [ 13, 0, 1, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13 ],
                [ 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 1, 0, 13 ],
            ],
            [
                [ 14, 0, 1, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14 ],
                [ 14, 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 1, 0, 14 ],
            ],
            [
                [ 15, 0, 1, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 ],
                [ 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 1, 0, 15 ],
            ],
            [
                [ 16, 0, 1, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 ],
                [ 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 1, 0, 16 ],
            ],
            [
                [ 17, 0, 1, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 ],
                [ 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 1, 0, 17 ],
            ],
            [
                [ 18, 0, 1, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 ],
                [ 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 1, 0, 18 ],
            ],
            [
                [ 19, 0, 1, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 ],
                [ 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 1, 0, 19 ],
            ],
            [
                [ 20, 1, 0, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 ],
                [ 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 0, 1, 20 ],
            ],
            [
                [ 21, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 1, 0, 21 ],
                [ 21, 0, 1, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 21 ],
            ],
            [
                [ 22, 0, 1, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 21, 22 ],
                [ 22, 21, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 1, 0, 22 ],
            ],
            [
                [ 23, 0, 1, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 21, 22, 23 ],
                [ 23, 22, 21, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 1, 0, 23 ],
            ],
            [
                [ 24, 0, 1, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 21, 22, 23, 24 ],
                [ 24, 23, 22, 21, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 1, 0, 24 ],
            ],
            [
                [ 25, 0, 1, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 21, 22, 23, 24, 25 ],
                [ 25, 24, 23, 22, 21, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 1, 0, 25 ],
            ],
            [
                [ 26, 0, 1, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 21, 22, 23, 24, 25, 26 ],
                [ 26, 25, 24, 23, 22, 21, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 1, 0, 26 ],
            ],
            [
                [ 27, 0, 1, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 21, 22, 23, 24, 25, 26, 27 ],
                [ 27, 26, 25, 24, 23, 22, 21, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 1, 0, 27 ],
            ],
            [
                [ 28, 0, 1, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 21, 22, 23, 24, 25, 26, 27, 28 ],
                [ 28, 27, 26, 25, 24, 23, 22, 21, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 1, 0, 28 ],
            ],
            [
                [ 29, 0, 1, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 2, 3, 4, 5, 21, 22, 23, 24, 25, 26, 27, 28, 29 ],
                [ 29, 28, 27, 26, 25, 24, 23, 22, 21, 5, 4, 3, 2, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 1, 0, 29 ],
            ],
            None,
            None,
            None,
        ],
    }
    return solutions

def get_solutions_costs_br17():
    return {
        'shp': [
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
        ],
        'tsp': [
            None,
            9999.0,
            6.0,
            11.0,
            104.0,
            104.0,
            70.0,
            36.0,
            39.0,
            39.0,
            39.0,
            39.0,
            39.0,
            39.0,
            39.0,
            39.0,
            39.0,
            39.0,
        ],
    }

def get_solutions_paths_br17():
    solutions = {
        'shp': [
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
            None,
        ],
        'tsp': [
            None,
            [
                [ 0, 0 ],
            ],
            [
                [ 0, 1, 0 ],
            ],
            [
                [ 2, 0, 1, 2 ],
            ],
            [
                [ 3, 0, 2, 1, 3 ],
            ],
            [
                [ 4, 1, 2, 0, 3, 4 ],
            ],
            [
                [ 5, 1, 2, 0, 3, 4, 5 ],
            ],
            [
                [ 6, 1, 2, 0, 5, 3, 4, 6 ],
            ],
            [
                [ 7, 1, 2, 0, 6, 5, 3, 4, 7 ],
            ],
            [
                [ 8, 4, 3, 6, 5, 1, 2, 0, 7, 8 ],
            ],
            [
                [ 9, 1, 2, 0, 6, 5, 4, 3, 7, 8, 9 ],
            ],
            [
                [ 10, 8, 7, 4, 3, 6, 5, 0, 2, 1, 9, 10 ],
            ],
            [
                [ 11, 0, 2, 10, 9, 1, 6, 5, 4, 3, 7, 8, 11 ],
            ],
            [
                [ 12, 9, 8, 7, 4, 3, 6, 5, 11, 0, 2, 1, 10, 12 ],
            ],
            [
                [ 13, 2, 11, 0, 8, 7, 4, 3, 6, 5, 10, 9, 1, 12, 13 ],
            ],
            [
                [ 14, 6, 5, 4, 3, 8, 7, 11, 0, 13, 2, 10, 9, 1, 12, 14 ],
            ],
            [
                [ 15, 6, 5, 4, 3, 8, 7, 12, 10, 9, 1, 13, 2, 11, 0, 14, 15 ],
            ],
            [
                [ 16, 8, 7, 4, 3, 15, 14, 6, 5, 11, 0, 13, 2, 10, 9, 1, 12, 16 ],
            ],
        ],
    }
    return solutions

def get_solutions_costs_bayg29():
    return {
        'shp': {
            29: None,
        },
        'tsp': {
            29: 1610
        }
    }

def get_solutions_paths_bayg29():
    solutions = {
        'shp': {
            29: None
        },
        'tsp': {
            29: [
                [ 28, 4, 20, 1, 19, 9, 3, 14, 17, 13, 16, 21, 10, 18, 24, 6, 22, 7, 26, 15, 12, 23, 0, 27, 5, 11, 8, 25, 2, 28, ]
            ],
        }
    }
    return solutions

def find_small_perm_sols(weights):
    import itertools
    n = len(weights)
    max_n = min(n, 10)
    for num_nodes in range(max_n + 1):
        nodes = list(range(num_nodes))
        min_cost = float('inf')
        best_path = None
        for perm in itertools.permutations(nodes[1:]):
            path = [0] + list(perm) + [0]
            cost = 0
            for i in range(1, len(path)):
                cost += weights[path[i - 1]][path[i]]
            if cost < min_cost:
                min_cost = cost
                best_path = path
        print(f"Cost={min_cost:.6f}, path={best_path}")


if __name__ == "__main__":
    main()
