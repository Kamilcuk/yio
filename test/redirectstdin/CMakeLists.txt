cmake_minimum_required(VERSION 3.1)

project(redirectstdin LANGUAGES C)

if(NOT "${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")
	include(ExternalProject)

	# we are called form test/CMakeLists.txt
	ExternalProject_Add(
		redirectstdin_proj
		DOWNLOAD_COMMAND ""
		UPDATE_COMMAND ""
		INSTALL_COMMAND ""
		PREFIX ${CMAKE_CURRENT_BINARY_DIR}
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
		BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/redirectstdin
	)
	add_executable(redirectstdin IMPORTED GLOBAL)
	set_target_properties(redirectstdin PROPERTIES
		IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/redirectstdin/redirectstdin)
	add_dependencies(redirectstdin redirectstdin_proj)
	add_test(NAME redirectstdin_works COMMAND $<TARGET_FILE:redirectstdin>)
	set_property(TEST redirectstdin_works APPEND PROPERTY LABELS nomemcheck)
	set_property(TEST redirectstdin_works APPEND PROPERTY LABELS redirectstdinexe)
	set_property(TEST redirectstdin_works APPEND PROPERTY DEPENDS redirectstdin_compile)

	return()
endif()

add_executable(${PROJECT_NAME} main.c)
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
	target_compile_options(${PROJECT_NAME} PRIVATE
		-Wall
		-Wextra
		-Wwrite-strings
		-pedantic
	)
endif()

