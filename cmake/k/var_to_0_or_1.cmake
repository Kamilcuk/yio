
macro(VAR_TO_0_OR_1)
  if(${ARGV0})
    set(${ARGV0} 1)
  else()
    set(${ARGV0} 0)
  endif()
endmacro()
