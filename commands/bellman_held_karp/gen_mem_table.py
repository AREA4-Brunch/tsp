import math
import sys


def main():
    cost_dtypes = ['uint8_t', 'uint16_t', 'uint32_t',
                   'uint64_t', 'float', 'double']
    vertex_type_size = 1  # in bytes, always int8_t

    # Check if markdown flag is provided
    markdown = '--markdown' in sys.argv or '-md' in sys.argv
    is_symmetric = '--asymmetric' not in sys.argv and '-asym' not in sys.argv
    no_path = '--no-path' in sys.argv or '-np' in sys.argv
    sym_pre_txt = 'S' if is_symmetric else 'A'
    symetry_text = "Symmetric" if is_symmetric else "Asymmetric"
    no_path_text = " - No path only cost" if no_path else ""
    print_strategy = print_markdown_table if markdown else print_table

    print_strategy(
        f"{sym_pre_txt}TSP ({symetry_text} Traveling Salesman Problem){no_path_text}",
        cost_dtypes, vertex_type_size, True, is_symmetric, no_path)
    print("\n")
    print_strategy(
        f"{sym_pre_txt}SHP ({symetry_text} Shortest Hamiltonian Path){no_path_text}",
        cost_dtypes, vertex_type_size, False, is_symmetric, no_path)


def print_table(title, cost_dtypes, vertex_t_size,
                cycle, is_sym, no_path: bool):
    """ Print a formatted table for memory requirements. """
    print(title)
    print("=" * 128)
    header = 'n'.ljust(6) \
           + ''.join([f'{dtype:>20}' for dtype in cost_dtypes])
    print(header)
    print('-' * 128)

    for n in range(10, 40):
        row_data = [str(n).ljust(6)]
        for cost_dtype in cost_dtypes:
            tc = dtype_size(cost_dtype)
            mem_space = space(n, cycle, is_sym, tc, vertex_t_size, no_path)
            row_data.append(f'{format_bytes(mem_space):>20}')

        print(''.join(row_data))


def print_markdown_table(title, cost_dtypes, vertex_t_size,
                         cycle, is_symmetric, no_path: bool):
    """ Print a Markdown table for memory requirements. """
    print(f"## {title}\n" if cycle else f"## {title}\n")
    header = "| n  | " + " | ".join(cost_dtypes) + " |"
    print(header)
    separator = "|----|" + "|".join(
        ["-"*9 for _ in cost_dtypes]) + "|"
    print(separator)
    for n in range(10, 40):
        row_data = [f"| {n:2d} |"]
        for cost_dtype in cost_dtypes:
            tc = dtype_size(cost_dtype)
            mem_space = space(n, cycle, is_symmetric, tc, vertex_t_size, no_path)
            row_data.append(f" {format_bytes(mem_space)} |")
        print(''.join(row_data))


def format_bytes(bytes_val):
    """ Format bytes to appropriate unit (GB, MB, KB, or B). """
    gb = bytes_val / 2**30
    if gb >= 1:
        return f'{gb:.3f} GB'
    mb = bytes_val / 2**20
    if mb >= 1:
        return f'{mb:.3f} MB'
    kb = bytes_val / 2**10
    if kb >= 1:
        return f'{kb:.3f} KB'
    return f'{bytes_val:.3f} B'


def space(n, cycle: bool, is_symmetric: bool, tc=2, tv=1, no_path=False):
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
    if no_path:
        return cost * tc
    max_k = n // 2 if is_symmetric else n - 1
    prev = sum([ math.comb(n, k - 1) * (n - (k - 1))
                 for k in range(3, max_k) ])
    return cost * tc + prev * tv


def dtype_size(dtype: str) -> int:
    size_map = {
        'uint8_t': 1,
        'uint16_t': 2,
        'uint32_t': 4,
        'uint64_t': 8,
        'float': 4,
        'double': 8
    }
    if dtype not in size_map:
        raise ValueError(f'Unsupported dtype: {dtype}')
    return size_map[dtype]


if __name__ == "__main__":
    main()