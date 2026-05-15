# Yio GCC Plugin Specification

The Yio GCC plugin provides compile-time validation for the Yio formatting language. It ensures that format strings provided to functions marked with the `yio_format` attribute are syntactically correct and compatible with the provided arguments.

## Core Mechanism

The plugin operates during the `PRE_GENERICIZE` phase of GCC, allowing it to inspect the AST before it is simplified. It identifies calls to functions possessing the `yio_format` attribute and parses the format string argument.

### Validation Rules

1.  **Argument Count**: The plugin ensures that the number of arguments provided matches the number of replacement fields in the format string. It handles both sequential `{}` and positional `{n}` fields.
2.  **Brace Balance**: It checks for unclosed `{` and unmatched `}` in the format string.
3.  **Dynamic Parameters**: When width or precision is provided as a dynamic argument (e.g., `{:.*f}` in printf, or `{:{}f}` in Yio), the plugin verifies that the corresponding argument is of an integer type.
4.  **Conversion Specifiers**: It validates the `!` conversion specifier (currently only `!a` is supported).
5.  **Type Compatibility**: It performs basic type-checking between the format specifier and the argument. For example, using a floating-point specifier (`f`, `e`, `g`, `a`) for a `bool` argument will trigger a warning.

## Attribute Trigger

Format validation is triggered by the `yio_format` attribute. The attribute takes an integer argument specifying the 1-based index of the format string argument.

```c
__attribute__((yio_format(1)))
void my_print(const char *fmt, ...);
```

## Examples

### Validating Dynamic Width
```c
yio_print("{:{}d}", 42, "invalid"); // Warning: dynamic width argument 1 must be an integer
```

### Validating Type Compatibility
```c
yio_print("{:f}", true); // Warning: invalid format specifier 'f' for bool
```
