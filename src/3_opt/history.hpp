#ifndef HISTORY_HPP
#define HISTORY_HPP

#include <string>
#include <vector>
#include <chrono>
#include <filesystem>
#include <fstream>


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

}


template<typename cost_t>
class History {
 public:
    History(const std::string &path_root_dir=".");
    void addCost(const cost_t cost);
    void storeCosts(const std::string &file_path, const bool do_append) const;
    void clear() { this->costs.clear(); this->costs_times.clear(); }
    void stop() { this->is_stopped = true; }
    void start() { this->is_stopped = false; }
    bool isStopped() const { return this->is_stopped; }
    std::string flush(const bool do_append_to_prev);
    auto size() const { return this->costs.size(); }

    template<typename... Args>
    void appendMarkersToLastFlush(Args&&... markers) const;

 private:
    bool is_stopped = false;
    std::vector<cost_t> costs;
    std::vector<unsigned long long> costs_times;
    std::string path_root_dir = ".";
    int last_flush_id = 0;
};


template<typename cost_t>
History<cost_t>::History(const std::string &path_root_dir)
    : path_root_dir(path_root_dir)
{ }

template<typename cost_t>
void History<cost_t>::addCost(const cost_t cost) {
    if (this->is_stopped) return;
    const auto time_now = std::chrono::system_clock::now();
    this->costs_times.push_back(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            time_now.time_since_epoch()
        ).count()
    );
    this->costs.push_back(cost);
}

template<typename cost_t>
void History<cost_t>::storeCosts(
    const std::string &file_path,
    const bool do_append
) const {
    printf("\nStoring histories into the file:\n%s\n", file_path.c_str());

    if (!this->costs.size()) {
        printf("\nCosts history empty, aborting store.\n");
        return;
    }

    if (!this->costs_times.size()) {
        printf("\nCosts' times history empty, aborting store.\n");
        return;
    }

    std::filesystem::path path(file_path);
    std::filesystem::path parent_path = path.parent_path();
    std::error_code err_code;
    std::filesystem::create_directories(parent_path, err_code);

    if (err_code) {
        printf("Failed to create neccessary folders: %s\n",
                err_code.message().c_str());
        throw "Failed to create neccessary folders: " + file_path;
    }

    std::ofstream file(
        file_path,
        std::ios::binary | (do_append ? std::ios::app : std::ios::trunc)
    );

    if (!file.is_open()) {
        printf("Failed to open file: %s\n", file_path.c_str());
        throw "Failed to open file: " + file_path;
    }

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
    printf("\nDone storing histories into the file:\n%s\n\n", file_path.c_str());
}

template<typename cost_t>
std::string History<cost_t>::flush(const bool do_append_to_prev) {
    if (!do_append_to_prev) ++this->last_flush_id;
    const std::string filename = std::to_string(this->last_flush_id)
                               + ".flush.bin";
    const std::string file_path = this->path_root_dir + "/" + filename;
    this->storeCosts(file_path, do_append_to_prev);
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

#endif
