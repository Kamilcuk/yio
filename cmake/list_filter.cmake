
macro(list_filter output list mode regex)
	set(${output} ${${list}})
	list(FILTER ${output} ${mode} REGEX ${regex})
	# message(STATUS "list_filter: output=${output}=${${output}}")
endmacro()
