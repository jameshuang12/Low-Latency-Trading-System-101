cmake_minimum_required(VERSION 3.0.0)
project(low-latency-trading-system VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
enable_testing()
enable_language()

add_executable()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)