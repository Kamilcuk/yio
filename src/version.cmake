include_guard()

execute_process(
	COMMAND git describe --tags --match "v[0-9]*"
	RESULT_VARIABLE _result
	OUTPUT_VARIABLE _output
	ERROR_VARIABLE _error
	WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/..
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(NOT _result EQUAL 0)
	execute_process(COMMAND git status WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/..)
  message(FATAL_ERROR "Getting version failed: ${_result} ${_output} ${_error}")
endif()
if(NOT _output MATCHES "v([0-9]+).([0-9]+).([0-9]+)[\\.-]?([0-9]*)?.*")
	execute_process(COMMAND git status WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/..)
	message(FATAL_ERROR "Version has wrong format: ${_output}")
endif()

# version numbers have to be max 255
foreach(ii IN ITEMS 1 2 3)
	if(CMAKE_MATCH_${ii} GREATER_EQUAL 255)
		message(FATAL_ERROR "Welcome to the future, version too high: ${_output}")
	endif()
endforeach()
# If no 4th match, set to zero
if(NOT DEFINED CMAKE_MATCH_4 OR CMAKE_MATCH_4 STREQUAL "")
	set(CMAKE_MATCH_4 0)
endif()
# If greater then 255, crop at 255
if(CMAKE_MATCH_4 GREATER 255)
	set(CMAKE_MATCH_4 255)
endif()

set(YIO_PROJECT_VERSION "${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}.${CMAKE_MATCH_4}")
set(YIO_PROJECT_SOVERSION "${CMAKE_MATCH_1}.${CMAKE_MATCH_2}")
math(EXPR YIO_VERSION
  "(${CMAKE_MATCH_1} << 24) | (${CMAKE_MATCH_2} << 16) | ${CMAKE_MATCH_3} << 8 | ${CMAKE_MATCH_4}"
  OUTPUT_FORMAT HEXADECIMAL)
set(YIO_VERSION_STRING "${_output}")

# change "0xabc" to "0x00000abc"
function(pad_version_with_zeros_to_8_characters var val)
	string(REGEX REPLACE "^0x" "" "val" "${val}")
	while(TRUE)
		string(LENGTH "${val}" _tmp)
		if(_tmp LESS 8)
			string(PREPEND val "0")
		else()
			break()
		endif()
	endwhile()
	set(${var} "0x${val}" PARENT_SCOPE)
endfunction()
pad_version_with_zeros_to_8_characters(YIO_VERSION "${YIO_VERSION}")

yio_debug_variables(
	YIO_PROJECT_VERSION
	YIO_VERSION
	YIO_VERSION_STRING
)

