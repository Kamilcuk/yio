# User Manual: Formatting with Yio

## 1. Quick Start

Yio is a modern C11 formatting library that is safe, fast, and easy to use. All functions return an `int`: the number of bytes written on success, or a negative error code on failure.

```c
#include <yio.h>
#include <stdio.h>
#include <time.h>

int main() {
    // 1. Simple, type-safe formatting with {} for any type
    int ret = yio_printf("Welcome! Local time: {:%Y-%m-%d %H:%M:%S}\n", 
                         yio_localtime(time(NULL)));
    if (ret < 0) {
        fprintf(stderr, "Yio Error: %s\n", yio_strerror(ret));
        return 1;
    }

    // 2. Alignment, precision, and dynamic parameters
    yio_printf("Users={:<5} Load={:.{}f}\n", 42, 0.123, 2); 
    // Output: "Users=42    Load=0.12"

    // 3. Positional arguments {index} for easy localization
    yio_printf("{1} {0} {1}!\n", "World", "Hello"); 
    // Output: "Hello World Hello!"

    // 4. The "Cool" Part: Type-safe streaming (No format string required!)
    yio_stream("Stream: ", 100, "% complete at ", 3.14, " MB/s\n");
    // Output: "Stream: 100% complete at 3.14 MB/s"
}
```

---

## 2. Functional Categories

### Standard I/O (Standard Streams)
- **`int yio_printf(const char *fmt, ...)`**: Output to `stdout`.
- **`int yio_fprintf(FILE *file, const char *fmt, ...)`**: Output to a C stream.
- **`int yio_dprintf(int fd, const char *fmt, ...)`**: Output to a POSIX file descriptor.

### Buffer Management (Fixed Size)
- **`int yio_snprintf(char *dest, size_t size, const char *fmt, ...)`**: Formats into `dest`. Guaranteed null-termination.

### Dynamic Allocation (Heap)
- **`int yio_asprintf(char **strp, const char *fmt, ...)`**: Formats into a heap-allocated string. If `*strp` is non-NULL, it **reuses** the existing buffer and overwrites from the start.
- **`int yio_append(char **strp, const char *fmt, ...)`**: Appends formatted output to the end of the string in `*strp`.

### Custom Output (Abstract)

#### Example: Writing to a custom buffer
```c
#include <yio.h>
#include <string.h>

static int my_buffer_cb(void *arg, const char *data, size_t count) {
    char *dest = (char *)arg;
    // Note: 'data' is NOT null-terminated!
    strncat(dest, data, count); 
    return 0;
}

int main() {
    char buf[128] = {0};
    yio_bprintf(my_buffer_cb, buf, "Value: {}\n", 42);
    // buf now contains "Value: 42\n"
}
```

- **`int yio_bprintf(int (*callback)(void *arg, const char *data, size_t count), void *arg, const char *fmt, ...)`**: Executes formatting via a user-defined write callback. All other formatting functions (like `yio_printf`) are built on top of this.

### Type-Safe Streaming (No Format String)
- **`int yio_stream(...)`**: Stream to `stdout`.
- **`int yio_fstream(FILE *file, ...)`**: Stream to a C stream.
- **`int yio_dprint(int fd, ...)`**: Stream to a file descriptor.
- **`int yio_asstream(char **strp, ...)`**: Stream into a heap-allocated string.
- **`int yio_bstream(int (*callback)(void *arg, const char *data, size_t count), void *arg, ...)`**: Streaming via write callback.

---

## 3. Argument Mapping

### Positional Arguments
Use `{n}` to access the $n$-th argument (0-indexed).
```c
yio_printf("{1} comes before {0}\n", "first", "second");
```

### Dynamic Parameters
Width and precision can be passed as runtime arguments using nested braces.
```c
yio_printf("{:{}.{}f}\n", 3.14159, 10, 2);
```

---

## 4. Custom Types

### Custom Formatters (`yio_callback`)
Extends Yio with user-defined printing logic. See [Custom Types Registration](custom_types.md).

```c
int my_formatter(yio_printctx_t *t) {
    MyType *val = yio_printctx_va_arg(t, MyType *);
    return yio_printctx_put(t, val->name, strlen(val->name));
}

yio_printf("Custom: {}\n", yio_callback(my_formatter, &my_obj));
```

---

## 5. Modifiers

### Monetary (`yio_mon`)
Formats `double` values as currency using system locale.
```c
yio_printf("Price: {}\n", yio_mon(1234.56));
```

### Time (`yio_localtime`, `yio_gmtime`)
Formats `time_t` values as `struct tm`.
```c
yio_printf("Current time: {}\n", yio_localtime(time(NULL)));
```

### Representation (`yio_repr`)
Provides escaped, C-readable output for debugging or logging. These modifiers transform strings or characters into safely escaped sequences.

- **`yio_repr(val)`**: Alias for `yio_repr_hex`.
- **`yio_repr_hex(val)`**: Escapes non-printables using hexadecimal codes (e.g., `\x1`).
- **`yio_repr_oct(val)`**: Escapes non-printables using octal codes (e.g., `\1`).
- **`yio_repr_caret(val)`**: Uses caret notation for control characters (e.g., `^A`).
- **`yio_repr_u(val)`**: Escapes non-printables using small Unicode sequences (`\u001`).
- **`yio_repr_U(val)`**: Escapes non-printables using large Unicode sequences (`\U0000001`).

```c
yio_printf("Hex: {}\n", yio_repr_hex("Feed\x01"));   // -> "Feed\x1"
yio_printf("Caret: {}\n", yio_repr_caret("Feed\x01")); // -> "Feed^A"
```

### Wide Characters (`yio_pwchar`, `yio_pwstring`)
Explicitly formats wide character types by converting them to multibyte strings (UTF-8).
```c
yio_printf("Wide: {}\n", yio_pwstring(L"Wide String"));
```

### Character Count (`yio_count`)
Stores bytes written so far into an `int *`.
```c
int n;
yio_printf("Progress: {}{}\n", data, yio_count(&n));
```

---

## 6. Further Reading

- [Technical Specification](specification.md)
- [Templating Architecture](TEMPLATING.md)
