# configuration.cmake

set(YIO_MAX_ARGS_COMMENT [=[
The maximum number of arguments that can be passed to yio_printf and similar functions.
This value determines the depth of the variadic macro expansion in the library headers.
A higher value allows more arguments per call but results in significantly larger and
slower-to-compile header files. 32 is a reasonable default for most projects.
]=])
set(YIO_MAX_ARGS 32 CACHE STRING "${YIO_MAX_ARGS_COMMENT}")
if(YIO_MAX_ARGS LESS 1)
	message(FATAL_ERROR "YIO_MAX_ARGS must be a positive integer. Got: ${YIO_MAX_ARGS}")
endif()

set(YIO_MAX_CUSTOM_SLOTS_COMMENT [=[
The upper limit for custom type overload slots (YIO_PRINT_SLOT_1XX).
This configuration defines the range of slots available for the _Generic-based
type dispatch system. Slots are numbered starting from 100. This value should
be at least 100. Increasing this has a minimal impact on compilation time.
]=])
set(YIO_MAX_CUSTOM_SLOTS 100 CACHE STRING "${YIO_MAX_CUSTOM_SLOTS_COMMENT}")
if(YIO_MAX_CUSTOM_SLOTS LESS 0)
	message(FATAL_ERROR "YIO_MAX_CUSTOM_SLOTS cannot be negative. Got: ${YIO_MAX_CUSTOM_SLOTS}")
endif()

set(YIO_FLOAT_BACKEND_COMMENT [=[
Select the underlying implementation for floating-point formatting.
Valid options are:
- STRFROM: Uses the C23 strfrom{f,d,l} family if available. Highest precision and performance on modern platforms.
- RYU: Uses the integrated Ryu library. High-performance, portable, but supports limited precision modes for long double.
- PRINTF: Falls back to the system's snprintf. May be affected by the current LC_NUMERIC locale.
- CUSTOM: Uses Yio's baseline internal formatter. Portable and dependency-free, but slower and less precise.
]=])
set(YIO_FLOAT_BACKEND "RYU" CACHE STRING "${YIO_FLOAT_BACKEND_COMMENT}")

set_property(CACHE YIO_FLOAT_BACKEND PROPERTY STRINGS "STRFROM" "CUSTOM" "PRINTF" "RYU")
set(YIO_FLOAT_BACKEND_valid_values "STRFROM;CUSTOM;PRINTF;RYU")
if(NOT YIO_FLOAT_BACKEND IN_LIST YIO_FLOAT_BACKEND_valid_values)
	message(FATAL_ERROR "Invalid YIO_FLOAT_BACKEND: '${YIO_FLOAT_BACKEND}'. Valid values are: ${YIO_FLOAT_BACKEND_valid_values}")
endif()

set(YIO_OUTPUT_BACKEND_COMMENT [=[
Configure the default output destination for yio_printf and yio_stream.
Valid options are:
- STDOUT: Uses standard C 'FILE * stdout'. Requires a functional standard library.
- FD: Writes directly to file descriptor 1. More efficient on POSIX systems, bypasses FILE overhead.
- PUTCHAR: Uses a custom putchar-based wrapper. Optimized for embedded targets like SDCC where full stdio is unavailable.
]=])
set(YIO_OUTPUT_BACKEND "STDOUT" CACHE STRING "${YIO_OUTPUT_BACKEND_COMMENT}")
set_property(CACHE YIO_OUTPUT_BACKEND PROPERTY STRINGS "STDOUT" "FD" "PUTCHAR")
set(YIO_OUTPUT_BACKEND_valid_values "STDOUT;FD;PUTCHAR")
if(NOT YIO_OUTPUT_BACKEND IN_LIST YIO_OUTPUT_BACKEND_valid_values)
	message(FATAL_ERROR "Invalid YIO_OUTPUT_BACKEND: '${YIO_OUTPUT_BACKEND}'. Valid values are: ${YIO_OUTPUT_BACKEND_valid_values}")
endif()
yio_config_gen_add_value(YIO_OUTPUT_BACKEND "${YIO_OUTPUT_BACKEND}")

