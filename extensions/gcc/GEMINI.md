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
