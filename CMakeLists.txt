cmake_minimum_required(VERSION 3.0)

string(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(debug_variables)

include(src/version.cmake)
project(yio 
	VERSION "${YIO_PROJECT_VERSION}"
	LANGUAGES C
	DESCRIPTION "Yio Input Output C Library that aims in type-safe printf replacement"
	HOMEPAGE_URL "https://gitlab.com/kamcuk/yio"
)

include(force_build_dir_without_cmakelists)

debug_variables(PREFIX "yio: "
	BUILD_TESTING
	YIO_BUILD_TESTING
	CMAKE_C_FLAGS
	CMAKE_BUILD_TYPE
	CMAKE_VERBOSE_MAKEFILE
	CMAKE_GENERATOR
)

add_subdirectory(src)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR YIO_BUILD_TESTING) 
		AND BUILD_TESTING)
	include(CTest)
    add_subdirectory(test)
endif()



