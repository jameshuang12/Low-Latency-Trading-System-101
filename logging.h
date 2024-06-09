#pragma once
#include <string>
#include <fstream>
#include <cstdio>

#include "macros.h"
#include "If_queue.h"
#include "thread_utils.h"
#include "time_utils.h"

namespace Common {
    constexpr size_t LOG_QUEUE_SIZE = 8 * 1024 * 1024;
    enum class LogType : int8_t {
        CHAR = 0,
        INTEGER = 1,
        LONG_INTEGER = 2,
        LONG_LONG_INTEGER = 3,
        UNSIGNED_INTEGER = 4,
        UNSIGNED_LONG_INTEGER = 5,
        UNSIGNED_LONG_LONG_INTEGER = 6,
        FLOAT = 7,
        DOUBLE = 8
    };

    struct LogElement {
        LogType type_ = LogType::CHAR;
        union {
            char c;
            int i;
            long l;
            long long ll;
            unsigned u;
            unsigned long ul;
            unsigned long long ull;
            float f;
            double d;
        } u_;
    };

    class Logger final {
    public:
        auto flushQueue() noexcept {
            while (running_) {

                for (auto next = queue_.getNextToRead(); queue.size() && next; next = queue_.getNextToRead()) {
                    switch(next->type_){
                        case LogType::CHAR:
                            file_ << next->u_.c;
                            break;
                        case LogType::INTEGER:
                            file_ << next->u_.i;
                            break;
                        case LogType::LONG_INTEGER:
                            file_ << next->u_.l;
                            break;
                        case LogType::LONG_LONG_INTEGER:
                            file_ << next->u_.ll;
                            break;
                        case LogType::UNSIGNED_INTEGER:
                            file_ << next->u_.u;
                            break;
                        case LogType::UNSIGNED_LONG_INTEGER:
                            file_ << next->u_.ul
                            break;
                        case LogType::UNSIGNED_LONG_LONG_INTEGER:
                            file_ << next->u_.ull;
                            break;
                        case LogType::FLOAT:
                            file_ << next->u_.f;
                            break;
                        case LogType::DOUBLE:
                            file_ << next->u_.d;
                            break;
                    }
                    queue_.updateReadIndex();
                }
                file_.flush();
                using namespace std::literals::chrono_literals;
                std::this_thread::sleep_for(10ms);
            }
        }
    };
}