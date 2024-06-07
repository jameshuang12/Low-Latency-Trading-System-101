#pragma once
#include <string>
#include <fstream>
#include <cstdio>
#include "types.h"
#include "macros.h"
#include "If_queue.h"
#include "thread_utils.h"
#include "time_utils.h"

namespace Common {
    constexpr size_t LOG_QUEUE_SIZE = 8 * 1024 * 1024;
    enum class LogType : int8_t {
        CHAR = 0;
        INTEGER = 1, LONG_INTEGER = 2, LONG_LONG_INTEGER = 3, UNSIGNED_INTEGER = 4, UNSIGNED_LONG_INTEGER = 5,
        UNSIGNED_LONG_LONG_INTEGER = 6, FLOAT = 7, DOUBLE = 8
    };
}