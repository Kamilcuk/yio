
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

# Use this function to output
macro(yio_config_gen_add_value symbol value)
	set(yio_config_gen_padding_size 25)
	cmake_parse_arguments(A "ADDIFNDEF" "" "" ${ARGN})

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

	if(A_ADDIFNDEF)
		string(APPEND yio_config_gen_content "#ifndef ${symbol}\n")
	endif()
	string(APPEND yio_config_gen_content "#define ${yio_config_gen_symbol} ${value}\n")
	if(A_ADDIFNDEF)
		string(APPEND yio_config_gen_content "#endif\n")
	endif()
	string(APPEND yio_config_gen_content "\n")
endmacro()

macro(yio_config_gen_add symbol)
	if(DEFINED "${symbol}")
		if("${${symbol}}")
			yio_config_gen_add_value("${symbol}" "1" ${ARGN})
		else()
			yio_config_gen_add_value("${symbol}" "0" ${ARGN})
		endif()
	else()
		yio_config_gen_add_value("${symbol}" "0" ${ARGN})
	endif()
endmacro()

macro(yio_config_gen_check_include_file)
	check_include_file(${ARGV})
	yio_config_gen_add(${ARGV1})
endmacro()

macro(yio_config_gen_check_type_exists)
	check_type_exists_bool(${ARGV})
	yio_config_gen_add(${ARGV1})
endmacro()

macro(yio_config_gen_check_symbol_exists)
	check_symbol_exists_bool(${ARGV})
	yio_config_gen_add(${ARGV2})
endmacro()

macro(yio_config_gen_check_c_source_compiles src var)
	check_c_source_compiles("${src}" "${var}" ${ARGN})
	yio_config_gen_add(${var})
endmacro()
