# environment_checks.cmake

# This script checks all that is needed from the environment
# Ie. all possible needed functions
# Posix compliance
# Available strfrom* functions
# Available headers
# And all the rest

include(src/environment_setup.cmake)

if(TARGET dfp)
	list(APPEND CMAKE_REQUIRED_LIBRARIES
		dfp
	)
endif()

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

if(YIO_SDCC)
	yio_config_gen_check_type_exists(FILE YYIO_HAS_FILE LANGUAGE C)
else()
	yio_config_gen_add_value(YYIO_HAS_FILE 1)
endif()

yio_config_gen_check_include_file("wchar.h"  YIO_HAS_WCHAR_H)
if(YIO_HAS_WCHAR_H)
	yio_config_gen_check_symbol_exists(wcwidth "wchar.h" YYIO_HAS_wcwidth LANGUAGE C)
	yio_config_gen_check_symbol_exists(wcsnrtombs "wchar.h" YYIO_HAS_wcsnrtombs LANGUAGE C)
else()
	yio_config_gen_add_value(YYIO_HAS_wcwidth 0)
	yio_config_gen_add_value(YYIO_HAS_wcsnrtombs 0)
endif()

yio_config_gen_check_include_file("uchar.h"  YIO_HAS_UCHAR_H)

# Check if signed char is unique (vs char).
yio_config_gen_check_c_source_compiles(
	"int main() { _Generic((signed char)0, char: 0, signed char: 1); }"
	YYIO_HAS_UNIQUE_SCHAR
)

# Check if unsigned char is unique (vs char).
yio_config_gen_check_c_source_compiles(
	"int main() { _Generic((unsigned char)0, char: 0, unsigned char: 1); }"
	YYIO_HAS_UNIQUE_UCHAR
)

# Check if wchar_t is a unique type or an alias.
if(YIO_HAS_WCHAR_H)
	set(add "")
  if(YYIO_HAS_UNIQUE_UCHAR)
  	set(add "unsigned char: 0, ")
  endif()
  if(YYIO_HAS_UNIQUE_SCHAR)
    set(add "signed char: 0, ")
  endif()
  yio_config_gen_check_c_source_compiles("
  #include <wchar.h>
  int main() {
      _Generic((wchar_t)0,
          char: 0,
          ${add}
          short: 0,
          unsigned short: 0,
          int: 0,
          unsigned int: 0,
          long: 0,
          unsigned long: 0,
          long long: 0,
          unsigned long long: 0,
          wchar_t: 1
      );
  }" YYIO_HAS_UNIQUE_WCHAR_T)
else()
	yio_config_gen_add_value(YYIO_HAS_UNIQUE_WCHAR_T 0)
endif()

if(UNISTRING_LIB)
	set(YYIO_HAS_UNISTRING 1)
endif()
yio_config_gen_add(YYIO_HAS_UNISTRING)

set(YIO_USE_STRFROM_PRINTF 1)
yio_config_gen_add_value(YIO_USE_STRFROM_PRINTF 1)
yio_config_gen_add_value(YIO_USE_STRFROM_RYU 1)

if(CMAKE_C_COMPILER_ID STREQUAL "SDCC")
	yio_config_gen_add_value(YYIO_HAS_LLONG 0)
else()
	yio_config_gen_add_value(YYIO_HAS_LLONG 1)
endif()

# Does _Generic differentiate between pointers to const and non-const elements?
yio_config_gen_check_C_source_compiles(
	"int main() { _Generic((const char *)0, char *: 0, const char *: 0); }"
	YYIO_HAS_UNIQUE_CONSTPOINTER
)

#########################################################################
# handle and detect _floats (function checks)

foreach(ii IN LISTS _floats)
	foreach_count_items(ii foreachstatevar
		type mathsuffix suffix strtosuffix
	)
	if(ii)
		continue()
	endif()

	if(YIO_HAS_FLOAT${suffix})
		foreach(func IN ITEMS exp2 exp10 floor fabs log10 log2 frexp modf pow nextafter)
			check_symbol_exists_bool(${func}${mathsuffix} "math.h" YYIO_HAS_${func}${suffix})
			yio_config_gen_add(YYIO_HAS_${func}${suffix})
		endforeach()
		if(TARGET dfp AND suffix MATCHES "^d")
			# If we link with libdfp, these functions MUST exist.
			yio_config_gen_add_value(YYIO_HAS_strfrom${suffix} 1)
			yio_config_gen_add_value(YYIO_HAS_strto${suffix} 1)
		else()
			check_symbol_exists_bool(strfrom${suffix}    "stdlib.h"  YYIO_HAS_strfrom${suffix})
			check_symbol_exists_bool(strto${strtosuffix} "stdlib.h"  YYIO_HAS_strto${suffix})
			foreach(ii IN ITEMS strfrom strto)
				yio_config_gen_add(YYIO_HAS_${ii}${suffix})
			endforeach()
		endif()
	endif()
endforeach()

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

# Detect stdfix support.
# Clang requires -ffixed-point flag to enable stdfix types.
yio_config_gen_check_type_exists(_Fract YYIO_HAS_STDFIX_TYPES BUILTIN_TYPES_ONLY LANGUAGE C)

set(_stdfix_types
	"short _Fract" "_Fract" "long _Fract" "long long _Fract"
	"short _Accum" "_Accum" "long _Accum" "long long _Accum"
	"unsigned short _Fract" "unsigned _Fract" "unsigned long _Fract" "unsigned long long _Fract"
	"unsigned short _Accum" "unsigned _Accum" "unsigned long _Accum" "unsigned long long _Accum"
)
set(_stdfix_suffixes
	"SFRACT" "FRACT" "LFRACT" "LLFRACT"
	"SACCUM" "ACCUM" "LACCUM" "LLACCUM"
	"USFRACT" "UFRACT" "ULFRACT" "ULLFRACT"
	"USACCUM" "UACCUM" "ULACCUM" "ULLACCUM"
)
if(YYIO_HAS_STDFIX_TYPES)
	yio_config_gen_check_include_file("stdfix.h"  YYIO_HAS_STDFIX_H)
	set(i 0)
	foreach(type IN LISTS _stdfix_types)
		list(GET _stdfix_suffixes ${i} suffix)
		yio_config_gen_check_type_exists("${type}" YYIO_HAS_STDFIX_${suffix} BUILTIN_TYPES_ONLY LANGUAGE C)
		math(EXPR i "${i} + 1")
	endforeach()
else()
	yio_config_gen_add(YYIO_HAS_STDFIX_H 0)
	foreach(suffix IN LISTS _stdfix_suffixes)
		yio_config_gen_add(YYIO_HAS_STDFIX_${suffix} 0)
	endforeach()
endif()

#########################################################################

yio_config_gen_fini()

#########################################################################
