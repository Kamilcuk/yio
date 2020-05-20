# dashboard_chors.cmake

set(CTEST_CONFIGURATION_TYPE Release)

set(CTEST_CMAKE_GENERATOR "Ninja")

cmake_host_system_information(RESULT CTEST_SITE QUERY HOSTNAME)

string(PREPEND CTEST_CONFIGURE_COMMAND "${CMAKE_COMMAND} ")
if(DEFINED CMAKE_TOOLCHAIN_FILE)
	string(APPEND CTEST_CONFIGURE_COMMAND "	\"-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}\"")
endif()
if(DEFINED CMAKE_CROSSCOMPILING_EMULATOR)
	string(APPEND CTEST_CONFIGURE_COMMAND "	\"-DCMAKE_CROSSCOMPILING_EMULATOR=${CMAKE_CROSSCOMPILING_EMULATOR}\"")
endif()
string(APPEND CTEST_CONFIGURE_COMMAND " -DCMAKE_BUILD_TYPE:STRING=${CTEST_CONFIGURATION_TYPE}")
string(APPEND CTEST_CONFIGURE_COMMAND " -DBUILD_TESTING:BOOL=ON")
string(APPEND CTEST_CONFIGURE_COMMAND " \"-G${CTEST_CMAKE_GENERATOR}\"")
string(APPEND CTEST_CONFIGURE_COMMAND " \"${CTEST_SOURCE_DIRECTORY}\"")

find_program(CTEST_COVERAGE_COMMAND NAMES gcov)
find_program(CTEST_MEMORYCHECK_COMMAND NAMES valgrind)

macro(set_default)
	if(NOT DEFINED "${ARGV0}")
		set("${ARGV0}" "${ARGV1}")
	endif()
endmacro()

# main ####################################################

set_default(MODEL Experimental)
set_default(WITH_UPDATE OFF)
set_default(WITH_MEMCHECK ON)
set_default(WITH_COVERAGE ON)
set_default(WITH_SANITIZE ON)
set_default(WITH_SUBMIT OFF)
set_default(SYSTEM "")

foreach(i IN ITEMS
	CTEST_SOURCE_DIRECTORY
	CTEST_BINARY_DIRECTORY
	MODEL
	WITH_UPDATE
	WITH_MEMCHECK
	WITH_COVERAGE
	WITH_SANITIZE
	WITH_SUBMIT
	SYSTEM
	CTEST_CONFIGURATION_TYPE
	CTEST_CMAKE_GENERATOR
	CTEST_SITE
	CTEST_CONFIGURE_COMMAND
	CTEST_COVERAGE_COMMAND
	CTEST_MEMORYCHECK_COMMAND
	CMAKE_TOOLCHAIN_FILE
	CMAKE_CROSSCOMPILING_EMULATOR
)
	if(NOT DEFINED ${i})
		message(STATUS "dashboard_chors.cmake: undef ${i}")
	else()
		message(STATUS "dashboard_chors.cmake: ${i}=${${i}}")
	endif()
endforeach()

function(do_chain name args)
	set(CTEST_BINARY_DIRECTORY ${CTEST_BINARY_DIRECTORY}/${name})
	set(CTEST_BUILD_NAME ${SYSTEM}${name})
	ctest_empty_binary_directory(${CTEST_BINARY_DIRECTORY})
	ctest_start(${MODEL})
	if(WITH_UPDATE)
		ctest_update()
	endif()
	ctest_configure(OPTIONS "${args}")
	ctest_build()
	ctest_test()
	if(${name} STREQUAL "coverage")
		ctest_coverage()
	endif()
	if(${name} STREQUAL "normal")
		ctest_memcheck(EXCLUDE_LABEL nomemcheck)
	endif()
	if(WITH_SUBMIT)
		ctest_submit(HTTPHEADER "Authorization: Bearer 51d736f4bc2243e93e5ce836e4c9020c")
	endif()
endfunction()

do_chain(normal "")
if(WITH_COVERAGE)
	do_chain(coverage "\"-DCMAKE_C_FLAGS=-g --coverage\"")
endif()
if(WITH_SANITIZE)
	do_chain(sanitize "\"-DCMAKE_C_FLAGS=-fsanitize=address -fsanitize=undefined -fsanitize=leak -fsanitize=pointer-subtract -fsanitize=pointer-compare -fno-omit-frame-pointer -fstack-protector-all -fstack-clash-protection -fcf-protection\"")
endif()


