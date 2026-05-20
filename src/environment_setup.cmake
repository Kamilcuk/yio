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

if(YIO_ENVIRONMENT_SETUP_DONE)
	return()
endif()
set(YIO_ENVIRONMENT_SETUP_DONE 1)

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

# Fast path for macro-based detection
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|Icx|Intel")
	execute_process(
		COMMAND ${CMAKE_C_COMPILER} ${CMAKE_C_FLAGS} -dM -E -
		INPUT_FILE /dev/null
		OUTPUT_VARIABLE _YIO_COMPILER_MACROS
		ERROR_QUIET
	)
	if(_YIO_COMPILER_MACROS MATCHES "__SIZEOF_INT128__")
		set(YIO_HAS_INT128 1 CACHE INTERNAL "")
	endif()
	if(_YIO_COMPILER_MACROS MATCHES "__FLT16_MANT_DIG__")
		set(YIO_HAS_FLOATf16 1 CACHE INTERNAL "")
	endif()
	if(_YIO_COMPILER_MACROS MATCHES "__FLT32_MANT_DIG__")
		set(YIO_HAS_FLOATf32 1 CACHE INTERNAL "")
	endif()
	if(_YIO_COMPILER_MACROS MATCHES "__FLT64_MANT_DIG__")
		set(YIO_HAS_FLOATf64 1 CACHE INTERNAL "")
	endif()
	if(_YIO_COMPILER_MACROS MATCHES "__FLT128_MANT_DIG__")
		set(YIO_HAS_FLOATf128 1 CACHE INTERNAL "")
	endif()
	if(_YIO_COMPILER_MACROS MATCHES "__FLT32X_MANT_DIG__")
		set(YIO_HAS_FLOATf32x 1 CACHE INTERNAL "")
	endif()
	if(_YIO_COMPILER_MACROS MATCHES "__FLT64X_MANT_DIG__")
		set(YIO_HAS_FLOATf64x 1 CACHE INTERNAL "")
	endif()
	if(_YIO_COMPILER_MACROS MATCHES "__FLT128X_MANT_DIG__")
		set(YIO_HAS_FLOATf128x 1 CACHE INTERNAL "")
	endif()
	if(_YIO_COMPILER_MACROS MATCHES "__DEC32_MANT_DIG__")
		set(YIO_HAS_FLOATd32 1 CACHE INTERNAL "")
	endif()
	if(_YIO_COMPILER_MACROS MATCHES "__DEC64_MANT_DIG__")
		set(YIO_HAS_FLOATd64 1 CACHE INTERNAL "")
	endif()
	if(_YIO_COMPILER_MACROS MATCHES "__DEC128_MANT_DIG__")
		set(YIO_HAS_FLOATd128 1 CACHE INTERNAL "")
	endif()
	if(_YIO_COMPILER_MACROS MATCHES "__DEC32X_MANT_DIG__")
		set(YIO_HAS_FLOATd32x 1 CACHE INTERNAL "")
	endif()
	if(_YIO_COMPILER_MACROS MATCHES "__DEC64X_MANT_DIG__")
		set(YIO_HAS_FLOATd64x 1 CACHE INTERNAL "")
	endif()
	if(_YIO_COMPILER_MACROS MATCHES "__DEC128X_MANT_DIG__")
		set(YIO_HAS_FLOATd128x 1 CACHE INTERNAL "")
	endif()
endif()

if(NOT DEFINED YIO_HAS_INT128)
	check_type_exists_bool("__int128" YIO_HAS_INT128 BUILTIN_TYPES_ONLY LANGUAGE C)
endif()
yio_config_gen_add(YIO_HAS_INT128)

#########################################################################
# handle and detect _floats

include(src/environment_floats.cmake)

# If two types are the same, set YIO_HAS_FLOAT${suffix} to 0, to exclude the second type from _Generic.
function(exclude_same type1 type2 suffix)
	check_c_source_compiles(
		"int main() { return _Generic((${type1})0, ${type1}: 0, ${type2}: 1); }"
		YIO_samecompiles_FLOAT${suffix}
	)
	if(NOT YIO_samecompiles_FLOAT${suffix})
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

	if(suffix MATCHES "^(f|d|l)$")
		yio_config_gen_add_default(YIO_HAS_FLOAT${suffix} 1)
		if(YIO_HAS_FLOAT${suffix})
			list(APPEND YIO_FLOAT_SUFFIXES ${suffix})
			yio_config_gen_add_value(YIO_FLOAT${suffix} ${type})
		endif()
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
		yio_config_gen_add_value(YIO_FLOAT${suffix} ${type})
	endif()
endforeach()
set(YIO_FLOAT_SUFFIXES "${YIO_FLOAT_SUFFIXES}" CACHE INTERNAL "")

#########################################################################
# handle and detect _floats
