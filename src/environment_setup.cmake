# environment_setup.cmake

# This script sets up the environment for checks and detects available types.
# It should be included before third_party as some third_party libraries
# might depend on the types detected here.

# cmake
include(CheckCSourceCompiles)
include(CheckTypeSize)
include(CheckIncludeFile)
include(CheckSymbolExists)
include(CMakePushCheckState)
include(k/foreach_count_items)
include(k/check_type_exists_bool)
include(k/check_symbol_exists_bool)

if(YYIO_ENVIRONMENT_SETUP_DONE)
	return()
endif()
set(YYIO_ENVIRONMENT_SETUP_DONE 1)

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

yio_config_gen_check_c_source_compiles(
	"int main() { __int128 x; return 0; }"
	YYIO_HAS_INT128
)

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
		"int main() { _Generic((${type1})0, ${type1}: 0, ${type2}: 0); }"
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
	if(NOT DEFINED YIO_HAS_FLOAT${suffix} AND NOT DEFINED HAVE_YIO_HAS_FLOAT${suffix})
		message(STATUS "Detecting type: '${type}'")
	endif()

	if(NOT DEFINED YIO_HAS_FLOAT${suffix})
		check_type_exists_bool(${type} YIO_HAS_FLOAT${suffix} BUILTIN_TYPES_ONLY LANGUAGE C)
	endif()
	yio_config_gen_add(YIO_HAS_FLOAT${suffix})
	if(YIO_HAS_FLOAT${suffix})
		list(APPEND YIO_FLOAT_SUFFIXES ${suffix})
		yio_config_gen_add_value(YYIO_FLOAT${suffix} ${type})
	endif()
endforeach()
set(YIO_FLOAT_SUFFIXES "${YIO_FLOAT_SUFFIXES}" CACHE INTERNAL "")
