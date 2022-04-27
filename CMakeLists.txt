# CMakeLists.txt
cmake_minimum_required(VERSION 3.11)
cmake_policy(SET CMP0054 NEW) #-- fixes IF() with quoted args
cmake_policy(SET CMP0057 NEW) #-- enables IF(.. IN_LIST ..)
cmake_policy(SET CMP0075 NEW) #-- uses CMAKE_REQUIRED_LIBRARIES

include(./kcmakelib/cmake/kcmakelib.cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(yio_debug_variables)

include(src/version.cmake)
project(yio
	VERSION "${YIO_PROJECT_VERSION}"
	LANGUAGES C
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

	set_from_env(YIO_CPPLINT)
	if(YIO_CPPLINT)
		set(_filter
			whitespace           # Yes, I use tabs.
			legal/copyright
			runtime/int
			build/include_subdir
			readability/casting  # This is C not C++.
			build/include_order  # Too much preprocssor stuff.
			readability/todo     # I'm programming alone.
			build/include        # This is hevily templated system, sometimes files don't include.
			readability/nolint   # I use clang-tidy too, don't complain.
			build/c++11          # This is C not C++.
		)
		string(REGEX REPLACE ";" ",-" _filter "${_filter}")
		set(CMAKE_C_CPPLINT "cpplint" "--filter=-${_filter}")
		unset(_filter)
	endif()

	set_from_env(YIO_CLANG_TIDY)
	if(YIO_CLANG_TIDY)
		set(_filter
			clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling
			clang-analyzer-valist.Uninitialized  # clang-tidy bug, can't return pointer to va_list from a function.
		)
		string(REGEX REPLACE ";" ",-" _filter "${_filter}")
		set(CMAKE_C_CLANG_TIDY "clang-tidy" "-checks=-${_filter}")
		unset(_filter)
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
	if(
			CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
			CMAKE_C_COMPILER_ID STREQUAL "Clang" OR
			CMAKE_C_COMPILER_ID STREQUAL "Intel"
	)
		set(_tmp
			-g
			$<$<CONFIG:DEBUG>:-Og>
			$<$<NOT:$<CONFIG:DEBUG>>:-O>
		)
		add_link_options(${_tmp})
		add_compile_options(
			${_tmp}
			-Wall
			-Wextra
			-Wwrite-strings
			-Wno-unused-function
			-Wno-unused-parameter
		)
	endif()
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		if(NOT YIO_CLANG_TIDY)
			check_c_compiler_flag(-fanalyzer _yIO_ANALYZER)
			if(_yIO_ANALYZER)
				add_compile_options(-fanalyzer)
			endif()
		endif()
		check_c_compiler_flag(-Wattribute-warning _yIO_ATTRIBUTE_WARNING)
		if(_yIO_ATTRIBUTE_WARNING)
			add_compile_options(-Wattribute-warning)
		endif()
		add_compile_options(-Wmissing-prototypes)
		add_compile_definitions(_FORTIFY_SOURCE=2)
	elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
		check_c_compiler_flag(--analyze _yIO_ANALYZER)
		if(_yIO_ANALYZER)
			#add_compile_options(--analyze)
		endif()
		check_c_compiler_flag(-ffixed-point _yIO_FIXED_POINT)
		if(_yIO_FIXED_POINT)
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
		CPPLINT_COMMAND
		DIRECTORY
		COMPILE_OPTIONS
		COMPILE_DEFINITIONS
		LINK_OPTIONS
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
		_yIO_CDT_PARSER=1
		_yIO_HAS_FLOATf32=0
		_yIO_HAS_FLOATf64=0
		_yIO_HAS_FLOATf32x=0
		_yIO_HAS_FLOATf64x=0
		_yIO_HAS_FLOATf128=0
		_yIO_HAS_FLOATf128x=0
		_yIO_HAS_FLOATd32=0
		_yIO_HAS_FLOATd64=0
		_yIO_HAS_FLOATd128=0
		_yIO_HAS_FLOATd32x=0
		_yIO_HAS_FLOATd64x=0
		_yIO_HAS_FLOATd128x=0
	)
	target_include_directories(${tgt} PRIVATE
		${GENDIR}
	)
	target_link_libraries(${tgt} PRIVATE library_that_does_not_exists)
endfunction()

###############################################################################

find_package(Python3 COMPONENTS Interpreter REQUIRED)
if(POLICY CMP0116)
	cmake_policy(SET CMP0116 NEW)
endif()
set(JINJASCRIPT ${CMAKE_CURRENT_BINARY_DIR}/jinja2script.pyc)
set(JINJAPROJECTSRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_custom_command(
	OUTPUT "${JINJASCRIPT}"
	COMMENT "jinja2: compiling preprocess script"
	COMMAND
		${Python3_EXECUTABLE}
		-c
		"import py_compile, sys ; py_compile.compile(sys.argv[1], cfile=sys.argv[2], doraise=True)"
		"${SRCDIR}/preprocess.py"
		"${JINJASCRIPT}"
	DEPENDS "${SRCDIR}/preprocess.py"
	VERBATIM
)
add_custom_target(yio_gen_prepare
	DEPENDS "${JINJASCRIPT}"
	COMMENT "jinja2: compiling preproces script target"
)

set_from_env(YIO_LINE) # Use to disable adding #line in source files

#!
function(jinja_generate)
	cmake_parse_arguments(A "" "SOURCE;OUTPUT;TARGET" "DEPENDS" ${ARGN})
	if(NOT A_TARGET)
		message(FATAL_ERROR "jinja_generate: You have to give a target to append source into")
	endif()
	string(MD5 depfilename "${A_OUTPUT}")
	set(depfile "${CMAKE_CURRENT_BINARY_DIR}/jinjadepfiles/${depfilename}.depfile")
	file(RELATIVE_PATH reloutputf ${CMAKE_CURRENT_BINARY_DIR} ${A_OUTPUT})
	if(IS_ABSOLUTE "${A_SOURCE}")
		file(RELATIVE_PATH A_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}" "${A_SOURCE}")
		if(A_SOURCE MATCHES "\\.\\.")
			message(FATAL_ERROR "A_SOURCE=${A_SOURCE} is not in SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
		endif()
	endif()
	if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${A_SOURCE}")
		message(FATAL_ERROR "Source file does not exist: ${A_SOURCE}")
	endif()
	set(debug)
	if((DEFINED YIO_LINE AND YIO_LINE) OR
			(NOT DEFINED YIO_LINE AND YIO_DEV AND CMAKE_BUILD_TYPE_UPPER STREQUAL "DEBUG"))
		set(debug --debug)
	endif()
	add_custom_command(
		OUTPUT
			"${A_OUTPUT}"
		COMMENT "jinja2: ${A_SOURCE} -> ${reloutputf}"
		COMMAND
			"${Python3_EXECUTABLE}"
			"${JINJASCRIPT}"
			-S "${JINJAPROJECTSRC}"
			-S "${CMAKE_CURRENT_SOURCE_DIR}"
			-D "j_MLVLS=${YIO_MLVLS}"
			-D "j_SLOTS_END=${YIO_SLOTS_END}"
			--depfile "${depfile}"
			--cachedir "${CMAKE_CURRENT_BINARY_DIR}/jinjacache"
			${debug}
			${A_UNPARSED_ARGUMENTS}
			"${A_SOURCE}"
			"${A_OUTPUT}"
		DEPFILE "${depfile}"
		DEPENDS
			yio_gen_prepare
			"${JINJASCRIPT}"
			"${A_SOURCE}"
			${A_DEPENDS}
		VERBATIM
	)
	set_property(TARGET "${A_TARGET}" APPEND PROPERTY SOURCES "${A_OUTPUT}")
endfunction()

###############################################################################

add_subdirectory(src)
if((YIO_BUILD_TESTING AND BUILD_TESTING) OR YIO_DEV)
	include(CTest)
	add_subdirectory(test)
	add_subdirectory(examples)
endif()

