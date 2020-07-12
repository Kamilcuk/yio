
## @def yio_debug_variables(PREFIX)
## Outputs all variables passed to it with values.
function(yio_debug_variables _yio_PREFIX)
	if(NOT YIO_TRACE)
		return()
	endif()
	
	set(_yio_list "${ARGV}")

	list(SORT _yio_list)
	list(REMOVE_DUPLICATES _yio_list)

	foreach(_yio_i IN LISTS _yio_list)
		if(DEFINED ${_yio_i})
			set(_yio_val "=${${_yio_i}}")
		else()
			set(_yio_val " undefined")
		endif()
		message(STATUS "yio: ${_yio_i}${_yio_val}")
	endforeach()	
endfunction()

