
## @def debug_variables([DV_PREFIX prefix] [DV_LEVEL FATAL_ERROR|SEND_ERROR|....] [DV_ONCE] [DV_NOSORT] vars...)
## @param DV_PREFIX prefix Prepend all lines with a prefix
## @param DV_LEVEL level Print on this level
## @param DV_ONCE Print the output only once per configuration
## @param DV_NOSORT Do not sort the variable before outputting
##
## Outputs all variables passed to it with values.
function(debug_variables)
	cmake_parse_arguments(_debug_variables "DV_ONCE;DV_NOSORT;DV_TRACE" "DV_PREFIX;DV_LEVEL" "" ${ARGN})
	
	if(_debug_variables_DV_ONCE)
		string(MD5 _debug_variables_var "${PROJECT_NAME} ${CMAKE_CURRENT_LIST_FILE} ${CMAKE_CURRENT_LIST_LINE}")
		string(PREPEND _debug_variables_var "_debug_variables_")
		if(DEFINED ${_debug_variables_var} AND ${_debug_variables_var})
			# message(STATUS "${_debug_variables_var}=${${_debug_variables_var}}")
			return()
		endif()
		set("${_debug_variables_var}" TRUE CACHE INTERNAL "" FORCE)
	endif()
	unset(_debug_variables_DV_ONCE)

	if(NOT DEFINED _debug_variables_DV_LEVEL)
		set(_debug_variables_DV_LEVEL STATUS)
	endif()
	if(_debug_variables_DV_TRACE)
		set(_debug_variables_DV_LEVEL TRACE)
	endif()

	set(_debug_variables_list "${_debug_variables_UNPARSED_ARGUMENTS}")

	if(NOT _debug_variables_DV_NOSORT)
		list(SORT _debug_variables_list)
	endif()

	foreach(_debug_variables_i IN LISTS _debug_variables_list)
		if(DEFINED ${_debug_variables_i})
			set(_debug_variables_val "=${${_debug_variables_i}}")
		else()
			set(_debug_variables_val " undefined")
		endif()
		message("${_debug_variables_DV_LEVEL}" "${_debug_variables_DV_PREFIX}${_debug_variables_i}${_debug_variables_val}")
	endforeach()
endfunction()

