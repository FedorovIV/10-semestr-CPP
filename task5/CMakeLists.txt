cmake_minimum_required(VERSION 3.20)

project(task5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/googletest vendor/googletest-build)
add_subdirectory(vendor/benchmark vendor/benchmark-build)

find_package(Boost REQUIRED COMPONENTS filesystem system)

add_executable(task5_09_01 09_01.cpp)
add_executable(task5_09_02 09_02.cpp)
add_executable(task5_09_03 09_03.cpp)
add_executable(task5_09_04 09_04.cpp)
add_executable(task5_09_05 09_05.cpp)
add_executable(task5_09_06 09_06.cpp)
add_executable(task5_09_07 09_07.cpp)
add_executable(task5_09_08 09_08.cpp)
add_executable(task5_09_09 09_09.cpp)
add_executable(task5_10_01 10_01.cpp)
add_executable(task5_10_03 10_03.cpp)
add_executable(task5_10_04 10_04.cpp)
add_executable(task5_10_05 10_05.cpp)
add_executable(task5_10_06 10_06.cpp)
add_executable(task5_11_01 11_01.cpp)
add_executable(task5_11_03 11_03.cpp)

foreach(target
    task5_09_01
    task5_09_02
    task5_09_03
    task5_09_04
    task5_09_05
    task5_09_06
    task5_09_07
    task5_09_08
    task5_09_09
    task5_10_01
    task5_10_03
    task5_10_04
    task5_10_05
    task5_10_06
    task5_11_01
    task5_11_03
)
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
endforeach()

target_link_libraries(task5_09_06 PRIVATE Boost::filesystem Boost::system)
target_link_libraries(task5_10_06 PRIVATE Boost::filesystem Boost::system)
target_link_libraries(task5_09_09 PRIVATE benchmark::benchmark)
