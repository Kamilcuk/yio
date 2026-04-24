# GEMINI.md - Yio Iconic Output Library

## Project Overview
Yio is a modern C11 library providing type-safe string formatting, inspired by Python's `str.format` and C++'s `std::format`. It leverages C11 `_Generic` to provide a seamless and safe alternative to traditional `printf`.

The library supports multiple character types:
- **Normal (`char`):** `yio_printf`, etc.
- **Wide (`wchar_t`):** `ywprintf`, etc.
- **UTF-16 (`char16_t`):** `yc16printf`, etc. (Requires `libunistring`)
- **UTF-32 (`char32_t`):** `yuprintf`, etc. (Requires `libunistring`)

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
  - `test/templated/`: Tests that are also processed by the preprocessor to cover all character modes.
- `examples/`: Usage examples.
- `doc/`: Documentation, including Doxygen configuration.
- `kcmakelib/`: Internal CMake utility library.

## Building and Running
The project uses CMake, but a `Makefile` wrapper is provided for convenience.

### Basic Workflow
```bash
# Configure the project
make conf

# Build everything
make build

# Run tests
make test
```

### Advanced Tasks
- **Linting:** `make lint` (runs `clang-tidy`, `cpplint`, `cppcheck`).
- **Memory Safety:** `make valgrind` (runs tests under Valgrind).
- **Documentation:** `make doxygen` (generates HTML docs in `public/doxygen`).
- **Clean:** `make clean` or `make distclean`.

## Development Conventions
### Templating System
Most of the codebase is generated from templates in `src/yio/yio/`. When editing implementation:
- Edit the files in `src/yio/yio/`, NOT the generated files in the build directory.
- Use `π` as a placeholder for the mode prefix (empty, `w`, `c16`, `u`).
- Use `Ω` as a placeholder for the uppercase mode prefix (empty, `W`, `C16`, `U`).
- Use `TC("...")` macro in templates to handle string literal prefixes.
- `preprocess.py` is invoked during the build process to generate the actual C source files for each mode.

### Testing
- New features should include tests in `test/`.
- Prefer adding tests to `test/templated/` if they apply to all character modes.
- Tests use a simple pass/fail mechanism, often checked by `ctest` against regex patterns in comments (e.g., `// PASS_REGULAR_EXPRESSION 1`).

### Namespace Guidelines
- `y*`: Common public symbols (e.g., `yio_printf`).
- `yio_*`: Public internal functions and custom modifier symbols.
- `YIO_*`: Public macros, configuration, and constants.
- `YYIO_*`: Private library symbols.
