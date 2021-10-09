cmake_minimum_required(VERSION 3.1)
project(yio_test LANGUAGES C)

find_package(yio HINTS $ENV{YIODIR}	REQUIRED)

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PUBLIC yio::yio)

include(CTest)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
set_tests_properties(${PROJECT_NAME} PROPERTIES
	PASS_REGULAR_EXPRESSION "Hello world")
