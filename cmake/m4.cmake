include_guard()

if(NOT _M4_SH_EXECUTABLE)
	find_program(_M4_SH_EXECUTABLE sh
		DOC "Patch to shell")
	if(NOT _M4_SH_EXECUTABLE)
		message(FATAL_ERROR "shell not found")
	endif()
endif()

if(NOT _M4_EXECUTABLE)
	find_program(_M4_EXECUTABLE m4
		DOC "Patch to m4 executable")
	if(NOT _M4_EXECUTABLE)
		message(FATAL_ERROR "m4 executable not found")
	endif()
endif()

# Options for m4 preprocessor
# Also see end of this file
set(_M4_OPTIONS_INIT "")

# Dependencies generated from include directories for m4 preprocessor
# Also see end of this file
set(_M4_OPTIONS_DEPENDS_INIT "")

# The executable script to run
set(_M4_SH_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/m4.sh)

define_property(TARGET
	PROPERTY M4_OPTIONS INHERITED
	BRIEF_DOCS "List of options to be passed to m4 preprocessor"
	FULL_DOCS "List of options to be passed to m4 preprocessor"
)

macro(m4_add_options)
	if(ARGC GREATER 1 AND TARGET ARGV1)
		message(FATAL_ERROR "m4_add_options passed target name ${ARGV1}")
	endif()
	list(APPEND _M4_OPTIONS_INIT ${ARGN})
endmacro()

function(m4_add_include_directories)
	if(ARGC GREATER 1 AND TARGET ARGV1)
		message(FATAL_ERROR "m4_add_options passed target name ${ARGV1}")
	endif()
	foreach(i IN LISTS ARGN)

		if(${i} MATCHES "^-I")
			string(REGEX REPLACE "^-I" "" i ${i})
		endif()

		list(APPEND _M4_OPTIONS_INIT -I${i})
		file(GLOB_RECURSE _tmp ${i}/*.m4)
		list(APPEND _M4_OPTIONS_DEPENDS_INIT ${_tmp})
	endforeach()
	set(_M4_OPTIONS_INIT ${_M4_OPTIONS_INIT} PARENT_SCOPE)
	set(_M4_OPTIONS_DEPENDS_INIT ${_M4_OPTIONS_DEPENDS_INIT} PARENT_SCOPE)
endfunction()

macro(m4_add_file_dependencies)
	list(APPEND _M4_OPTIONS_DEPENDS_INIT ${ARGN})
endmacro()

# @def m4_target_options(target options...)
# @param target The name of the target
# @param options Options to add to m4 program
function(m4_target_options target)
	set_property(TARGET ${target} APPEND
		PROPERTY M4_OPTIONS ${ARGN}
	)
endfunction()

# @def m4_target_include_directories(target directories...)
# @param target The name of the target
# @param directories Directories to add as m4 include directories
function(m4_target_include_directories target)
	foreach(arg IN LISTS ARGN)
		if("${arg}" STREQUAL "PUBLIC" OR
			"${arg}" STREQUAL "PRIVATE" OR
			"${arg}" STREQUAL "INTERFACE")
			message(FATAL_ERROR
				"m4_target_include_directories: doesn't support ${arg}")
		endif()

		if(${i} MATCHES "^-I")
			string(REGEX REPLACE "^-I" "" i ${i})
		endif()

		m4_target_options(${target} -I${arg})

		file(GLOB_RECURSE _tmp ${arg}/*.m4)
		add_dependencies(${target} ${_tmp})

	endforeach()
endfunction()

# @def m4_get_command(output)
# @param output Output variable
# @brief Get's the command to run m4 with options into 
# variable ${output}
function(m4_get_command output)
	set(${output} ${_M4_EXECUTABLE} "${_M4_OPTIONS_INIT}" PARENT_SCOPE)
endfunction()

function(_m4_add_command_in output opts)
	file(RELATIVE_PATH outputnice ${CMAKE_SOURCE_DIR} ${output})
	list(GET ARGN -1 _last)
	file(RELATIVE_PATH sourcenice ${CMAKE_SOURCE_DIR} ${_last})
	add_custom_command(
		OUTPUT ${output}
		MAIN_DEPENDENCY ${source}
		DEPENDS ${_M4_SH_SCRIPT} ${_M4_OPTIONS_DEPENDS_INIT}
		COMMAND ${_M4_SH_EXECUTABLE} ${_M4_SH_SCRIPT}
			${_M4_EXECUTABLE} ${output} ${opts} ${ARGN}
		COMMENT "m4: Generating ${sourcenice} from ${outputnice}"
		VERBATIM
	)
endfunction()

# add_m4_command(output source ...)
# @param output The output file to generate
# @param source An option to pass to m4.
# @param ... Additional options to pass to m4
function(m4_add_command output source)
	_m4_add_command_in(${output}
		"${_M4_OPTIONS_INIT}" ${source} ${ARGN})
endfunction()

# add_m4_target(target output source ...)
# @param target The target name to add
# @param source An option to pass to m4.
# @param ... Additional options to pass to m4
function(m4_add_target target output source)
	_m4_add_command_in(${output}
		$<TARGET_PROPERTY:${target},M4_OPTIONS> ${source} ${ARGN})
	add_custom_target(${target} ALL
		DEPENDS ${output}
	)
	set_property(TARGET ${target} PROPERTY LOCATION ${output})
	m4_target_options(${target} ${_M4_OPTIONS_INIT})
endfunction()

# by default builtins are prefixed with `m4_` is added to m4
m4_add_options(--prefix-builtins)

# the default (ie. "system") include directory is the m4 dir for this project
m4_add_include_directories(${CMAKE_CURRENT_LIST_DIR}/../m4)







