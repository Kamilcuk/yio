cmake_minimum_required(VERSION 3.1)

project(redirectstdin LANGUAGES C)

if(NOT "${CMAKE_PROJECT_NAME}" STREQUAL "${PROJECT_NAME}")
	include(ExternalProject)
	
	# we are called form test/CMakeLists.txt
	ExternalProject_Add(
		redirectstdin
		DOWNLOAD_COMMAND ""
		UPDATE_COMMAND ""
		INSTALL_COMMAND ""
		PREFIX ${CMAKE_CURRENT_BINARY_DIR}
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
		EXCLUDE_FROM_ALL ON
	)

    add_test(
        NAME redirectstdin_compile
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target redirectstdin
    )
    set_property(TEST redirectstdin_compile APPEND PROPERTY LABELS redirectstdin)
	set_property(TEST redirectstdin_compile APPEND PROPERTY LABELS nomemcheck)
	set_property(TEST redirectstdin_compile APPEND PROPERTY LABELS compile)
	set_property(TEST redirectstdin_compile APPEND PROPERTY FIXTURES_SETUP redirectstdin)

    add_test(
        NAME redirectstdin_works
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/src/redirectstdin-build/redirectstdin
    )
	set_property(TEST redirectstdin_works APPEND PROPERTY LABELS nomemcheck)
	set_property(TEST redirectstdin_works APPEND PROPERTY LABELS redirectstdin)
	set_property(TEST redirectstdin_works APPEND PROPERTY FIXTURES_SETUP redirectstdin)
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

