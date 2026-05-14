# Floating Point Macros

The `yio` library uses a set of macros to handle different floating point types in a type-safe and consistent way. These macros are defined in `src/yio/private/yio_float.h` and `src/yio/private/yio_float_rp.h`.

## Macro Naming Convention

Most floating point macros follow a suffix-based naming convention, where the suffix indicates the float type (e.g., `f` for `float`, `d` for `double`, `f128` for `_Float128`).

### Availability Macros

- `YIO_HAS_FLOAT<suffix>`: Defined to 1 if the float type is supported, 0 otherwise.

### Type and Math Macros

These macros are defined in `yio_float.h`:

- `YIO_FLOAT<suffix>`: The C type (e.g., `float`, `double`).
- `YIO_modf<suffix>`: Suffix-based `modf` function.
- `YIO_frexp<suffix>`: Suffix-based `frexp` function.
- `YIO_floor<suffix>`: Suffix-based `floor` function.
- `YIO_fabs<suffix>`: Suffix-based `fabs` function.
- `YIO_pow<suffix>`: Suffix-based `pow` function.
- `YIO_exp10<suffix>`: Suffix-based `exp10` function (emulated if not available).
- `YIO_FLOAT_MANT_DIG<suffix>`: Number of base-FLT_RADIX digits in the mantissa.
- `YIO_FLOAT_MAX<suffix>`: Maximum value of the type.
- `YIO_FLOAT_MIN<suffix>`: Minimum normalized positive value.
- `YIO_FLOAT_EPSILON<suffix>`: Difference between 1.0 and the next representable value.
- `YIO_FLOAT_HUGE_VAL<suffix>`: Large positive value, used for overflow.

## Representation Macros (RP)

Macros defined in `yio_float_rp.h` provide a common interface for types that share the same underlying representation (e.g., Binary32, Binary64).

- `YIO_RP_OF_<suffix>_IS_<RP>`: Defined to 1 if the type has the given representation.
- `YIO_FLOAT_RP_<RP>`: The canonical type for the representation.
- `YIO_modf_RP_<RP>`: Math functions for the representation.
- `YIO_frexp10_RP_<RP>`: Base-10 frexp implementation for the representation.

Supported representations (`<RP>`) include:
- `B16`: Binary16 (half precision)
- `B32`: Binary32 (single precision)
- `B64`: Binary64 (double precision)
- `B80`: Binary80 (extended precision)
- `B128`: Binary128 (quad precision)
- `D32`: Decimal32
- `D64`: Decimal64
- `D128`: Decimal128

## Conversion Backends

The library provides several backends for float-to-string conversion, chosen automatically based on availability in the following priority:

1.  `strfrom`: Uses the C23 `strfrom` family of functions. This includes standard types (`strfromf`, `strfromd`, `strfroml`), interchange types (`strfromfN`), and decimal types (`strfromdN`).
2.  `ryu`: Uses the integrated Ryu library.
    -   Supports `B32` (float) and `B64` (double).
    -   Extended precision (`B80`) and quad precision (`B128`) are only supported if the compiler provides `__int128` support.
3.  `printf`: Falls back to system `snprintf`. Requires a compatible format string prefix (e.g., `L` for `long double`).
4.  `naive`: Yio's internal fallback engine. Uses basic digit extraction and scaling.

Availability macros: `YIO_has_float_astrfrom_<backend>_<repr>`.
Function signature: `int YIO_float_astrfrom_<backend>_<repr>(YIO_string *v, int precision, char spec, YIO_FLOAT_RP_<repr> val)`.
