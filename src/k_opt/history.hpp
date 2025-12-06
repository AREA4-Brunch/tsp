#ifndef TSP_K_OPT_HISTORY_HPP
#define TSP_K_OPT_HISTORY_HPP

#pragma GCC diagnostic ignored "-Winline"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <filesystem>
#include <fstream>



namespace k_opt {

template<typename cost_t>
class History {
 public:
    History(const std::string &path_root_dir=".");
    inline void addCost(const cost_t cost);
    inline void addPath(
        const std::vector<int> &path,
        const int i,
        const int j,
        const int k,
        const int iter
    );
    void storeCosts(const std::string &file_path, const bool do_append) const;
    void storePaths(const std::string &file_path, const bool do_append) const;
    void clear() { this->costs.clear(); this->costs_times.clear(); this->paths.clear(); }
    void stop() { this->is_stopped = true; }
    void start() { this->is_stopped = false; }
    void stopPath() { this->do_not_record_path = true; }
    void startPath() { this->do_not_record_path = false; }
    void stopCost() { this->do_not_record_cost = true; }
    void startCost() { this->do_not_record_cost = false; }
    bool isStopped() const { return this->is_stopped; }
    std::string flush(const bool do_append_to_prev);
    auto size() const { return this->costs.size(); }

    template<typename... Args>
    void appendMarkersToLastFlush(Args&&... markers) const;

 private:
    bool is_stopped = false;
    bool do_not_record_cost = false;
    bool do_not_record_path = false;
    std::vector<cost_t> costs;
    std::vector<std::vector<int>> paths;
    std::vector<unsigned long long> costs_times;
    std::string path_root_dir = ".";
    int last_flush_id = 0;
};


/// clears run(s) dir if it already exists
template<typename cost_t>
void startNewHistory(
    const int runs_per_history,
    const std::string& path_history_dir,
    const int run_idx,
    History<cost_t>* &cur_history
);


namespace detail {

/// @brief Bottom of recursion for forwarding args within
///        detail::writeDataToBinaryFileHelper(ofstream, T, Args...)
void writeDataToBinaryFileHelper(std::ofstream &) { }

template<typename T, typename... Args>
void writeDataToBinaryFileHelper(
    std::ofstream &file, T &&datum, Args&& ...data
) {
    file.write(reinterpret_cast<char *> (&datum), sizeof(datum));
    writeDataToBinaryFileHelper(file, std::forward<Args>(data)...);
}

template<typename... Args>
void writeDataToBinaryFile(std::ofstream &file, Args&&... args) {
    writeDataToBinaryFileHelper(file, std::forward<Args>(args)...);
}

// could be moved to anonymous namespace in its own cpp file
std::ofstream openFile(const std::string &file_path, const bool do_append);

}  // namespace detail


template<typename cost_t>
History<cost_t>::History(const std::string &path_root_dir)
    : path_root_dir(path_root_dir)
{ }

template<typename cost_t>
void History<cost_t>::addCost(const cost_t cost) {
    if (this->is_stopped || this->do_not_record_cost) return;
    const auto time_now = std::chrono::system_clock::now();
    this->costs_times.push_back(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            time_now.time_since_epoch()
        ).count()
    );
    this->costs.push_back(cost);
}

template<typename cost_t>
void History<cost_t>::addPath(
    const std::vector<int> &path,
    const int i,
    const int j,
    const int k,
    const int iter
) {
    if (this->is_stopped || this->do_not_record_path) return;
    this->paths.emplace_back(path);
    this->paths.back().push_back(i);
    this->paths.back().push_back(j);
    this->paths.back().push_back(k);
    this->paths.back().push_back(iter);
}

template<typename cost_t>
void History<cost_t>::storeCosts(
    const std::string &file_path,
    const bool do_append
) const {
    std::cout << "Storing costs histories into the file:\n"
              << file_path << std::endl;
    if (!this->costs.size()) {
        std::cout << std::endl << "Costs history empty, aborting store."
                  << std::endl;
        return;
    }
    if (!this->costs_times.size()) {
        std::cout << std::endl << "Costs' times history empty, aborting store."
                  << std::endl;
        return;
    }

    std::ofstream file = detail::openFile(file_path, do_append);

    // number of vectors of data to be stored
    const size_t num_vectors = 2;
    file.write(reinterpret_cast<const char*>(&num_vectors), sizeof(num_vectors));

    // store costs
    const size_t costs_size = this->costs.size();
    file.write(
        reinterpret_cast<const char *>(&costs_size),
        sizeof(costs_size)
    );
    file.write(
        reinterpret_cast<const char *>(this->costs.data()),
        costs_size * sizeof(cost_t)
    );

    // store costs times
    const size_t costs_times_size = this->costs_times.size();
    file.write(
        reinterpret_cast<const char *>(&costs_times_size),
        sizeof(costs_times_size)
    );
    file.write(
        reinterpret_cast<const char *>(this->costs_times.data()),
        costs_times_size * sizeof(unsigned long long)
    );

    file.close();
    std::cout << std::endl << "Done storing costs histories into the file:"
              << std::endl << file_path << std::endl << std::endl;
}

