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
	list(APPEND CMAKE_REQUIRED_FLAGS "-fno-lto")
endif()

list(APPEND CMAKE_REQUIRED_DEFINITIONS
	-D_GNU_SOURCE=1
	-D_XOPEN_SOURCE=1
	-D_POSIX_C_SOURCE=1
	-D_BSD_SOURCE=1
	-D_SVID_SOURCE=1
	-D_DEFAULT_SOURCE=1
	-D__STDC_WANT_LIB_EXT2__=1
	-D__STDC_WANT_DEC_FP__=1
	-D__STDC_WANT_IEC_60559_DFP_EXT__=1
	-D__STDC_WANT_IEC_60559_BFP_EXT__=1
	-D__STDC_WANT_IEC_60559_FUNCS_EXT__=1
	-D__STDC_WANT_IEC_60559_TYPES_EXT__=1
)
list(APPEND CMAKE_REQUIRED_LIBRARIES
	m
)

#########################################################################
# Check if _Generic is supported

check_c_source_compiles("int main() { return _Generic(1, int: 1, default: 0); }" _yIO_HAS__Generic)
if(NOT _yIO_HAS__Generic)
	message(SEND_ERROR "Selected compiler has no C11 _Generic() support, compilation WILL fail")
endif()


#########################################################################
# Add configuration

foreach(i IN ITEMS
	YIO_PRINT_FLOATS_WITH
)
	if(NOT DEFINED "${i}")
		message(FATAL_ERROR "YIO Internal error: ${i} is not defined")
	endif()
	if(NOT DEFINED "${i}_COMMENT")
		message(FATAL_ERROR "YIO Internal error: ${i}_COMMENT is not defined")
	endif()
	yio_config_gen_add_value("${i}" "${${i}}")
endforeach()

if(_yIO_HAS_UNISTD_H)
	foreach(i IN ITEMS
		YIO_USE_INPUT_FD
		YIO_USE_OUTPUT_FD
	)
		if(NOT DEFINED "${i}")
			message(FATAL_ERROR "YIO Internal error: ${i} is not defined")
		endif()
		if(NOT DEFINED "${i}_COMMENT")
			message(FATAL_ERROR "YIO Internal error: ${i}_COMMENT is not defined")
		endif()
		yio_config_gen_add("${i}")
	endforeach()
endif()

#########################################################################
# some generic checks

if(_yIO_HAS_UNISTD_H)
	list(APPEND CMAKE_EXTRA_INCLUDE_FILES
		"unistd.h"
		"sys/time.h"
	)
endif()

set(_yIO_HAS_FLOAT_H_COMMENT "Do we have float.h?")
yio_config_gen_check_include_file("float.h" _yIO_HAS_FLOAT_H)

yio_config_gen_check_type_exists(__int128 _yIO_HAS_INT128 BUILTIN_TYPES_ONLY LANGUAGE C)
yio_config_gen_check_symbol_exists(asprintf "stdio.h" _yIO_HAS_asprintf LANGUAGE C)

yio_config_gen_check_type_exists("struct timespec" _yIO_HAS_timespec LANGUAGE C)
yio_config_gen_check_type_exists("struct timeval" _yIO_HAS_timeval LANGUAGE C)
yio_config_gen_check_symbol_exists(localtime_r "time.h" _yIO_HAS_localtime_r LANGUAGE C)

yio_config_gen_check_symbol_exists(strnlen "string.h" _yIO_HAS_strnlen LANGUAGE C)

yio_config_gen_check_include_file("wchar.h"  _yIO_HAS_WCHAR_H)
yio_config_gen_check_symbol_exists(wcswidth "wchar.h" _yIO_HAS_wcswidth LANGUAGE C)

yio_config_gen_check_include_file("uchar.h"  _yIO_HAS_UCHAR_H)

if(UNISTRING_LIB)
	set(_yIO_HAS_UNISTRING 1)
endif()
yio_config_gen_add(_yIO_HAS_UNISTRING)

#########################################################################
# handle and detect floats

# the list of floats
set(floats
	# type          MACRO_SUFFIX  suffix_for_math  suffix_for_strfrom*  suffix_for_strto*
	"float"         "FLT"         "f"      "f"      "f"
	"double"        "DBL"          ""      "d"      "d"
	"long double"   "LDBL"        "l"      "l"      "ld"

	#"_Float16"      "FLT16"       "f16"    "f16"    "f16"
	#"_Float32"      "FLT32"       "f32"    "f32"    "f32"
	#"_Float64"      "FLT64"       "f64"    "f64"    "f64"
	#"_Float128"     "FLT128"      "f128"   "f128"   "f128"
	#"_Float32x"     "FLT32X"      "f32x"   "f32x"   "f32x"
	#"_Float64x"     "FLT64X"      "f64x"   "f64x"   "f64x"
	#"_Float128x"    "FLT128X"     "f128x"  "f128x"  "f128x"

	"_Decimal32"    "DEC32"       "d32"    "d32"    "d32"
	"_Decimal64"    "DEC64"       "d64"    "d64"    "d64"
	"_Decimal128"   "DEC128"      "d128"   "d128"   "d128"
	#"_Decimal32x"   "DEC32X"      "d32x"   "d32x"   "d32x"
	#"_Decimal64x"   "DEC64X"      "d64x"   "d64x"   "d64x"
	#"_Decimal128x"  "DEC128X"     "d128x"  "d128x"  "d128x"
)

set(be_verbose FALSE)

foreach(i IN LISTS floats)
	foreach_count_items(i foreachstatevar
		type msuffix suffix suffix2 suffix_strto
	)
	if(i)
		continue()
	endif()
	if(NOT DEFINED _yIO_HAS_FLOAT${suffix2} OR be_verbose)
		log("Detecting: '${type}' '${msuffix}' '${suffix}' '${suffix2}'")

		check_type_exists_bool(${type} _yIO_HAS_FLOAT${suffix2} BUILTIN_TYPES_ONLY LANGUAGE C)
		if(_yIO_HAS_FLOAT${suffix2})
			check_symbol_exists_bool(exp10${suffix} "math.h" _yIO_HAS_exp10${suffix2})
			check_symbol_exists_bool(strfrom${suffix2} "stdlib.h" _yIO_HAS_strfrom${suffix2})
			check_symbol_exists_bool(strto${suffix_strto} "stdlib.h" _yIO_HAS_strto${suffix2})
		endif()
	endif()

	foreach(j IN ITEMS FLOAT${suffix2} exp10${suffix2} strfrom${suffix2} strto${suffix2})
		yio_config_gen_add(_yIO_HAS_${j})
	endforeach()

endforeach()

yio_config_gen_check_c_source_compiles([=[
float _Imaginary fi = 1;
double _Imaginary di = 2;
long double _Imaginary li = 3;
int main() {}
]=] _yIO_HAS_IMAGINARY)

yio_config_gen_check_c_source_compiles([=[
#include <complex.h>
#if defined __STDC_NO_COMPLEX__ || ! defined __STDC_IEC_559_COMPLEX__
#error
#endif
int main() {}
]=] _yIO_HAS_COMPLEX)

#########################################################################
# handle and detect stdfix

yio_config_gen_check_include_file("stdfix.h"  _yIO_HAS_STDFIX_H)
yio_config_gen_check_type_exists(_Fract _yIO_HAS_STDFIX_TYPES BUILTIN_TYPES_ONLY LANGUAGE C)

#########################################################################

yio_config_gen_fini()

#########################################################################
