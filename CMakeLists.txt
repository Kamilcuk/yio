# CMakeLists.txt
cmake_minimum_required(VERSION 3.11)
cmake_policy(SET CMP0054 NEW) #-- fixes IF() with quoted args
cmake_policy(SET CMP0057 NEW) #-- enables IF(.. IN_LIST ..)
cmake_policy(SET CMP0075 NEW) #-- uses CMAKE_REQUIRED_LIBRARIES
if(POLICY CMP0116)
  cmake_policy(SET CMP0116 NEW) #-- DEPFILE is relative
endif()
if(POLICY CMP0177)
  cmake_policy(SET CMP0177 NEW) #-- install() DESTINATION paths are normalized
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(kcmakelib)
include(yio_debug_variables)

project(yio
  LANGUAGES C CXX
	DESCRIPTION "YIO Iconic Output C Library that aims in type-safe printf replacement"
	HOMEPAGE_URL "https://gitlab.com/kamcuk/yio"
)
include(src/version.cmake)
set(PROJECT_VERSION "${YIO_PROJECT_VERSION}")

include(CheckCCompilerFlag)
include(k/force_build_dir_without_cmakelists)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Upper build type to be used in if statements
string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)
# Source directory
get_filename_component(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/src ABSOLUTE)
# Generated directory
get_filename_component(GENDIR ${CMAKE_CURRENT_BINARY_DIR}/gen ABSOLUTE)

find_library(UNISTRING_LIB
	NAMES unistring
	DOC "Path to unistring library"
)

###############################################################################

if(YIO_DEV)
	include(yio_dev)
endif(YIO_DEV)

###############################################################################

function(yio_cdt_parser tgt)
	add_library(${tgt} EXCLUDE_FROM_ALL ${ARGN})
	target_compile_definitions(${tgt} PRIVATE
		__CDT_PARSER__=1
		YYIO_CDT_PARSER=1
		YIO_HAS_FLOATf32=0
		YIO_HAS_FLOATf64=0
		YIO_HAS_FLOATf32x=0
		YIO_HAS_FLOATf64x=0
		YIO_HAS_FLOATf128=0
		YIO_HAS_FLOATf128x=0
		YIO_HAS_FLOATd32=0
		YIO_HAS_FLOATd64=0
		YIO_HAS_FLOATd128=0
		YIO_HAS_FLOATd32x=0
		YIO_HAS_FLOATd64x=0
		YIO_HAS_FLOATd128x=0
	)
	target_include_directories(${tgt} PRIVATE
		${GENDIR}
	)
	target_link_libraries(${tgt} PRIVATE library_that_does_not_exists)
endfunction()

###############################################################################

find_program(PYTHON_EXE NAMES python3 python REQUIRED)
set(JINJASCRIPT ${SRCDIR}/preprocess.py)
set(JINJAPROJECTSRC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_custom_target(yio_gen_prepare
	DEPENDS "${JINJASCRIPT}"
	COMMENT "jinja2: preproces script target"
)

#!
set_property(GLOBAL PROPERTY YIO_JINJA_TASKS)
set_property(GLOBAL PROPERTY YIO_JINJA_OUTPUTS)
set_property(GLOBAL PROPERTY YIO_JINJA_DEPENDS)

function(jinja_generate)
	cmake_parse_arguments(A "" "SOURCE;OUTPUT;TARGET" "DEPENDS" ${ARGN})
	if(NOT A_TARGET)
		message(FATAL_ERROR "jinja_generate: You have to give a target to append source into")
	endif()

	if(IS_ABSOLUTE "${A_SOURCE}")
		set(source_abs "${A_SOURCE}")
	else()
		set(source_abs "${CMAKE_CURRENT_SOURCE_DIR}/${A_SOURCE}")
	endif()

	# Handle CMake dependencies for the target
	set_source_files_properties("${A_OUTPUT}" PROPERTIES GENERATED TRUE)
	set_property(TARGET "${A_TARGET}" APPEND PROPERTY SOURCES "${A_OUTPUT}")
	add_dependencies(yio_gen ${A_TARGET})

	# Collect task
	set_property(GLOBAL APPEND PROPERTY YIO_JINJA_TASKS "${source_abs}" "${A_OUTPUT}")
	set_property(GLOBAL APPEND PROPERTY YIO_JINJA_OUTPUTS "${A_OUTPUT}")
	set_property(GLOBAL APPEND PROPERTY YIO_JINJA_DEPENDS "${source_abs}" ${A_DEPENDS})
endfunction()

function(yio_jinja_finalize)
	get_property(tasks GLOBAL PROPERTY YIO_JINJA_TASKS)
	get_property(outputs GLOBAL PROPERTY YIO_JINJA_OUTPUTS)
	get_property(depends GLOBAL PROPERTY YIO_JINJA_DEPENDS)

	if(NOT tasks)
		return()
	endif()

	set(depfile "${CMAKE_CURRENT_BINARY_DIR}/yio_jinja_batch.depfile")

	add_custom_command(
		OUTPUT ${outputs}
		COMMAND
			"${PYTHON_EXE}" "${JINJASCRIPT}"
			-S "${JINJAPROJECTSRC}"
			-S "${CMAKE_CURRENT_SOURCE_DIR}/src"
			-S "${CMAKE_CURRENT_SOURCE_DIR}/test/templated"
			-D "j_MAX_ARGS=${YIO_MAX_ARGS}"
			-D "j_MAX_CUSTOM_SLOTS=${YIO_MAX_CUSTOM_SLOTS}"
			-D "j_BITINT_MAXWIDTH=${YIO_BITINT_MAXWIDTH}"
			--cachedir "${CMAKE_CURRENT_BINARY_DIR}/jinjacache"
			--depfile "${depfile}"
			-j 8
			${tasks}
		DEPFILE "${depfile}"
		DEPENDS "${JINJASCRIPT}" ${depends}
		COMMENT "jinja2: batch processing all templates"
		VERBATIM
	)
	add_custom_target(yio_jinja_batch DEPENDS ${outputs})
	add_dependencies(yio_gen yio_jinja_batch)
endfunction()

###############################################################################

option(YIO_BUILD_BENCHMARKS "Build benchmarks" OFF)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile
	${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
	@ONLY
)

if(CMAKE_C_COMPILER_ID STREQUAL "SDCC")
	set(YIO_SDCC 1)
else()
	set(YIO_SDCC 0)
endif()

include(CMakeDependentOption)
include(CheckIncludeFile)
include(k/list_filter)
include(k/var_to_0_or_1)
include(src/yio_config_gen.cmake)

# CPack
include(src/cpack.cmake)
# Pre checks
# unistd is used in configuration and environmental checks
yio_config_gen_check_include_file("unistd.h" YYIO_HAS_UNISTD_H)
# User configuration
include(src/configuration.cmake)
# Environment setup (types, definitions)
include(src/environment_setup.cmake)

add_subdirectory(./third_party EXCLUDE_FROM_ALL)
# Environment checks (functions, headers)
include(src/environment_checks.cmake)
add_subdirectory(src)
if(((YIO_BUILD_TESTING AND BUILD_TESTING) OR YIO_DEV) AND NOT YIO_SDCC)
	include(CTest)
	add_subdirectory(./test)
  add_subdirectory(./examples)
endif()
if(YIO_BUILD_BENCHMARKS)
  add_subdirectory(benchmark)
endif()

yio_jinja_finalize()

