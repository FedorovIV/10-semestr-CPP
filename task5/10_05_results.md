# 10.05 Results

- Dataset: 1048576 unique random strings.
- String format: fixed length 10, alphabet a-z.
- Hash functions: RS, JS, PJW, ELF, BKDR, SDBM, DJB, DEK, AP.
- Build mode: -O3 and -m32.

## Final ranking

- SDBM: final collisions = 113, cumulative collisions = 2354
- JS: final collisions = 116, cumulative collisions = 2744
- DJB: final collisions = 134, cumulative collisions = 3139
- AP: final collisions = 135, cumulative collisions = 3099
- RS: final collisions = 145, cumulative collisions = 3043
- BKDR: final collisions = 161, cumulative collisions = 3384
- DEK: final collisions = 226, cumulative collisions = 5332
- ELF: final collisions = 2738, cumulative collisions = 60494
- PJW: final collisions = 2738, cumulative collisions = 60494

## Conclusion

- Best function in this run: SDBM.
- Worst function in this run: PJW.
- The curves grow slowly at first and then bend upward because collisions in a nearly uniform 32-bit hash space grow approximately like n * (n - 1) / (2 * 2^32).
- Functions with flatter curves distribute values more evenly on this dataset, while functions with earlier or steeper growth produce more repeated hash values.
