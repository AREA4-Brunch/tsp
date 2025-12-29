"""
python plot_history.py <problem_name> <num_points> [<mode>] [<run_start>] [<run_end>] [<window_timeframe>] [<start_time>] [--no-cache|-nc] [--loglog|-ll]

python commands/auto_opt/plot_history.py 263 263 shp 0 -1 1 100 -ll
python commands/auto_opt/plot_history.py 263 263 shp 0 -1 1 -ll
python commands/auto_opt/plot_history.py 263 263 shp 0 1000 1 20 -ll
python commands/auto_opt/plot_history.py 263 263 shp 0 -1 1 20 -nc -ll
"""

import sys
import os
import pickle
import hashlib
import numpy as np
import time
import math
import gc
import struct
import plotly.graph_objects as go

K = 3
ALGOS = [
    # f'auto_opt_best_cut',  # remove from plot since too long too large cost
    f'auto_opt_classical',
    f'auto_opt_funky',
    f'auto_opt_rand',
    f'auto_opt_rand_no_2_opt',
]
MODE = 'shp'
PATH_CWD = os.path.dirname(os.path.realpath(__file__))
PATH_RES = os.path.join(PATH_CWD, f'../../results')
PATH_ANALYSIS = os.path.join(PATH_CWD, f'../../analysis', f'all_auto_opt')
PATH_CACHED = os.path.join(PATH_CWD, f'../../analysis', 'cached')
COST_DTYPE = 'double'
TIME_DTYPE = 'unsigned long long'


def main():
    args = parse_args()
    start_time, conf = build_conf(args)
    setup_paths(args['prob_name'])

    cum_avg_plt, all_runs_plt = setup_plots(args, start_time)
    for algo in ALGOS:
        print(f'\nProcessing algo: {algo}\n')
        cum_avgs, all_runs = get_or_compute_results(algo, args, conf)
        add_cum_avg_to_plot(
            cum_avg_plt, cum_avgs, start_time,
            loglog=args['loglog'], label=algo)
        add_to_all_runs_plot(
            all_runs_plt, all_runs, start_time,
            loglog=args['loglog'], label=algo)
        gc.collect()
    cum_avg_plt['plot_or_save']()
    all_runs_plt['plot_or_save']()

def parse_args():
    flag_args = ['--no-cache-res', '-ncr', '--no-cache', '-nc', '--loglog', '-ll']
    filtered_argv = [arg for arg in sys.argv if arg.lower() not in flag_args]
    return {
        'no_cache_res_only': any(arg.lower() in ['--no-cache-res', '-ncr'] for arg in sys.argv),
        'no_cache': any(arg.lower() in ['--no-cache', '-nc'] for arg in sys.argv),
        'loglog': any(arg.lower() in ['--loglog', '-ll'] for arg in sys.argv),
        'prob_name': filtered_argv[1] if len(filtered_argv) > 1 else '263',
        'num_points': int(filtered_argv[2]) if len(filtered_argv) > 2 else 263,
        'mode': filtered_argv[3] if len(filtered_argv) > 3 else 'shp',
        'run_start': int(filtered_argv[4]) if len(filtered_argv) > 4 else 0,
        'run_end': int(filtered_argv[5]) if len(filtered_argv) > 5 else float('inf'),
        'window_timeframe': float(filtered_argv[6]) if len(filtered_argv) > 6 else 1,
        'start_time': float(filtered_argv[7]) if len(filtered_argv) > 7 else 0,
    }

def build_conf(args):
    if args['run_end'] < 0:
        args['run_end'] = float('inf')
    return (args['start_time'], {
        'run_start': args['run_start'],
        'run_end': args['run_end'],
        'cost_dtype_size': dtype_desc(COST_DTYPE)[0],
        'cost_dtype_format': dtype_desc(COST_DTYPE)[1],
        'cost_np_dtype': dtype_desc(COST_DTYPE)[2],
        'time_dtype_size': dtype_desc(TIME_DTYPE)[0],
        'time_dtype_format': dtype_desc(TIME_DTYPE)[1],
        'time_np_dtype': dtype_desc(TIME_DTYPE)[2],
        'do_select_min': True,
        'window_timeframe': args['window_timeframe'],
        'num_points': args['num_points'],
        'flatten': True,
    })

