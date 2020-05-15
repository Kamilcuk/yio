# dashboard_chors.cmake

set(CTEST_CONFIGURATION_TYPE Release)

set(CTEST_CMAKE_GENERATOR "Ninja")

cmake_host_system_information(RESULT CTEST_SITE QUERY HOSTNAME)

set(CTEST_CONFIGURE_COMMAND "${CMAKE_COMMAND}")

string(APPEND CTEST_CONFIGURE_COMMAND " -DCMAKE_BUILD_TYPE:STRING=${CTEST_CONFIGURATION_TYPE}")
string(APPEND CTEST_CONFIGURE_COMMAND " -DBUILD_TESTING:BOOL=ON")
string(APPEND CTEST_CONFIGURE_COMMAND " \"-G${CTEST_CMAKE_GENERATOR}\"")
string(APPEND CTEST_CONFIGURE_COMMAND " \"${CTEST_SOURCE_DIRECTORY}\"")

find_program(CTEST_COVERAGE_COMMAND NAMES gcov)
find_program(CTEST_MEMORYCHECK_COMMAND NAMES valgrind)

function(set_temp_configure_options)
	if(NOT "${set_temp_configure_options}" STREQUAL "")
		string(REGEX REPLACE
			"${last_temp_configure_options}"
			"" 
			CTEST_CONFIGURE_COMMAND ${CTEST_CONFIGURE_COMMAND}
		)
	endif()
	foreach(i IN LISTS ARGN)
		string(APPEND last_temp_configure_options " ${i}")
	endforeach()
	string(APPEND CTEST_CONFIGURE_COMMAND "${last_temp_configure_options}")
	
	set(last_temp_configure_options "${last_temp_configure_options}" PARENT_SCOPE)
	set(CTEST_CONFIGURE_COMMAND "${CTEST_CONFIGURE_COMMAND}" PARENT_SCOPE)
endfunction()

function(handle_ret)
	if(ret)
		message(FATAL_ERROR "ERROR: Exit code: ${ret}")
	endif()
endfunction()

function(maybe_ctest_submit)
	if(DO_SUBMIT)
		ctest_submit(HTTPHEADER "Authorization: Bearer 51d736f4bc2243e93e5ce836e4c9020c")
	endif()
endfunction()

# main ##########################################################

function(do_chain name args)
	set(CTEST_BINARY_DIRECTORY ${CTEST_BINARY_DIRECTORY}/${name})
	set(CTEST_BUILD_NAME ${name})
	ctest_empty_binary_directory(${CTEST_BINARY_DIRECTORY})
	ctest_start(${MODEL})
	set_temp_configure_options(${args})
	ctest_configure(OPTIONS ${args})
	ctest_build()
	ctest_test()
	if(${name} STREQUAL "coverage")
		ctest_coverage()
	endif()
	if(${name} STREQUAL "normal")
		ctest_memcheck(EXCLUDE_LABEL nomemcheck)
	endif()
	maybe_ctest_submit()
endfunction()

do_chain(normal "")
do_chain(coverage "\"-DCMAKE_C_FLAGS=-g --coverage\"")
do_chain(sanitize "\"-DCMAKE_C_FLAGS=-fsanitize=address -fsanitize=undefined -fsanitize=leak -fsanitize=pointer-subtract -fsanitize=pointer-compare -fno-omit-frame-pointer -fstack-protector-all -fstack-clash-protection -fcf-protection\"")
