"""
    python extract_results.py <results_subdir> [-asym]
    e.g:
    python extract_results.py bellman_held_karp/br17 -asym
    python extract_results.py bellman_held_karp/29
"""

import os
import sys
import re
from collections import defaultdict


def main():
    res_dir = sys.argv[1] if len(sys.argv) > 1 else 'bellman_held_karp/263'
    is_symmetrical = '-asym' not in sys.argv[2] if len(sys.argv) > 2 else True
    cwd = os.path.realpath(os.path.dirname(__file__))
    results_dir = os.path.join(cwd, '..', 'results', res_dir)
    solutions, max_n = find_bellman_results(results_dir)
    print_paths(is_symmetrical, solutions, max_n)
    print_costs(solutions, max_n)


def print_paths(is_symmetrical, solutions, n):
    for mode in ['shp', 'tsp']:
        print(f"'{mode}': [")
        tab = ' ' * 4
        for i in range(0, n + 1):
            path = solutions[mode].get(i, [None, None])[1]
            if path is None:
                print(f"{tab}None,")
            else:
                print(f'{tab}[')
                print(f"{tab}{tab}[ {', '.join(map(str, path))} ],")
                if is_symmetrical:
                    path_rev = list(reversed(path))
                    print(f"{tab}{tab}[ {', '.join(map(str, path_rev))} ],")
                print(f'{tab}],')
        print(f'],')

def print_costs(solutions, n):
    for mode in ['shp', 'tsp']:
        print(f"'{mode}': [")
        tab = ' ' * 4
        for i in range(0, n + 1):
            cost = solutions[mode].get(i, [None, None])[0]
            if cost is None:
                print(f"{tab}None,")
            else:
                print(f'{tab}{cost},')
        print(f'],')

def find_bellman_results(results_dir):
    results: dict[str, dict[tuple[float, list[int]]]] = defaultdict(dict)
    pattern = re.compile(r"Optimal total distance for (\d+) points: ([\d\.Ee+-]+)")
    path_pattern = re.compile(r"Point #(\d+)")
    max_n = 0
    for dirpath, _, filenames in os.walk(results_dir):
        if '\\double' not in dirpath:
            continue
        for fname in filenames:
            fpath = os.path.join(dirpath, fname)
            mode = 'shp' if ('shp\\' in fpath) else 'tsp'
            short_fpath = fpath.replace(results_dir, '')
            n = int(short_fpath[ short_fpath.rfind('\\') + 1
                              : short_fpath.rfind('.txt')])
            max_n = max(max_n, n)
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
                for line in lines:
                    m = path_pattern.match(line.strip())
                    if m:
                        path.append(int(m.group(1)))
                results[mode][n] = (value, path)
            except Exception as e:
                print(f"Could not read {fpath}: {e}")
                results[mode][n] = (value, path)
    return results, max_n



if __name__ == "__main__":
    main()
