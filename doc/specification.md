# Technical Specification: Yio Formatting Language

This document defines the formal grammar and behavioral specification of the Yio formatting engine.

---

## 1. Replacement Field Grammar

Format strings contain "replacement fields" surrounded by curly braces `{` and `}`. Literal braces are escaped by doubling: `{{` and `}}`.

```ebnf
replacement_field ::= "{" [arg_id] ["!" conversion] [":" format_spec] "}"
arg_id            ::= digit+
digit             ::= "0"..."9"
conversion        ::= "a"

format_spec       ::= standard_format_spec | type_specific_format_spec
standard_format_spec ::= [[fill]align][sign]["#"]["0"][width][grouping][.precision]["L"][type]
fill              ::= <any character except "{" or "}">
align             ::= "<" | ">" | "=" | "^"
sign              ::= "+" | "-" | " "
width             ::= digit+ | "{" [arg_id] "}"
grouping          ::= "_" | ","
precision         ::= digit+ | "{" [arg_id] "}"
type              ::= "a"|"A"|"b"|"B"|"c"|"d"|"e"|"E"|"f"|"F"|"g"|"G"|"o"|"O"|"s"|"x"|"X"|"n"|"p"|"u"
type_specific_format_spec ::= <any characters parsed by the type-specific formatter>
```

> **Note on Parsing:** The parsing of the `format_spec` (the string after the `:`) is **solely dependent on the argument type**. While most types follow the `standard_format_spec` grammar, some types (like `struct tm`) define their own specialized mini-languages.

> **Note on Macros:** Since `yio_printf` and related functions are C macros, arguments containing commas (like compound literals) must be wrapped in additional parentheses.
> 
> **Important Constraint:** An expression parsed by the library can contain **at most 62 commas**. This hardcoded limit is used by the preprocessor to detect and handle `yio_callback` invocations.
> 
> *Incorrect:* `yio_printf("{}", (struct timespec){.tv_sec=1, .tv_nsec=0})`
> 
> *Correct:* `yio_printf("{}", ((struct timespec){.tv_sec=1, .tv_nsec=0}))`

---

## 2. Formatting Options

The options below apply to types following the `standard_format_spec`. For types with custom parsing (like Chrono), see Section 3.

### Alignment and Fill
The presence of a `fill` character is signaled by the character following it, which must be a valid `align` option. Note: Alignment only has an effect if a `width` greater than the data size is specified.

| Option | Meaning | Default For |
| :--- | :--- | :--- |
| `<` | Left-aligned within the available space. | Non-numeric types |
| `>` | Right-aligned within the available space. | Numeric types |
| `=` | Padding is placed after the sign/base but before the digits. | - |
| `^` | Centered within the available space. | - |

### Sign
Specifies how signs are handled for numeric types.

| Option | Meaning |
| :--- | :--- |
| `+` | Sign is used for both positive and negative numbers. |
| `-` | Sign is used only for negative numbers (default). |
| ` ` (space) | Leading space for positive numbers, minus for negative. |

### Alternate Form (`#`)
The alternate form triggers type-specific presentation (detailed per-type in Section 3).
- **Integers:** Adds base prefixes (`0b`, `0`, `0x`).
- **Floating-point:** Forces a decimal point.

### Zero Padding (`0`)
Preceding the width with `0` is a shorthand for `0=` alignment (zero-fill after the sign/base).

### Precision
- **Floating-point:** Number of digits after the decimal point (`f`) or total significant digits (`g`).
- **Strings:** Maximum number of characters to write.
- **Integers/Pointers:** Not supported (error).

### Locale (`L`)
Uses the current `LC_NUMERIC` locale for decimal points and grouping separators (requires `YIO_ENABLE_LOCALE`).

---

## 3. Detailed Type Presentation

The behavior of the `type` specifier and the `#` flag depends on the underlying argument type.

### Strings (`char *`, `const char *`, `wchar_t *`, `const wchar_t *`, `char16_t *`, `const char16_t *`, `char32_t *`, `const char32_t *`)
- **none, `s`**: Copies the string to the output. Wide (`wchar_t`) and fixed-width (`char16_t`, `char32_t`) character strings are transparently converted to multibyte strings using the current locale's encoding (e.g., UTF-8 when in a UTF-8 locale).
- **`precision`**: Specifies the maximum number of **bytes** written to the output. 
  - If the string is shorter than the precision, the whole string is copied.
  - If the string is longer, it is truncated to the specified byte length.
  - The source string does **not** have to be null-terminated if a precision is provided.
- **`#` flag**: No effect.

### Display Width and Alignment
When performing alignment (e.g., `{:<10}`), the library calculates the **display width** of the string rather than its byte length.
- If unistring library usage is enabled (`YIO_HAS_UNISTRING`), it uses `libunistring` for precise width calculation.
- Otherwise, if `wchar_t` is supported (`YIO_HAS_WCHAR_H`), it uses `wcwidth()` on each character of the string (after internal conversion to `wchar_t`).
- This ensures that multibyte characters (like CJK characters or Emojis) are padded correctly according to their visual footprint.

### Characters (`char`)
- **none, `c`**: Copies the character to the output.
- **`b`, `B`, `d`, `o`, `x`, `X`**: Interprets the character as an integer and uses the Integer rules below.
- **`#` flag**: Applicable if using integer presentation.

### Booleans (`bool`)
- **none, `s`**: Textual representation (`true`/`false`). If `L` is present and locale is set, will use locale-specific forms (e.g., `YES`/`NO`).
- **Any other type**: Interprets the boolean as a numeric digit (`0` or `1`) and uses the numeric presentation rules.
- **`#` flag**: Adds base-specific prefixes (e.g., `0b`, `0x`) when using numeric presentation types like `b` or `x`.

