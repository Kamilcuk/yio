
# All output is buffered to this variable
set(yio_config_gen_content)

string(APPEND yio_config_gen_content [=[
/**
 * @file yio_config_gen.h
 * This file was automatically generated from environment_checks.cmake
 */
/**
 * @addtogroup config
 * @{
 */
]=])

# @brief Finalizes the generated configuration file.
#
# Closes the Doxygen group and writes the accumulated configuration content to the
# target file if the content has changed, which prevents unnecessary rebuilds.
macro(yio_config_gen_fini)
	string(APPEND yio_config_gen_content [=[
/**
 * @}
 */
]=])
	set(yio_config_gen_fini_f "${GENDIR}/yio/yio_config_gen.h")
	if(EXISTS "${yio_config_gen_fini_f}")
		file(READ "${yio_config_gen_fini_f}" yio_config_gen_fini_tmp)
	else()
		set(yio_config_gen_fini_tmp)
	endif()
	if(NOT "${yio_config_gen_fini_tmp}" STREQUAL "${yio_config_gen_content}")
		file(WRITE "${yio_config_gen_fini_f}" "${yio_config_gen_content}")
	endif()
	unset(yio_config_gen_fini_tmp)
endmacro()

# @brief Adds a configuration symbol with a specific value to the generated config file.
#
# This function is responsible for formatting the `#define` directive,
# applying padding, and adding documentation comments if they exist.
#
# @param symbol The name of the configuration symbol.
# @param value The value to assign to the symbol.
macro(yio_config_gen_add_value symbol value)
	set(yio_config_gen_padding_size 35)

	if(DEFINED ${symbol}_COMMENT)
		string(APPEND yio_config_gen_content "/**\n")
		string(APPEND yio_config_gen_content "@def ${symbol}\n")
		string(APPEND yio_config_gen_content "${${symbol}_COMMENT}\n")
		string(APPEND yio_config_gen_content "*/\n")
	endif()
	if(NOT DEFINED ${symbol})
		if(DEFINED ${symbol}_COMMENT)
			set(${symbol} ${value} CACHE INTERNAL "${${symbol}_COMMENT}")
		else()
			set(${symbol} ${value} CACHE INTERNAL "")
		endif()
	endif()

	set(yio_config_gen_symbol "${symbol}")
	string(LENGTH "${symbol}" yio_config_gen_tmp)
	if("${yio_config_gen_tmp}" GREATER "${yio_config_gen_padding_size}")
		message(FATAL_ERROR "${symbol} is longer then padding size, adjust it")
	endif()
	foreach(yio_config_gen_i RANGE "${yio_config_gen_tmp}" "${yio_config_gen_padding_size}" 1)
		string(APPEND yio_config_gen_symbol " ")
	endforeach()

	string(APPEND yio_config_gen_content "#define ${yio_config_gen_symbol} ${value}\n")
	string(APPEND yio_config_gen_content "\n")
endmacro()

# @brief Adds a configuration symbol to the generated config file.
#
# If called with one argument, it evaluates the boolean value of the symbol and outputs 1 or 0.
# If called with two arguments, it sets the symbol to the provided value in the parent scope
# and outputs the symbol with that explicit value.
#
# @param symbol The name of the configuration symbol.
# @param ... [ARGV1] Optional explicit value to set and output.
macro(yio_config_gen_add symbol)
	if("${ARGC}" EQUAL 1)
		if(DEFINED "${symbol}")
			if("${${symbol}}")
				yio_config_gen_add_value("${symbol}" "1")
			else()
				yio_config_gen_add_value("${symbol}" "0")
			endif()
		else()
			yio_config_gen_add_value("${symbol}" "0")
		endif()
	elseif("${ARGC}" EQUAL 2)
		set("${symbol}" "${ARGV1}")
		yio_config_gen_add_value("${symbol}" "${ARGV1}")
	else()
		message(FATAL_ERROR "yio_config_gen_add expects 1 or 2 arguments, got ${ARGC}")
	endif()
endmacro()

# @brief Checks if a C header file exists and adds the result to the configuration.
#
# @param header The header file to check.
# @param variable The variable to store the boolean result and output to config.
macro(yio_config_gen_check_include_file)
	check_include_file(${ARGV})
	yio_config_gen_add(${ARGV1})
endmacro()

# @brief Checks if a C type exists and adds the result to the configuration.
#
# @param type The type to check.
# @param variable The variable to store the boolean result and output to config.
macro(yio_config_gen_check_type_exists)
	check_type_exists_bool(${ARGV})
	yio_config_gen_add(${ARGV1})
endmacro()

# @brief Checks if a C symbol exists and adds the result to the configuration.
#
# @param symbol The symbol to check.
# @param files The header files to include for the check.
# @param variable The variable to store the boolean result and output to config.
macro(yio_config_gen_check_symbol_exists)
	check_symbol_exists_bool(${ARGV})
	yio_config_gen_add(${ARGV2})
endmacro()

# @brief Checks if C source code compiles and adds the result to the configuration.
#
# @param src The C source code to compile.
# @param var The variable to store the boolean result and output to config.
macro(yio_config_gen_check_c_source_compiles src var)
	check_c_source_compiles("${src}" "${var}" ${ARGN})
	yio_config_gen_add(${var})
endmacro()
