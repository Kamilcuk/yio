# environment_checks.cmake

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
include(k/foreach_count_items)
include(k/check_type_exists_bool)
include(k/check_symbol_exists_bool)

cmake_push_check_state(RESET)

# In case of compiling with lto (or just for safety with gcc)
# add -fno-lto, otherwise size checking does not work perfectly
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_FLAGS MATCHES "-flto")
	list(APPEND CMAKE_REQUIRED_FLAGS "-fno-lto")
endif()

list(APPEND CMAKE_REQUIRED_DEFINITIONS
	-D_GNU_SOURCE=1
	-D_XOPEN_SOURCE=700
	-D_POSIX_C_SOURCE=200809L
	-D_BSD_SOURCE=1
	-D_SVID_SOURCE=1
	-D_DEFAULT_SOURCE=1
	-D__STDC_WANT_LIB_EXT2__=1
	-D__STDC_WANT_DEC_FP__=1
	-D__STDC_WANT_IEC_60559_EXT__=1
	-D__STDC_WANT_IEC_60559_DFP_EXT__=1
	-D__STDC_WANT_IEC_60559_BFP_EXT__=1
	-D__STDC_WANT_IEC_60559_FUNCS_EXT__=1
	-D__STDC_WANT_IEC_60559_TYPES_EXT__=1
)
list(APPEND CMAKE_REQUIRED_LIBRARIES
	m
)

#########################################################################
# some generic checks

if(YYIO_HAS_UNISTD_H)
	list(APPEND CMAKE_EXTRA_INCLUDE_FILES
		"unistd.h"
		"sys/time.h"
	)
endif()

set(YYIO_HAS_FLOAT_H_COMMENT "Do we have float.h?")
yio_config_gen_check_include_file("float.h" YYIO_HAS_FLOAT_H)
set(YYIO_HAS_FENV_H_COMMENT "Do we have fenv.h?")
yio_config_gen_check_include_file("fenv.h" YYIO_HAS_FENV_H)
set(YYIO_HAS_NUMERIC_H_COMMENT "Do we have numeric.h?")
if(YYIO_HAS_UNISTD_H)
	yio_config_gen_check_include_file("monetary.h" YYIO_HAS_MONETARY_H)
else()
	yio_config_gen_add_value(YYIO_HAS_MONETARY_H 0)
endif()

yio_config_gen_check_symbol_exists(asprintf "stdio.h" YYIO_HAS_asprintf LANGUAGE C)

yio_config_gen_check_type_exists("struct timespec" YYIO_HAS_timespec LANGUAGE C)
yio_config_gen_check_type_exists("struct timeval" YYIO_HAS_timeval LANGUAGE C)

yio_config_gen_check_symbol_exists(strnlen "string.h" YYIO_HAS_strnlen LANGUAGE C)

yio_config_gen_check_include_file("wchar.h"  YIO_HAS_WCHAR_H)
yio_config_gen_check_symbol_exists(wcswidth "wchar.h" YYIO_HAS_wcswidth LANGUAGE C)

yio_config_gen_check_include_file("uchar.h"  YIO_HAS_UCHAR_H)

if(UNISTRING_LIB)
	set(YYIO_HAS_UNISTRING 1)
endif()
yio_config_gen_add(YYIO_HAS_UNISTRING)

set(YIO_USE_STRFROM_PRINTF 1)
yio_config_gen_add_value(YIO_USE_STRFROM_PRINTF 1)
yio_config_gen_add_value(YIO_USE_STRFROM_RYU 1)

#########################################################################
# handle and detect _floats

# the list of _floats
set(_floats
	#type            mathsuffix suffix    strtosuffix
	"float"          "f"        "f"       "f"
	"double"         ""         "d"       "d"
	"long double"    "l"        "l"       "ld"

	"_Float16"       "f16"      "f16"     "f16"
	"_Float32"       "f32"      "f32"     "f32"
	"_Float64"       "f64"      "f64"     "f64"
	"_Float128"      "f128"     "f128"    "f128"
	"_Float32x"      "f32x"     "f32x"    "f32x"
	"_Float64x"      "f64x"     "f64x"    "f64x"
	"_Float128x"     "f128x"    "f128x"   "f128x"

	"_Decimal32"     "d32"      "d32"     "d32"
	"_Decimal64"     "d64"      "d64"     "d64"
	"_Decimal128"    "d128"     "d128"    "d128"
	"_Decimal32x"    "d32x"     "d32x"    "d32x"
	"_Decimal64x"    "d64x"     "d64x"    "d64x"
	"_Decimal128x"   "d128x"    "d128x"   "d128x"
)

