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

include(./kcmakelib/cmake/kcmakelib.cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(yio_debug_variables)

include(src/version.cmake)
project(yio
	VERSION "${YIO_PROJECT_VERSION}"
  LANGUAGES C CXX
	DESCRIPTION "YIO Iconic Output C Library that aims in type-safe printf replacement"
	HOMEPAGE_URL "https://gitlab.com/kamcuk/yio"
)

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

# YIO_DEV is internal variable set if the developer of this library is building it
if(NOT DEFINED YIO_DEV)
	set(YIO_DEV 0)
endif()
if(YIO_DEV)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

	if(NOT CMAKE_BUILD_TYPE MATCHES "^Debug$|^Release$|^RelWithDebInfo$")
		message(FATAL_ERROR "You mixed up CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
	endif()

	# Use ccache if available
	find_program(CCACHE_COMMAND "ccache")
	if(CCACHE_COMMAND)
		set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_COMMAND}")
	endif()

	# Create symlink from gen to GENDIR for development purpouses
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E create_symlink ${GENDIR} ${CMAKE_CURRENT_SOURCE_DIR}/gen
	)

	# Create symlink to compile_commands.json for IDE to pick it up
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E create_symlink
			${CMAKE_BINARY_DIR}/compile_commands.json
			${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
	)

	# Add compiler options.
	if(CMAKE_C_COMPILER_ID MATCHES "^(GNU|Clang|Intel)$")
		add_compile_options(
			-g
			-Wall
			-Wextra
			-Wwrite-strings
			-Wno-unused-function
			-Wno-unused-parameter
		)
		add_link_options(
			-g
		)
	endif()
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		if(NOT YIO_CLANG_TIDY AND NOT YIO_BUILD_BENCHMARKS)
			check_c_compiler_flag(-fanalyzer HAS_ANALYZER)
			if(HAS_ANALYZER)
				add_compile_options(-fanalyzer)
			endif()
		endif()
		check_c_compiler_flag(-Wattribute-warning YYIO_ATTRIBUTE_WARNING)
		if(YYIO_ATTRIBUTE_WARNING)
			add_compile_options(-Wattribute-warning)
		endif()
		add_compile_definitions(_FORTIFY_SOURCE=2)
		add_compile_options(
			$<$<COMPILE_LANGUAGE:C>:-Wmissing-prototypes>
			-O  # _FORTIFY_SOURCE needs it
		)
	elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
		check_c_compiler_flag(--analyze HAS_ANALYZER)
		if(HAS_ANALYZER)
			#add_compile_options(--analyze)
		endif()
		check_c_compiler_flag(-ffixed-point HAS_FIXED_POINT)
		if(HAS_FIXED_POINT)
			add_compile_options(-ffixed-point)
		endif()
		add_compile_options(
			-Wno-varargs
			-Wno-array-bounds
			-Wno-sizeof-array-decay
		)
	endif()

	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		add_compile_options(-fdiagnostics-color=always)
	elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
		add_compile_options(-fcolor-diagnostics)
	endif()

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
		CCACHE_COMMAND
		UNISTRING_LIB
		DIRECTORY:COMPILE_OPTIONS
		DIRECTORY:COMPILE_DEFINITIONS
		DIRECTORY:LINK_OPTIONS
	)
	if(0)
		yio_debug_variables(
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
			CMAKE_FIND_APPBUNDLE
			CMAKE_FIND_FRAMEWORK
			CMAKE_FIND_LIBRARY_CUSTOM_LIB_SUFFIX
			CMAKE_FIND_ROOT_PATH
			CMAKE_FIND_ROOT_PATH_BOTH
			CMAKE_FIND_ROOT_PATH_MODE_LIBRARY
			CMAKE_FIND_USE_CMAKE_ENVIRONMENT_PATH
			CMAKE_FIND_USE_CMAKE_PATH
			CMAKE_FIND_USE_CMAKE_SYSTEM_PATH
			CMAKE_FIND_USE_PACKAGE_ROOT_PATH
			CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH
			CMAKE_FRAMEWORK_PATH
			CMAKE_LIBRARY_ARCHITECTURE
			CMAKE_LIBRARY_PATH
			CMAKE_PREFIX_PATH
			CMAKE_STAGING_PREFIX
			CMAKE_SYSROOT
			CMAKE_SYSTEM_FRAMEWORK_PATH
			CMAKE_SYSTEM_LIBRARY_PATH
			CMAKE_SYSTEM_PREFIX_PATH
		)
	endif()
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

find_package(Python3 COMPONENTS Interpreter REQUIRED)
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
			"${Python3_EXECUTABLE}" "${JINJASCRIPT}"
			-S "${JINJAPROJECTSRC}"
			-S "${CMAKE_CURRENT_SOURCE_DIR}/src"
			-S "${CMAKE_CURRENT_SOURCE_DIR}/test/templated"
			-D "j_MLVLS=${YIO_MLVLS}"
			-D "j_SLOTS=${YIO_SLOTS}"
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

add_subdirectory(src)
if(((YIO_BUILD_TESTING AND BUILD_TESTING) OR YIO_DEV) AND NOT YIO_SDCC)
    include(CTest)
    add_subdirectory(./third_party EXCLUDE_FROM_ALL)
    add_subdirectory(./third_party/sstest EXCLUDE_FROM_ALL)
    add_subdirectory(./test)
    add_subdirectory(./examples)
endif()
if(YIO_BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

yio_jinja_finalize()

