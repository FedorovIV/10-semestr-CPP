# 06.04 Build Automation Report

## What Was Automated
- Build `Education/projects/library_v1`.
- Build `Education/projects/library_v2`.
- Copy produced libraries to `Education/libraries`.
- Run `Education/education.sh` and save full terminal output to a log file.
- Save directory listings for:
  - `Education/libraries`
  - `Education/projects/library_v1/output`
  - `Education/projects/library_v2/output`
  - `Education/projects/examples/output`

## Script
- Path: `Problems/task4/06_04/06_04.sh`
- Run from repository root:
  - `bash Problems/task4/06_04/06_04.sh`
- The script now prints a short summary and always stores detailed logs in `artifacts/`.
- It also forces fresh CMake configure for `library_v1`/`library_v2` and prefers `g++-14` automatically if available.

## Artifacts
- The script stores text artifacts in `Problems/task4/06_04/artifacts`.
- Files `*_ls.txt` can be used as textual proof for the same data that is usually shown in screenshots.

## Notes
- Task 06.04 depends on system-wide packages (Boost 1.85, CMake >= 3.28, TBB, Google.Test, Google.Benchmark, and others).
- If some dependencies are missing, `education.sh` can fail; the script still keeps a complete log in `education_build.log` for debugging.

## Current Run Result In This Environment
- `library_v1` is built successfully (`g++-14` is used automatically).
- `library_v2` is skipped because local Boost version is `1.83.0`, while project requires `>= 1.85`.
- All details are saved in:
  - `artifacts/library_v1_configure.log`
  - `artifacts/library_v1_build.log`
  - `artifacts/library_v2_configure.log` (when configured)
  - `artifacts/library_v2_build.log` (when built)
  - `artifacts/education_build.log`