def setup_paths(prob_name):
    global PATH_ANALYSIS, PATH_CACHED
    PATH_ANALYSIS = os.path.join(PATH_ANALYSIS, prob_name)
    PATH_CACHED = os.path.join(PATH_CACHED, prob_name)
    os.makedirs(os.path.join(PATH_ANALYSIS, 'plots'), exist_ok=True)
    os.makedirs(PATH_CACHED, exist_ok=True)

def setup_plots(args, st: float):
    path_plots = os.path.join(PATH_ANALYSIS, 'plots')
    shared_plt_conf = { 'xlabel': 'time (ms)', 'ylabel': 'cost', 'fontsize': 20 }
    st = int(st)
    cum_avg_plt = create_cum_avg_plot(
        save_path=os.path.join(path_plots, f'cum_avg_{args["mode"]}_{args["num_points"]}_st_{st}ms.png'),
        title=f'Cumulative averages on problem: {args["prob_name"]} with {args["num_points"]} points',
        **shared_plt_conf
    )
    all_runs_plt = create_all_runs_plot(
        save_path=os.path.join(path_plots, f'all_runs_plot_{args["mode"]}_{args["num_points"]}_st_{st}ms.png'),
        title=f'Minima found on problem: {args["prob_name"]} with {args["num_points"]} points',
        **shared_plt_conf
    )
    return cum_avg_plt, all_runs_plt

def get_or_compute_results(algo, args, conf):
    ca_cache_fname = get_cum_avg_cache_fname(args['prob_name'], args['num_points'], args['mode'], algo, conf)
    ca_path_cached = os.path.realpath(os.path.join(PATH_CACHED, ca_cache_fname))
    ar_cache_fname = get_all_runs_cache_fname(args['prob_name'], args['num_points'], args['mode'], algo, conf)
    ar_path_cached = os.path.realpath(os.path.join(PATH_CACHED, ar_cache_fname))
    has_cache = os.path.isfile(ca_path_cached) and os.path.isfile(ar_path_cached)

    if not args['no_cache'] and not args['no_cache_res_only'] and has_cache:
        cum_avgs = load_preprocessed(ca_path_cached)
        print(f'\nWARNING: using cached results for {algo} from:\n{ca_path_cached}\n')
        all_runs = load_preprocessed(ar_path_cached)
    else:
        histories = fetch_algo_histories(
            PATH_CACHED, PATH_RES,
            algo, args['prob_name'], args['mode'], args['no_cache'], conf
        )[1]
        all_runs = get_all_runs(histories, args['window_timeframe'])
        store_preprocessed(all_runs, ar_path_cached)
        print(f'Cached all runs data to: {ar_path_cached}')
        gc.collect()
        cum_avgs = compute_cum_avgs(histories, args['window_timeframe'])
        store_preprocessed(cum_avgs, ca_path_cached)
        print(f'Cached cumulative avgs to: {ca_path_cached}')
        gc.collect()
    return cum_avgs, all_runs

def get_cum_avg_cache_fname(prob_name, num_pts, mode, algo, conf):
    return shorten_file_name(
        f'cum_avg_{prob_name}_{mode}_{num_pts}_{algo}'
        + stringify_args('', **conf)
    )

def get_all_runs_cache_fname(prob_name, num_pts, mode, algo, conf):
    return shorten_file_name(
        f'all_runs_{prob_name}_{mode}_{num_pts}_{algo}'
        + stringify_args('', **conf)
    )

