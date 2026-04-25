#include <benchmark/benchmark.h>
#include "yio_wrappers.h"
#include <vector>
#include <string>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <cmath>
#include <limits>
#include <atomic>

#if __has_include(<perfmon/pfmlib.h>)
#include <perfmon/pfmlib.h>
#endif

std::atomic<bool> g_any_benchmark_failed{false};

// Simple deterministic RNG
struct RNG {
    uint64_t state = 0x123456789ABCDEF0ULL;
    uint64_t next() {
        uint64_t x = state;
        x ^= x << 13;
        x ^= x >> 7;
        x ^= x << 17;
        return state = x;
    }
    double next_double() {
        uint64_t v = next();
        // Check for edge cases roughly every 100 calls
        if ((v % 100) == 0) {
            switch (v % 4) {
                case 0: return std::numeric_limits<double>::infinity();
                case 1: return -std::numeric_limits<double>::infinity();
                case 2: return std::numeric_limits<double>::quiet_NaN();
                case 3: return 0.0;
            }
        }
        return (double)v / (double)std::numeric_limits<uint64_t>::max();
    }
};

#define CHECK_RES(res, name) \
    if ((res) < 0) { \
        g_any_benchmark_failed = true; \
        char errbuf[256]; \
        snprintf(errbuf, sizeof(errbuf), "%s failed: %s (errno=%d)", (name), wrap_yio_strerror(res), errno); \
        state.SkipWithError(errbuf); \
        break; \
    }

#ifdef BENCHMARK_HAS_PERF_COUNTERS
#define REGISTER_BM(name) \
    BENCHMARK(name)->Iterations(1000000)->PerfCounters({"CYCLES", "INSTRUCTIONS"})
#else
#define REGISTER_BM(name) \
    BENCHMARK(name)->Iterations(1000000)
#endif

// --- Integers ---

