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
        explicit Logger(const std::string &file_name): file_name(file_name), queue_(LOG_QUEUE_SIZE) {
            file_.open(file_name);
            ASSERT(file_.is_open(), "Cold not open log file:" + file_name);
            logger_thread_ = createAndStartThread(-1, "Common/Logger " + file_name_, [this]() {flushQueue(); });
            ASSERT(logger_thread_ != nullptr, "Failed to start Logger thread.");
        }

        ~Logger() {
            std::string time_str;
            std::cerr << Common::getCurrentTimeStr(&time_str)
            << "Flushing and closing Logger for " << file_name_ << std::endl;

            while (queue_.size()) {
                using namespace std::literals::chrono_literals;
                std::this_thread::sleep_for(1s);
            }
            running_ = false;
            logger_thread_->join();

            file_.close();
            std::cerr << Common::getCurrentTimeStr(&time_str) << " Logger for "
            << file_name_ << " exiting." << std::endl;
        }

        auto pushValue(const LogElement &log_element) noexcept {
            *(queue_.getNextToWriteTo()) = log_element;
            queue_.updateWriteIndex();
        }

        auto pushValue(const char value) noexcept {
            pushValue(LongElement{LogType::CHAR, {.c=value}});
        }
        auto pushValue(const int value) noexcept {
            pushValue(LongElement{LogType::INTEGER, {.i=value}});
        }
        auto pushValue(const long value) noexcept {
            pushValue(LongElement{LogType::LONG_INTEGER, {.l=value}});
        }
        auto pushValue(const long long value) noexcept {
            pushValue(LongElement{LogType::LONG_LONG_INTEGER, {.ll=value}});
        }
        auto pushValue(const unsigned value) noexcept {
            pushValue(LongElement{LogType::UNSIGNED, {.u=value}});
        }
        auto pushValue(const unsigned long value) noexcept {
            pushValue(LongElement{LogType::UNSIGNED_LONG, {.ul=value}});
        }
        auto pushValue(const unsigned long long value) noexcept {
            pushValue(LongElement{LogType::CHAR, {.ull=value}});
        }
        auto pushValue(const float value) noexcept {
            pushValue(LongElement{LogType::FLOAT, {.f=value}});
        }
        auto pushValue(const double  value) noexcept {
            pushValue(LongElement{LogType::DOUBLE, {.d=value}});
        }
        auto pushValue(const char *value) noexcept {
            while(*value) {
                pushValue(*value);
                ++value;
            }
        }

        auto pushValue(const std::string &value) noexcept {
            pushValue(value.c_str());
        }

        template<typename T, typename... A>
        auto log(const char *s, const T &value, A... args) noexcept {
            while (*s) {
                if (*s == '%') {
                    if (UNLIKELY(*(s + 1) == '%')) {
                        ++s;
                    } else {
                        pushValue(value);
                        log(s + 1, args...);
                        return;
                    }
                }
                pushValue(*s++);
            }
            FATAL("extra arguments provided to log()");
        }

        auto log(const char *s) noexcept {
            while (*s) {
                if(*s == '%') {
                    if (UNLIKELY(*(s+1) == '%')) {
                        ++s;
                    } else {
                        FATAL("missing arguments to log()");
                    }
                }
                pushValue(*s++);
            }
        }

        Logger() = delete;
        Logger(const Logger &) = delete;
        Logger(const Logger &&) = delete;
        Logger &operator=(const Logger &) = delete;
        Logger &operator=(const Logger &&) = delete;

    private:
        const std::string file_name_;
        std::ofstream file_;

        LFQueue<LogElement> queue_;
        std::atomic<bool> running_ = {true};
        std::thread *logger_thread_ = nullptr;
    };
}
