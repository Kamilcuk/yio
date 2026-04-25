cmake_minimum_required(VERSION 3.14)

include(FetchContent)

# Benchmark configuration
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark testing")
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Disable benchmark installation")
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "Disable benchmark gtest tests")
set(BENCHMARK_ENABLE_LIBPFM ON CACHE BOOL "Enable libpfm support")

FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.8.3
)

FetchContent_MakeAvailable(googlebenchmark)

add_definitions(-DBENCHMARK_ENABLE_LIBPFM=1)

# Wrapper library (C)
add_library(yio_bench_wrappers STATIC
    yio_wrappers.c
    yio_wrappers.h
)
target_include_directories(yio_bench_wrappers PRIVATE ${GENDIR})
# Linking against yio and compatlib
target_link_libraries(yio_bench_wrappers PRIVATE yio yio_compat m)

# Benchmark executable (C++)
add_executable(yio_benchmark yio_benchmark.cpp)
target_link_libraries(yio_benchmark PRIVATE
    benchmark::benchmark
    yio_bench_wrappers
    yio
    yio_compat
    m
    pfm
)

# Use GC sections to match tests and handle optional backends like Ryu
if(CMAKE_C_COMPILER_ID MATCHES "^(GNU|Clang)$")
    target_compile_options(yio_benchmark PRIVATE -ffunction-sections -fdata-sections)
    if(APPLE)
        target_link_options(yio_benchmark PRIVATE -Wl,-dead_strip)
    else()
        target_link_options(yio_benchmark PRIVATE -Wl,--gc-sections)
    endif()
endif()

# Release mode is highly recommended for benchmarks
if(NOT CMAKE_BUILD_TYPE MATCHES "Release|RelWithDebInfo")
    message(WARNING "Benchmarks should be run in Release mode for accurate results. Current mode: ${CMAKE_BUILD_TYPE}")
endif()
