#!/usr/bin/env bash
set -uo pipefail

# Task 06.04 automation helper.
# The script is intended for Linux/WSL and expects the repository layout:
#   <repo>/Education
#   <repo>/Problems/task4/06_04/06_04.sh

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/../../.." && pwd)"
EDUCATION_DIR="${REPO_ROOT}/Education"
LOG_DIR="${SCRIPT_DIR}/artifacts"
STEP_STATUS_FILE="${LOG_DIR}/status.txt"

# Prefer GCC 14 for <print> support in C++23 examples.
if command -v g++-14 >/dev/null 2>&1; then
    export CC="${CC:-gcc-14}"
    export CXX="${CXX:-g++-14}"
fi

# Optional: point CMake to custom Boost installation (for example 1.85).
if [[ -n "${BOOST_ROOT:-}" ]]; then
    export CMAKE_PREFIX_PATH="${BOOST_ROOT}:${CMAKE_PREFIX_PATH:-}"
fi

mkdir -p "${LOG_DIR}"
: > "${STEP_STATUS_FILE}"

cmake_args=()
if [[ -n "${CC:-}" ]]; then
    cmake_args+=("-DCMAKE_C_COMPILER=${CC}")
fi
if [[ -n "${CXX:-}" ]]; then
    cmake_args+=("-DCMAKE_CXX_COMPILER=${CXX}")
fi
if [[ -n "${CMAKE_PREFIX_PATH:-}" ]]; then
    cmake_args+=("-DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")
fi

boost_header="/usr/include/boost/version.hpp"
if [[ -n "${BOOST_ROOT:-}" && -f "${BOOST_ROOT}/include/boost/version.hpp" ]]; then
    boost_header="${BOOST_ROOT}/include/boost/version.hpp"
fi

boost_version="unknown"
if [[ -f "${boost_header}" ]]; then
    boost_version="$(awk '/#define BOOST_LIB_VERSION/{gsub(/"/, "", $3); gsub(/_/, ".", $3); print $3}' "${boost_header}")"
fi

have_boost_185=0
if [[ "${boost_version}" != "unknown" ]]; then
    if printf '%s\n%s\n' "1.85.0" "${boost_version}" | sort -C -V; then
        have_boost_185=1
    fi
fi

echo "[1/4] Building library_v1"
rm -rf "${EDUCATION_DIR}/projects/library_v1/output"
if ! cmake -S "${EDUCATION_DIR}/projects/library_v1" -B "${EDUCATION_DIR}/projects/library_v1/output" \
    "${cmake_args[@]}" >"${LOG_DIR}/library_v1_configure.log" 2>&1; then
    echo "library_v1: configure failed (see artifacts/library_v1_configure.log)" | tee -a "${STEP_STATUS_FILE}"
elif ! cmake --build "${EDUCATION_DIR}/projects/library_v1/output" -j \
    >"${LOG_DIR}/library_v1_build.log" 2>&1; then
    echo "library_v1: build failed (see artifacts/library_v1_build.log)" | tee -a "${STEP_STATUS_FILE}"
else
    echo "library_v1: OK" | tee -a "${STEP_STATUS_FILE}"
fi

echo "[2/4] Building library_v2"
if [[ "${have_boost_185}" -eq 0 ]]; then
    echo "library_v2: skipped (Boost ${boost_version}, need >= 1.85.0; set BOOST_ROOT to Boost 1.85+)" \
        | tee -a "${STEP_STATUS_FILE}"
else
    rm -rf "${EDUCATION_DIR}/projects/library_v2/output"
    if ! cmake -S "${EDUCATION_DIR}/projects/library_v2" -B "${EDUCATION_DIR}/projects/library_v2/output" \
        "${cmake_args[@]}" >"${LOG_DIR}/library_v2_configure.log" 2>&1; then
        echo "library_v2: configure failed (see artifacts/library_v2_configure.log)" | tee -a "${STEP_STATUS_FILE}"
    elif ! cmake --build "${EDUCATION_DIR}/projects/library_v2/output" -j \
        >"${LOG_DIR}/library_v2_build.log" 2>&1; then
        echo "library_v2: build failed (see artifacts/library_v2_build.log)" | tee -a "${STEP_STATUS_FILE}"
    else
        echo "library_v2: OK" | tee -a "${STEP_STATUS_FILE}"
    fi
fi

mkdir -p "${EDUCATION_DIR}/libraries"
cp -f "${EDUCATION_DIR}/projects/library_v1/output/libstatic.a" "${EDUCATION_DIR}/libraries/" 2>/dev/null || true
cp -f "${EDUCATION_DIR}/projects/library_v2/output/libshared.so" "${EDUCATION_DIR}/libraries/" 2>/dev/null || true

echo "[3/4] Building examples via Education/education.sh (if dependencies are installed)"
(
    cd "${EDUCATION_DIR}"
    bash ./education.sh
) >"${LOG_DIR}/education_build.log" 2>&1 || true
echo "education.sh: completed with possible partial failures (see artifacts/education_build.log)" \
    | tee -a "${STEP_STATUS_FILE}"

echo "[4/4] Saving directory listings instead of screenshots"
(
    cd "${EDUCATION_DIR}"
    ls -la libraries >"${LOG_DIR}/libraries_ls.txt" 2>/dev/null || true
    ls -la projects/library_v1/output >"${LOG_DIR}/library_v1_output_ls.txt" 2>/dev/null || true
    ls -la projects/library_v2/output >"${LOG_DIR}/library_v2_output_ls.txt" 2>/dev/null || true
    ls -la projects/examples/output >"${LOG_DIR}/examples_output_ls.txt" 2>/dev/null || true
)
echo "directory snapshots: saved" | tee -a "${STEP_STATUS_FILE}"

echo "06.04 helper completed."
echo "See artifacts in: ${LOG_DIR}"
echo "Summary:"
cat "${STEP_STATUS_FILE}"
