#ifndef TIMING_HPP
#define TIMING_HPP

#include <iostream>
#include <chrono>

namespace timing {

template<typename Func, typename... Args>
unsigned long long executeAndMeasureExecTime(const Func &func, Args&&... args) {
    const auto start_time = std::chrono::steady_clock::now();
    func(std::forward<Args>(args)...);
    const auto end_time = std::chrono::steady_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>
                          (end_time - start_time);
    const auto dur_ms = duration.count();
    std::cout << "Execution time: " << dur_ms << " ms" << std::endl;
    return dur_ms;
}

template<typename Func, typename... Args>
int executeAndMeasureAvgExecTime(
    const int num_runs,
    const unsigned long long timeout_ms,
    const Func &func,
    Args&&... args
) {
    unsigned long long total_time_ms = 0ULL;
    int executed_runs = 0;
    for (int i = 1; i <= num_runs; ++i) {
        std::cout << "\n\nExecution #" << i << "/" << num_runs << "\n\n";
        total_time_ms += executeAndMeasureExecTime(func, std::forward<Args>(args)...);
        ++executed_runs;
        std::cout << "\nElapsed time: " << total_time_ms << " ms" << std::endl;

        if (total_time_ms >= timeout_ms) {
            std::cout << "\n\nTerminating due to timeout: " << total_time_ms
                      << " ms >= " << timeout_ms << " ms\n\n";
            break;
        }
    }

    const double avg_time_ms = 1. * total_time_ms / executed_runs;
    std::cout << "Executed runs: " << executed_runs << "/" << num_runs
              << std::endl
              << "Total execution time: " << (1. * total_time_ms / 1000) << " s"
              << std::endl
              << "Avg execution time: " << avg_time_ms << " ms" << std::endl;
    return executed_runs;
}

} // timing namesspace

#endif
