# arm-none-eabi-gcc stdfix.h Support

## Supported Types
All variants (short, normal, long, long long) supported for:
- `_Fract` / `unsigned _Fract`
- `_Accum` / `unsigned _Accum`
- `_Sat` variants of all the above.

## `_Generic` Behavior
- Every base type is distinct.
- **`_Sat` is a distinct qualifier**: It is **NOT** dropped by lvalue conversion.
- Full coverage requires 32 associations (8 base types × unsigned × saturated).

## Suffixes
`hr`, `r`, `lr`, `llr`, `hk`, `k`, `lk`, `llk` (and `u` prefixes).
