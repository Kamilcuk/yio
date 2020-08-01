
set(YIO_M4_MLVLS 62 CACHE STRING
	"The count of levels variadic macros expand to")
set(YIO_M4_SLOTS_END 110 CACHE STRING
	"The upper count of slots available in overloads")

set(YIO_PRINT_FLOATS_WITH 0 CACHE STRING
	"Choose the floating point printing function. Possible values are:"
	" YIO_PRINT_FLOATS_WITH_UNSET YIO_PRINT_FLOATS_WITH_STRFROM "
	" YIO_PRINT_FLOATS_WITH_PRINTF YIO_PRINT_FLOATS_WITH_CUSTOM "
	" or 0,1,2,3 "
	" By default strfrom is used is vailable, else printf."
)

set(YIO_FLOATS_PREFER_CUSTOM FALSE CACHE BOOL
	"When strfrom is not available, you can set this to "
	" prefer printing floats with custom algorithm rather"
	" then with printf."
)

if(_yIO_HAS_UNISTD_H)
cmake_dependent_option(YIO_USE_INPUT_FD 1 CACHE BOOL
	"When set to true, yscan will read from file descriptor 0 instead of FILE *stdin")
var_to_0_or_1(YIO_USE_INPUT_FD)
cmake_dependent_option(YIO_USE_OUTPUT_FD 1 CACHE BOOL
	"When set to true, yprint will read from file descriptor 1 instead of FILE *stdout")
var_to_0_or_1(YIO_USE_OUTPUT_FD)
endif()
