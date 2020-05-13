cmake_minimum_required(VERSION 3.2)
project(yio C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
include(CheckTypeSize)
include(CheckIncludeFile)
include(m4)
include(list_filter)
include(var_to_0_or_1)
include(check_type_size_bool)

macro(dbg)
	message(STATUS "yio: ${ARGN}")
endmacro()

set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})
set(GENDIR ${CMAKE_CURRENT_BINARY_DIR}/gen)
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
	${SRCDIR}/yio_config.m4.in
	${GENDIR}/m4/yio_config.m4
	@ONLY
)

# generate yio_config.h file #########################################
check_type_size_bool(__int128  INT128 BUILTIN_TYPES_ONLY LANGUAGE C)
check_type_size_bool(_Float32 FLOAT32 BUILTIN_TYPES_ONLY LANGUAGE C)
check_type_size_bool(int64_t  INT64_T LANGUAGE C)
check_include_file("unistd.h" HAVE_UNISTD_H)
configure_file(
	${SRCDIR}/yio/yio_config.h.in
	${GENDIR}/yio/yio_config.h
	@ONLY
)

# generate .m4 files ##################################################
m4_add_options(lib.m4)
m4_add_include_directories(${SRCDIR} ${GENDIR}/m4)
m4_add_file_dependencies(${GENDIR}/m4/yio_config.m4)
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

# Generate all files target ###########################################
set(gensrcs
	${m4_outputs}
	${GENDIR}/yio/yio_config.h
	${GENDIR}/yio/yio_error_messages.h
	${GENDIR}/m4/yio_config.m4
)
add_custom_target(_yio_gen DEPENDS ${gensrcs})

# Remove files in GENDIR not in gensrcs ###############################
file(GLOB_RECURSE files ${GENDIR}/*.c ${GENDIR}/*.h ${GENDIR}/*.m4)
function(list_exclude result listin listexclude)
	set(tmp)
	foreach(i IN LISTS ${listin})
		list(FIND "${listexclude}" "${i}" index)
		if (${index} EQUAL -1)
			list(APPEND tmp "${i}")
		endif()
	endforeach()
	set(${result} "${tmp}" PARENT_SCOPE)
endfunction()
list_exclude(files_not_in_gensrcs files gensrcs)
if(files_not_in_gensrcs)
	foreach(i IN LISTS files_not_in_gensrcs)
		message(STATUS "Yio:Removing: ${i}")
	endforeach()
	file(REMOVE ${files_not_in_gensrcs})
endif()

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
		-Wno-unused-parameter
	>
)
macro(yio_add_library name)
	add_library(${name} ${ARGN})
	set_property(TARGET ${name} PROPERTY ${properties})
	target_compile_options(${name} PRIVATE ${compile_options})
	if (NOT ${name} STREQUAL "_yio_common")
		target_link_libraries(${name} _yio_common)
	endif()
endmacro()

# add yio executable ##############################################
yio_add_library(_yio_common OBJECT
	${GENDIR}/yio/intprops.h
	${GENDIR}/yio/yio_error.c
	${GENDIR}/yio/yio_error.h
	${GENDIR}/yio/yio_error_messages.h
	${GENDIR}/yio/yio_macros_gen.h
	${GENDIR}/yio/yio_config.h
	${GENDIR}/yio/yio_common.h
	${GENDIR}/yio/yio_macros.h
	${GENDIR}/yio/yio_private.h
	${GENDIR}/yio/yio_private.c
	${GENDIR}/yio/yio_public.h
)
target_include_directories(_yio_common PUBLIC ${GENDIR})
add_dependencies(_yio_common _yio_gen)

yio_add_library(yio
	${common_srcs}
	${GENDIR}/yio.h

	${GENDIR}/yio/yio/ctx.h
	${GENDIR}/yio/yio/commonctx.h
	${GENDIR}/yio/yio/commonctx.c
	${GENDIR}/yio/yio/fmt_public.h
	${GENDIR}/yio/yio/fmt.h
	${GENDIR}/yio/yio/fmt.c
	${GENDIR}/yio/yio/ctx_public.h
	${GENDIR}/yio/yio/ctx.h
	${GENDIR}/yio/yio/ctx.c
	${GENDIR}/yio/yio/io.h
	${GENDIR}/yio/yio/io.c
	${GENDIR}/yio/yio/io_fd.c
	${GENDIR}/yio/yio/io_vb.c
	${GENDIR}/yio/yio/private.h
	${GENDIR}/yio/yio/public.h
	
	${GENDIR}/yio/yio/manip/manip.h
	${GENDIR}/yio/yio/manip/print_chars.c
	${GENDIR}/yio/yio/manip/print_int.c
	${GENDIR}/yio/yio/manip/print_modifiers.c
	${GENDIR}/yio/yio/manip/print_float.c
	${GENDIR}/yio/yio/manip/scan_int.c
	${GENDIR}/yio/yio/manip/scan_modifiers.c
	${GENDIR}/yio/yio/manip/scan_string.c
)

#yio_add_library(ywio
#	${common_srcs}
#)


