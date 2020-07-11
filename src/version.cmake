include_guard()

execute_process(
	COMMAND git describe --tags --match "v[0-9]*"
	RESULT_VARIABLE _result
	OUTPUT_VARIABLE _output
	ERROR_VARIABLE _error
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(NOT _result EQUAL 0)
	message(FATAL_ERROR "Getting version failed: ${_error}")
endif()
if(NOT _output MATCHES "v([0-9]+).([0-9]+).([0-9]+)[\\.-]?([0-9]*)?.*")
	message(FATAL_ERROR "Version has wrong format: ${_output}")
endif()

# version numbers have to be max 255
foreach(i IN ITEMS 1 2 3)
	if(CMAKE_MATCH_${i} GREATER_EQUAL 255)
		message(FATAL_ERROR "Welcome to the future, version too high: ${_output}")
	endif()
endforeach()
# If no 4th match, set to zero
if("${CMAKE_MATCH_4}" STREQUAL "")
	set(CMAKE_MATCH_4 0)
endif()
# If greater then 255, crop at 255
if(CMAKE_MATCH_4 GREATER 255)
	set(CMAKE_MATCH_4 255)
endif()

set(YIO_PROJECT_VERSION "${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}.${CMAKE_MATCH_4}")
math(EXPR YIO_VERSION "(${CMAKE_MATCH_1} << 24) | (${CMAKE_MATCH_2} << 16) | ${CMAKE_MATCH_3} << 8 | ${CMAKE_MATCH_4}" OUTPUT_FORMAT HEXADECIMAL)
set(YIO_VERSION_STRING "${_output}")

debug_variables(DV_PREFIX "yio: " DV_LEVEL TRACE DV_ONCE
	YIO_PROJECT_VERSION
	YIO_VERSION
	YIO_VERSION_STRING
)

