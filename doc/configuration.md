# Technical Configuration & Backend Guide

Yio is designed with a "pay-only-for-what-you-use" philosophy. Its behavior and architectural footprint are controlled via CMake configuration options that trigger specialized code generation.

---

## 1. Core Backends

### Floating-Point Formatting (`YIO_FLOAT_BACKEND`)
The `YIO_FLOAT_BACKEND` option determines the internal engine used to transform floating-point types into strings.

| Backend | Technical Implementation | Notes |
| :--- | :--- | :--- |
| **`STRFROM`** | Uses C23 `strfromf/d/l`. | Industry standard, highest precision.<br>Requires modern libc. |
| **`RYU`** | Integrated [Ryu](https://github.com/ulfjack/ryu) submodule. | Consistent across platforms. Fully supports `float` and `double`. Support for `long double` is complete if it is equal to `double`; otherwise, it requires `__int128` and supports only "shortest" scientific representation (falling back to `NAIVE` for others). |
| **`PRINTF`** | Wraps system `snprintf`. | Minimal code footprint: reuses existing system `snprintf`. |
| **`NAIVE`** | Yio's baseline internal formatter. | Zero external dependencies.<br>Slow, not precise. Naive digit extraction subject to cumulative floating-point errors. |

#### The Fallback Chain
If `YIO_FLOAT_BACKEND` is set to `STRFROM` but the environment check fails, it automatically downgrades to **`RYU`**, and then to **`NAIVE`**.

### Default Output (`YIO_OUTPUT_BACKEND`)
Configures where `yio_printf` and `yio_stream` send their bytes by default.

*   **`STDOUT`**: Wraps standard C `stdout`. Requires `stdio.h`.
*   **`FD`**: Writes directly to file descriptor `1` using `write()`. Efficient on POSIX.
*   **`PUTCHAR`**: Uses a single-character `putchar` wrapper. Intended for bare-metal embedded targets.

---

## 2. Memory & Optimization

### Static Buffer / SSO Size (`YIO_SSO_BUFFER_SIZE`)
Default: `31`.
*   **When `YIO_ENABLE_MALLOC` is `1`:** Defines the threshold for Small String Optimization (SSO). Results smaller than this size are stored on the stack to avoid heap allocation.
*   **When `YIO_ENABLE_MALLOC` is `0`:** Defines the **fixed capacity** of the internal buffer. Any formatting exceeding this size will fail.

> [!IMPORTANT]
> **Interdependence in Constrained Mode:**
> When heap allocation is disabled, `YIO_SSO_BUFFER_SIZE` is the **only** buffer available for internal formatting (floats, integers, etc.). It must be large enough to hold the **longest actual string** you intend to print.
>
> *   **Requirements vary significantly depending on use:** Printing a `long double` in hexadecimal (`{:a}`) is compact (~30 bytes), but fixed-point (`{:f}`) with high precision can require 100+ bytes.
> *   **Safety:** If the generated string exceeds this buffer, the operation will fail. Always size this buffer based on your "worst-case" formatting needs.

---

## 3. Feature Stripping

These options allow you to remove complex logic to reduce the binary footprint.

| Option | Impact when Disabled (`0`) |
| :--- | :--- |
| **`YIO_ENABLE_LOCALE`** | Removes dependency on `locale.h` and locale-aware formatting (`L`). |
| **`YIO_ENABLE_DYNAMIC_PFMT`** | Strips the recursive parser for nested fields like `{:{}.{}}`. |
| **`YIO_ENABLE_DIGIT_GROUPING`** | Removes thousands-separator logic (e.g., `{:n}`). |
| **`YIO_BITINT_MAXWIDTH`** | Controls support for `_BitInt(N)`. Set to `0` to disable completely. |

---

## 4. Technical Limits (Preprocessor)

These options affect the complexity of generated headers and compilation speed.

*   **`YIO_MAX_ARGS`**: Maximum arguments per `yio_printf` call. This value determines the depth of the variadic macro expansion in the library headers.
*   **`YIO_MAX_CUSTOM_SLOTS`**: Maximum number of `YIO_ADD_TYPE` calls allowed for custom type overloads.

---

## 5. Targeting Constrained Devices (Embedded)

### Recommended "nano" Configuration
| Option | Recommended | Impact |
| :--- | :--- | :--- |
| **`YIO_OUTPUT_BACKEND`** | `PUTCHAR` | Minimum I/O overhead. |
| **`YIO_ENABLE_MALLOC`** | `0` | No heap required. |
| **`YIO_SSO_BUFFER_SIZE`** | `64`+ | Necessary to support float formatting without heap. |
| **`YIO_MAX_ARGS`** | `5` | Simplifies macro expansion. |
| **`YIO_ENABLE_DYNAMIC_PFMT`** | `0` | Removes recursive parser. |
| **`YIO_ENABLE_LOCALE`** | `0` | No locale overhead. |

---

## 6. Development Options

*   **`YIO_GENERATE_LINE_DIRECTIVES`**: Includes `#line` directives in preprocessed files pointing to Jinja2 templates.
