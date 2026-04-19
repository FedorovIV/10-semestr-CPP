# Task 5 Solutions

## Covered Tasks
- `09.01` to `09.10`
- `10.01`
- `10.03` to `10.06`
- `11.01`
- `11.03`

## What You Should Learn
- How RAII, `std::source_location`, smart pointers, custom allocation and Pimpl work in practice.
- How to write iterator-based algorithms and custom iterators, including a Boost.Iterator facade.
- How to inspect container growth behavior and implement specialized stack and graph algorithms.
- How to benchmark memory-management strategies and reason about algorithmic complexity.
- How to compare several string hash functions on the same random dataset and analyze collision growth.
- How to keep a generic sorting algorithm reusable by passing a comparator as a template argument.

## Short Theory Summary
- RAII ties resource lifetime to object lifetime and makes cleanup deterministic.
- `shared_ptr` and `weak_ptr` help model ownership graphs without leaks from cyclic references.
- Iterator-based code is more generic than index-based code and naturally extends to more containers.
- Inline-storage Pimpl trades heap allocations for a fixed in-object storage block and can reduce overhead.
- `std::vector` typically grows geometrically; the exact factor depends on the implementation.
- Collision curves for 32-bit hashes stay low at first and then bend upward because the expected number of repeated values grows roughly quadratically with the number of inserted keys.
- To compare hash functions fairly, the input dataset should be generated once and then reused for every function.
- Matrix exponentiation computes Fibonacci numbers in `O(log N)` multiplications.
- Comparator-based algorithms stay generic as long as the comparator defines a strict weak ordering.

## Build And Run
Use WSL from the repository root:

```bash
./Problems/task5/run_all.sh
```

## Generated Artifacts
- `10_05_collisions.csv`
- `10_05_collisions.png`
- `10_05_results.md`

`task5_10_05` enables `-O3` unconditionally and also enables `-m32` when the local toolchain has the required 32-bit runtime.
