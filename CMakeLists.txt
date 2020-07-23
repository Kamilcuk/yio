# CMakeLists.txt
cmake_minimum_required(VERSION 3.11)

string(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(yio_debug_variables)

include(src/version.cmake)
project(yio
	VERSION "${YIO_PROJECT_VERSION}"
	LANGUAGES C
	DESCRIPTION "Yio Input Output C Library that aims in type-safe printf replacement"
	HOMEPAGE_URL "https://gitlab.com/kamcuk/yio"
)

include(force_build_dir_without_cmakelists)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(0)
	find_program(CPPCHECK_COMMAND "cppcheck")
	if(CPPCHECK_COMMAND)
		set(CMAKE_C_CPPCHECK "${CPPCHECK_COMMAND}")
	endif()
endif()

# Upper build type to be used in if statements
string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)
# Source directory
get_filename_component(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/src ABSOLUTE)
# Generated directory
get_filename_component(GENDIR ${CMAKE_CURRENT_BINARY_DIR}/gen ABSOLUTE)

# YIO_DEV is internal variable set if the developer of this library is building it
if(YIO_DEV)
	# Use ccache if available
	find_program(CCACHE_COMMAND "ccache")
	if(CCACHE_COMMAND)
	    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_COMMAND}")
	endif()

	# Create symlink from gen to GENDIR for development purpouses
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E create_symlink ${GENDIR} ${CMAKE_CURRENT_SOURCE_DIR}/gen
	)

	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tags
		COMMAND sh -c '
			git ls-files -z --exclude-standard --others --cached test/ src/ |
			xargs -z -t ctags --recurse --append --extras=+q --fields=+aimS --c-kinds=+p --c++-kinds=+p
			'
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		VERBATIM
	)
	add_custom_command(
		OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/tags
		COMMAND ${CMAKE_COMMAND} -E create_symlink
			${CMAKE_CURRENT_BINARY_DIR}/tags
			${CMAKE_CURRENT_SOURCE_DIR}/tags
	)
	add_custom_target(yio_tags
			DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tags
	)
else()
	add_custom_target(yio_tags
			COMMAND ${CMAKE_COMMAND} -E true
	)
endif()

find_library(UNISTRING_LIB unistring)

yio_debug_variables(
	BUILD_TESTING
	YIO_BUILD_TESTING
	CMAKE_TOOLCHAIN_FILE
	CMAKE_CROSSCOMPILING
	CMAKE_CROSSCOMPILING_EMULATOR
	CMAKE_BINARY_DIR
	CMAKE_CURRENT_BINARY_DIR
	CMAKE_SOURCE_DIR
	CMAKE_CURRENT_SOURCE_DIR
	CMAKE_BUILD_TYPE
	CMAKE_BUILD_TYPE_UPPER
	CMAKE_SYSTEM_NAME
	CMAKE_SYSTEM_PROCESSOR
	CMAKE_C_COMPILER
	CMAKE_C_COMPILER_ID
	CMAKE_C_COMPILER_VERSION
	CMAKE_C_FLAGS
	CMAKE_C_FLAGS_${CMAKE_BUILD_TYPE_UPPER}
	CMAKE_EXE_LINKER_FLAGS
	CMAKE_EXE_LINKER_FLAGS_${CMAKE_BUILD_TYPE_UPPER}
	CMAKE_GENERATOR
	CMAKE_VERBOSE_MAKEFILE
	CMAKE_EXPORT_COMPILE_COMMANDS
	CMAKE_SYSTEM_PREFIX_PATH
	CMAKE_INCLUDE_PATH
	CMAKE_LIBRARY_PATH
	CMAKE_PROGRAM_PATH
	CMAKE_IGNORE_PATH
	CMAKE_SYSTEM_PREFIX_PATH
	CMAKE_LIBRARY_ARCHITECTURE
	CMAKE_LIBRARY_PATH
	CMAKE_FIND_ROOT_PATH
	CMAKE_SYSROOT
	CMAKE_IGNORE_PATH
	CMAKE_FIND_ROOT_PATH_MODE_PROGRAM
	CMAKE_FIND_ROOT_PATH_MODE_LIBRARY
	CMAKE_FIND_ROOT_PATH_MODE_INCLUDE
	CMAKE_FIND_ROOT_PATH_MODE_PACKAGE
	CCACHE_FOUND
	UNISTRING_LIB
)

foreach(i IN ITEMS CMAKE_TOOLCHAIN_FILE CMAKE_CROSSCOMPILING_EMULATOR)
	if(DEFINED "${i}" AND NOT EXISTS "${${i}}")
		message(FATAL_ERROR "${i} is defined but file does not exists")
	endif()
endforeach()

add_subdirectory(src)

if((YIO_BUILD_TESTING OR YIO_DEV) AND BUILD_TESTING)
	include(CTest)
    add_subdirectory(test)
endif()



