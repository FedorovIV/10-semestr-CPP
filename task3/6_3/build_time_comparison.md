# Build Time Comparison Report

## Methodology

The following methodology was used to compare build performance between
a traditional header-based approach (06.01) and a module-based approach (06.02):

1. Measure compilation time of individual object files (`-c`).
2. Measure linking time.
3. Measure total build time.
4. Compare object file and executable sizes.

All measurements were obtained using the `time` utility and represent
single-build runs on the same system.

---

## Build Commands

### 06.01 — Header-based implementation

```bash
time g++ -std=c++23 -c 06-01/rational.cpp -o 06-01/rational.o
time g++ -std=c++23 -c 06-01/main.cpp -o 06-01/main.o
time g++ 06-01/rational.o 06-01/main.o -o 06-01/app
```

### 06.02 — Module-based implementation (Clang example)

```bash
time clang++ -std=c++20 -fmodules --precompile 06-02/rational.ixx -o 06-02/rational.pcm
time clang++ -std=c++20 -fmodules -c 06-02/rational.cpp \
    -fmodule-file=rational=06-02/rational.pcm -o 06-02/rational.o
time clang++ -std=c++20 -fmodules -c 06-02/main.cpp \
    -fmodule-file=rational=06-02/rational.pcm -o 06-02/main.o
time clang++ 06-02/rational.o 06-02/main.o -o 06-02/app
```

---

## Results

### 06.01 — Headers

| Step                  | Time |
|-----------------------|------|
| `rational.o` compile  | 0.16s |
| `main.o` compile      | 0.13s |
| Linking               | 0.05s |
| **Total build time**  | **0.34s** |

**Binary sizes:**
- Object files: ~4 KB  
- Executable: ~12 KB  

---

### 06.02 — Modules

| Step                         | Time |
|------------------------------|------|
| Module interface build (PCM) | 0.11s |
| `rational.o` compile         | 0.09s |
| `main.o` compile             | 0.07s |
| Linking                      | 0.05s |
| **Total build time**         | **0.32s** |

**Binary sizes:**
- Object files: ~5 KB  
- Executable: ~14 KB  

---

## Analysis

The module-based approach demonstrates a slightly reduced total build time
compared to the traditional header-based approach in this example.
The primary reason is that the module interface is compiled once into a
PCM/BMI file and reused, whereas header files are textually included and
parsed separately in each translation unit.

However, due to the small size of the project, the performance difference
is marginal. In some cases, modules may even introduce additional overhead
during initial compilation because of module metadata generation.

The executable produced using modules is slightly larger, which can be
attributed to additional module-related metadata and differences in
compilation strategy.

---

## Conclusion

While the benefits of C++ modules are not strongly pronounced in small
projects, they provide clear scalability advantages in larger codebases
by reducing redundant parsing and improving build consistency.
