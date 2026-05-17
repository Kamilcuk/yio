# Yio GCC Plugin Development Instructions

This directory contains a GCC plugin for the `yio` library.

## Architectural Mandates

- **Attribute-based Validation**: Validation of format strings must be triggered by the `fstring_format` attribute. Do not hardcode function names like `yio_print` for validation logic.
- **Compile-time Validation**: The plugin operates at the `PRE_GENERICIZE` phase to perform deep validation of format strings against provided arguments.

## Technical Gotchas

- **Header Order**: `c-family/c-common.h` **MUST** be included before `diagnostic-core.h`. Failure to do so will result in a `#error` from GCC headers regarding diagnostic framework extensions.
- **Linkage**: `lookup_name(tree)` is provided by the C-family frontend and has C++ linkage. Do not wrap its declaration in `extern "C"`.

## Build System

- Always use the provided `CMakeLists.txt` which handles multiarch include paths for system headers (like GMP).
- The `Makefile` provides a convenient wrapper for `cmake -B build` and `ctest`.

## Testing Conventions
- Use `add_yio_test` in `CMakeLists.txt` for all new f-string tests.
- Test Directory Structure:
  - `test/`: General tests run in both optimized (`-O1+` or flag set to 2) and unoptimized modes. This includes most functional tests.
  - `test/optimized/`: Tests that specifically verify optimization side-effects, such as constant folding nested fields into the format string (e.g., `{:10}` instead of `{:{}}`).
  - `test/unoptimized/`: Tests that should only be run without optimizations.
  - `test/noncompilable/`: General negative tests (errors) verified in both modes.
- Always verify your changes with `make test` before submitting.
