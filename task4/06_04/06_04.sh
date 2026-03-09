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

mkdir -p "${LOG_DIR}"

echo "[1/4] Building library_v1"
if ! cmake -S "${EDUCATION_DIR}/projects/library_v1" -B "${EDUCATION_DIR}/projects/library_v1/output"; then
    echo "library_v1 configure failed" | tee "${LOG_DIR}/library_v1_error.log"
fi
if ! cmake --build "${EDUCATION_DIR}/projects/library_v1/output" -j; then
    echo "library_v1 build failed" | tee "${LOG_DIR}/library_v1_error.log"
fi

echo "[2/4] Building library_v2"
if ! cmake -S "${EDUCATION_DIR}/projects/library_v2" -B "${EDUCATION_DIR}/projects/library_v2/output"; then
    echo "library_v2 configure failed" | tee "${LOG_DIR}/library_v2_error.log"
fi
if ! cmake --build "${EDUCATION_DIR}/projects/library_v2/output" -j; then
    echo "library_v2 build failed" | tee "${LOG_DIR}/library_v2_error.log"
fi

mkdir -p "${EDUCATION_DIR}/libraries"
cp -f "${EDUCATION_DIR}/projects/library_v1/output/libstatic.a" "${EDUCATION_DIR}/libraries/" || true
cp -f "${EDUCATION_DIR}/projects/library_v2/output/libshared.so" "${EDUCATION_DIR}/libraries/" || true

echo "[3/4] Building examples via Education/education.sh (if dependencies are installed)"
(
    cd "${EDUCATION_DIR}"
    bash ./education.sh
) >"${LOG_DIR}/education_build.log" 2>&1 || true

echo "[4/4] Saving directory listings instead of screenshots"
(
    cd "${EDUCATION_DIR}"
    ls -la libraries >"${LOG_DIR}/libraries_ls.txt"
    ls -la projects/library_v1/output >"${LOG_DIR}/library_v1_output_ls.txt"
    ls -la projects/library_v2/output >"${LOG_DIR}/library_v2_output_ls.txt"
    ls -la projects/examples/output >"${LOG_DIR}/examples_output_ls.txt" || true
)

echo "06.04 helper completed."
echo "See artifacts in: ${LOG_DIR}"
