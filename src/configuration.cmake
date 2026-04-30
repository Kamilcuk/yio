# configuration.cmake

if(CMAKE_C_COMPILER_ID STREQUAL "SDCC")
	set(YIO_MLVLS_DEFAULT 5)
else()
	set(YIO_MLVLS_DEFAULT 32)
endif()
set(YIO_MLVLS_COMMENT [=[
The count of levels variadic macros expand to.  This specifies the maximum
number of arguments that can be passed to yio_printf functions.
A bigger number will generate longer and bigger include files.
]=])
set(YIO_MLVLS "${YIO_MLVLS_DEFAULT}" CACHE STRING "${YIO_MLVLS_COMMENT}")

set(YIO_SLOTS_COMMENT [=[
The upper count of slots available for custom overloads.
The slots for custom overloads named YIO_PRINT_SLOT_1XX
available for custom overloads of the _Generic print function chooser.
This number should be greater then 100 and is the ending number at which
the supported slots end. A bigger number is actually fine, it's not that
much to process.
]=])
set(YIO_SLOTS 100 CACHE STRING "${YIO_SLOTS_COMMENT}")
if(YIO_SLOTS LESS 0)
	message(FATAL_ERROR "YIO_SLOTS less then 0, needs to be positive")
endif()

set(YIO_PRINT_FLOATS_WITH_COMMENT [=[
Choose the floating point printing function. By default strfrom{f,d,l} are
used if they are available. If they are not available not available, then
printf with appropriate format specifier is preferred.
Note that using printf may break because of locale issues.
Possible values of this variable are:
YIO_PRINT_FLOATS_WITH_UNSET YIO_PRINT_FLOATS_WITH_STRFROM
YIO_PRINT_FLOATS_WITH_PRINTF YIO_PRINT_FLOATS_WITH_CUSTOM
or alternatively in order equivalent 0, 1, 2 or 3.
]=])
set(YIO_PRINT_FLOATS_WITH 0 CACHE STRING "${YIO_PRINT_FLOATS_WITH_COMMENT}")
yio_config_gen_add(YIO_PRINT_FLOATS_WITH ADDIFNDEF)

set(YIO_USE_OUTPUT_FD_COMMENT [=[
When set to true, yio_print will write from file descriptor 1 instead of using FILE * stdout.
]=])
set(YIO_USE_OUTPUT_FD 1 CACHE BOOL "${YIO_USE_OUTPUT_FD_COMMENT}")
yio_config_gen_add(YIO_USE_OUTPUT_FD)

set(YIO_CACHE_STACK_SIZE_COMMENT [=[
When using functions that can potentially use dynamic allocation, this is the count
of memory that is allocated statically. If the number of bytes needed is greater than
this number, only than the memory is allocated dynamically. This is to reduce the number
of malloc calls for small allocations
]=])
set(YIO_CACHE_STACK_SIZE 31 CACHE STRING "${YIO_CACHE_STACK_SIZE_COMMENT}")
yio_config_gen_add_value(YIO_CACHE_STACK_SIZE "${YIO_CACHE_STACK_SIZE}")

if(CMAKE_C_COMPILER_ID STREQUAL "SDCC")
	set(YIO_USE_MALLOC_DEFAULT 0)
else()
	set(YIO_USE_MALLOC_DEFAULT 1)
endif()
set(YIO_USE_MALLOC_COMMENT [=[
Set to 0 to not use malloc at all. Will just fail with ENOMEM.
Increase YIO_CACHE_STACK_SIZE to the size you want to handle.
]=])
set(YIO_USE_MALLOC "${YIO_USE_MALLOC_DEFAULT}" CACHE BOOL "${YIO_USE_MALLOC_COMMENT}")
yio_config_gen_add(YIO_USE_MALLOC)

set(YIO_USE_LOCALE_COMMENT [=[
Enable usage of 'L' specifier and use nl_langinfo for decimal separators.
]=])
set(YIO_USE_LOCALE 1 CACHE BOOL "${YIO_USE_LOCALE_COMMENT}")
yio_config_gen_add(YIO_USE_LOCALE)

set(YIO_LINE_COMMENT [=[
Add #line to generated output files to properly pinpoint the problem in source files.
Used mostly in development.
]=])
set(YIO_LINE 0 CACHE BOOL "${YIO_LINE_COMMENT}")

set(YIO_BITINT_MAXWIDTH_COMMENT [=[
Maximum width for _BitInt support. Default 128.
]=])
set(YIO_BITINT_MAXWIDTH 128 CACHE STRING "${YIO_BITINT_MAXWIDTH_COMMENT}")
yio_config_gen_add_value(YIO_BITINT_MAXWIDTH "${YIO_BITINT_MAXWIDTH}")

if(CMAKE_C_COMPILER_ID STREQUAL "SDCC")
    set(YIO_USE_VAR_FORMAT_DEFAULT 0)
else()
    set(YIO_USE_VAR_FORMAT_DEFAULT 1)
endif()
set(YIO_USE_VAR_FORMAT_COMMENT [=[
Enable support for variable width and precision in format strings (the '*' specifier).
Example: yio_printf("{:{}.{}}", string, width, precision);
Disabling this reduces code size and stack usage, especially on 8-bit targets.
]=])
set(YIO_USE_VAR_FORMAT "${YIO_USE_VAR_FORMAT_DEFAULT}" CACHE BOOL "${YIO_USE_VAR_FORMAT_COMMENT}")
yio_config_gen_add(YIO_USE_VAR_FORMAT)
