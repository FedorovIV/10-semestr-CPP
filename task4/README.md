# Task 4 Solutions (C++)

## What You Should Learn
- How to build reusable arithmetic operators with CRTP mixins (`05.05`).
- How to structure robust error handling with custom and standard exceptions (`07.02`).
- How to model complex return values with `std::optional` + `std::variant` (`07.01`).
- How to reason about normal control flow and exception flow in real code (`07.03`).
- How to test algorithms with `GoogleTest` (`07.04`).
- How to profile algorithms with `GoogleBenchmark` and tunable parameters (`07.05`).
- How object layout can be abused (`reinterpret_cast`, raw memory patching) (`08.01`).
- How to extend big integer arithmetic with `%`, `pow`, `sign`, `abs` (`08.02`).
- How to work with integer/float bit-level representations (`08.03`).
- How to implement a stochastic optimization process (Dawkins weasel) (`08.04`).
- How to design a multi-measurement timer with average statistics (`08.05`).
- How to load function implementations from dynamic libraries at runtime (`06.05`).
- How to automate project-wide builds and collect build artifacts (`06.04`).

## Short Theory Summary
- **Mixins and CRTP**: put shared operators in base templates and reuse logic from `+=`, `-=`, `*=`, `/=` without copy-paste.
- **Exceptions**: throw early on invalid state (for example, zero denominator), catch at system boundaries, and report meaningful diagnostics.
- **`optional` + `variant`**: model "no result" separately from "different valid result shapes".
- **Testing**: automated tests should validate correctness on edge cases and random data.
- **Benchmarking**: performance is workload-dependent; parameterized microbenchmarks help find practical thresholds.
- **Bit-level numeric logic**: `int` logarithm is highest set bit index; `float` logarithm is derived from IEEE 754 exponent and mantissa.
- **Randomized search**: many weak random mutations + selection can converge quickly to a target string.
- **Dynamic loading**: plugin architecture allows switching implementations at runtime without rebuilding the executable.

## Build and Run
From repository root (WSL/Linux):

```bash
cmake -S Problems/task4 -B Problems/task4/build -DCMAKE_BUILD_TYPE=Release
cmake --build Problems/task4/build -j
```

Run selected binaries:

```bash
./Problems/task4/build/task4_05_05
./Problems/task4/build/task4_07_01
./Problems/task4/build/task4_07_02
./Problems/task4/build/task4_07_03
./Problems/task4/build/task4_07_04
./Problems/task4/build/task4_07_05 --benchmark_min_time=0.01
./Problems/task4/build/task4_08_01
./Problems/task4/build/task4_08_02
./Problems/task4/build/task4_08_03
./Problems/task4/build/task4_08_04
./Problems/task4/build/task4_08_05
```

For `06.04`:
- script: `Problems/task4/06_04/06_04.sh`
- report: `Problems/task4/06_04/06_04_report.md`

For `06.05`:
- CMake project in `Problems/task4/06_05`
- executable `task4_06_05` + two shared libraries
