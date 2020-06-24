
set(YIO_M4_MLVLS 62 CACHE STRING
	"The count of levels variadic macros expand to")
set(YIO_M4_SLOTS_END 110 CACHE STRING
	"The upper count of slots available in overloads")

if(CMAKE_BUILD_TUPE_UPPER STREQUAL "DEBUG")
	set(tmp true)
else()
	set(tmp false)
endif()
set(YIO_CHECK_VA_ARG_SIZES ${tmp} CACHE BOOL
	"If checking of sizes of variable arguments should be enabled."
	"Enabeld by default in debugging builds")

set(YIO_FLOATS_PREFER_CUSTOM 0 CACHE BOOL
	"For managing standard floating point numbers, prefer using custom stupid library")
var_to_0_or_1(YIO_FLOATS_PREFER_CUSTOM)

set(YIO_IEC_60559_BFP_PREFER_CUSTOM 0 CACHE BOOL
	"For managing IEC 60559 binary floating point numbers _FloatN, prefer using custom stupid library")
var_to_0_or_1(YIO_IEC_60559_BFP_PREFER_CUSTOM)

set(YIO_IEC_60559_DFP_PREFER_CUSTOM 0 CACHE BOOL
	"For managing IEC 60559 decimal floating point numbers _DecimalN, prefer using custom stupid library")
var_to_0_or_1(YIO_IEC_60559_DFP_PREFER_CUSTOM)

if(_yIO_HAS_UNISTD_H)
cmake_dependent_option(YIO_USE_INPUT_FD 1 CACHE BOOL
	"When set to true, yscan will read from file descriptor 0 instead of FILE *stdin")
var_to_0_or_1(YIO_USE_INPUT_FD)
cmake_dependent_option(YIO_USE_OUTPUT_FD 1 CACHE BOOL
	"When set to true, yprint will read from file descriptor 1 instead of FILE *stdout")
var_to_0_or_1(YIO_USE_OUTPUT_FD)
endif()
