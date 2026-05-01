
# Same as check_symbol_exists but the result is a 0 albo 1.
function(check_symbol_exists_bool symbol files variable)
  check_symbol_exists(${symbol} ${files} ${variable})
  var_to_0_or_1(${variable})
  set(${variable} ${${variable}} PARENT_SCOPE)
endfunction()

