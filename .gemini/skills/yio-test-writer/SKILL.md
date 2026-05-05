---
name: yio-test-writer
description: Specialized guidance for writing C11 tests for the Yio library. Use when creating or updating tests in test/ or test/templated/, utilizing the YIO_TEST_EQ macro and handling platform-specific feature checks.
---

# Yio Test Writer

This skill provides expert guidance for writing behavioral tests for the Yio formatting engine.

## Core Patterns

### 1. Basic Test Structure
Every test should be a standalone C program with a `main` function.

```c
#include <yio.h>
#include <yio_test.h>
#include <string.h>

int main() {
    // Test logic here
    return 0;
}
```

### 2. Exact Comparison with `YIO_TEST_EQ`
Always prefer `YIO_TEST_EQ` for validating formatting output. It handles buffer management and comparison automatically.

```c
YIO_TEST_EQ("expected_output", "{:format_spec}", argument);
```

### 3. Feature Detection
Many types are platform or configuration dependent. Wrap tests in `#if` blocks.

```c
#if YYIO_HAS_timespec
    struct timespec t = { .tv_sec = 1, .tv_nsec = 0 };
    YIO_TEST_EQ("0:00:01.000000000", "{}", t);
#endif
```

### 4. Handling Skipped Tests
If a test cannot run because a feature is missing, return **77**. This allows `ctest` to mark the test as "Skipped" instead of "Passed".

```c
#if YYIO_HAS_FEATURE
    // ... tests ...
    return 0;
#else
    return 77;
#endif
```

## Workflows

### Target Execution
To run only the test you are working on, use the `ONLY` variable:
```bash
make test ONLY=".*my_test_name.*"
```

### Templated Tests
When writing tests in `test/templated/`, the file will be processed by Jinja2. This allows for generating multiple test cases from a single template.

## Troubleshooting

- **Regex Failure:** If `ctest` fails with a regex error, ensure your expected string in `YIO_TEST_EQ` matches exactly, including padding and dots.
- **Missing Symbols:** Ensure `yio_test.h` is included.
- **Linker Errors:** If adding a new test file, check if it's picked up by `test/CMakeLists.txt` (it usually glob-searches for `.c` files).
