cmake_minimum_required(VERSION 3.2)
project(yio C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
include(CheckTypeSize)
include(m4)
include(list_filter)
include(var_to_0_or_1)
include(check_type_size_bool)

macro(dbg)
	message(STATUS "yio: ${ARGN}")
endmacro()

set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})
set(GENDIR ${CMAKE_CURRENT_BINARY_DIR})
set(GENDIR ${CMAKE_CURRENT_SOURCE_DIR}/../gen)
get_filename_component(GENDIR "${GENDIR}" ABSOLUTE)

# Configuration options #############################################
set(YIO_M4_MLVLS 10 CACHE STRING
	"The count of levels variadic macros expand to")
set(YIO_M4_SLOTS_END 110 CACHE STRING
	"The upper count of slots available in overloads")
set(YIO_COMPILES_PEDANTIC 0 CACHE BOOL
	"Set to true if compiling with pedantic")
var_to_0_or_1(YIO_COMPILES_PEDANTIC)

# generate config.m4 #################################################
configure_file(
	${SRCDIR}/config.m4.in
	${GENDIR}/config.m4
	@ONLY
)

# generate yio_config.h file #########################################
check_type_size_bool(__int128  INT128 BUILTIN_TYPES_ONLY LANGUAGE C)
check_type_size_bool(_Float32 FLOAT32 BUILTIN_TYPES_ONLY LANGUAGE C)
configure_file(
	${SRCDIR}/yio/yio_config.h.in
	${GENDIR}/yio/yio_config.h
	@ONLY
)

# generate .m4 files ##################################################
m4_add_include_directories(${GENDIR})
m4_add_file_dependencies(${GENDIR}/config.m4)
file(GLOB_RECURSE m4srcs *.c *.h)
foreach(source IN LISTS m4srcs)
	file(RELATIVE_PATH sourcenice ${SRCDIR} ${source})
	set(output ${GENDIR}/${sourcenice})
	# dbg("m4_add_command ${output} ${source}")
	m4_add_command(${output} ${source})
	list(APPEND m4_outputs ${output})
endforeach()

# generate error messages ####################
add_custom_command(
	OUTPUT
		${GENDIR}/yio/yio_error_messages.h
	DEPENDS
		${SRCDIR}/yio/yio_error.h
		${SRCDIR}/yio/yio_error_messages.cmake
	COMMAND ${CMAKE_COMMAND}
		-D OUTPUT=${GENDIR}/yio/yio_error_messages.h
		-D INPUT=${SRCDIR}/yio/yio_error.h
		-P ${CMAKE_CURRENT_SOURCE_DIR}/yio/yio_error_messages.cmake
	COMMENT "Generate yio/yio_error_messages.h file"
)
list(APPEND srcs ${GENDIR}/yio/yio_error_messages.h)

# Generate all files target ###########################################
add_custom_target(_yio_gen DEPENDS ${m4_outputs} ${srcs})

# Compile options #######################################################
set(properties
	C_EXTENSIONS YES
	C_STANDARD 11
	C_STANDARD_REQUIRED NO
	# INTERPROCEDURAL_OPTIMIZATION TRUE
)
set(compile_options
	$<$<C_COMPILER_ID:GNU>:
		-Wall
		-Wextra
		-Wwrite-strings
		-Wno-unused-function
		-Wno-unused-argument
	>
)
macro(yio_add_library name)
	add_library(${name} ${ARGN})
	target_include_directories(${name} PUBLIC ${GENDIR})
	set_property(TARGET ${name} PROPERTY ${properties})
	target_compile_options(${name} PRIVATE ${compile_options})
endmacro()

# add yio executable ##############################################
set(common_srcs
	${GENDIR}/yio/yio_error.c
	${GENDIR}/yio/yio_error.h
	${GENDIR}/yio/yio_error_messages.h
	${GENDIR}/yio/yio_macros_gen.h
	${GENDIR}/yio/yio_config.h
	${GENDIR}/yio/yio_common.h
	${GENDIR}/yio/yio_macros.h
	${GENDIR}/yio/intprops.h
)
yio_add_library(yio
	${common_srcs}
	${GENDIR}/yio.h
	${GENDIR}/yio/yio.c
	${GENDIR}/yio/yio.h
	${GENDIR}/yio/yio_public.h
	${GENDIR}/yio/yio_private.c
	${GENDIR}/yio/yio_private.h
	${GENDIR}/yio/yio_ctx.c
	${GENDIR}/yio/yio_ctx.h
	${GENDIR}/yio/yio_fmt.c
	${GENDIR}/yio/yio_fmt.h
	${GENDIR}/yio/manip/yio_print_chars.c
	${GENDIR}/yio/manip/yio_print_int.c
	${GENDIR}/yio/manip/yio_print_modifiers.c
	${GENDIR}/yio/manip/yio_print_float.c
	${GENDIR}/yio/manip/yio_scan_int.c
	${GENDIR}/yio/manip/yio_scan_modifiers.c
	${GENDIR}/yio/manip/yio_scan_string.c
)

