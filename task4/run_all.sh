#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

cmake -S "${SCRIPT_DIR}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release
cmake --build "${BUILD_DIR}" -j

"${BUILD_DIR}/task4_05_05"
printf "1 -3 2\n" | "${BUILD_DIR}/task4_07_01"
"${BUILD_DIR}/task4_07_02"
"${BUILD_DIR}/task4_07_03"
"${BUILD_DIR}/task4_07_04"
"${BUILD_DIR}/task4_07_05" --benchmark_min_time=0.01s --benchmark_repetitions=1
"${BUILD_DIR}/task4_08_01"
"${BUILD_DIR}/task4_08_02"
"${BUILD_DIR}/task4_08_03"
"${BUILD_DIR}/task4_08_04"
"${BUILD_DIR}/task4_08_05"

if [[ "$OSTYPE" == "darwin"* ]]; then
    LIB1="libtask4_plugin_v1.dylib"
    LIB2="libtask4_plugin_v2.dylib"
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    LIB1="task4_plugin_v1.dll"
    LIB2="task4_plugin_v2.dll"
else
    LIB1="libtask4_plugin_v1.so"
    LIB2="libtask4_plugin_v2.so"
fi

printf "%s\n" "${LIB1}" | "${BUILD_DIR}/06_05/task4_06_05"
printf "%s\n" "${LIB2}" | "${BUILD_DIR}/06_05/task4_06_05"

echo "All task4 checks completed."