### Integers
Supported types include:
- **Standard:** `signed char`, `unsigned char`, `short`, `unsigned short`, `int`, `unsigned int`, `long`, `unsigned long`, `long long`, `unsigned long long`.
- **Extended:** `__int128`, `unsigned __int128` (if supported by compiler).
- **Arbitrary-width:** `_BitInt(N)` (for compilers supporting C23 BitInt).

**Formatting:**
- **none, `d`**: Decimal integer (base 10).
- **`u`**: Unsigned decimal integer.
- **`b`**: Binary (base 2). `#` adds `0b` prefix.
- **`B`**: Binary (base 2). `#` adds `0B` prefix.
- **`o`**: Octal (base 8). `#` adds `0` prefix if the value is non-zero.
- **`x`**: Hexadecimal (base 16, lowercase). `#` adds `0x` prefix.
- **`X`**: Hexadecimal (base 16, uppercase). `#` adds `0X` prefix.
- **`#` flag**: Adds base-specific prefix as noted above.

### Floating-Point
Supported types include:
- **Standard:** `float`, `double`, `long double`.
- **Interchange (C11):** `_Float16`, `_Float32`, `_Float64`, `_Float128`.
- **Extended (C11):** `_Float32x`, `_Float64x`, `_Float128x`.
- **Decimal (C11):** `_Decimal32`, `_Decimal64`, `_Decimal128`.
- **Decimal Extended (C11):** `_Decimal32x`, `_Decimal64x`, `_Decimal128x`.
- **Complex:** `float _Complex`, `double _Complex`, `long double _Complex`.
- **Imaginary:** `float _Imaginary`, `double _Imaginary`, `long double _Imaginary`.

**Formatting:**
- **none (Default), `g`, `G`**: General format; adaptive selection between `f` and `e`.
- **`f`, `F`**: Fixed-point notation.
- **`e`, `E`**: Scientific notation.
- **`a`, `A`**: Hexadecimal floating-point notation.
- **`precision`**: Default is **6** (for `a`, it defaults to the minimum required for an exact representation).
- **`#` flag**: Forces the output to always include a decimal point, even if no digits follow.

### Complex and Imaginary
Complex and imaginary numbers are handled by separate formatters. The format string is currently ignored.

**Supported types:**
- **Complex:** `float _Complex`, `double _Complex`, `long double _Complex`.
- **Imaginary:** `float _Imaginary`, `double _Imaginary`, `long double _Imaginary`.

**Output Format:**
- **Complex:** Printed as `[+-]real[+-]imagi`. 
  - *Example:* `1.0 + 2.0i` -> `1.000000+2.000000i`
- **Imaginary:** Printed as `[+-]imaginaryi`.
  - *Example:* `2.0i` -> `2.000000i`

### Fixed-Point (`_Accum`, `_Fract`)
- **none (Default), `f`, `F`**: Formats as a decimal fraction.
- **`g`, `G`**: Adaptive fractional format; strips trailing zeros and the decimal point if empty.
- **`a`, `A`**: Hexadecimal floating-point notation.
- **`d`, `u`, `x`**: Interprets the underlying bits as an integer and uses the Integer rules (after taking the absolute value for signed types).
- **`precision`**: Default is **6**.

### Pointers (`void *`, `const void *`)
- **none, `p`**: Formats the address as hexadecimal with a `0x` prefix.
- **`#` flag**: No effect (base prefix is always present).

### System Types
- **`struct tm`**: Time formatting using `strftime` with optional alignment and width.
  ```ebnf
  chrono_format_spec ::= [[fill]align][width]["." precision][strftime_specs]
  ```
  The `strftime_specs` part is passed directly to the system `strftime` function. If empty, it defaults to `%c`.
  *Example:* `{:%Y-%m-%d %H:%M:%S}` -> `2026-05-05 14:30:00`.
- **`struct timespec`, `struct timeval`**:
  - **none (Default)**: Formats as a breakdown interval: `HH:MM:SS.fraction`. `HH` represents total hours (can exceed 24), while `MM` and `SS` are zero-padded to 2 digits.
  - **`f`**: Formats as flat decimal seconds: `total_seconds.fraction`.
  - **`g`**: Same as above, but strips trailing zeros and the decimal point if the fractional part is empty.
  - **`#` flag**: When used with the default format, it strips trailing zeros (Note: **Currently unimplemented**).
  - **`precision`**: Defines the number of fractional digits. Defaults: `timespec` (9), `timeval` (6).

---

## 4. Argument Resolution

### Sequential and Positional
- **Sequential:** `{}` fields consume arguments in the order they are provided.
- **Positional:** `{n}` accesses the $n$-th argument (0-indexed).
- **Mixed:** Sequential tracking continues from the last positional index plus one. (Note: This is current behavior and may change in the future).

### Dynamic Parameters
Width and precision can be provided as runtime arguments using nested fields:
`{:{}.{}f}` (takes 3 arguments: value, width, precision).

---

## 5. References

Yio tries to align its format specification with modern high-level language standards:
- **Python:** [Format Specification Mini-Language](https://docs.python.org/3/library/string.html#formatspec)
- **C++:** [Standard Format Specification (std::format)](https://en.cppreference.com/w/cpp/utility/format/formatter#Standard_format_specification)
- **Rust:** [std::fmt syntax](https://doc.rust-lang.org/std/fmt/index.html#syntax)
