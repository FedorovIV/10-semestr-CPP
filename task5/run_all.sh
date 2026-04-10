#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++-14
cmake --build "${BUILD_DIR}" -j

"${BUILD_DIR}/task5_09_01"
"${BUILD_DIR}/task5_09_02"
"${BUILD_DIR}/task5_09_03"
"${BUILD_DIR}/task5_09_04"
"${BUILD_DIR}/task5_09_05"
"${BUILD_DIR}/task5_09_06"
"${BUILD_DIR}/task5_09_07"
"${BUILD_DIR}/task5_09_08"
"${BUILD_DIR}/task5_09_09" --benchmark_min_time=0.01s --benchmark_repetitions=1
"${BUILD_DIR}/task5_09_10"
"${BUILD_DIR}/task5_10_01"
"${BUILD_DIR}/task5_10_03"
"${BUILD_DIR}/task5_10_04"
"${BUILD_DIR}/task5_10_05"
"${BUILD_DIR}/task5_10_06"
"${BUILD_DIR}/task5_11_01"
"${BUILD_DIR}/task5_11_03"

echo "All task5 checks completed."
