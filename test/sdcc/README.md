# SDCC Testing Infrastructure

This directory contains tests specifically targeting the SDCC (Small Device C Compiler) environment.

## Structure

- `conftest.h` / `conftest.c`: Provides the test runner infrastructure for SDCC, including `putchar` implementation for simulators, `abort` logic, and basic `YIO_TEST` macros.
- `sdcc_conftest` library: A static library built from `conftest.c` that every test in this directory links against.
- `test_*.c`: Individual test files. These are automatically discovered using a glob pattern.

## Behavior

1. **Dual Compatibility**: These tests are designed to pass on both x86 (using `sstest`) and SDCC.
2. **Entry Point**: Tests should implement `int libtest_main(void)` instead of `main()`. The `conftest` layer handles the appropriate wrapping for each platform.
3. **Execution**:
   - When building for SDCC (`YIO_SDCC` enabled), only the tests in this directory are included.
   - When building for other platforms (like x86), these tests are included alongside the standard test suite.
4. **SDCC Specifics**:
   - `putchar` is redirected to a simulator-specific control register (`0xffff`).
   - `abort` triggers a simulator breakpoint via an illegal opcode.
   - A watchdog counter in `putchar` prevents infinite loops in the simulator.

## Adding Tests

Simply create a new file named `test_<name>.c` in this directory and implement `libtest_main`. It will be automatically picked up by CMake.
