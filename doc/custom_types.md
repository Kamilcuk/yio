# Developer API: Custom Type Integration

Yio allows developers to extend the formatting engine to support user-defined structures. Integration is achieved via a standardized callback contract and the C11 `_Generic` dispatcher.

---

## 1. The Callback Contract

A custom printer is a function matching the `yio_printdata_t` signature:
`int (*)(yio_printctx_t *t)`

### Technical Implementation Example
```c
static int my_type_printer(yio_printctx_t *t) {
    // 1. Retrieval (Must be first)
    struct my_type val = yio_printctx_va_arg(t, struct my_type);
    // 2. Initialization
    int err = yio_printctx_init(t);
    if (err) return err;
    // 3. Output (Propagate all non-zero errors)
    err = yio_printctx_put(t, "Type(", 5);
    if (err) return err;
    err = yio_printctx_printf(t, "x={}, y={}", val.x, val.y);
    if (err) return err;
    return yio_printctx_put(t, ")", 1);
}
```

To maintain technical integrity (positional arguments, nesting, recursion), every callback **must** follow this 3-step sequence:

### Step 1: Argument Retrieval
Use `yio_printctx_va_arg(t, TYPE)` or `yio_printctx_va_arg_promote(t, TYPE)` to extract the value from the `va_list`. This **must** be the first operation.

> **Note on Promotion:** In C, variadic arguments undergo default promotions: `char`/`short` become `int`, and `float` becomes `double`. Use `yio_printctx_va_arg_promote` to handle these conversions automatically.

### Step 2: Initialization
Call `yio_printctx_init(t)` to trigger format specification parsing (e.g., width, alignment). 

For **integer-like types**, use `yio_printctx_init_or_number(t, val)`. This function handles both standard field initialization and the consumption of values for dynamic specifiers (e.g., the nested `{}` in `{:{}}`). If the function returns a non-zero value, it means the value was used as a parameter or an error occurred; in either case, the callback must return the value immediately.
### Step 3: Execution & Output
Format the data and write to the output stream. All output functions return an `int` (**0 on success, or any non-zero value on failure**). The number of characters written is tracked internally in `t->writtencnt`.

- **`int yio_printctx_printf(yio_printctx_t *t, const char *fmt, ...)`**
  Recursive, type-safe formatting. Allows your custom type to be defined in terms of other Yio types.
- **`int yio_printctx_put(yio_printctx_t *t, const char *str, size_t len)`**
  Writes a string buffer. It automatically applies the alignment, padding, and width rules parsed during Step 2.
- **`int yio_printctx_put_number(yio_printctx_t *t, const char *str, size_t len, bool is_positive)`**
  Writes a buffer containing digits. It applies numeric-specific formatting like sign handling (`+`, `-`, or space) and digit grouping.

---

## 2. Integration Mechanisms

### A. Global Dispatch (`YIO_ADD_TYPE`)
Integrates the type into the library's `_Generic` dispatcher. This allows `yio_printf` to automatically detect and print your type without explicit wrappers.

```c
#include <yio.h>

// 1. Define your structure
struct point { int x, y; };

// 2. Define the printer callback
static int point_printer(yio_printctx_t *t) {
    struct point p = yio_printctx_va_arg(t, struct point);
    int err = yio_printctx_init(t);
    if (err) return err;
    return yio_printctx_printf(t, "Point({},{})", p.x, p.y);
}

// 3. Register the type into a global slot
YIO_ADD_TYPE(struct point, point_printer)
#include YIO_ADD_TYPE_INC()

int main() {
    struct point p = {10, 20};
    // 4. Use it directly in any Yio function
    yio_printf("Current point: {}\n", p);
    return 0;
}
```

#### How it works:
The `YIO_ADD_TYPE_INC()` header (typically `slots.h`) uses preprocessor logic to incrementally redefine the `YIO_PRINT_FUNC_GENERIC_SLOTS` macro. 