# If two types are the same, set YIO_HAS_FLOAT${suffix} to 0, to exclude the second type from _Generic.
function(exclude_same type1 type2 suffix)
	check_c_source_compiles(
		"int main() { return _Generic((${type1})0, ${type1}: 0, ${type2}: 0); }"
		YYIO_samecompiles_FLOAT${suffix}
	)
	if(NOT YYIO_samecompiles_FLOAT${suffix})
		# cmake-lint: disable=C0103
		set(YIO_HAS_FLOAT${suffix} 0 CACHE INTERNAL "")
	endif()
endfunction()
# Intel compiler
exclude_same("float" "_Float32" f32)
exclude_same("double" "_Float32x" f32x)
exclude_same("double" "_Float64" f64)

set(YIO_FLOAT_SUFFIXES)
foreach(ii IN LISTS _floats)
	foreach_count_items(ii foreachstatevar
		type mathsuffix suffix strtosuffix
	)
	if(ii)
		continue()
	endif()
	if(NOT DEFINED YIO_HAS_FLOAT${suffix})
		message(STATUS "Detecting: '${type}' '${mathsuffix}' '${suffix}' '${strtosuffix}'")
	endif()

	if(NOT DEFINED YIO_HAS_FLOAT${suffix})
		check_type_exists_bool(${type} YIO_HAS_FLOAT${suffix} BUILTIN_TYPES_ONLY LANGUAGE C)
	endif()
	yio_config_gen_add(YIO_HAS_FLOAT${suffix})
	if(YIO_HAS_FLOAT${suffix})
		list(APPEND YIO_FLOAT_SUFFIXES ${suffix})
		yio_config_gen_add_value(YYIO_FLOAT${suffix} ${type})
		check_symbol_exists_bool(exp10${mathsuffix}  "math.h"    YYIO_HAS_exp10${suffix})
		check_symbol_exists_bool(strfrom${suffix}    "stdlib.h"  YYIO_HAS_strfrom${suffix})
		check_symbol_exists_bool(strto${strtosuffix} "stdlib.h"  YYIO_HAS_strto${suffix})
		foreach(ii IN ITEMS exp10 strfrom strto)
			yio_config_gen_add(YYIO_HAS_${ii}${suffix})
		endforeach()
	endif()
endforeach()
set(YIO_FLOAT_SUFFIXES "${YIO_FLOAT_SUFFIXES}" PARENT_SCOPE)

yio_config_gen_check_c_source_compiles([=[
float _Imaginary fi = 1;
double _Imaginary di = 2;
long double _Imaginary li = 3;
int main() {}
]=] YYIO_HAS_IMAGINARY)

yio_config_gen_check_c_source_compiles([=[
#include <complex.h>
#if defined __STDC_NO_COMPLEX__ || ! defined __STDC_IEC_559_COMPLEX__
#error
#endif
int main() {}
]=] YYIO_HAS_COMPLEX)

if(NOT DEFINED YYIO_MUSL_BROKEN_EXP10)
	if(NOT CMAKE_CROSSCOMPILING AND YYIO_HAS_exp10l AND YYIO_HAS_FLOAT_H)
		set(_o  ${CMAKE_CURRENT_BINARY_DIR}/checkexp10)
		file(MAKE_DIRECTORY ${_o})
		file(WRITE ${_o}/checkexp10.c [=[
#include <math.h>
int main() {
	return powl(10.0, -4933) == 0;
}
]=]
		)
		try_run(
			runres compileres
			${_o} ${_o}/checkexp10.c
			LINK_LIBRARIES m
		)
		if(compileres AND runres EQUAL 1)
			set(YYIO_MUSL_BROKEN_EXP10 1 CACHE INTERNAL "")
		endif()
	endif()
endif()
if(NOT DEFINED YYIO_MUSL_BROKEN_EXP10)
	set(YYIO_MUSL_BROKEN_EXP10 0 CACHE INTERNAL "")
endif()
yio_config_gen_add(YYIO_MUSL_BROKEN_EXP10)

#########################################################################
# handle and detect stdfix

yio_config_gen_check_include_file("stdfix.h"  YYIO_HAS_STDFIX_H)
yio_config_gen_check_type_exists(_Fract YYIO_HAS_STDFIX_TYPES BUILTIN_TYPES_ONLY LANGUAGE C)

#########################################################################

yio_config_gen_fini()

#########################################################################
