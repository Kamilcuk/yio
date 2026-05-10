# Fixed-Point Support (ISO/IEC TR 18037)

This document summarizes the support for fixed-point arithmetic types in various compilers.

## Compiler Support Matrix

| Compiler | Version | Target | Options | `_Fract` | `_Accum` | `_Sat` | `_Generic` Uniqueness |
|----------|---------|--------|---------|----------|----------|--------|-----------------------|
| `arm-none-eabi-gcc` | 15.2.1 | ARM | (default) | Yes | Yes | Yes | Yes (including `_Sat`) |
| `clang` | 21.1.8 | x86_64 | `-ffixed-point` | Yes | Yes | Yes | Yes (including `_Sat`) |
| `gcc` | 13.3.0 | x86_64 | (default) | No [1] | No [1] | No [1] | N/A |
| `sdcc` | 4.5.0 | mcs51/stm8 | (default) | No [2] | No [2] | No [2] | N/A |

[1] `stdfix.h` exists but types are not supported on this target.
[2] SDCC 4.5.0 claims support "for all targets" but keywords `_Fract`/`_Accum` were not recognized in empirical tests on `mcs51`/`stm8` without further configuration.

## Type Variants Support

### `arm-none-eabi-gcc`
- **Base types:** `short`, `(normal)`, `long`, `long long`.
- **Signedness:** `signed`, `unsigned`.
- **Saturation:** `_Sat` supported for all combinations.
- **Uniqueness:** All 32 associations are distinct in `_Generic`. `_Sat` is NOT dropped by lvalue conversion.

### `clang` (`-ffixed-point`)
- **Base types:** `short`, `(normal)`, `long`. (`long long` is NOT supported).
- **Signedness:** `signed`, `unsigned`.
- **Saturation:** `_Sat` supported for all combinations.
- **Uniqueness:** All 24 associations are distinct in `_Generic`. `_Sat` is NOT dropped by lvalue conversion.

## Suffixes
The following suffixes are used for fixed-point constants:
- `hr`, `r`, `lr`, `llr` for `_Fract` variants (and `u` prefixes).
- `hk`, `k`, `lk`, `llk` for `_Accum` variants (and `u` prefixes).

## `_Generic` and `_Sat`
In both `arm-none-eabi-gcc` and `clang`, `_Sat` is a distinct type qualifier that participates in `_Generic` selection. It is not an alias for the non-saturated type.

Example of distinct associations:
```c
_Generic((_Sat _Fract)0,
    _Fract: 0,
    _Sat _Fract: 1
) // Returns 1
```
