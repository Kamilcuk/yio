
## @def yio_debug_variables(PREFIX)
## Outputs all variables passed to it with values.
function(yio_debug_variables)
	if(NOT YIO_DEV)
		return()
	endif()

	if($ENV{YIO_DEBUG_VARIABLES_ALWAYS})
	else()
		string(MD5 varname "${ARGV}")
		string(PREPEND varname "YIO_DEBUG_VARIABLES_")
		if(${varname})
			return()
		endif()
		set(${varname} TRUE CACHE INTERNAL
			"Internal variable used to make yio_debug_variable print only once")
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