def shorten_file_name(file_name):
    """Shorten long filenames using hash."""
    max_chars = 20
    if len(file_name) <= max_chars:
        return file_name
    hashed = hashlib.sha256(file_name.encode()).digest()
    return hashed[2 : 2 + max_chars // 2].hex()

def compute_cum_avgs(
    histories,
    calc_window_size=None,
) -> tuple[list, list]:
    def time_at(run_times, i):
        time = run_times[i] - run_times[0]
        if calc_window_size is not None:
            time = math.floor(time / calc_window_size)
        return time
    # cumulative_avgs[time] = [ cost, freq ]
    cum_avgs_times = set()
    for run_idx, run_costs, run_times in histories:
        for i, cost in enumerate(run_costs):
            time = time_at(run_times, i)
            cum_avgs_times.add(time)

    cum_avgs_times = sorted(cum_avgs_times)
    cumulative_avgs_vals = [ [0, 0] for _ in range(len(cum_avgs_times)) ]
    for run_idx, run_costs, run_times in histories:
        cur_min_cost = float('inf')
        cum_avg_time_iter = iter(cum_avgs_times)
        cum_avg_val_iter = iter(cumulative_avgs_vals)
        cum_avg_time = next(cum_avg_time_iter)
        cum_avg_val = next(cum_avg_val_iter)

        for i, cost in enumerate(run_costs):
            time = time_at(run_times, i)
            if cum_avg_time < time:
                cum_avg_time = next(cum_avg_time_iter)
                cum_avg_val = next(cum_avg_val_iter)

            while cum_avg_time < time:
                if i > 0:
                    cum_avg_val[0] += cur_min_cost
                    cum_avg_val[1] += 1
                cum_avg_time = next(cum_avg_time_iter)
                cum_avg_val = next(cum_avg_val_iter)

            cur_min_cost = min(cur_min_cost, cost)
            cum_avg_val[0] += cost
            cum_avg_val[1] += 1

        # reuse the last cost for all the next time windows
        try:
            cum_avg_val = next(cum_avg_val_iter)
            while True:
                cum_avg_val[0] += cur_min_cost
                cum_avg_val[1] += 1
                cum_avg_val = next(cum_avg_val_iter)

        except StopIteration:
            pass

    # calc the avg in sorted time order
    cumulative_avgs_vals = [
        val[0] / val[1]
        for val in cumulative_avgs_vals
    ]

    return cum_avgs_times, cumulative_avgs_vals

def fetch_algo_histories(
    cache_dir_path,
    results_dir_path,
    algo, prob_name, mode,
    no_cache,
    config,
):
    path_histories = os.path.join(
        results_dir_path, algo, 'histories',
        prob_name, mode, str(config['num_points'])
    )
    print(f'Fetching histories from:\n{path_histories}')
    run_end, histories, num_runs = get_histories(
        cache_dir_path, path_histories, no_cache, config
    )
    print(f'run end: {run_end}, num_runs: {num_runs}')
    return run_end, histories, num_runs

def get_histories(
    cache_dir_path,
    path_histories,
    no_cache,
    config,
):
    start_time = time.time()
    cache_fname = stringify_args(
        'fd_',  # fetch_data_
        os.path.realpath(path_histories),
        cost_dtype_size=config['cost_dtype_size'],
        cost_dtype_format=config['cost_dtype_format'],
        time_dtype_size=config['time_dtype_size'],
        time_dtype_format=config['time_dtype_format'],
        run_start=config['run_start'],
        run_end=config['run_end'],
    )
    cache_fname += stringify_args(
        're_',  # resolve_equal_
        do_select_min=config['do_select_min'],
        cost_np_dtype=config['cost_np_dtype'],
        time_np_dtype=config['time_np_dtype'],
        window_timeframe=config['window_timeframe'],
        num_points=config['num_points'],
    )

    print(f'Cache hash resembles to:\n{cache_fname}\n')
    cache_fname = shorten_file_name(cache_fname)
    path_cached_data = os.path.join(cache_dir_path, cache_fname)

    histories = None if no_cache else load_preprocessed(path_cached_data)
    if histories is None:
        histories = fetch_histories(path_histories, config)
        histories = resolve_equal_times(histories, **config)
        # if histories are are going tobe reused flatten instead of yield
        if config['flatten']:
            histories = get_all_and_flatten(histories)
        gc.collect()
        store_preprocessed(histories, path_cached_data)
        print(f'Stored preprocessed hash to file:\n{path_cached_data}')
    else:
        print(f'WARNING: using cached preprocessed data'
              f'\n{path_cached_data}\n')

    # !important only after preprocessing is history grouped by run_idx
    num_runs = len(histories)
    run_end = config['run_start'] + num_runs - 1

    dur = time.time() - start_time
    print(f'Fetching data and preprocessing time: {dur} secs')
    return run_end,histories,num_runs

def stringify_args(prefix, *args, **kwargs):
    out = prefix + '_'.join(args)
    for key, val in kwargs.items():
        out += f'{key}:{val}_'
    return out

def fetch_histories(path_histories: str, config):
    """ run_end is inclusive. """
    run_start, run_end = config['run_start'], config['run_end']
    paths_dirs = []
    for dirpath, dirnames, filenames in os.walk(path_histories):
        for dirname in dirnames:
            if not dirname.startswith('run'):
                continue
            if dirname.startswith('runs_'):
                run_start_idx = int(dirname[ len('runs_') : dirname.rfind('_') ])
                if run_start_idx > run_end:
                    continue
                run_end_idx = int(dirname[ dirname.rfind('_') + len('_') : ])
                if run_end_idx < run_start:
                    continue
            else:
                run_idx = int(dirname[ len('run_') : ])
                if not (run_start <= run_idx <= run_end):
                    continue
            paths_dirs.append(os.path.join(dirpath, dirname))

    paths_dirs = sorted(
        paths_dirs,
        key=lambda dirname: int(dirname[
            dirname.rfind('runs_') + len('runs_') : dirname.rfind('_')
        ])
    )
    for path_dir in paths_dirs:
        yield fetch_history(path_dir, **config)

def fetch_history(
    path_run_dir,
    run_start,
    run_end,
    cost_dtype_size,
    cost_dtype_format,
    time_dtype_size,
    time_dtype_format,
    **_
):
    run_idx = None
    costs_cur_run, times_cur_run = [], []
    batch_idx = 0  # for debugging
    total_costs_cnt = 0  # for debugging
    # sort flush files by index, not lexicographically
    filenames = []
    for fname in os.listdir(path_run_dir):
        if fname.endswith('.flush.bin'):
            filenames.append(fname)
    sorted_flush_files = sorted(
        filenames,
        key=lambda filename: int(filename[ : filename.rfind('.flush.bin')])
    )

    for filename in sorted_flush_files:
        if not filename.endswith('.flush.bin'):
            continue
        file_path = os.path.join(path_run_dir, filename)
        with open(file_path, 'rb') as file:
            print(f'\nProcessing run: {file_path}\n')
            if run_idx is None:  # very first flush must have run_idx
                try:
                    run_idx_pt_fmt_start = struct.unpack('Q', file.read(8))[0]
                    if run_idx_pt_fmt_start != 0:
                        print(f'Got run_idx_pt_fmt_start = {run_idx_pt_fmt_start}')
                        raise Exception(f'First flush in run dir must start with 0ULL')

                    run_idx = struct.unpack('i', file.read(4))[0]
                    print(f'\nStarting to read new run: {run_idx}')
                except struct.error as e:
                    run_idx = None
                    raise Exception('Missing run idx !')

            while True:  # keep going through unknown number of iters
                try:
                    num_metrics_arrays = struct.unpack('Q', file.read(8))[0]
                except struct.error as e:
                    # print(f'Merged {batch_idx} metrics batches of run {run_idx}. {len(costs_cur_run)} costs.')
                    break

                if num_metrics_arrays == 0:  # pt_fmt for the start of a new run
                    if run_start <= run_idx <= run_end:
                        total_costs_cnt += len(costs_cur_run)
                        yield run_idx, costs_cur_run, times_cur_run
                    else:
                        print(f'Not yielding run: {run_idx}')

                    costs_cur_run, times_cur_run = [], []
                    batch_idx = 0
                    run_idx = struct.unpack('i', file.read(4))[0]
                    if run_idx > run_end:
                        print(f'Early stopping due to run idx: {run_idx} > {run_end}')
                        break

                    if run_idx % 100 == 0:
                        print(f'Starting to read new run: {run_idx}')
                    try:  # refetch num metrics arrays
                        num_metrics_arrays = struct.unpack('Q', file.read(8))[0]
                    except struct.error as e:
                        # print(f'Merged {batch_idx} metrics batches of run {run_idx}. {len(costs_cur_run)} costs.')
                        break

                if num_metrics_arrays != 2:
                    msg = f'Expecting costs and times but got {num_metrics_arrays} metrics\n'
                    msg += f'run_idx: {run_idx}, num costs cur run: {len(costs_cur_run)}'
                    msg += f', num times cur run: {len(times_cur_run)}, batch_idx: {batch_idx}'
                    msg += f', total_costs_cnt: {total_costs_cnt}\n'
                    raise Exception(msg)

                # load costs
                num_costs = struct.unpack('Q', file.read(8))[0]
                batch = struct.unpack(
                    f'{num_costs}{cost_dtype_format}',
                    file.read(cost_dtype_size * num_costs)
                )
                if run_start <= run_idx <= run_end:
                    costs_cur_run.extend(batch)

                # load times
                num_times = struct.unpack('Q', file.read(8))[0]
                if num_times != num_costs:
                    msg = f'Num times ({num_times}) and num of costs ({num_costs}) do not match'
                    raise Exception(msg)
                batch = struct.unpack(
                    f'{num_times}{time_dtype_format}',
                    file.read(time_dtype_size * num_times)
                )
                if run_start <= run_idx <= run_end:
                    times_cur_run.extend(batch)

                batch_idx += 1

        # process leftover from the file of currently ongoing run_history
        total_costs_cnt += len(costs_cur_run)
        print(f'Yielded costs from this file: {total_costs_cnt}.')
        if ( run_idx is not None
         and run_start <= run_idx <= run_end
         and costs_cur_run and times_cur_run
        ):
            yield run_idx, costs_cur_run, times_cur_run

def resolve_equal_times(
    histories,
    do_select_min=False,
    cost_np_dtype=np.float64,
    time_np_dtype=np.ulonglong,
    window_timeframe=1,
    **_
):
    def is_same_moment(time1, time2):
        return math.floor(time1 / window_timeframe) \
            == math.floor(time2 / window_timeframe)

    def moment(time):
        return window_timeframe * math.floor(time / window_timeframe)

    for runs_history in histories:
        costs, times = [], []
        prev_run_idx = None
        start_at = None
        for run_idx, batch_costs, batch_times in runs_history:
            if prev_run_idx != run_idx and costs:
                costs = np.array(costs, dtype=cost_np_dtype)
                times = np.array(times, dtype=time_np_dtype)
                yield [ prev_run_idx, costs, times ]
                costs, times = [], []
                start_at = batch_times[0]
            elif prev_run_idx is None:
                start_at = batch_times[0]

            if do_select_min:
                cur_min = float('inf')
                for i in range(len(batch_costs)):
                    if batch_times[i] < start_at:
                        continue
                    if times and is_same_moment(
                        batch_times[i], times[-1]
                    ):
                        if batch_costs[i] < cur_min:
                            cur_min = batch_costs[i]
                            costs[-1] = cur_min
                    else:
                        cur_min = min(cur_min, batch_costs[i])
                        costs.append(cur_min)
                        times.append(moment(batch_times[i]))
            else:
                after_start = batch_times >= start_at
                costs.extend(batch_costs[after_start])
                times.extend(batch_times[after_start])
                for i in range(len(times)):
                    times[i] = moment(times[i])

            prev_run_idx = run_idx
            gc.collect()

        # add leftover from the last run mini batch
        costs = np.array(costs, dtype=cost_np_dtype)
        times = np.array(times, dtype=time_np_dtype)
        assert len(costs) == len(times)
        yield [ prev_run_idx, costs, times ]

def get_all_and_flatten(histories):
    # unpack yielded generator values and flatten runs' batches
    flattened = []
    total_costs_cnt = 0
    for idx, (run_idx, run_costs, times) in enumerate(histories):
        total_costs_cnt += run_costs.shape[0]
        flattened.append(
            [ run_idx, run_costs, times ]
        )
    print(f'Preprocessed for plotting {total_costs_cnt} costs.')
    return flattened

def load_preprocessed(file_path):
    if not os.path.isfile(file_path):
        return None

    with open(file_path, 'rb') as file:
        return pickle.load(file)

def store_preprocessed(all_flattened, file_path):
    file_path = os.path.realpath(file_path)
    if os.path.isfile(file_path):
        print(f'WARNING: overwriting existing preprocessed '
              f'cache:\n{file_path}')
    else:
        os.makedirs(os.path.dirname(file_path), exist_ok=True)
    with open(file_path, 'wb') as file:
        pickle.dump(all_flattened, file)

def dtype_desc(dtype: str):
    # dtype_descs[dtype_str] = (size_bytes, struct_format, np_dtype)
    dtype_descs = {
        'double': (8, 'd', np.float64),
        'long long': (8, 'q', np.longlong),
        'unsigned long long': (8, 'Q', np.ulonglong),
        'int': (4, 'i', np.int32),
        'float': (4, 'f', np.float32),
    }
    return dtype_descs[dtype]

def create_cum_avg_plot(
    save_path: str = None,
    title: str = '',
    xlabel: str = '',
    ylabel: str = '',
    fontsize: int = 12,
):
    fig = go.Figure()
    fig.update_layout(
        title={'text': title, 'font': {'size': fontsize}, 'x': 0.2},
        xaxis={'title': {'text': xlabel, 'font': {'size': fontsize}}},
        yaxis={'title': {'text': ylabel, 'font': {'size': fontsize}}},
        legend={'font': {'size': fontsize}},
        font={'size': fontsize},
        width=1200,
        height=900,
    )

    def finalize():
        nonlocal save_path
        if save_path is not None:
            fig.write_image(save_path, scale=2)
            print(f'Saved plot to:\n{os.path.realpath(save_path)}')
        else:
            fig.show()

    return {
        'fig': fig,
        'plot_or_save': finalize,
    }

def add_cum_avg_to_plot(
    plot: dict[str, any],
    cum_avgs: tuple[list, list],
    start_time: int = 0,
    loglog: bool = False,
    label: str = '',
    smooth_window: int = None
):
    times, avgs = cum_avgs
    times = [ t for t in times if t >= start_time ]
    avgs = avgs[ len(avgs) - len(times) : ]
    if not times:
        print(f'WARNING: empty cumulative averages, nothing to plot.')
        return
    print(f'Min time: {times[0]}, max time: {times[-1]}')
    print(f'Max avg: {avgs[0]}, min avg: {avgs[-1]}')

    if smooth_window and smooth_window > 1 and len(avgs) >= smooth_window:
        avgs = np.convolve(avgs, np.ones(smooth_window)/smooth_window, mode='valid')
        times = times[:len(avgs)]

    fig = plot['fig']
    mode = 'lines' if len(times) >= 2 else 'markers'
    fig.add_trace(go.Scatter(
        x=times,
        y=avgs,
        mode=mode,
        name=label,
        line={'shape': 'linear'} if mode == 'lines' else None,
    ))
    if loglog:
        min_tick = max(1, int(np.floor(np.log10(max(times[0], 1)))))
        max_tick = int(np.ceil(np.log10(times[-1])))
        min_tick = min(plot.get('min_time', min_tick), min_tick)
        max_tick = max(plot.get('max_time', max_tick), max_tick)
        plot['min_time'], plot['max_time'] = min_tick, max_tick
        tickvals = [10**i for i in range(min_tick, max_tick+1)]
        midpoints = []
        for i in range(min_tick, max_tick+1):
            midpoints.extend([2*10**i, 5*10**i])
        tickvals += midpoints
        tickvals = sorted(set(tickvals))
        fig.update_xaxes(
            type="log",
            tickmode='array',
            tickvals=tickvals,
            ticktext=[str(v) for v in tickvals],
            minor=dict(ticks="outside", showgrid=True)
        )
        fig.update_yaxes(
            type="log",
            tickmode='auto',
            nticks=10,
            minor=dict(ticks="outside", showgrid=True)
        )
    else:
        fig.update_xaxes(tickmode='auto', nticks=20)
        fig.update_yaxes(tickmode='auto', nticks=20)

def create_all_runs_plot(*args, **kwargs):
    return create_cum_avg_plot(*args, **kwargs)

def add_to_all_runs_plot(
    plot: dict[str, any],
    all_runs: tuple[list, list],
    start_time: int = 0,
    loglog: bool = False,
    label: str = '',
    smooth_window: int = None
):
    times_, avgs_ = all_runs
    times, avgs = [], []
    prev_avg = float('inf')
    for time, avg in zip(times_, avgs_):
        if time < start_time:
            continue
        if abs(avg - prev_avg) < 0.1:
            continue
        times.append(time)
        avgs.append(avg)
        prev_avg = avg
    if not times:
        print(f'WARNING: empty all runs data, nothing to plot.')
        return
    print(f'Min time: {times[0]}, max time: {times[-1]}')
    print(f'Max cost: {avgs[0]}, min cost: {avgs[-1]}')
    print(f'Length of times: {len(times)}')

    if smooth_window and smooth_window > 1 and len(avgs) >= smooth_window:
        avgs = np.convolve(avgs, np.ones(smooth_window)/smooth_window, mode='valid')
        times = times[:len(avgs)]

    fig = plot['fig']
    if len(times) >= 2:
        # Create stair-step (stepwise) effect for minima
        # Duplicate each point except the first, for horizontal-vertical steps
        step_times = [times[0]]
        step_avgs = [avgs[0]]
        for t, a in zip(times[1:], avgs[1:]):
            step_times.append(t)
            step_avgs.append(step_avgs[-1])  # horizontal step
            step_times.append(t)
            step_avgs.append(a)  # vertical drop
        # Remove the first duplicate (keeps correct length)
        step_times = step_times[1:]
        step_avgs = step_avgs[1:]
        fig.add_trace(go.Scatter(
            x=step_times,
            y=step_avgs,
            mode='lines',
            name=label,
            line={'shape': 'hv'},
        ))
    else:
        fig.add_trace(go.Scatter(
            x=times,
            y=avgs,
            mode='markers',
            name=label,
        ))

    nticks = 20
    plot['min_time'] = min(plot.get('min_time', float('inf')), min(times))
    plot['max_time'] = max(plot.get('max_time', 0), max(times))
    tickvals = np.linspace(plot['min_time'], plot['max_time'], nticks, dtype=int)
    ticktext = [_format_timepoint(int(v)) for v in tickvals]
    fig.update_xaxes(
        tickmode='array',
        tickvals=tickvals,
        ticktext=ticktext
    )
    if loglog:
        fig.update_yaxes(type="log", tickmode='auto', nticks=10,
                         minor=dict(ticks="outside", showgrid=True))
    else:
        fig.update_yaxes(tickmode='auto', nticks=20)

def get_all_runs(
    histories,
    calc_window_size=None,
    time_incr=0
) -> tuple[list, list]:
    start_time = None
    def time_at(run_times, i):
        nonlocal start_time
        time = run_times[i] - start_time
        if calc_window_size is not None:
            time = math.floor(time / calc_window_size)
        time += time_incr
        return time

    times, costs = [], []
    for run_idx, run_costs, run_times in histories:
        for i, cost in enumerate(run_costs):
            if start_time is None:
                start_time = run_times[0]
            time = time_at(run_times, i)
            times.append(time)
            costs.append(min(costs[-1], cost) if costs else cost)
    print(f'max time: {times[-1]}, min cost: {min(costs)}, max cost: {max(costs)}')
    return times, costs

def _format_timepoint(ms):
    secs = ms // 1000
    if secs == 0: return f'{ms} ms'
    ms %= 1000
    mins = secs // 60
    if mins == 0: return f'{secs}.{ms} s'
    secs %= 60
    hours = mins // 60
    if hours == 0: return f'{mins}:{secs}.{ms} m'
    mins %= 60
    return f'{hours}:{mins}:{secs}.{ms} h'


if __name__ == "__main__":
    main()
