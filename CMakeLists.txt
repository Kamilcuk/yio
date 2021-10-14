# CMakeLists.txt
cmake_minimum_required(VERSION 3.11)
cmake_policy(SET CMP0054 NEW) #-- fixes IF() with quoted args
cmake_policy(SET CMP0057 NEW) #-- enables IF(.. IN_LIST ..)

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
include(set_from_env)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Upper build type to be used in if statements
string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)
# Source directory
get_filename_component(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/src ABSOLUTE)
# Generated directory
get_filename_component(GENDIR ${CMAKE_CURRENT_BINARY_DIR}/gen ABSOLUTE)

# YIO_DEV is internal variable set if the developer of this library is building it
if(YIO_DEV)
	set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

	# Use ccache if available
	find_program(CCACHE_COMMAND "ccache")
	if(CCACHE_COMMAND)
	  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_COMMAND}")
	endif()

	set_from_env(YIO_CPPLINT)
	if(YIO_CPPLINT)
    set(filter
      whitespace           # Yes, I use tabs.
      legal/copyright
      runtime/int
      build/include_subdir
      readability/casting  # This is C not C++.
      build/include_order  # Too much preprocssor stuff.
      readability/todo     # I'm programming alone.
      build/include        # This is hevily templated system, sometimes files don't include.
      readability/nolint   # I use clang-tidy too, don't complain.
    )
    string(REGEX REPLACE ";" ",-" filter "${filter}")
		set(CMAKE_C_CPPLINT "cpplint" "--filter=-${filter}")
    unset(filter)
	endif()

  set_from_env(YIO_CLANG_TIDY)
  if(YIO_CLANG_TIDY)
    set(filter
      clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling
      clang-analyzer-valist.Uninitialized  # clang-tidy bug, can't return pointer to va_list from a function.
    )
    string(REGEX REPLACE ";" ",-" filter "${filter}")
    set(CMAKE_C_CLANG_TIDY "clang-tidy" "-checks=-${filter}")
    unset(filter)
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
			$<$<CONFIG:DEBUG>:-Og>
			$<$<NOT:$<CONFIG:DEBUG>>:-O>
		)
		add_link_options(${tmp})
		add_compile_options(
			${tmp}
			-Wall
			-Wextra
			-Wwrite-strings
			-Wno-unused-function
			-Wno-unused-parameter
		)
	endif()
  include(CheckCCompilerFlag)
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		check_c_compiler_flag(-Wattribute-warning _yIO_ATTRIBUTE_WARNING)
		if(_yIO_ATTRIBUTE_WARNING)
			add_compile_options(
				-Wattribute-warning
			)
		endif()
		add_compile_options(
			-Wmissing-prototypes
		)
		add_compile_definitions(
			_FORTIFY_SOURCE=2
		)
	elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    check_c_compiler_flag(-ffixed-point _yIO_FIXED_POINT)
    if(_yIO_FIXED_POINT)
      add_compile_options(
        -ffixed-point
      )
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

  function(yio_cdt_parser tgt)
    add_library(${tgt} EXCLUDE_FROM_ALL ${ARGN})
    target_compile_definitions(${tgt} PRIVATE
      __CDT_PARSER__=1
      YIO_CDT_PARSER=1
      YIO_REALLY_CDT_PARSER=1
    )
    target_include_directories(${tgt} PRIVATE
      ${SRCDIR}
      ${GENDIR}
    )
    target_link_libraries(${tgt} PRIVATE library_that_does_not_exists)
  endfunction()
endif(YIO_DEV)

######################################################################

find_library(UNISTRING_LIB
	NAMES unistring
	DOC "Path to unistring library"
)

######################################################################

include(m4)
m4_add_options(
	--fatal-warnings
	--fatal-warnings
	# pass context
	-D m4_SRCDIR=${SRCDIR}
	-D m4_GENDIR=${GENDIR}
)
if(CMAKE_BUILD_TYPE_UPPER STREQUAL "DEBUG")
	m4_add_options(-D m4_DEBUG=1)
else()
	m4_add_options(-D m4_NDEBUG=1 -D m4_RELEASE=1)
endif()
m4_add_files(
	# Always load library.
	${CMAKE_CURRENT_SOURCE_DIR}/m4/lib.m4
)
# Note: SRCDIR is included, so I can do `m4_include(yio/blabla)`.
m4_add_include_directories(${SRCDIR})

m4_add_files(
	# Load our configuration.
	# Note - file is generated from src/CMakeLists.txt
	${CMAKE_CURRENT_BINARY_DIR}/src/m4gen/yio_config.m4
)

######################################################################

get_directory_property(dir_prop_COMPILE_OPTIONS COMPILE_OPTIONS)
get_directory_property(dir_prop_COMPILE_DEFINITIONS COMPILE_DEFINITIONS)
get_directory_property(dir_prop_LINK_OPTIONS LINK_OPTIONS)
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
	CCACHE_FOUND
	UNISTRING_LIB
	CPPLINT_COMMAND
	_M4_OPTIONS_INIT
	_M4_FILES_INIT
	dir_prop_COMPILE_OPTIONS
	dir_prop_COMPILE_DEFINITIONS
	dir_prop_LINK_OPTIONS
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

foreach(ii IN ITEMS CMAKE_TOOLCHAIN_FILE CMAKE_CROSSCOMPILING_EMULATOR)
	if(DEFINED "${ii}")
		if (NOT EXISTS "${${ii}}")
			message(FATAL_ERROR "${ii} is defined but file does not exists")
		endif()
	endif()
endforeach()

add_subdirectory(src)

if(YIO_DEV OR (YIO_BUILD_TESTING AND BUILD_TESTING))
	include(CTest)
  add_subdirectory(test)
endif()



