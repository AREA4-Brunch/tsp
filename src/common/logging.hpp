#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <iostream>

namespace logging {

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

} // logging namesspace

#endif
