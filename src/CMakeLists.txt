cmake_minimum_required(VERSION 3.2)
project(yio C)

include(CheckTypeSize)

file(GLOB_RECURSE srcs *.c *.h)
list(FILTER srcs EXCLUDE REGEX yio/gen/.*)

# generate .m4 files ##################################################
file(GLOB_RECURSE m4srcs *.h.m4 *.c.m4)
include(${CMAKE_CURRENT_SOURCE_DIR}/../m4/m4.cmake)
foreach(script IN LISTS m4srcs)	
	get_filename_component(header ${script} NAME)
	string(REGEX REPLACE "\.m4$" "" header ${header})
	get_filename_component(dir ${script} DIRECTORY)
	set(header ${dir}/${header})
	add_m4_target(${header} ${script})
	list(APPEND srcs ${header})
endforeach()

# generate error messages, must be after .m4 files ####################
add_custom_command(
	OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/yio/gen/yio_error_messages_gen.h
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/yio/gen/yio_error_messages_gen.cmake
	COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/yio/gen/yio_error_messages_gen.cmake
	COMMENT Generate yio/gen/yio_error_messages_gen.h file
)
list(APPEND srcs ${CMAKE_CURRENT_SOURCE_DIR}/yio/gen/yio_error_messages_gen.h)

# generate yio_config file #########################################
check_type_size("__int128" INT128 BUILTIN_TYPES_ONLY LANGUAGE C)
if(HAVE_INT128)
	set(HAVE_INT128 1)
else()
	set(HAVE_INT128 0)
endif()
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/yio/gen/yio_config_gen.h.in
	${CMAKE_CURRENT_SOURCE_DIR}/yio/gen/yio_config_gen.h
	@ONLY
)
list(APPEND srcs ${CMAKE_CURRENT_SOURCE_DIR}/yio/gen/yio_config_gen.h)

# the library #######################################################
list(REMOVE_DUPLICATES srcs)
add_library(yio ${srcs})
target_include_directories(yio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET yio PROPERTY 
	C_EXTENSIONS YES
	C_STANDARD 11
	C_STANDARD_REQUIRED 11
)
if(CMAKE_COMPILER_IS_GNUCC)
	target_compile_options(yio PRIVATE
		-Werror
		-Wall
		-Wextra
	)
endif()

