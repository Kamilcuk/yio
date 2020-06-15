
## @def assert_is_valid_bool_only([PREFIX prefix] var...)
## Checks if variables are valid bools only.
## If they are not, FATAL_ERROR is issued
function(assert_is_valid_bool_only)

	cmake_parse_arguments(ARG "" "PREFIX" "" ${ARGN})
	set(ARG_PREFIX "${ARG_PREFIX}assert_is_valid_bool_only")
	
	foreach(i IN LISTS ARG_UNPARSED_ARGUMENTS)
		if(NOT DEFINED ${i})
			message(FATAL_ERROR "${ARG_PREFIX}: ${i} is not defined")
		endif()
		
		set(valid FALSE)
		foreach(j IN ITEMS 
			1 ON YES TRUE Y
			0 OFF NO FALSE N IGNORE NOTFOUND
		)
			if(${i} STREQUAL j)
				set(valid TRUE)
				break()
			endif()
		endforeach()
		if(NOT valid)
			if(${i} MATCHES "^[0-9]+$" OR ${i} MATCHES "^.*-NOTFOUND$")
				set(valid TRUE)
			endif()
		endif()
		if(NOT valid)
			message(FATAL_ERROR "${ARG_PREFIX}: Variable ${i} is not ON nor OFF: ${i}=${${i}}")
		endif()
	endforeach()
endfunction()


## @def assert_is_ON_or_OFF_only([PREFIX prefix] var...)
## @see assert_is_valid_bool_only
## Like assert_is_valid_bool_only but only checks for OFF and ON
function(assert_is_ON_or_OFF_only)

	cmake_parse_arguments(ARG "" "PREFIX" "" ${ARGN})
	set(ARG_PREFIX "${ARG_PREFIX}assert_is_ON_or_OFF_only")
	
	foreach(i IN LISTS ARG_UNPARSED_ARGUMENTS)
		if(NOT DEFINED ${i})
			message(FATAL_ERROR "${ARG_PREFIX}: ${i} is not defined")
		endif()
		
		set(valid FALSE)
		foreach(j IN ITEMS ON OFF)
			if(${i} STREQUAL j)
				set(valid TRUE)
				break()
			endif()
		endforeach()
		if(NOT valid)
			message(FATAL_ERROR "${ARG_PREFIX}: Variable ${i} is not ON nor OFF: ${i}=${${i}}")
		endif()
		
	endforeach()
endfunction()


     