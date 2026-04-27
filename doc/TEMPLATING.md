# Templating System

The Yio library uses a template-based code generation system to handle type-safe
overloading and avoid repetitive code across different character and numeric types.

## Preprocessor (`src/preprocess.py`)

The primary tool is `preprocess.py`, which:
1.  Loads templates (usually `.c` or `.h` files in `src/yio/yio/`).
2.  Provides custom Jinja2 globals and filters (e.g., `j_FLOATS`, `j_match`).
3.  Automatically imports macros from `src/library.jinja`.
4.  Renders the templates into the build directory (`_build/.../gen/`).

## Library Macros (`src/library.jinja`)

Shared templating logic is stored in `src/library.jinja`. Key macros include:
- `j_FOREACHAPPLY(array)`: Iterates over an array and yields elements to a caller block.
- `j_APPLY(...)`: Maps arguments to 1-indexed variables (`V.1`, `V.2`, etc.).
- `j_seq(FROM, TO)`: Generates a sequence of numbers.

## Build Integration

The preprocessor is integrated into CMake. Changes to any template file will
trigger a regeneration of the corresponding C/H files during the `make build`
or `cmake --build` phase.

## Error Generation (`src/geterrors.py`)

All source files are scanned for `YYIO_ERROR(...)` invocations. These are
collected into `yio_error_genmsg.h` and `yio_error_genenum.h` to provide
consistent, centralized error handling without manual registration.
