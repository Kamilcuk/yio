
include(var_to_0_or_1)

# check_type_size_bool
# small wrapper around check_type_size to get "1" or "0" as result
macro(check_type_size_bool)
	check_type_size(${ARGN})
	var_to_0_or_1(${ARGV1})
endmacro()
