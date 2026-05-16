# Yio GCC Plugin

A GCC plugin to enhance the yio formatting library experience in C.

## Features

- Format String Diagnostics: Adds a `fstring_format` attribute to trigger diagnostics for `{}` specifiers against arguments at compile time.
- `__builtin_fstring` Support: Reports diagnostics and transforms Python-style f-strings into optimized library calls.
- Type-Safe Specifier Mapping: Reports diagnostics when specifiers do not match the provided argument types.
- Comprehensive Checks:
    - Argument count mismatches (too many/too few).
    - Unclosed or unmatched braces.
    - Invalid conversion specifiers and complex format strings (e.g., `{:*>+10.5d}`).
    - Correct types for dynamic width and precision replacement fields.
    - Duplicate handler detection in f-strings.

## Specific Behaviors

### Constant Folding (f-strings only)
The plugin performs constant folding for nested replacement fields in f-strings to optimize performance:
- Scope: Applied ONLY to nested fields found within a format specifier (after the first `:`).
- Rule: If a nested field references a variable that is a `const int` with a compile-time initializer, its literal value is embedded directly into the format string.
- Example: `f"{x:{width}d}"` becomes `"{:10d}"` at compile time if `width` is a constant 10.
- Exclusion: The main expression (e.g., `{x}`) is NEVER folded into its value, even if `x` is constant. Attribute-based format strings are also never modified.

### Recursive Argument Flattening
To support macros like `yio_f()`, the plugin implements recursive call expansion:
- If a function call (like `yio_print`) contains an argument that is itself a call to `__builtin_fstring`, the plugin "flattens" the result.
- The format string and extracted variables from the inner f-string are merged directly into the outer call's argument list.

### Internal Mechanism: Magic String Swallowing
The `yio_f` macro is typically defined using a comma expression to satisfy function signatures before the plugin runs:
```c
#define yio_f(str) __builtin_fstring(str, ...), ""
```
When used in a call like `engine(my_f("x: {x}"))`, it expands to `engine(__builtin_fstring(...), "")`. The plugin solves the following:
1. Without the plugin: The compiler sees a call with two arguments: the builtin result and `""`.
2. Transformation: The plugin transforms the builtin into a `handler_array`, `transformed_fmt`, and extracted variables.
3. Swallowing: If the plugin only replaced the builtin, the call would remain `engine(hs, fmt, vars, "")`.
4. Observable Behavior: The plugin detects the builtin is followed by the "magic" `""` and removes it, resulting in a clean call: `engine(hs, fmt, vars)`.

### Type-Safe Whitelists
The plugin reports diagnostics for mappings between C types and format specifiers:
- Integral/Bool: Allowed specifiers are `duoxbDUOXBc`.
- Floating-point: Allowed specifiers are `fegafEGA`.
- Any other trailing alphabetic character in a specifier will trigger a diagnostic for these types.

### Diagnostic Reporting vs. Transformation
- Attribute-based Functions: Functions with the `fstring_format` attribute only report diagnostics for format string errors. The plugin does not modify the source code or arguments for these calls.
- `__builtin_fstring`: Calls to this builtin are transformed into a static array of handlers and a new format string, effectively rewriting the call site in addition to reporting diagnostics.

## Diagnostic Control

The plugin provides granular control over formatting diagnostics via `-fplugin-arg` flags:

- `-fplugin-arg-fstring_plugin-Wfstring-format=0`: Disables all f-string and attribute-based formatting diagnostics.
- `-fplugin-arg-fstring_plugin-Werror-fstring-format=1`: Promotes all formatting warnings to errors.

By default, diagnostics are reported as warnings but will respect the global `-Werror` flag.

## Requirements

- GCC 13+ with plugin support.
- GCC Plugin development headers (`gcc-13-plugin-dev` on Ubuntu/Debian).
- GMP development headers (`libgmp-dev`).

## Building

```bash
make build
```

The generated plugin is located at `build/fstring_plugin.so`.

## Usage

To use the plugin with GCC, use the `-fplugin` flag:

```bash
gcc -fplugin=path/to/build/fstring_plugin.so [other-flags] source.c
```

### Plugin Arguments

You can pass arguments to the plugin using `-fplugin-arg-fstring_plugin-<key>=<value>`:

- `Wfstring-format=0`: Disable all diagnostics.
- `Werror-fstring-format=1`: Promote diagnostics to errors.

Example:
```bash
gcc -fplugin=build/fstring_plugin.so -fplugin-arg-fstring_plugin-Werror-fstring-format=1 source.c
```

## Example

The following is a self-contained example of how to use the plugin's `__builtin_fstring` transformation.

```c
#include <stdio.h>
#include <stdarg.h>

// 1. Declare plugin builtin
const void *__builtin_fstring(const char *str, ...);

// 2. Define a macro for the format engine.
// The plugin will replace (__builtin_fstring(...), "") with: 
// handler_array, transformed_fmt, ...extracted_vars
#define my_f(str) __builtin_fstring(str, (int*)0, h_int), ""

// 3. Define a handler function pointer.
// The transformed expression returns a POINTER to the handler type 
// (e.g., if handlers are 'H', the result is 'H*'). 
// A cast is typically required because the builtin is declared as 'void*'.
typedef int (*f_handler)(va_list *va);

// 4. Implement simple engine and handler
void engine(f_handler hs[], const char *fmt, ...) {
    va_list va; va_start(va, fmt);
    printf("Rewritten Format: %s\n", fmt);
    // The static array generated by the plugin is NULL-terminated
    for (int i = 0; hs[i]; i++) hs[i](&va);
    va_end(va);
}

int h_int(va_list *va) {
    printf("Value: %d\n", va_arg(*va, int));
    return 0;
}

int main() {
    int x = 42;
    // Transformed to: engine(static_array, "x: {}", x)
    engine(my_f("x: {x}\n"));
    return 0;
}
```

Compile with: `gcc -fplugin=build/fstring_plugin.so demo.c -o demo`

## Testing

```bash
make test
```

The test suite validates both `fstring_format` attributes and `__builtin_fstring` behavior, including specialized tests for diagnostic control flags.
