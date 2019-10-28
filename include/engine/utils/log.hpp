#pragma once

#include <iostream>

class log_t {
public:
    ~log_t() {
        std::cout << std::endl;
    }

    template <class T> log_t& operator<<(const T& arg) {
        std::cout << arg;
        return *this;
    }
};

#define LOG log_t()
