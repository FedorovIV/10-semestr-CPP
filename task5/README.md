# Task 5 Solutions

## Covered Tasks
- `09.01` to `09.09`
- `09.10` is empty in the provided PDF, so there is no implementation for it
- `10.01`
- `10.03` to `10.06`
- `11.01`
- `11.03`

## What You Should Learn
- How RAII, `std::source_location`, smart pointers, custom allocation and Pimpl work in practice.
- How to write iterator-based algorithms and custom iterators, including a Boost.Iterator facade.
- How to inspect container growth behavior and implement specialized stack and graph algorithms.
- How to benchmark memory-management strategies and reason about algorithmic complexity.

## Short Theory Summary
- RAII ties resource lifetime to object lifetime and makes cleanup deterministic.
- `shared_ptr` and `weak_ptr` help model ownership graphs without leaks from cyclic references.
- Iterator-based code is more generic than index-based code and naturally extends to more containers.
- Inline-storage Pimpl trades heap allocations for a fixed in-object storage block and can reduce overhead.
- `std::vector` typically grows geometrically; the exact factor depends on the implementation.
- Longest common substring uses dynamic programming with contiguous matches only.
- Matrix exponentiation computes Fibonacci numbers in `O(log N)` multiplications.
- Brute-force TSP for 10 vertices is still feasible with `std::next_permutation` if one start vertex is fixed.

## Build And Run
Use WSL from the repository root:

```bash
./Problems/task5/run_all.sh
```
