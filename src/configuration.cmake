
set(YIO_M4_MLVLS_COMMENT [=[
The count of levels variadic macros expand to.  This specifies the maximum
number of arguments that can be passed to yprintf and yscanf functions.
A bigger number will generate longer and bigger include files.
]=])
set(YIO_M4_MLVLS 62 CACHE STRING "${YIO_M4_MLVLS_COMMENT}")

set(YIO_M4_SLOTS_END_COMMENT [=[
The upper count of slots available for custom overloads.
The slots for custom overloads named YIO_PRINT_SLOT_1XX and YIO_SCAN_SLOT_1XX
available for custom overloads of the _Generic print function chooser.
This number should be greater then 100 and is the ending number at which
the supported slots end. A bigger number is actually fine, it's not that
much to process.
]=])
set(YIO_M4_SLOTS_END 120 CACHE STRING "${YIO_M4_SLOTS_END_COMMENT}")
if(YIO_M4_SLOTS_END LESS 100)
	message(FATAL_ERROR "YIO_M4_SLOTS_END less then 100, needs to be greater")
endif()


set(YIO_PRINT_FLOATS_WITH_COMMENT [=[
Choose the floating point printing function. By default strfrom{f,d,l} are
used if they are available. If they are not available not available, then
printf with appriopriate format specifier is preferred.
Note that using printf may break because of locale issues.
Possible values of this variable are:
YIO_PRINT_FLOATS_WITH_UNSET YIO_PRINT_FLOATS_WITH_STRFROM
YIO_PRINT_FLOATS_WITH_PRINTF YIO_PRINT_FLOATS_WITH_CUSTOM
or alternatively in order equivalent 0, 1, 2 or 3.
]=])
set(YIO_PRINT_FLOATS_WITH 0 CACHE STRING "${YIO_PRINT_FLOATS_WITH_COMMENT}")

if(_yIO_HAS_UNISTD_H)

set(YIO_USE_INPUT_FD_COMMENT [=[
When set to true, yscan() will read from file descriptor 0 instead of FILE * stdin.
]=])
set(YIO_USE_INPUT_FD 1 CACHE BOOL "${YIO_USE_INPUT_FD_COMMENT}")

set(YIO_USE_OUTPUT_FD_COMMENT [=[
When set to true, yprint will read from file descriptor 1 instead of using FILE * stdout.
]=])
set(YIO_USE_OUTPUT_FD 1 CACHE BOOL "${YIO_USE_OUTPUT_FD_COMMENT}")

endif()

