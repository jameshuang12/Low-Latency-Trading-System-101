cmake_minimum_required(VERSION 3.0.0)
project(low-latency-trading-system VERSION 0.1.0 LANGUAGES C CXX)

find_package(Threads REQUIRED)

include(CTest)
enable_testing()
enable_language()

add_executable(thread_example.cpp mem_pool_example.cpp lf_queue_example.cpp)

target_link_libraries(Threads::Threads)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)