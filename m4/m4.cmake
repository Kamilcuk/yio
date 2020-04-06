include_guard()

set(_M4_INCLUDE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
file(GLOB _M4_INCLUDE_FILES ${CMAKE_CURRENT_LIST_DIR}/*.m4 ${CMAKE_CURRENT_LIST_DIR}/*.sed)

function(add_m4_target output source)
	if(NOT ${ext} MATCHES ".m4$")
		message(FATAL_ERROR "add_m4_target: file ${source} doesn't has .m4 extension")
	endif()
	
	file(RELATIVE_PATH outputnice ${CMAKE_SOURCE_DIR} ${output})
	file(RELATIVE_PATH scriptnice ${CMAKE_SOURCE_DIR} ${script})
	
	add_custom_command(
		OUTPUT ${header}
		MAIN_DEPENDENCY ${script}
		DEPENDS ${_M4_INCLUDE_FILES} 
		# COMMAND sh -c "if ! m4 -I\"$1\" \"$2\" > \"$3\"; then tail -n2 \"$3\"; exit 1; else chmod ugo-w \"$3\"; fi" _ ${_M4_INCLUDE_DIRECTORY} ${script} ${header}
		COMMAND sh -c "m4 -I\"$1\" \"$2\" > \"$3\" && chmod ugo-w \"$3\"" _ 
				${_M4_INCLUDE_DIRECTORY} ${source} ${header}
		COMMENT "Generating ${scriptnice} from ${headernice}"
		WORKING_DIRECTORY ${dir}
		VERBATIM
	)
	
	set(${output} ${header} PARENT_SCOPE)
endfunction()
