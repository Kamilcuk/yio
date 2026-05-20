# environment_checks_types.cmake

# This script checks for available types and language features.
# It should be included before third_party libraries.

if(YIO_HAS_UNISTD_H)
	list(APPEND CMAKE_EXTRA_INCLUDE_FILES
		"unistd.h"
		"sys/time.h"
	)
endif()

set(YIO_HAS_FLOAT_H_COMMENT "Do we have float.h?")
yio_config_gen_check_include_file("float.h" YIO_HAS_FLOAT_H)
set(YIO_HAS_FENV_H_COMMENT "Do we have fenv.h?")
yio_config_gen_check_include_file("fenv.h" YIO_HAS_FENV_H)
set(YIO_HAS_NUMERIC_H_COMMENT "Do we have numeric.h?")
if(YIO_HAS_UNISTD_H)
	yio_config_gen_check_include_file("monetary.h" YIO_HAS_MONETARY_H)
else()
	yio_config_gen_add_value(YIO_HAS_MONETARY_H 0)
endif()

yio_config_gen_check_type_exists("struct timespec" YIO_HAS_timespec LANGUAGE C)
yio_config_gen_check_type_exists("struct timeval" YIO_HAS_timeval LANGUAGE C)

if(YIO_SDCC)
	yio_config_gen_check_type_exists(FILE YIO_HAS_FILE LANGUAGE C)
else()
	yio_config_gen_add_value(YIO_HAS_FILE 1)
endif()

yio_config_gen_check_include_file("wchar.h"  YIO_HAS_WCHAR_H)
yio_config_gen_check_include_file("uchar.h"  YIO_HAS_UCHAR_H)

# Check if signed char is unique (vs char).
yio_config_gen_check_c_source_compiles(
	"int main() { return _Generic((signed char)0, char: 0, signed char: 1); }"
	YIO_HAS_UNIQUE_SCHAR
)

# Check if unsigned char is unique (vs char).
yio_config_gen_check_c_source_compiles(
	"int main() { return _Generic((unsigned char)0, char: 0, unsigned char: 1); }"
	YIO_HAS_UNIQUE_UCHAR
)

# Check if wchar_t is a unique type or an alias.
if(YIO_HAS_WCHAR_H)
	set(add "")
  if(YIO_HAS_UNIQUE_UCHAR)
  	set(add "unsigned char: 0, ")
  endif()
  if(YIO_HAS_UNIQUE_SCHAR)
    set(add "signed char: 0, ")
  endif()
  yio_config_gen_check_c_source_compiles("
  #include <wchar.h>
  int main() {
      return _Generic((wchar_t)0,
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
  }" YIO_HAS_UNIQUE_WCHAR_T)
else()
	yio_config_gen_add_value(YIO_HAS_UNIQUE_WCHAR_T 0)
endif()

if(UNISTRING_LIB)
	set(YIO_HAS_UNISTRING 1)
endif()
yio_config_gen_add(YIO_HAS_UNISTRING)

set(YIO_USE_STRFROM_PRINTF 1)
yio_config_gen_add_value(YIO_USE_STRFROM_PRINTF 1)
yio_config_gen_add_value(YIO_USE_STRFROM_RYU 1)

if(CMAKE_C_COMPILER_ID STREQUAL "SDCC")
	yio_config_gen_add_value(YIO_HAS_LLONG 0)
else()
	yio_config_gen_add_value(YIO_HAS_LLONG 1)
endif()

# Does _Generic differentiate between pointers to const and non-const elements?
yio_config_gen_check_C_source_compiles(
	"int main() { return _Generic((const char *)0, char *: 0, const char *: 1); }"
	YIO_HAS_UNIQUE_CONSTPOINTER
)

yio_config_gen_check_c_source_compiles([=[
float _Imaginary fi = 1;
double _Imaginary di = 2;
long double _Imaginary li = 3;
int main() {}
]=] YIO_HAS_IMAGINARY)

yio_config_gen_check_c_source_compiles([=[
float _Complex fi = 1;
double _Complex di = 2;
long double _Complex li = 3;
int main() {}
]=] YIO_HAS_COMPLEX)

# Detect stdfix support.
yio_config_gen_check_type_exists(_Fract YIO_HAS_STDFIX_TYPES BUILTIN_TYPES_ONLY LANGUAGE C)

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
if(YIO_HAS_STDFIX_TYPES)
	yio_config_gen_check_include_file("stdfix.h"  YIO_HAS_STDFIX_H)
	set(i 0)
	foreach(type IN LISTS _stdfix_types)
		list(GET _stdfix_suffixes ${i} suffix)
		yio_config_gen_check_type_exists("${type}" YIO_HAS_STDFIX_${suffix} BUILTIN_TYPES_ONLY LANGUAGE C)
		math(EXPR i "${i} + 1")
	endforeach()
else()
	yio_config_gen_add(YIO_HAS_STDFIX_H 0)
	foreach(suffix IN LISTS _stdfix_suffixes)
		yio_config_gen_add(YIO_HAS_STDFIX_${suffix} 0)
	endforeach()
endif()
