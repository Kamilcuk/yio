
## @def debug_variables([PREFIX prefix] vars...)
function(debug_variables)
	cmake_parse_arguments(ARG "" "PREFIX" "" ${ARGN})
	
	set(_debug_variables_list "${ARG_UNPARSED_ARGUMENTS}")
	list(SORT _debug_variables_list)
	foreach(_debug_variables_i IN LISTS _debug_variables_list)
		if(DEFINED ${_debug_variables_i})
			message(STATUS "${ARG_PREFIX}${_debug_variables_i}=${${_debug_variables_i}}")
		else()
			message(STATUS "${ARG_PREFIX}${_debug_variables_i} undefined")
		endif()
	endforeach()
endfunction()
