import os
import pickle
import matplotlib.pyplot as plt
from collections import defaultdict
import hashlib
import sys
import math


DO_LOGLOG = True
PATH_CWD = os.path.dirname(os.path.realpath(__file__))
PATH_RES = os.path.join(PATH_CWD, f'../../results/bellman_held_karp/263')
PATH_ANALYSIS = os.path.join(PATH_CWD, f'../../analysis/bellman_held_karp/')
PATH_CACHED = os.path.join(PATH_ANALYSIS, 'cached')
PATH_PLOTS = os.path.join(PATH_CWD,  f'../../analysis/bellman_held_karp/')

AXIS_FONT_SIZE = 14


def main():
    # Check if markdown output is requested
    output_markdown = any(arg.lower() in ['--markdown', '-md']
                          for arg in sys.argv)
    
    os.makedirs(PATH_ANALYSIS, exist_ok=True)
    os.makedirs(PATH_CACHED, exist_ok=True)

    filename = shorten_file_name(PATH_CACHED + 'merged')
    path_cached_avg_times = os.path.join(PATH_CACHED, filename)
    path_cached_avg_times = os.path.realpath(path_cached_avg_times)
    if os.path.isfile(path_cached_avg_times):
        with open(path_cached_avg_times, 'rb') as file:
            avg_times_totals = pickle.load(file)
        print(f'\nWARNING: using cached avg times totals'
              f' from:\n{path_cached_avg_times}\n')
    else:
        avg_times_totals = fetch_avg_exec_times(PATH_RES)
        with open(path_cached_avg_times, 'wb') as file:
            pickle.dump(avg_times_totals, file)
        print(f'Cached avg times to: {path_cached_avg_times}')

    do_loglog = DO_LOGLOG
    path_analysis = os.path.join(
        PATH_ANALYSIS,
        f'analysis{"_markdown.md" if output_markdown else ".txt"}')
    path_avg_exec_times_plt = os.path.join(
        PATH_PLOTS,
        f'avg_exec_times{"_loglog" if do_loglog else ""}.png'
    )
    analize(
        avg_times_totals,
        path_analysis,
        path_avg_exec_times_plt,
        do_loglog=False,
        min_number_points=14,
        max_number_points=31,
        output_markdown=output_markdown,
    )
    return


def fetch_avg_exec_times(path_dir):
    # avg_times_totals[(num_points, dtype)] = [ total_exec_dur_ms, cnt ]
    avg_times_totals: dict[tuple[int, str], list[float, int]] = dict()

    for root, dirs, filenames in os.walk(path_dir):
        for filename in filenames:
            file_path = os.path.join(root, filename)
            num_pts = int(filename[ : -len('.txt')])
            cur_dtype = ''
            prob_type = None  # SHP or TSP
            with open(file_path, 'r') as file:
                for idx, line in enumerate(file, start=1):
                    def set_cur_dtype():
                        needle = f'Selected cost_t = '
                        if needle not in line: return False;
                        nonlocal cur_dtype
                        cur_dtype = line.strip()[len(needle) : ] 
                        return True

                    def add_avg_time():
                        needle = f'Execution time: '
                        if needle not in line: return;
                        dur = float(line[len(needle) : -len(' ms')])
                        n = num_pts if prob_type == 'SHP' else num_pts - 1
                        if (n, cur_dtype) not in avg_times_totals:
                            avg_times_totals[(n, cur_dtype)] = [dur, 1]
                        else:
                            entry = avg_times_totals[(n, cur_dtype)]
                            entry[0] += dur
                            entry[1] += 1

                    if prob_type is None:
                        needle = 'Solving '
                        if line.startswith(needle):
                            prob_type = line[len(needle) : ].strip()
                    elif not set_cur_dtype():
                        add_avg_time()

    return avg_times_totals

