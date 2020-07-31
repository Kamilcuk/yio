include_guard()

##########################################################################

find_program(_M4_SH_EXECUTABLE sh DOC "Patch to shell")
if(NOT _M4_SH_EXECUTABLE)
	message(FATAL_ERROR "shell not found")
endif()

find_program(_M4_EXECUTABLE m4 DOC "Patch to m4 executable")
if(NOT _M4_EXECUTABLE)
	message(FATAL_ERROR "m4 executable not found")
endif()

# The executable script to run
set(_M4_SH_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/m4.sh)

##########################################################################
# m4_add_* options

# Options for m4 preprocessor
# Also see end of this file
set(_M4_OPTIONS_INIT "")

# Dependencies generated from include directories for m4 preprocessor
# Also see end of this file
set(_M4_FILES_INIT "")

macro(m4_add_options)
	if(ARGC GREATER 1 AND TARGET ARGV1)
		message(FATAL_ERROR "m4_add_options: passed target name ${ARGV1}")
	endif()
	list(APPEND _M4_OPTIONS_INIT ${ARGN})
endmacro()

function(m4_add_include_directories)
	if(ARGC GREATER 1 AND TARGET ARGV1)
		message(FATAL_ERROR "m4_add_options: passed target name ${ARGV1}")
	endif()

	foreach(i IN LISTS ARGN)

		string(REGEX REPLACE "^-I *" "" i ${i})
		string(REGEX REPLACE "^--include=" "" i ${i})
		get_filename_component(i "${i}" ABSOLUTE)

		list(APPEND _M4_OPTIONS_INIT -I "${i}")
	endforeach()
	set(_M4_OPTIONS_INIT ${_M4_OPTIONS_INIT} PARENT_SCOPE)
endfunction()

function(m4_add_files)
	if(ARGC GREATER 1 AND TARGET ARGV1)
		message(FATAL_ERROR "m4_add_files: passed target name ${ARGV1}")
	endif()
	foreach(i IN LISTS ARGN)
		get_filename_component(i "${i}" ABSOLUTE)
		list(APPEND _M4_FILES_INIT ${i})
	endforeach()
	set(_M4_FILES_INIT ${_M4_FILES_INIT} PARENT_SCOPE)
endfunction()

##########################################################################

macro(_m4_get_command_args)
	cmake_parse_arguments(M4_GET_COMMAND_ARGS
		""
		"OUTPUT"
		"SOURCE;OPTIONS"
		${ARGN}
	)

	foreach(i IN ITEMS OUTPUT SOURCE)
		if(NOT DEFINED M4_GET_COMMAND_ARGS_${i} OR M4_GET_COMMAND_ARGS_${i} STREQUAL "")
			message(FATAL_ERROR "m4_add_command: M4_GET_COMMAND_ARGS_${i} parameter is mandatory")
		endif()
	endforeach()

	set(output "${M4_GET_COMMAND_ARGS_OUTPUT}")
	list(GET M4_GET_COMMAND_ARGS_SOURCE -1 source)
	file(RELATIVE_PATH outputnice "${CMAKE_CURRENT_BINARY_DIR}" "${output}")
	file(RELATIVE_PATH sourcenice "${CMAKE_SOURCE_DIR}" "${source}")
	get_filename_component(sourcename "${source}" NAME)
	get_filename_component(outputname "${output}" NAME)
	get_filename_component(depfile "${CMAKE_CURRENT_BINARY_DIR}/${output}.d" ABSOLUTE)
	file(RELATIVE_PATH outputbinrela "${CMAKE_BINARY_DIR}" "${output}")
	file(RELATIVE_PATH sourcebinrela "${CMAKE_BINARY_DIR}" "${source}")

	if(CMAKE_GENERATOR STREQUAL "Ninja")
		# When use Nninja we can use DEPFILE
		set(add_custom_command_depfile_args
			DEPFILE ${depfile}
		)
		set(script_depfile_args
			-MT ${outputbinrela} -MF ${depfile}
		)
	else()
		# Otherwise we have a problem to be figured out.
		set(add_custom_command_depfile_args)
		set(script_depfile_args)
	endif()

	set(m4_script_opts
		-e ${_M4_EXECUTABLE}
		-o ${output}
		--
		-D m4_SOURCE=${source}
		${_M4_OPTIONS_INIT}
		${M4_GET_COMMAND_ARGS_OPTIONS}
		${_M4_FILES_INIT}
		${M4_GET_COMMAND_ARGS_SOURCE}
	)

	set(add_custom_command_args
		COMMENT "m4: Generating ${outputnice} from ${sourcenice}"
		OUTPUT
			${output}
		DEPENDS
			${M4_GET_COMMAND_ARGS_SOURCE}
			${_M4_FILES_INIT}
			${_M4_SH_SCRIPT}
		${add_custom_command_depfile_args}
		COMMAND
			${_M4_SH_EXECUTABLE} ${_M4_SH_SCRIPT}
			${script_depfile_args}
			${m4_script_opts}
		VERBATIM
	)

	set(execute_process_args
		COMMAND
			${_M4_SH_EXECUTABLE} ${_M4_SH_SCRIPT}
			${m4_script_opts}
	)

endmacro()

# @def add_m4_command
# add_m4_command(
#    OUTPUT outputfielname
#    SOURCE sourcefilename...
#    OPTIONS -D A=1 -D B=2...
# )
function(m4_add_command)
	_m4_get_command_args(${ARGN})
	if($ENV{YIO_M4_CMAKE_DEBUG})
		message(STATUS "m4.cmake:add_custom_command(${add_custom_command_args})")
	endif()
	add_custom_command(${add_custom_command_args})
endfunction()

# @def m4_execute_process
# Same options as add_m4_command
function(m4_execute_process)
	_m4_get_command_args(${ARGN})
	if($ENV{YIO_M4_CMAKE_DEBUG})
		message(STATUS "m4.cmake:m4_execute_process(${execute_process_args})")
	endif()
	execute_process(
		RESULT_VARIABLE res
		OUTPUT_VARIABLE out
		ERROR_VARIABLE  err
		${execute_process_args}
	)
	if(NOT res EQUAL 0)
		message(FATAL_ERROR
			"m4_execute_process: ERROR: m4 process exited with ${res} and messaged:\n"
			"m4_execute_process: error: ${out}${err}"
		)
	endif()
endfunction()

###################################################################################

# by default builtins are prefixed with `m4_` is added to m4
m4_add_options(--prefix-builtins)

# the default (ie. "system") include directory is the m4 dir for this project
m4_add_include_directories(${CMAKE_CURRENT_LIST_DIR}/../m4)

###################################################################################





