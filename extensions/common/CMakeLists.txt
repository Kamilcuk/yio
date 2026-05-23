cmake_minimum_required(VERSION 3.10)
project(fstring_common)

add_library(extensions_common INTERFACE)
target_include_directories(extensions_common INTERFACE src)

add_library(extensions_common_conftest INTERFACE)
target_include_directories(extensions_common_conftest INTERFACE test)

if(NOT TARGET yio)
	link_libraries(m)
  add_subdirectory(../../ yio)
  set_target_properties(yio PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
# Include the test extraction tool
include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/k/test_extract_properties_from_file.cmake)

###############################################################################

function(add_fstring_plugin_ready_target)
	if(NOT TARGET fstring_plugin_ready_target)
		# Proxy file to enforce dependency for Ninja
		add_custom_command(
			OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fstring_plugin_ready
			COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/fstring_plugin_ready
			DEPENDS fstring_plugin
		)
		add_custom_target(fstring_plugin_ready_target DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/fstring_plugin_ready)
	endif()
endfunction()

###############################################################################

# @def add_fstring_plugin_test(NAME name FILE file ...)
# Adds a compilation and optionally a runtime test for a plugin.
function(add_fstring_plugin_test)
	set(options "")
	set(oneValueArgs NAME FILE EXTRA_FLAGS PLUGIN_TARGET)
	set(multiValueArgs)
	cmake_parse_arguments(T "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	set(exec_name "${T_NAME}_exe")

	add_fstring_plugin_ready_target()
	if(T_FILE MATCHES "noncompilable/")
		add_executable(${exec_name} EXCLUDE_FROM_ALL ${T_FILE})
	else()
		add_executable(${exec_name} ${T_FILE})
	endif()
	set_target_properties(${exec_name} PROPERTIES C_STANDARD 11)
	add_dependencies(${exec_name} fstring_plugin fstring_plugin_ready_target)
	set_source_files_properties(${T_FILE} PROPERTIES OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/fstring_plugin_ready)
	target_compile_options(${exec_name} PRIVATE ${T_EXTRA_FLAGS})
	target_link_libraries(${exec_name} PRIVATE ${T_PLUGIN_TARGET} extensions_common_conftest)

	if(T_FILE MATCHES "yio")
		target_link_libraries(${exec_name} PRIVATE yio)
	endif()

	file(READ "${T_FILE}" content)

	if(content MATCHES "//// PASS_REGULAR_EXPRESSION")
		message(FATAL_ERROR "Test ${T_FILE} MUST NOT have //// PASS_REGULAR_EXPRESSION - it is either RUN or COMPILATION")
	endif()

	# Non-compilable tests: build at test time and expect failure
	if(T_FILE MATCHES "noncompilable/")
		if(NOT content MATCHES "//// COMPILATION PASS_REGULAR_EXPRESSION")
			message(FATAL_ERROR "Noncompilable test ${T_FILE} MUST have //// COMPILATION PASS_REGULAR_EXPRESSION")
		endif()
		if(content MATCHES "//// WILL_FAIL")
			message(FATAL_ERROR "Test ${T_FILE} MUST NOT have //// WILL_FAIL")
		endif()

		# Use shell wrapper to invert exit code
		set(comp_test "${T_NAME}_comp")
		add_test(NAME ${comp_test} COMMAND sh -c "! \"$@\"" -- ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target
																			 ${exec_name}
		)
		test_extract_properties_from_file(fstring_plugin ${comp_test} ${T_FILE} PREFIX "//// COMPILATION")
		return()
	endif()

	# Normal tests: build at build time
	if(content MATCHES "//// COMPILATION PASS_REGULAR_EXPRESSION")
		message(
			FATAL_ERROR
				"Compilable test ${T_FILE} MUST NOT use COMPILATION PASS_REGULAR_EXPRESSION. Use noncompilable/ directory for failure tests."
		)
	endif()

	if(content MATCHES "//// COMPILATION WILL_FAIL" OR content MATCHES "//// RUN WILL_FAIL")
		message(
			FATAL_ERROR "Compilable test ${T_FILE} MUST NOT use WILL_FAIL. Use noncompilable/ directory for failure tests."
		)
	endif()

	add_test(NAME ${T_NAME} COMMAND ${exec_name})
	test_extract_properties_from_file(${exec_name} ${T_NAME} ${T_FILE} PREFIX "//// RUN")
endfunction()

## @brief Asserts condition; fatal error if false.
function(assert msg)
	# ${ARGN} contains all arguments after the first one (the message)
	if(NOT (${ARGN}))
		# Join the condition list into a readable string for the error log
		string(REPLACE ";" " " cond_str "${ARGN}")
		message(FATAL_ERROR "Assertion Failed: ${msg}\nCondition evaluated: (${cond_str})")
	endif()
endfunction()

## @brief Cycles optimization levels (global).
set_property(GLOBAL PROPERTY GLOBAL_OPT_COUNTER 0)
function(get_next_opt_level RESULT_VAR)
	set(OPT_LEVELS "-O1" "-O2" "-O3" "-Os" "-Og")
	get_property(OPT_COUNTER GLOBAL PROPERTY GLOBAL_OPT_COUNTER)
	if("${OPT_COUNTER}" STREQUAL "")
		set(OPT_COUNTER 0)
	endif()
	list(GET OPT_LEVELS ${OPT_COUNTER} NEXT_LEVEL)
	list(LENGTH OPT_LEVELS LIST_LEN)
	math(EXPR NEW_COUNTER "(${OPT_COUNTER} + 1) % ${LIST_LEN}")
	set_property(GLOBAL PROPERTY GLOBAL_OPT_COUNTER ${NEW_COUNTER})
	set(${RESULT_VAR}
			${NEXT_LEVEL}
			PARENT_SCOPE
	)
endfunction()

# @def add_fstring_plugin_test_dir(dir)
# Discovers and adds tests from a directory.
function(add_fstring_plugin_test_dir D_TEST_DIR)
	if(NOT TARGET fstring_plugin)
		message(FATAL_ERROR "Required target 'fstring_plugin' is not defined.")
	endif()
	if(NOT TARGET fstring_plugin_unopt)
		message(FATAL_ERROR "Required target 'fstring_plugin_unopt' is not defined.")
	endif()
	if(NOT TARGET fstring_plugin_opt)
		message(FATAL_ERROR "Required target 'fstring_plugin_opt' is not defined.")
	endif()

	file(GLOB_RECURSE TEST_FILES "${D_TEST_DIR}/test_*.c" "${D_TEST_DIR}/**/test_*.c")
	foreach(test_path ${TEST_FILES})
		file(RELATIVE_PATH rel_path ${D_TEST_DIR} ${test_path})
		string(REPLACE "/" "_" test_name ${rel_path})
		string(REPLACE ".c" "" test_name ${test_name})

		get_filename_component(dir_name ${D_TEST_DIR} NAME)
		set(test_name "${dir_name}_${test_name}")
		# 1. Handle unoptimized tests (runs unless path is strictly /optimized/)
		if(NOT rel_path MATCHES "^optimized/")
			add_fstring_plugin_test(NAME ${test_name}_unopt FILE "${test_path}" PLUGIN_TARGET fstring_plugin_unopt)
		endif()
		# 2. Handle optimized tests (runs unless path is strictly /unoptimized/)
		if(NOT rel_path MATCHES "^unoptimized/")
			get_next_opt_level(CURRENT_OPT)
			add_fstring_plugin_test(
				NAME
				${test_name}_opt
				FILE
				"${test_path}"
				EXTRA_FLAGS
				"${CURRENT_OPT}"
				PLUGIN_TARGET
				fstring_plugin_opt
			)
		endif()
	endforeach()
endfunction()

###############################################################################

function(add_testprogram_from_env)
	set(env_var "TESTPROGRAM")
	set(target_name "testprogram")
	set(snippet_content "$ENV{${env_var}}")
	if(NOT DEFINED snippet_content OR snippet_content STREQUAL "")
		return()
	endif()
	set(snippet_c "${CMAKE_CURRENT_BINARY_DIR}/${target_name}.c")
	string(FIND "${snippet_content}" "yio" yio_found)
	set(source "")
	if(NOT ${yio_found} EQUAL -1)
		string(APPEND source "#include <yio.h>\n")
	endif()
	string(APPEND source "#include <stdio.h>\n")
	if(snippet_content MATCHES "main[ \t\r\n]*\\(")
		string(APPEND source "${snippet_content}\n")
	else()
		string(APPEND source "int main() {\n  ${snippet_content}\n  return 0;\n}\n")
	endif()
	file(WRITE "${snippet_c}" "${source}")
	add_executable(${target_name} EXCLUDE_FROM_ALL "${snippet_c}")
	if(NOT ${yio_found} EQUAL -1)
		target_link_libraries(${target_name} PRIVATE yio)
	endif()
	add_dependencies(${target_name} fstring_plugin)
endfunction()

function(add_fstring_testprogram_from_env)
	add_testprogram_from_env()
	if(TARGET testprogram)
		add_dependencies(testprogram fstring_plugin)
		target_link_libraries(testprogram PRIVATE fstring_plugin_opt)
	endif()
endfunction()

