
function(debug_variables prefix)
	set(_debug_variables_list "${ARGN}")
	list(SORT _debug_variables_list)
	foreach(_debug_variables_i IN LISTS _debug_variables_list)
		if(DEFINED ${_debug_variables_i})
			message(STATUS "${prefix} ${_debug_variables_i}=${${_debug_variables_i}}")
		else()
			message(STATUS "${prefix} undef ${_debug_variables_i}")
		endif()
	endforeach()
endfunction()