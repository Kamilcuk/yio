
set(YIO_M4_MLVLS 62 CACHE STRING [=[
The count of levels variadic macros expand to.  This specifies the maximum
number of arguments that can be passed to yprintf and yscanf functions.
A bigger number will generate longer and bigger include files.
]=])

set(YIO_PRINT_FLOATS_WITH 0 CACHE STRING [=[
Choose the floating point printing function. By default strfrom{f,d,l} are
used if they are available. If they are not available not available, then
printf with appriopriate format specifier is preferred.
Note that using printf may break because of locale issues.
Possible values of this variable are:
YIO_PRINT_FLOATS_WITH_UNSET YIO_PRINT_FLOATS_WITH_STRFROM
YIO_PRINT_FLOATS_WITH_PRINTF YIO_PRINT_FLOATS_WITH_CUSTOM
or alternatively in order equivalent 0, 1, 2 or 3.
]=])

set(YIO_FLOATS_PREFER_CUSTOM FALSE CACHE BOOL [=[
When strfrom is not available, you can set this to prefer printing floats
with custom algorithm developed within the library rather then with printf.
The YIO_PRINT_FLOATS_WITH takes precedence over this option.
]=])

if(_yIO_HAS_UNISTD_H)

set(YIO_USE_INPUT_FD 1 CACHE BOOL [=[
When set to true, yscan() will read from file descriptor 0 instead of FILE * stdin.
]=])
var_to_0_or_1(YIO_USE_INPUT_FD)

set(YIO_USE_OUTPUT_FD 1 CACHE BOOL [=[
When set to true, yprint will read from file descriptor 1 instead of using FILE * stdout.
]=])
var_to_0_or_1(YIO_USE_OUTPUT_FD)

endif()

