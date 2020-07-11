#!/bin/cmake 
# dashboard.cmake
# The dashboard script.

macro(usage errorstr)
	message([=[
Usage: ctest -S${CMAKE_CURRENT_LIST_FILE} -DDASHBOARD_MODE=<mode>  [ctestoptions...]

Runs the testing and all other stuff that is needed to test this yio project.
The 'DASHBOARD_MODE' variable is used to specify the current mode.
 
Available DASHBOARD_MODE modes:
- gcc    - Runs the tests with gcc as the compiler.
- clang  - Runs tests with clang as the compiler.
- arm    - Compiles with arm-none-eabi-gcc 
           and runs tests under simulator with arm-none-eabi-gdb.

Additionally user set-able cmake variables are available:
- MODEL            - The ctest model to use: Experimental, Nightly and Continous.
- WITH_UPDATE      - Run git checkout on the source directory.
                     WARNING: This will potentially overwrite your changes.
- WITH_NORMAL_TEST - Run tests under normal environment. 
- WITH_MEMCHECK    - Run tests under memcheck.
- WITH_COVERAGE    - Recompile the project with coverage flag and run tests
                     and get coverage values.
- WITH_SANITIZE    - Recompile the project with sanitize flags and run tests.
- WITH_SUBMIT      - Submits the results to the cdash server.

The following environment variables are used:
  - CDASH_KARTA_DYZIO_PL_PASSWORD
                 - The password to my cdash server.
                 
Written by Kamil Cukrowski
SPDX-License-Identifier: GPL-3.0-or-later
]=])
	message(FATAL_ERROR "${errorstr}")
endmacro()

#######################################################################################
# Load our library with most of the functions
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(dashboard_lib)
include(fail_if_git_is_dirty)

#######################################################################################
# Choose the configuration we will running for
# The configuration is chosen with DASHBOARD_MODE

if(NOT DEFINED DASHBOARD_MODE OR DASHBOARD_MODE STREQUAL "")
	usage("You have to set DASHBOARD_MODE variable to use this dashboard file")
		
elseif(DASHBOARD_MODE STREQUAL "gcc")
	find_program(CMAKE_C_COMPILER gcc)
	set_ifunset(SYSTEM "x86-64-linux-gcc")
	
elseif(DASHBOARD_MODE STREQUAL "clang")
	find_program(CMAKE_C_COMPILER clang)
	set_ifunset(SYSTEM "x86-64-linux-clang")
	
elseif(DASHBOARD_MODE STREQUAL "arm")
	set_ifunset(SYSTEM "armv4t-none-gcc")
	find_file(CMAKE_TOOLCHAIN_FILE toolchain-arm-none-eabi.cmake PATHS ./cmake)
	find_file(CMAKE_CROSSCOMPILING_EMULATOR cmake_crosscompiling_emulator_arm_none_gdb.sh PATHS ./scripts)
	
else()
	usage("Unknown configuration DASHBOARD_MODE=${DASHBOARD_MODE}")
endif()

set_ifunset(MODEL "Experimental")
set_ifunset(WITH_UPDATE OFF CACHE BOOL "Run ctest_update")
set_ifunset(WITH_SUBMIT OFF CACHE BOOL "Run ctest_submit")
set_ifunset(WITH_NORMAL_TEST ON CACHE BOOL "Run normally ctest_test")
if(DASHBOARD_MODE MATCHES "gcc")
	set_ifunset(WITH_MEMCHECK ON CACHE BOOL "Run memcheck after normal ctest_test")
	set_ifunset(WITH_COVERAGE ON CACHE BOOL "Run coverage build and ctest_test")
	set_ifunset(WITH_SANITIZE ON CACHE BOOL "Run sanitize build and ctest_test")
elseif(DASHBOARD_MODE MATCHES "clang")
	set_ifunset(WITH_MEMCHECK OFF)
	set_ifunset(WITH_COVERAGE OFF)
	set_ifunset(WITH_SANITIZE OFF)
elseif(DASHBOARD_MODE MATCHES "arm")
	set_ifunset(WITH_MEMCHECK OFF)
	set_ifunset(WITH_COVERAGE OFF)
	set_ifunset(WITH_SANITIZE OFF)
endif()

#######################################################################################
# Set some initial variables

cmake_host_system_information(RESULT CTEST_SITE QUERY HOSTNAME)

set(CTEST_CONFIGURATION_TYPE Release)

set(CTEST_CMAKE_GENERATOR "Ninja")

cmake_host_system_information(RESULT NUMBER_OF_LOGICAL_CORES QUERY NUMBER_OF_LOGICAL_CORES)
set(CTEST_PARALLEL_LEVEL ${NUMBER_OF_LOGICAL_CORES})

