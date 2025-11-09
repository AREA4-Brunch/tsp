#ifndef UTILS_HPP
#define UTILS_HPP

namespace utils {

template<typename CONT>
void displayContainer(const CONT &cont) {
    for (const auto &el : cont) {
        std::cout << el << ", ";
    }
    std::cout << std::endl;
}

template<typename CONT>
void displayMatrix(const CONT &cont) {
    for (const auto &row : cont) {
        displayContainer(row);
    }
}

} // detail namesspace

#endif
