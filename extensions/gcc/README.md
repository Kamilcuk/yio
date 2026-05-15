# Yio GCC Plugin

A GCC plugin to enhance the `yio` formatting library experience in C.

## Features

- **Format Validation**: Adds a `yio_format` attribute to validate `{}` specifiers against arguments at compile time.
    - Argument count mismatches.
    - Unclosed or unmatched braces.
    - Invalid conversion specifiers.
    - Type compatibility (e.g., prevents `f` for `bool`).
    - Correct types for dynamic width and precision.

## Requirements

- GCC 13+ with plugin support.
- GCC Plugin development headers (`gcc-13-plugin-dev` on Ubuntu/Debian).
- GMP development headers (`libgmp-dev`).

## Building

```bash
cd compiler/gcc
make build
```

## Testing

```bash
make test
```

The test suite validates that:
1. `yio_format` attributes correctly trigger warnings for invalid format strings.
2. Type mismatches between specifiers and arguments are caught.
3. Dynamic width/precision arguments are checked for integer types.