Each inclusion:
1.  **Increments** an internal counter (`YIO_COUNTER`).
2.  **Appends** your new type mapping (e.g., `struct my_type: my_type_printer`) to the list of types handled by the core `_Generic` dispatcher.
3.  **Redefines** `YIO_PRINT_FUNC_GENERIC_SLOTS` to contain all types registered so far.

This effectively "stitches" your user-defined types into the library's type-detection logic at compile time, allowing `yio_printf` to recognize them as if they were built-in types. The maximum number of available slots is defined by `YIO_MAX_CUSTOM_SLOTS` in CMake (default: 100).

### B. Explicit Callbacks (`yio_callback`)
Used for one-off formatting or when a single type requires multiple formatting strategies (e.g., `yio_mon(v)` or `yio_repr_hex(v)`).

```c
#include <yio.h>

// 1. Define your structure
struct my_type { int value; };

// 2. Define the printer callback
static int my_type_printer(yio_printctx_t *t) {
    struct my_type val = yio_printctx_va_arg(t, struct my_type);
    int err = yio_printctx_init(t);
    if (err) return err;
    return yio_printctx_printf(t, "Value={}", val.value);
}

// 3. Define a type-safe wrapper macro
#define yio_my_custom(v) yio_callback(my_type_printer, _Generic((v), struct my_type: (v)))

int main() {
    struct my_type obj = {42};
    // 4. Use the wrapper in a formatting call
    yio_printf("Custom: {}\n", yio_my_custom(obj));
    return 0;
}
```

#### Behavior:
`yio_callback` is a macro that "wraps" a value and a specific handler function. At compile-time, it marks the argument with a unique signature that Yio's variadic macro engine recognizes. When `yio_printf` processes this argument, it bypasses the global `_Generic` dispatcher and directly executes the provided handler.

#### Why use `_Generic` in the wrapper?
While `yio_callback` itself is variadic and accepts any type, the handler function (`my_type_printer`) is hardcoded to expect a specific type in its `yio_printctx_va_arg(t, struct my_type)` call.
Using `_Generic` in the wrapper macro acts as a **compile-time guard**. If a developer accidentally passes an `int` to `yio_my_custom(v)`, the compiler will issue an error at the call site, preventing stack corruption or undefined behavior inside the handler's `va_list` extraction.

---

## 3. Formatting Context API Reference

### The Context Object (`yio_printctx_t *t`)
The `t` parameter is an opaque pointer to the library's internal state. It manages the output backend, current formatting specifiers, variadic arguments, and internal counters.

### API Functions
The following functions provide the core interface for interacting with the formatting engine. All functions returning `int` return **`0` on success or any non-zero value on failure**.

#### `type yio_printctx_va_arg(yio_printctx_t *t, type)`
- **Purpose:** Macro wrapper around `va_arg` that safely extracts the next argument from the context's internal `va_list`. **This must be the first function called in your callback.**
- **Parameters:**
  - `t`: The printing context.
  - `type`: The expected C type of the argument.
- **Returns:** The extracted value of type `type`.

#### `numtype yio_printctx_va_arg_promote(yio_printctx_t *t, numtype)`
- **Purpose:** Automatically handles default C variadic promotions for numeric types (e.g., `float` to `double`). Use this for types that might undergo promotion when passed to variadic functions.
- **Parameters:**
  - `t`: The printing context.
  - `numtype`: The numeric type.
- **Returns:** The promoted value of type `numtype`.

#### `int yio_printctx_init(yio_printctx_t *t)`
- **Purpose:** Parses the format specification string (e.g., `"{:08x}"`) for the current field and populates `t->pf`.
- **Parameters:**
  - `t`: The printing context.
- **Returns:** `0` on success; `YIO_ERROR_DYNAMIC_NOT_NUMBER` if a dynamic parameter was expected but not provided.

#### `int yio_printctx_init_or_number(yio_printctx_t *t, int val)`
- **Purpose:** Combined field initialization and dynamic parameter handler. It either parses the current field's formatting or provides `val` as a value for a pending dynamic width/precision specifier.
- **Parameters:**
  - `t`: The printing context.
  - `val`: The value to print or to use as a formatting parameter.
