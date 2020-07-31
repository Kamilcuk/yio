cmake_minimum_required(VERSION 3.12)

# This script checks all that is needed from the environment
# Ie. all possible needed functions
# Posix compliance
# Available strfrom* functions
# Available headers
# And all the rest

# cmake
include(CheckCSourceCompiles)
include(CheckTypeSize)
include(CheckIncludeFile)
include(CheckSymbolExists)
include(CMakePushCheckState)
include(foreach_count_items)
include(check_type_exists_bool)
include(check_symbol_exists_bool)

cmake_push_check_state(RESET)

# In case of compiling with lto (or just for safety with gcc)
# add -fno-lto, otherwise size checking does not work perfectly
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_FLAGS MATCHES "-flto")
	set(CMAKE_REQUIRED_FLAGS "-fno-lto")
endif()

set(CMAKE_REQUIRED_DEFINITIONS 
	-D_GNU_SOURCE=1
	-D _POSIX_C_SOURCE=1
	-D _BSD_SOURCE=1
	-D _SVID_SOURCE=1
	-D__STDC_WANT_LIB_EXT2__=1
	-D__STDC_WANT_DEC_FP__=1
	-D__STDC_WANT_IEC_60559_DFP_EXT__=1
	-D__STDC_WANT_IEC_60559_BFP_EXT__=1
	-D__STDC_WANT_IEC_60559_FUNCS_EXT__=1
	-D__STDC_WANT_IEC_60559_TYPES_EXT__=1
)
set(CMAKE_REQUIRED_LIBRARIES
	m
)

#########################################################################
# Check if _Generic is supported

check_c_source_compiles("int main() { return _Generic(1, int: 1, default: 0); }" _yIO_HAS__Generic)
if(NOT _yIO_HAS__Generic)
	message(SEND_ERROR "Selected compiler has no C11 _Generic() support, compilation WILL fail")
endif()

#########################################################################
# Setup yio_config_gen.h

# All output is buffered to this vairable
set(yio_config_gen_content)

string(APPEND yio_config_gen_content [=[
// yio_config_gen.h
]=])

# Use this function to output
macro(yio_config_gen_add symbol)
	if(DEFINED ${symbol} AND ${symbol})
		string(APPEND yio_config_gen_content "#define ${symbol}  ERROR\n")
	else()
		string(APPEND yio_config_gen_content "/* #undef  ${symbol} */\n")
	endif()
endmacro()

macro(yio_config_gen_check_include_file)
	check_include_file(${ARGV})
	yio_config_gen_add(${ARGV1})
endmacro()

macro(yio_config_gen_check_type_exists)
	check_type_exists_bool(${ARGV})
	yio_config_gen_add(${ARGV1})
endmacro()

macro(yio_config_gen_check_symbol_exists)
	check_symbol_exists_bool(${ARGV})
	yio_config_gen_add(${ARGV2})
endmacro()

#########################################################################
# some generic checks

yio_config_gen_check_include_file("unistd.h" _yIO_HAS_UNISTD_H)
if(_yIO_HAS_UNISTD_H)
	list(APPEND CMAKE_REQUIRED_INCLUDES
		"unistd.h"
		"sys/time.h"
	)
endif()

yio_config_gen_check_include_file("float.h"  _yIO_HAS_FLOAT_H)
yio_config_gen_check_include_file("stdfix.h"  _yIO_HAS_STDFIX)

yio_config_gen_check_type_exists(__int128 _yIO_HAS_INT128 BUILTIN_TYPES_ONLY LANGUAGE C)
check_symbol_exists_bool(asprintf "stdio.h" _yIO_HAS_asprintf LANGUAGE C)

yio_config_gen_check_type_exists("struct timespec" _yIO_HAS_timespec LANGUAGE C)
yio_config_gen_check_type_exists("struct timeval" _yIO_HAS_timeval LANGUAGE C)
yio_config_gen_check_symbol_exists(localtime_r "time.h" _yIO_HAS_localtime_r LANGUAGE C)

yio_config_gen_check_symbol_exists(strnlen "string.h" _yIO_HAS_strnlen LANGUAGE C)

#########################################################################
# handle and detect floats

# the list of floats
#TODO: typedef _Complex float __attribute__((mode(TC))) _Complex128;
set(floats
	# type          MACRO_SUFFIX  suffix   suffix_for_str{from,to}
	"float"         "FLT"         "f"      "f"
	"double"        "DBL"         ""       "d" # GOSH!
	"long double"   "LDBL"        "l"      "l"
	
	"_Float16"      "FLT16"       "f16"    "f16"
	"_Float32"      "FLT32"       "f32"    "f32"
	"_Float64"      "FLT64"       "f64"    "f64"
	"_Float128"     "FLT128"      "f128"   "f128"
	"_Float32x"     "FLT32X"      "f32x"   "f32x"
	"_Float64x"     "FLT64X"      "f64x"   "f64x"
#	"_Float128x"    "FLT128X"     "f128x"  "f128x"
	
	"_Decimal32"    "DEC32"       "d32"    "d32"
	"_Decimal64"    "DEC64"       "d64"    "d64"
	"_Decimal128"   "DEC128"      "d128"   "d128"
#	"_Decimal32x"   "DEC32X"      "d32x"   "d32x"
#	"_Decimal64x"   "DEC64X"      "d64x"   "d64x"
#	"_Decimal128x"  "DEC128X"     "d128x"  "d128x"
)

set(be_verbose FALSE)

foreach(i IN LISTS floats)
	foreach_count_items(i foreachstatevar 
		type msuffix suffix suffix2
	)
	if(i)
		continue()
	endif()
	if(NOT DEFINED _yIO_HAS_FLOAT${suffix} OR be_verbose)
		log("Detecting: '${type}' '${msuffix}' '${suffix}' '${suffix2}'")
		
		check_type_exists_bool(${type} _yIO_HAS_FLOAT${suffix} BUILTIN_TYPES_ONLY LANGUAGE C)
		if(_yIO_HAS_FLOAT${suffix})
			check_symbol_exists_bool(exp10${suffix} "math.h" _yIO_HAS_exp10${suffix})
			check_symbol_exists_bool(strfrom${suffix2} "stdlib.h" _yIO_HAS_strfrom${suffix2})
			check_symbol_exists_bool(strto${suffix2} "stdlib.h" _yIO_HAS_strto${suffix2})
		endif()
	endif()
	
	foreach(j IN ITEMS FLOAT${suffix} exp10${suffix} strfrom${suffix2} strto${suffix2})
		yio_config_gen_add(_yIO_HAS_${j})
	endforeach()
	
endforeach()

set(_yIO_HAS_strfrom ${_yIO_HAS_strfromd})
yio_config_gen_add(_yIO_HAS_strfrom)

#########################################################################
# handle and detect stdfix

foreach(type IN ITEMS _Fract _Accum)
	if(NOT DEFINED _yIO_HAS_${type} OR be_verbose)
		log("Detecting: ${type}")
	endif()
	check_type_exists_bool(${type} _yIO_HAS_${type} BUILTIN_TYPES_ONLY)
	yio_config_gen_add(_yIO_HAS_${type})
endforeach()

#########################################################################
# Output variable yio_config_gen_content to yio_config_gen.h
# Generate the file only if anything changed

string(APPEND yio_config_gen_content "\n")
set(f ${GENDIR}/yio/yio_config_gen.h)
if(EXISTS ${f})
	file(READ ${f} fcontent)
else()
	set(fcontent "")
endif()
if(NOT fcontent STREQUAL ${yio_config_gen_content})
	file(WRITE ${f} ${yio_config_gen_content})
endif() 

#########################################################################
