# GEMINI.md - Yio Iconic Output Library

## Project Overview
Yio is a modern C11 library providing type-safe string formatting, inspired by Python's `str.format` and C++'s `std::format`. It leverages C11 `_Generic` to provide a seamless and safe alternative to traditional `printf`.

The library focuses on a single "normal" character mode (`char`), but handles `wchar_t`, `char16_t`, and `char32_t` by converting them to multibyte strings (UTF-8).

### Key Technologies
- **C11:** Core language, uses `_Generic`.
- **CMake:** Primary build system.
- **Python 3 + Jinja2:** Used for code generation from templates.
- **Bash/Make:** Used for task automation.

## Project Structure
- `src/`: Core library source code.
  - `src/yio/yio/`: Source templates (using `.c`/`.h` extensions) processed by `preprocess.py`.
  - `src/preprocess.py`: The Jinja2-based preprocessor.
  - `src/library.jinja`: Jinja2 macros and shared template logic.
- `test/`: Comprehensive test suite.
  - `test/templated/`: Tests that are also processed by the preprocessor to cover the library character mode.
- `examples/`: Usage examples.
- `doc/`: Documentation, including Doxygen configuration.
- `kcmakelib/`: Internal CMake utility library.

## Building and Running
The project uses CMake, but a `Makefile` wrapper is provided for convenience.

### Basic Workflow
```bash
# Configure, build and run tests (recommended)
make test

# To only build:
make build
```

### Quick Test
For quick one-liner tests:
```bash
./scripts/compile_test_run.sh 'yio_print("{}\n", 123);'
```

### Advanced Tasks
- **Linting:** `make lint` (runs `clang-tidy`, `cpplint`, `cppcheck`).
- **Memory Safety:** `make valgrind` (runs tests under Valgrind).
- **Documentation:** `make doxygen` (generates HTML docs in `public/doxygen`).
- **Clean:** `make clean` or `make distclean`.

### Development Conventions
- **Preserve Comments:** When refactoring or replacing code blocks, DO NOT remove existing comments or documentation unless explicitly told to do so or if they are entirely invalidated by the change.
### NEVER USE GIT
- NEVER stage or commit any changes using git unless the user explicitly told you to.

### Temporary Files
- Always create temporary files in `/tmp` to avoid polluting the workspace.

### Templating System
Most of the codebase is generated from templates in `src/yio/yio/`. When editing implementation:
- Edit the files in `src/yio/yio/`, NOT the generated files in the build directory.
- `preprocess.py` is invoked during the build process to generate the actual C source files.
- **NOLINE:** To disable automatic `#line` directive generation for a template, add `// NOLINE` to the file. This is useful when `#line` directives cause syntax errors due to complex Jinja2 logic or line joining.

### Format Specification
- Supports nested replacement fields: `{:{}.{}f}` (compatible with Python and C++20).
- Supports positional arguments: `{0:{1}.{2}f}`.

### Testing
- New features should include tests in `test/`.
- Prefer adding tests to `test/templated/`.
- Tests use a simple pass/fail mechanism, often checked by `ctest` against regex patterns in comments (e.g., `// PASS_REGULAR_EXPRESSION 1`).
- Filter tests using the `ONLY` variable (supports regex):
  ```bash
  make test ONLY=".*time(spec|val).*"
  ```
- **Large Test Output:** Tests for `float` and `decimal` produce extremely high-volume output. When running these, always redirect to a file or pipe through `tail`/`grep` to avoid overwhelming the session context.

### Namespace Guidelines
- `yio_print`, `yio_print`: Primary type-safe API.
- `yio_count`, `yio_mon`: Standardized modifier naming.
- `yio_callback`: Custom callback registration.
- `yio_*`: Public internal functions and custom modifier symbols.
- `YIO_*`: Public macros, configuration, and constants.
- `YIO_*`: Private library symbols.
