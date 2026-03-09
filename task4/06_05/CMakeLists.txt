cmake_minimum_required(VERSION 3.20)

project(task4_06_05 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS filesystem system)

add_library(task4_plugin_v1 SHARED plugin_v1.cpp)
add_library(task4_plugin_v2 SHARED plugin_v2.cpp)

add_executable(task4_06_05 main.cpp)
target_link_libraries(task4_06_05 PRIVATE Boost::filesystem Boost::system)

if(UNIX AND NOT APPLE)
    target_link_libraries(task4_06_05 PRIVATE dl)
endif()

foreach(target task4_plugin_v1 task4_plugin_v2 task4_06_05)
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
endforeach()