set(YIO_SSO_BUFFER_SIZE_COMMENT [=[
Size of the internal Small String Optimization (SSO) buffer.
When using dynamic string functions (e.g., yio_asprintf), this is the amount of
statically-allocated memory used before falling back to heap allocation.
Set this to handle your most frequent string sizes to significantly reduce malloc() overhead.
]=])
set(YIO_SSO_BUFFER_SIZE 31 CACHE STRING "${YIO_SSO_BUFFER_SIZE_COMMENT}")
yio_config_gen_add_value(YIO_SSO_BUFFER_SIZE "${YIO_SSO_BUFFER_SIZE}")

set(YIO_ENABLE_MALLOC_COMMENT [=[
Enable the use of dynamic memory allocation (malloc/realloc).
If disabled (0), all operations requiring a buffer larger than YIO_SSO_BUFFER_SIZE
will fail with a YIO_ERROR_ENOMEM. This is essential for hard real-time or
constrained embedded environments where heap use is prohibited.
]=])
set(YIO_ENABLE_MALLOC 1 CACHE BOOL "${YIO_ENABLE_MALLOC_COMMENT}")
yio_config_gen_add_value(YIO_ENABLE_MALLOC "${YIO_ENABLE_MALLOC}")

set(YIO_ENABLE_LOCALE_COMMENT [=[
Enable locale-aware formatting support.
When enabled, the library supports the 'L' specifier and respects nl_langinfo settings
for decimal separators and thousands grouping. Disabling this reduces binary size
and removes dependencies on system locale headers.
]=])
set(YIO_ENABLE_LOCALE 1 CACHE BOOL "${YIO_ENABLE_LOCALE_COMMENT}")
yio_config_gen_add_value(YIO_ENABLE_LOCALE "${YIO_ENABLE_LOCALE}")

set(YIO_GENERATE_LINE_DIRECTIVES_COMMENT [=[
Control the generation of #line directives in preprocessed source files.
When enabled (1), generated code will include pointers back to the original templates,
making it much easier to debug template-related issues in development.
]=])
set(YIO_GENERATE_LINE_DIRECTIVES 0 CACHE BOOL "${YIO_GENERATE_LINE_DIRECTIVES_COMMENT}")

set(YIO_BITINT_MAXWIDTH_COMMENT [=[
The maximum bit-width supported for _BitInt types.
This value determines how many specialized printers are generated for arbitrary-width
integers. The default is 128. Only useful if your compiler (like GCC 14+) supports _BitInt.
]=])
set(YIO_BITINT_MAXWIDTH 128 CACHE STRING "${YIO_BITINT_MAXWIDTH_COMMENT}")
yio_config_gen_add_value(YIO_BITINT_MAXWIDTH "${YIO_BITINT_MAXWIDTH}")

set(YIO_ENABLE_DYNAMIC_PFMT_COMMENT [=[
Enable support for dynamic width and precision specifiers (e.g., '{:*.*f}').
When enabled, the library can parse and apply formatting parameters passed at runtime.
Disabling this (0) significantly reduces stack usage and binary size by stripping
the complex parsing and parameter-tracking logic.
Example: yio_printf("{:{}.{}}", string, width, precision);
]=])
set(YIO_ENABLE_DYNAMIC_PFMT 1 CACHE BOOL "${YIO_ENABLE_DYNAMIC_PFMT_COMMENT}")
yio_config_gen_add_value(YIO_ENABLE_DYNAMIC_PFMT "${YIO_ENABLE_DYNAMIC_PFMT}")

set(YIO_ENABLE_DIGIT_GROUPING_COMMENT [=[
Enable support for thousands grouping separators (e.g., '{:n}').
When enabled, the library can format large numbers with separators (like ',' or ' ')
based on the current locale or explicit request. Disabling this reduces the
complexity of the integer and floating-point formatting loops.
Example: yio_printf("{:n}", 1234567);
]=])
set(YIO_ENABLE_DIGIT_GROUPING 1 CACHE BOOL "${YIO_ENABLE_DIGIT_GROUPING_COMMENT}")
yio_config_gen_add_value(YIO_ENABLE_DIGIT_GROUPING "${YIO_ENABLE_DIGIT_GROUPING}")
