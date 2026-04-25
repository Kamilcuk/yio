# Yio Benchmarking

This directory contains performance benchmarks comparing the Yio library against standard glibc `printf` family functions.

## Methodology
- **Framework**: [Google Benchmark](https://github.com/google/benchmark) (C++).
- **C-Wrappers**: Bridge Yio's C11 `_Generic` macros to C++.
- **Repetitions**: Each test runs 10 times to calculate **Mean**, **Median**, and **StdDev**.
- **Isolation**: Formats into stack buffers to avoid I/O noise.
- **Determinism**: Pinned to the last CPU core (`taskset`) and uses a deterministic Xorshift RNG for values.
- **Hardware Counters**: Attempting to use `libpfm4` for `CYCLES` and `INSTRUCTIONS` (if supported by environment).

## Building and Running

```bash
# Setup environment for hardware counters (optional, requires sudo)
make benchmark_setup

# Run benchmarks
make benchmark
```

### Note on WSL2
Hardware performance counters (`CYCLES`, `INSTRUCTIONS`) generally **do not work** in WSL2 because the PMU (Performance Monitoring Unit) is not virtualized by default. On WSL2, you will only see `Time` and `CPU` columns. For full metrics, run on a native Linux host.

## Current Observations (2026-04-25)

| Type | Libc (ns) | Yio (ns) | Status/Notes |
|------|-----------|----------|--------------|
| **int** | ~38.5 | ~39.1 | Equivalent. |
| **char** | ~13.7 | ~23.1 | Glibc is faster (direct copy). |
| **uint8_t** | ~27.0 | ~27.6 | Equivalent. |
| **uint16_t** | ~26.4 | ~27.8 | Equivalent. |
| **int16_t** | ~32.9 | ~35.9 | Comparable. |
| **long** | ~39.7 | ~45.6 | Comparable. |
| **ullong** | ~45.9 | ~43.1 | **Yio is slightly faster.** |
| **int128** | N/A | ~249 | Native Yio support. |
| **uint128** | N/A | ~244 | Native Yio support. |
| **float** | ~89.0 | **FAILED** | Missing `strfrom` in environment. |
| **double** | ~90.1 | **FAILED** | Missing `strfrom` in environment. |
| **ldouble** | ~98.5 | **FAILED** | Missing `strfrom` in environment. |
| **string** | ~19.5 | ~25.4 | ~30% overhead. |
| **wchar_t** | ~27.5 | ~25.0 | **Yio is faster.** |
| **wchar_t*** | ~66.5 | ~374 | Yio handles UTF-8 conversion. |

*Note: Results are averages (mean) from 10 repetitions.*

### Note on Failures
The benchmark returns exit code 1 if any test fails. Currently, floating-point tests fail because the environment lacks the `strfrom` implementation in `libm`.
