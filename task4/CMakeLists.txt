cmake_minimum_required(VERSION 3.20)

project(task4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

add_executable(task4_05_05 05_05.cpp)
add_executable(task4_07_01 07_01.cpp)
add_executable(task4_07_02 07_02.cpp)
add_executable(task4_07_03 07_03.cpp)
add_executable(task4_08_01 08_01.cpp)
add_executable(task4_08_02 08_02.cpp)
add_executable(task4_08_03 08_03.cpp)
add_executable(task4_08_04 08_04.cpp)
add_executable(task4_08_05 08_05.cpp)

foreach(target
    task4_05_05
    task4_07_01
    task4_07_02
    task4_07_03
    task4_08_01
    task4_08_02
    task4_08_03
    task4_08_04
    task4_08_05
)
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
endforeach()

add_subdirectory(06_05)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    benchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.9.1.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(googletest benchmark)

add_executable(task4_07_04 07_04.cpp)
target_compile_options(task4_07_04 PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(task4_07_04 PRIVATE gtest)

add_executable(task4_07_05 07_05.cpp)
target_compile_options(task4_07_05 PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(task4_07_05 PRIVATE benchmark::benchmark)
