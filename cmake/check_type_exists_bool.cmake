
include(var_to_0_or_1)

# check_type_exists_bool
# small wrapper around check_type_size to get "1" or "0" as result
function(check_type_exists_bool)
	if(DEFINED HAVE_${ARGV1})
		return()
	endif()

	if(NOT CMAKE_REQUIRED_QUIET)
		message(STATUS "Check size of ${ARGV0}")
	endif()

	set(CMAKE_REQUIRED_QUIET_save ${CMAKE_REQUIRED_QUIET})
	set(CMAKE_REQUIRED_QUIET 1)
	check_type_size(${ARGN})
	set(CMAKE_REQUIRED_QUIET ${CMAKE_REQUIRED_QUIET_save})

	if(NOT CMAKE_REQUIRED_QUIET)
		set(_msg " - ${ARGV1}=${${ARGV1}} HAVE_${ARGV1}=${HAVE_${ARGV1}}")
		if(NOT ${ARGV1})
			message(STATUS "Check size of ${ARGV0} - failed${_msg}")
		else()
			message(STATUS "Check size of ${ARGV0} - done${_msg}")
		endif()
	endif()

	# Depend on HAVE_* variable
	# The varibale with size does not work when -flto is enabled
	if(HAVE_${ARGV1})
		set(${ARGV1} 1)
	else()
		set(${ARGV1} 0)
	endif()

	set(${ARGV1} "${${ARGV1}}" PARENT_SCOPE)

endfunction()

