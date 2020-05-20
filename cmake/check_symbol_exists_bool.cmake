
macro(check_symbol_exists_bool symbol files variable)
	check_symbol_exists(${symbol} ${files} ${variable})
	var_to_0_or_1(${variable})
endmacro()
