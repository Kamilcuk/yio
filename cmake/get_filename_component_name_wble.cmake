# get_filename_component_name_wble(result "dir/filename.a.b.c" [last])
# name_wble - name without before last extension
# Extracts the filename out of second argument 
# and removes the one before last extension and places it in <result>.
# If the 3rd argument is present, places the
# one before last extension in the 3rd argument <last>.
# Examples in the form of tests below.
# Note this is a macro and uses string(REGEX REPLACE
# so it overwrites MATCHES and CMAKE_MATCH_<n> variables.
macro(get_filename_component_name_wble var filename)
	if(${ARGC} GREATER 3)
		message(FATAL_ERROR "get_filename_component_name_wble: too many arguments: ${ARGC}")
	endif()
	get_filename_component("${var}" "${filename}" NAME)
	if(NOT "${filename}" MATCHES "(\\.[^.]*)(\\.[^.]*)$")
		if(${ARGC} GREATER 2)
			set("${ARGV2}" "")
		endif()
	else()
		if(${ARGC} GREATER 2)
			string(REGEX REPLACE "^(.*)(\\.[^.]*)(\\.[^.]*)$" 
				"\\2" "${ARGV2}" "${${var}}")
		endif()
		string(REGEX REPLACE "(\\.[^.]*)(\\.[^.]*)$" 
			"\\2" "${var}" "${${var}}")
	endif()
endmacro()

# Examples in form of tests:
function(test_get_filename_component_name_wble)
	get_filename_component_name_wble(var "dir/file.a.b.c" last)
	if(NOT "${var}" STREQUAL "file.a.c" OR
		NOT "${last}" STREQUAL ".b")
		message(FATAL_ERROR "${var}!=file.a.c ${last}!=.b")
	endif()
	
	get_filename_component_name_wble(var "dir/file.a" last)
	if(NOT "${var}" STREQUAL "file.a" OR
		NOT "${last}" STREQUAL "")
		message(FATAL_ERROR "${var}!=file.a ${last}!=\"\"")
	endif()
	
	get_filename_component_name_wble(var "dir/file.a")
	if(NOT "${var}" STREQUAL "file.a")
		message(FATAL_ERROR "${var}!=file.a")
	endif()
	
	get_filename_component_name_wble(var "dir/file.a.b.c")
	if(NOT "${var}" STREQUAL "file.a.c")
		message(FATAL_ERROR "${var}!=file.a.c")
	endif()	
endfunction()