def analize(
    avg_times_totals,
    save_analysis,
    savefig=None,
    do_loglog=True,
    min_number_points=0,
    max_number_points=float('inf'),
    output_markdown=False,
):
    data_per_dtype = defaultdict(list)
    data_per_num_points = defaultdict(list)
    min_num_pts, max_num_pts = float('inf'), 0
    for (num_points, dtype), descs in avg_times_totals.items():
        if not (min_number_points <= num_points < max_number_points):
            continue
        total_dur, count = descs[0], descs[1]
        avg_time = total_dur / count
        mem = space(num_points, cycle=True, is_symmetric=True,
                    tc=_get_num_bytes(dtype), tv=1, no_path=False) / 2**30
        data_per_dtype[dtype].append((num_points, avg_time, count, mem))
        data_per_num_points[num_points].append((avg_time, dtype, count, mem))
        min_num_pts = min(min_num_pts, num_points)
        max_num_pts = max(max_num_pts, num_points)

    for dtype, descs in data_per_dtype.items():
        data_per_dtype[dtype] = sorted(descs)
    for num_points, descs in data_per_num_points.items():
        data_per_num_points[num_points] = sorted(descs)

    fig, ax = plt.subplots()
    with open(save_analysis, 'w') as file:
        for dtype, descs in data_per_dtype.items():
            avg_times = []
            nums_points = []
            for desc in descs:
                num_points, avg_time, count, mem = desc
                avg_time /= 1000  # [secs]
                avg_times.append(max(avg_time, 1e-10))
                nums_points.append(num_points)
            if do_loglog:
                ax.loglog(nums_points,avg_times, '-o',
                          label=f'dtype={dtype}', alpha=0.4)
            else:
                ax.plot(nums_points, avg_times, '-o',
                        label=f'dtype={dtype}', alpha=0.4)

        orders = [ '(u)int8_t', '(u)int16_t', '(u)int32_t',
                   '(u)int64_t', 'float', 'double'  ]
        order_map = {dtype: i for i, dtype in enumerate(orders)}
        data_per_num_points = dict(sorted(data_per_num_points.items()))
        
        file.write('## Execution Report\n\n')
        for num_points, descs in data_per_num_points.items():
            descs.sort(key=lambda x: order_map.get(x[1], len(orders)))
            data_per_num_points[num_points] = descs
            if output_markdown:
                # log TSP num pts not as SHP
                file.write(f'### {num_points + 1} points\n\n')

                # dtype row
                file.write('| dtype |')
                for desc in descs:
                    file.write(f' {desc[1]} |')
                file.write('\n|---|' + '---|'*len(descs) + '\n')

                # avg time row
                file.write('| avg time [s] |')
                for desc in descs:
                    file.write(f' {(desc[0]/1000):.3f} |')
                file.write('\n')

                # time diff row
                min_time = min([ desc[0] for desc in descs ])
                file.write('| time diff [%] |')
                for desc in descs:
                    file.write(f' {((desc[0] + 1e-11)
                               / (min_time + 1e-11) - 1) * 100:.3f} |')
                file.write('\n')

                # reruns row
                file.write('| reruns |')
                for desc in descs:
                    file.write(f' {desc[2]} |')
                file.write('\n')

                # mem row
                file.write('| mem [GB] |')
                for desc in descs:
                    file.write(f' {desc[3]:.3f} |')
                file.write('\n')

                # mem diff row
                min_mem = min([ desc[3] for desc in descs ])
                file.write('| mem diff [%] |')
                for desc in descs:
                    file.write(f' {(desc[3] / min_mem - 1) * 100:.3f} |')
                file.write('\n\n')
            else:
                # log TSP num pts not as SHP
                file.write(f'{num_points + 1} points:\n')
                file.write(f'\tdtype:           ')
                for desc in descs:
                    file.write(f'{desc[1]}, ')
                file.write(f'\n\tavg time    [s]: ')
                for desc in descs:
                    file.write(f'{(desc[0] / 1000):.3f}, ')
                file.write(f'\n\ttime diff   [%]: ')
                min_time = min([ desc[0] for desc in descs ])
                for desc in descs:
                    file.write(f'{((desc[0] + 1e-11)
                               / (min_time + 1e-11) - 1) * 100:.3f}, ')
                file.write(f'\n\treruns         : ')
                for desc in descs:
                    file.write(f'{desc[2]}, ')
                file.write(f'\n\tmem        [GB]: ')
                for desc in descs:
                    file.write(f'{desc[3]:.3f}, ')
                file.write(f'\n\tmem diff    [%]: ')
                min_mem = min([ desc[3] for desc in descs ])
                for desc in descs:
                    file.write(f'{(desc[3] / min_mem - 1) * 100:.3f}, ')
                file.write('\n\n')

    xticks = [ el for el in range(min_num_pts, max_num_pts, 5) ] \
           + [ max_num_pts ]
    ax.set_xticks(xticks)
    ax.set_xticklabels(xticks)
    ax.set_title(f'Execution times per cost\'s dtype')
    ax.set_xlabel('Number of points', fontsize=AXIS_FONT_SIZE)
    ax.set_ylabel(f'Execution time [sec]', fontsize=AXIS_FONT_SIZE)
    ax.legend(loc='upper left')
    if savefig is not None:
        savefig = os.path.realpath(savefig)
        os.makedirs(os.path.dirname(savefig), exist_ok=True)
        plt.savefig(savefig)
        print(f'Stored avg exec times plot to file:\n{savefig}')
        plt.close()
    else:
        plt.show()


def _get_num_bytes(dtype: str):
    num_bytes = {
        'double': 8,
        'int64_t': 8,
        'float': 4,
        'int32_t': 4,
        'int16_t': 2,
        'int8_t': 1,
        '(u)int64_t': 8,
        '(u)int32_t': 4,
        '(u)int16_t': 2,
        '(u)int8_t': 1,
    }
    return num_bytes[dtype]

def shorten_file_name(file_name):
    max_chars = 20  # divisible by 2
    if len(file_name) <= max_chars:
        return file_name
    hashed_fname = hashlib.sha256(file_name.encode()).digest()
    # 1 hex chr => 4 bits, 2 chrs = 1 byte
    hashed_fname = hashed_fname[ 2 : 2 + max_chars // 2].hex()
    return hashed_fname

def space(n, cycle: bool, is_symmetric: bool,
          tc=2, tv=1, no_path=False):
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



if __name__ == '__main__':
    main()
