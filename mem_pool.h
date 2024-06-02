#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "macros.h"

namespace Common {
    template<typename T>
    class MemPool final {
    private:
        struct ObjectBlock {
            T object;
            bool is_free_ = true;
        };
        std::vector<ObjectBlock> store_;
        size_t next_free_index_ = 0;
    };
};