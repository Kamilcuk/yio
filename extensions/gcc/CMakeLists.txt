cmake_minimum_required(VERSION 3.10)
project(YioGccPlugin CXX C)

enable_testing()

# Assert C compilers are GCC/G++
if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
	message(FATAL_ERROR "This plugin requires GCC for testing. Found: ${CMAKE_C_COMPILER_ID}")
endif()
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	message(FATAL_ERROR "This plugin requires G++ for building. Found: ${CMAKE_CXX_COMPILER_ID}")
endif()

set(CMAKE_CXX_STANDARD 17)
set(COMMON_FLAGS -Werror)

execute_process(
	COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=plugin
	OUTPUT_VARIABLE GCC_PLUGIN_DIR
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "GCC Plugin directory: ${GCC_PLUGIN_DIR}")

add_subdirectory(../common common)

add_library(fstring_plugin SHARED src/fstring_plugin.cpp)
target_compile_options(fstring_plugin PRIVATE ${COMMON_FLAGS} -fPIC -fno-rtti -fno-exceptions -Wno-literal-suffix)
target_include_directories(fstring_plugin PRIVATE "${GCC_PLUGIN_DIR}/include")
target_link_libraries(fstring_plugin PRIVATE yio extensions_common)
set_target_properties(fstring_plugin PROPERTIES PREFIX "" OUTPUT_NAME yio_gcc_fstring_plugin)

install(TARGETS fstring_plugin LIBRARY DESTINATION lib)

# Plugin configuration targets
add_library(fstring_plugin_test_base INTERFACE)
target_compile_options(fstring_plugin_test_base INTERFACE ${COMMON_FLAGS})

add_library(fstring_plugin_unopt INTERFACE)
target_link_libraries(fstring_plugin_unopt INTERFACE fstring_plugin_test_base)
target_compile_options(
	fstring_plugin_unopt
	INTERFACE "SHELL:-fplugin=$<TARGET_FILE:fstring_plugin> -fplugin-arg-yio_gcc_fstring_plugin-ffstring-optimizations=0"
)

add_library(fstring_plugin_opt INTERFACE)
target_link_libraries(fstring_plugin_opt INTERFACE fstring_plugin_test_base)
target_compile_options(
	fstring_plugin_opt
	INTERFACE "SHELL:-fplugin=$<TARGET_FILE:fstring_plugin> -fplugin-arg-yio_gcc_fstring_plugin-ffstring-optimizations=1"
)

add_fstring_plugin_test_dir("${CMAKE_CURRENT_SOURCE_DIR}/test")
add_fstring_plugin_test_dir("${CMAKE_CURRENT_SOURCE_DIR}/../common/test")

add_test(
	NAME test_plugin_flag_disable
	COMMAND
		${CMAKE_C_COMPILER} ${COMMON_FLAGS} -fplugin=$<TARGET_FILE:fstring_plugin>
		-fplugin-arg-yio_gcc_fstring_plugin-Wfstring-format=0 -I ${CMAKE_CURRENT_SOURCE_DIR}/test -c
		${CMAKE_CURRENT_SOURCE_DIR}/test/plugin_flags.c -o /dev/null
)

add_test(
	NAME test_plugin_flag_error
	COMMAND
		sh -c "! \"$@\"" -- ${CMAKE_C_COMPILER} -fplugin=$<TARGET_FILE:fstring_plugin>
		-fplugin-arg-yio_gcc_fstring_plugin-Werror-fstring-format=1 -I ${CMAKE_CURRENT_SOURCE_DIR}/test -c
		${CMAKE_CURRENT_SOURCE_DIR}/test/plugin_flags.c -o /dev/null
)
test_extract_properties_from_file(
	fstring_plugin test_plugin_flag_error ${CMAKE_CURRENT_SOURCE_DIR}/test/plugin_flags.c PREFIX "//// COMPILATION"
)

add_test(NAME check_test_prefixes COMMAND grep -r "^//// PASS_REGULAR_EXPRESSION" ${CMAKE_CURRENT_SOURCE_DIR}/test)
set_tests_properties(check_test_prefixes PROPERTIES WILL_FAIL TRUE)

add_fstring_testprogram_from_env()
