cmake_minimum_required(VERSION 3.1)
project(yio_test LANGUAGES C)

add_subdirectory(../../../ EXCLUDE_FROM_ALL yio)
if(NOT TARGET yio)
	message(FATAL_ERROR "Target yio not found")
endif()

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PUBLIC yio)

include(CTest)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
set_tests_properties(${PROJECT_NAME} PROPERTIES
	PASS_REGULAR_EXPRESSION "Hello world")