static void BM_LibcInt(benchmark::State& state) {
  char buf[64]; RNG rng;
  for (auto _ : state) {
    int res = wrap_libc_int(buf, sizeof(buf), (int)rng.next());
    CHECK_RES(res, "libc_int");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_LibcInt);

static void BM_YioInt(benchmark::State& state) {
  char buf[64]; RNG rng;
  for (auto _ : state) {
    int res = wrap_yio_int(buf, sizeof(buf), (int)rng.next());
    CHECK_RES(res, "yio_int");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_YioInt);

// --- i8 / u8 / char ---

static void BM_LibcChar(benchmark::State& state) {
  char buf[64]; RNG rng;
  for (auto _ : state) {
    int res = wrap_libc_char(buf, sizeof(buf), (char)rng.next());
    CHECK_RES(res, "libc_char");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_LibcChar);

static void BM_YioChar(benchmark::State& state) {
  char buf[64]; RNG rng;
  for (auto _ : state) {
    int res = wrap_yio_char(buf, sizeof(buf), (char)rng.next());
    CHECK_RES(res, "yio_char");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_YioChar);

static void BM_LibcU8(benchmark::State& state) {
  char buf[64]; RNG rng;
  for (auto _ : state) {
    int res = wrap_libc_u8(buf, sizeof(buf), (uint8_t)rng.next());
    CHECK_RES(res, "libc_u8");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_LibcU8);

static void BM_YioU8(benchmark::State& state) {
  char buf[64]; RNG rng;
  for (auto _ : state) {
    int res = wrap_yio_u8(buf, sizeof(buf), (uint8_t)rng.next());
    CHECK_RES(res, "yio_u8");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_YioU8);

// --- i16 / u16 ---

static void BM_LibcU16(benchmark::State& state) {
  char buf[64]; RNG rng;
  for (auto _ : state) {
    int res = wrap_libc_u16(buf, sizeof(buf), (uint16_t)rng.next());
    CHECK_RES(res, "libc_u16");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_LibcU16);

static void BM_YioU16(benchmark::State& state) {
  char buf[64]; RNG rng;
  for (auto _ : state) {
    int res = wrap_yio_u16(buf, sizeof(buf), (uint16_t)rng.next());
    CHECK_RES(res, "yio_u16");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_YioU16);

static void BM_LibcI16(benchmark::State& state) {
  char buf[64]; RNG rng;
  for (auto _ : state) {
    int res = wrap_libc_i16(buf, sizeof(buf), (int16_t)rng.next());
    CHECK_RES(res, "libc_i16");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_LibcI16);

static void BM_YioI16(benchmark::State& state) {
  char buf[64]; RNG rng;
  for (auto _ : state) {
    int res = wrap_yio_i16(buf, sizeof(buf), (int16_t)rng.next());
    CHECK_RES(res, "yio_i16");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_YioI16);

// --- long / ull ---

static void BM_LibcLong(benchmark::State& state) {
  char buf[64]; RNG rng;
  for (auto _ : state) {
    int res = wrap_libc_long(buf, sizeof(buf), (long)rng.next());
    CHECK_RES(res, "libc_long");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_LibcLong);

static void BM_YioLong(benchmark::State& state) {
  char buf[64]; RNG rng;
  for (auto _ : state) {
    int res = wrap_yio_long(buf, sizeof(buf), (long)rng.next());
    CHECK_RES(res, "yio_long");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_YioLong);

static void BM_LibcUll(benchmark::State& state) {
  char buf[64]; RNG rng;
  for (auto _ : state) {
    int res = wrap_libc_ull(buf, sizeof(buf), (unsigned long long)rng.next());
    CHECK_RES(res, "libc_ull");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_LibcUll);

static void BM_YioUll(benchmark::State& state) {
  char buf[64]; RNG rng;
  for (auto _ : state) {
    int res = wrap_yio_ull(buf, sizeof(buf), (unsigned long long)rng.next());
    CHECK_RES(res, "yio_ull");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_YioUll);

// --- int128 ---

#ifdef __SIZEOF_INT128__
static void BM_YioI128(benchmark::State& state) {
  char buf[128]; RNG rng;
  for (auto _ : state) {
    int128_t val = ((int128_t)rng.next() << 64) | rng.next();
    int res = wrap_yio_i128(buf, sizeof(buf), val);
    CHECK_RES(res, "yio_i128");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_YioI128);

static void BM_YioU128(benchmark::State& state) {
  char buf[128]; RNG rng;
  for (auto _ : state) {
    uint128_t val = ((uint128_t)rng.next() << 64) | rng.next();
    int res = wrap_yio_u128(buf, sizeof(buf), val);
    CHECK_RES(res, "yio_u128");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_YioU128);
#endif

// --- Floats ---

static void BM_LibcFloat(benchmark::State& state) {
  char buf[128]; RNG rng;
  for (auto _ : state) {
    int res = wrap_libc_float(buf, sizeof(buf), (float)rng.next_double());
    CHECK_RES(res, "libc_float");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_LibcFloat);

static void BM_YioFloat(benchmark::State& state) {
  char buf[128]; RNG rng;
  for (auto _ : state) {
    int res = wrap_yio_float(buf, sizeof(buf), (float)rng.next_double());
    CHECK_RES(res, "yio_float");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_YioFloat);

static void BM_LibcDouble(benchmark::State& state) {
  char buf[128]; RNG rng;
  for (auto _ : state) {
    int res = wrap_libc_double(buf, sizeof(buf), rng.next_double());
    CHECK_RES(res, "libc_double");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_LibcDouble);

static void BM_YioDouble(benchmark::State& state) {
  char buf[128]; RNG rng;
  for (auto _ : state) {
    int res = wrap_yio_double(buf, sizeof(buf), rng.next_double());
    CHECK_RES(res, "yio_double");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_YioDouble);

static void BM_LibcLongDouble(benchmark::State& state) {
  char buf[256]; RNG rng;
  for (auto _ : state) {
    int res = wrap_libc_ldouble(buf, sizeof(buf), (long double)rng.next_double());
    CHECK_RES(res, "libc_ldouble");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_LibcLongDouble);

static void BM_YioLongDouble(benchmark::State& state) {
  char buf[256]; RNG rng;
  for (auto _ : state) {
    int res = wrap_yio_ldouble(buf, sizeof(buf), (long double)rng.next_double());
    CHECK_RES(res, "yio_ldouble");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_YioLongDouble);

// --- Strings ---

static void BM_LibcStr(benchmark::State& state) {
  char buf[256];
  const char* s = "The quick brown fox jumps over the lazy dog";
  for (auto _ : state) {
    int res = wrap_libc_str(buf, sizeof(buf), s);
    CHECK_RES(res, "libc_str");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_LibcStr);

static void BM_YioStr(benchmark::State& state) {
  char buf[256];
  const char* s = "The quick brown fox jumps over the lazy dog";
  for (auto _ : state) {
    int res = wrap_yio_str(buf, sizeof(buf), s);
    CHECK_RES(res, "yio_str");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_YioStr);

// --- Wide Characters ---

static void BM_LibcWChar(benchmark::State& state) {
  char buf[64];
  for (auto _ : state) {
    int res = wrap_libc_wchar(buf, sizeof(buf), L'A');
    CHECK_RES(res, "libc_wchar");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_LibcWChar);

static void BM_YioWChar(benchmark::State& state) {
  char buf[64];
  for (auto _ : state) {
    int res = wrap_yio_wchar(buf, sizeof(buf), L'A');
    CHECK_RES(res, "yio_wchar");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_YioWChar);

static void BM_LibcWStr(benchmark::State& state) {
  char buf[256];
  const wchar_t* s = L"The quick brown fox jumps over the lazy dog";
  for (auto _ : state) {
    int res = wrap_libc_wstr(buf, sizeof(buf), s);
    CHECK_RES(res, "libc_wstr");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_LibcWStr);

static void BM_YioWStr(benchmark::State& state) {
  char buf[256];
  const wchar_t* s = L"The quick brown fox jumps over the lazy dog";
  for (auto _ : state) {
    int res = wrap_yio_wstr(buf, sizeof(buf), s);
    CHECK_RES(res, "yio_wstr");
    benchmark::DoNotOptimize(res);
  }
}
REGISTER_BM(BM_YioWStr);

int main(int argc, char** argv) {
    setlocale(LC_ALL, "en_US.UTF-8");
#ifdef BENCHMARK_ENABLE_LIBPFM
    if (pfm_initialize() != PFM_SUCCESS) {
        fprintf(stderr, "FATAL: libpfm initialization failed. Hardware counters are required but unavailable.\n");
        fprintf(stderr, "Note: Hardware PMU is often unavailable in WSL2 or restricted by kernel.perf_event_paranoid.\n");
        return 1;
    }
#else
#error "BENCHMARK_ENABLE_LIBPFM must be defined. Hardware counters are required for this benchmark."
#endif
    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    benchmark::RunSpecifiedBenchmarks();
    return g_any_benchmark_failed ? 1 : 0;
}
