include_guard()

macro(SET_FROM_ENV sfe_var)
  set(${sfe_var})
  if(DEFINED "ENV{${sfe_var}}")
    set(${sfe_var} "$ENV{${sfe_var}}")
  endif()
endmacro()

