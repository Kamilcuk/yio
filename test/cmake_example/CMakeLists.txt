cmake_minimum_required(VERSION 3.1)
project(yio_test LANGUAGES C)

find_package(yio HINTS $ENV{YIODIR}	REQUIRED)

foreach(i IN ITEMS yio_decimallib yio yuio)
	if(NOT TARGET yio::${i})
		message(FATAL_ERROR "yio::${i} target not found")
	endif()
	get_property(tmp TARGET yio::${i} PROPERTY IMPORTED_LOCATION)
	message(STATUS "yio::${i} -> ${tmp}")
endforeach()

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PUBLIC yio::yio)

include(CTest)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
set_tests_properties(${PROJECT_NAME} PROPERTIES
	PASS_REGULAR_EXPRESSION "Hello world")
