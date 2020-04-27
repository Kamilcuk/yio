
macro(var_to_0_or_1)
	if(${ARGV0})
		set(${ARGV0} 1)
	else()
		set(${ARGV0} 0)
	endif()
endmacro()