- **Returns:** `0` on success, or a non-zero value if the value was consumed for a dynamic specifier or if an error occurred. The return value **must** be propagated by the callback.

#### `int yio_printctx_printf(yio_printctx_t *t, const char *fmt, ...)`
- **Purpose:** Recursively prints to the current context using Yio's type-safe engine.
- **Parameters:**
  - `t`: The printing context.
  - `fmt`: A Yio-style format string.
  - `...`: Variadic arguments matching `fmt`.
- **Returns:** `0` on success, or a negative error code.

#### `int yio_printctx_put(yio_printctx_t *t, const char *str, size_t str_len)`
- **Purpose:** Outputs a string while respecting the formatting options in `t->pf` (alignment, padding, width).
- **Parameters:**
  - `t`: The printing context.
  - `str`: Pointer to the string buffer.
  - `str_len`: Number of bytes to write.
- **Returns:** `0` on success, or a negative error code.

#### `int yio_printctx_put_number(yio_printctx_t *t, const char *str, size_t str_len, bool is_positive)`
- **Purpose:** Outputs a numeric string (integers, floating-point, or INF/NAN) while applying numeric-specific formatting. It handles the sign, thousands grouping, and decimal separators.
- **Parameters:**
  - `t`: The printing context.
  - `str`: Pointer to the numeric string buffer. **Must not contain a leading sign character.**
  - `str_len`: Number of bytes in `str`.
  - `is_positive`: Boolean sign indicator. Used to determine if `+`, `-`, or a space should be prepended based on `t->pf.sign`.
- **Special Handling:**
  - If `is_number` is true, the engine detects `inf`/`nan` (case-insensitive) and adjusts padding/alignment (stripping grouping).
  - Handles decimal points (`.` or `,`) and applies locale-aware grouping to the integer part if enabled.
- **Returns:** `0` on success, or a negative error code.

#### `int yio_printctx_raw_write(yio_printctx_t *t, const char *ptr, size_t size)`
- **Purpose:** Direct write to the output backend, bypassing all Yio formatting logic.
- **Parameters:**
  - `t`: The printing context.
  - `ptr`: Pointer to the data buffer.
  - `size`: Number of bytes to write.
- **Returns:** `0` on success, or a negative error code.

#### `int yio_printctx_next(yio_printctx_t *t)`
- **Purpose:** Invokes the next formatting function in the chain. Primarily used by modifier callbacks to pass control to the actual printer.
- **Parameters:**
  - `t`: The printing context.
- **Returns:** The return value of the next function in the chain.

### The Format Specification (`struct yio_printfmt_s`)
Accessible via `t->pf` after `yio_printctx_init(t)`. By default, the structure is zero-initialized (`0` or `\0` means the value is **unset**).

```c
struct yio_printfmt_s {
    uint16_t width;      // Field width (0 if unset, else value + 1)
    uint16_t precision;  // Field precision (0 if unset, else value + 1)
    char     fill;       // Padding character (0 if unset)
    char     align;      // Alignment: '<', '>', '^', '=' or 0 if unset
    char     sign;       // Sign: '+', '-', ' ' or 0 if unset
#if YIO_ENABLE_DIGIT_GROUPING
    char     grouping;   // Grouping: 'L', '_', ',' or 0 if unset
#endif
    char     type;       // Specifier character (e.g., 'f', 'x', 's') or 0 if unset
    bool     hash;       // True if '#' alternate form was requested
};
```

**Helper Macros:**
- `size_t yio_width_get_default(uint16_t width, size_t default_val)`
  Retrieves the field width specified by the user in the format string (e.g., the `5` in `{:5}` or `{:{}}`). If the user did not specify a width, it returns `default_val`.
- `size_t yio_precision_get_default(uint16_t precision, size_t default_val)`
  Retrieves the precision specified by the user in the format string (e.g., the `.2` in `{:.2f}` or `{:.{}f}`). If the user did not specify a precision, it returns `default_val`.