# Setup configure flags
list(APPEND CONFIGURE_FLAGS "-DBUILD_TESTING=1")
list(APPEND CONFIGURE_FLAGS "-DYIO_BUILD_TESTING=1")
list(APPEND CONFIGURE_FLAGS "--log-level=DEBUG")
foreach(i IN ITEMS 
		CMAKE_TOOLCHAIN_FILE
		CMAKE_CROSSCOMPILING_EMULATOR
		CMAKE_C_COMPILER
)
	if(DEFINED ${i})
		list(APPEND CONFIGURE_FLAGS "-D${i}=${${i}}")
	endif()
endforeach()

find_program(CTEST_UPDATE_COMMAND NAMES git)
find_program(CTEST_COVERAGE_COMMAND NAMES gcov)
find_program(CTEST_MEMORYCHECK_COMMAND NAMES valgrind)
	
#######################################################################################
# Debug variables

debug_variables(PREFIX "dashboard: "
	DASHBOARD_MODE
	CTEST_SOURCE_DIRECTORY
	CTEST_BINARY_DIRECTORY
	CTEST_CONFIGURATION_TYPE
	CTEST_CMAKE_GENERATOR
	CTEST_SITE
	CTEST_CONFIGURE_COMMAND
	CTEST_COVERAGE_COMMAND
	CTEST_MEMORYCHECK_COMMAND
	CMAKE_TOOLCHAIN_FILE
	CMAKE_CROSSCOMPILING_EMULATOR
	CTEST_PARALLEL_LEVEL
	CMAKE_C_COMPILER
	MODEL
	SYSTEM
	CONFIGURE_FLAGS
	WITH_UPDATE
	WITH_NORMAL_TEST
	WITH_MEMCHECK
	WITH_COVERAGE
	WITH_SANITIZE
	WITH_SUBMIT
)

assert_is_valid_bool_only(PREFIX "dashboard: " 
	WITH_UPDATE
	WITH_NORMAL_TEST
	WITH_MEMCHECK
	WITH_COVERAGE
	WITH_SANITIZE
	WITH_SUBMIT
)

#######################################################################################
# Some more config

# If we are upading, make TOTALLY SURE, we will not loose any data. 
if(WITH_UPDATE)
	fail_if_git_is_dirty()
endif()

# If we are submitting and we have passwrod to my server, set it.
if(WITH_SUBMIT AND DEFINED ENV{CDASH_KARTA_DYZIO_PL_PASSWORD})
	log("submit: Using Authorization: Bearer ***")
	set(LIB_SUBMIT_FLAGS HTTPHEADER "Authorization: Bearer $ENV{CDASH_KARTA_DYZIO_PL_PASSWORD}")
endif()

#######################################################################################
# main
# lib* functions defined in dashboard_lib.cmake

set(CTEST_BUILD_NAME "${SYSTEM}")
if(WITH_SUBMIT)
	lib_ctest_empty_binary_directory(${CTEST_BINARY_DIRECTORY})
endif()
lib_ctest_start(${MODEL})
lib_ctest_update()
if(WITH_NORMAL_TEST)
	log("Running normal tests")
	lib_ctest_configure("${CONFIGURE_FLAGS}" "" APPEND)
	lib_ctest_build()
	lib_ctest_test()	
	lib_ctest_memcheck(EXCLUDE_LABEL nomemcheck)
	
	set(parts Start;Update;Configure;Build;Test)
	if(WITH_MEMCHECK)
		list(APPEND parts Memcheck)
	endif()
	lib_ctest_submit(PARTS ${parts})
endif()

# Coverage results are appended to 
if(WITH_COVERAGE)
	log("Running coverage")
	lib_ctest_configure("${CONFIGURE_FLAGS}" "-g --coverage -fprofile-abs-path")
	lib_ctest_build()
	lib_ctest_test()
	lib_ctest_coverage()
	lib_ctest_submit(PARTS Coverage)
endif()

if(WITH_SANITIZE)
	log("Running sanitize")
	set(CTEST_BUILD_NAME "${SYSTEM}-sanitize")
	if(WITH_SUBMIT)
		lib_ctest_empty_binary_directory(${CTEST_BINARY_DIRECTORY})
	endif()	
	lib_ctest_start(${MODEL} GROUP Sanitize)	
	set(sanitize_flags "-fsanitize=address -fsanitize=undefined -fsanitize=leak -fsanitize=pointer-subtract -fsanitize=pointer-compare -fno-omit-frame-pointer -fstack-protector-all -fstack-clash-protection -fcf-protection")
	lib_ctest_configure("${CONFIGURE_FLAGS}" "${sanitize_flags}")
	lib_ctest_build()
	lib_ctest_test()
	lib_ctest_submit()
endif()

log("Success")

