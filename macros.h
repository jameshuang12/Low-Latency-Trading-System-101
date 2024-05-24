//
// Created by james on 5/23/2024.
//

#ifndef MACROS_H
#define MACROS_H
#include <iostream>
#include <cstring>

#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

inline auto ASSERT(bool cond, const std::string& msg) noexcept {
    if(UNLIKELY(!cond)) {
        std::cerr << msg << std::endl;
        exit(EXIT_FAILURE);
    }
}

inline auto FATAL(const std::string& msg) noexcept {
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}

#endif
