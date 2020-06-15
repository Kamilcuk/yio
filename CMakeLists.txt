cmake_minimum_required(VERSION 3.0)

execute_process(
	COMMAND git describe --tags --match "v*"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	OUTPUT_VARIABLE version
	RESULT_VARIABLE result
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(NOT result EQUAL 0)
	set(version 0.0.0)
else()
	string(REGEX REPLACE 
		"^v([^\\.]+)\\.([^\\.]+)\\.([^\\.-]+)[\\.-]?([0-9]*)?.*" "\\1.\\2.\\3"
		version ${version})
endif()

project(yio 
	VERSION "${version}"
	LANGUAGES C
	DESCRIPTION "Yio Input Output Library"
	HOMEPAGE_URL "https://gitlab.com/kamcuk/yio"
)

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/debug_variables.cmake)
debug_variables(PREFIX "yio: "
	BUILD_TESTING
	YIO_BUILD_TESTING
	CMAKE_C_FLAGS
	CMAKE_BUILD_TYPE
	CMAKE_VERBOSE_MAKEFILE
)

add_subdirectory(src)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR YIO_BUILD_TESTING) 
		AND BUILD_TESTING)
	include(CTest)
    add_subdirectory(test)
endif()