template<typename cost_t>
std::string History<cost_t>::flush(const bool do_append_to_prev) {
    if (!do_append_to_prev) ++this->last_flush_id;
    std::string filename = std::to_string(this->last_flush_id) + ".flush.bin";
    std::string file_path = this->path_root_dir + "/" + filename;
    this->storeCosts(file_path, do_append_to_prev);
    filename = std::to_string(this->last_flush_id) + ".paths.csv";
    file_path = this->path_root_dir + "/" + filename;
    this->storePaths(file_path, do_append_to_prev);
    this->clear();
    return file_path;
}

template<typename cost_t>
template<typename... Args>
void History<cost_t>::appendMarkersToLastFlush(Args&&... markers) const {
    const std::string filename = std::to_string(this->last_flush_id)
                               + ".flush.bin";
    const std::string file_path = this->path_root_dir + "/" + filename;

    printf("\nAppending markers to the file:\n%s\n", file_path.c_str());

    std::filesystem::path path(file_path);
    std::filesystem::path parent_path = path.parent_path();
    std::error_code err_code;
    std::filesystem::create_directories(parent_path, err_code);

    if (err_code) {
        printf("Failed to create neccessary folders: %s\n",
                err_code.message().c_str());
        throw "Failed to create neccessary folders: " + file_path;
    }

    std::ofstream file(file_path, std::ios::binary | std::ios::app);

    detail::writeDataToBinaryFile(file, std::forward<Args>(markers)...);

    file.close();
    printf("\nDone appending markers to the file:\n%s\n",
            file_path.c_str());
}

template<typename cost_t>
void k_opt::startNewHistory(
    const int runs_per_history,
    const std::string& path_history_dir,
    const int run_idx,
    History<cost_t>* &cur_history
) {
    const std::string path_history_run
        = runs_per_history == 1 ?
        path_history_dir + "/run_" + std::to_string(run_idx)
        : path_history_dir + "/runs_"
        + std::to_string(1 + runs_per_history * (run_idx / runs_per_history))
        + "_"
        + std::to_string(runs_per_history * (1 + run_idx / runs_per_history));

    std::cout << "Run(s) folder: " << path_history_run << std::endl;
    // clear the run folder if it already exists
    if (std::filesystem::is_directory(path_history_run)) {
        std::error_code err_code;
        std::filesystem::remove_all(path_history_run, err_code);
        if (err_code) throw ("Failed to delete run folder: " + path_history_run);
        std::cout << "Deleted already existing run folder: "
            << path_history_run << std::endl;
    }

    if (cur_history != nullptr) delete cur_history;
    cur_history = new History<cost_t>(path_history_run);
}

template<typename cost_t>
void History<cost_t>::storePaths(
    const std::string &file_path,
    const bool do_append
) const {
    std::cout << "Storing paths histories into the file:\n"
              << file_path << std::endl;

    if (this->paths.empty()) {
        std::cout << std::endl << "nPaths history empty, aborting store."
                  << std::endl;
        return;
    }

    std::ofstream file = detail::openFile(file_path, do_append);

    for (const auto &path_desc : this->paths) {
        for (int i = 0, n = path_desc.size(); i < n - 1; ++i) {
            file << path_desc[i] << ",";
        }
        file << path_desc.back() << "\n";
    }

    file.close();
    std::cout << std::endl << "Done storing paths histories into the file:"
              << std::endl << file_path << std::endl << std::endl;
}

std::ofstream detail::openFile(
    const std::string &file_path,
    const bool do_append
) {
    std::filesystem::path path(file_path);
    std::filesystem::path parent_path = path.parent_path();
    std::error_code err_code;
    std::filesystem::create_directories(parent_path, err_code);
    if (err_code) {
        std::cout << "Failed to create neccessary folders: "
                  << err_code.message() << std::endl;
        throw "Failed to create neccessary folders: " + file_path;
    }

    std::ofstream file(
        file_path,
        std::ios::binary | (do_append ? std::ios::app : std::ios::trunc)
    );

    if (!file.is_open()) {
        std::cout << "Failed to open file: " << file_path << std::endl;
        throw "Failed to open file: " + file_path;
    }
    return file;
}

}  // namespace 3_opt

#endif
