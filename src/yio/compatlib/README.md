Compatibility library

In case of floating-point and decimal floating-point operations the library
uses symbols from optional interfaces that are not implemented in all
compilers.

In such case, compiling with the library may be not possible, cause the library
will throw "undefined reference" errors. This library is provided, in a similar
fashion to newlib's `--specs=nano.specs`, to provide stub implementations
of external optional interfaces. These stubs will throw errors.
