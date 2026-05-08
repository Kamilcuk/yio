
# cmake-lint: disable=C0103
include(${CMAKE_CURRENT_LIST_DIR}/var_to_0_or_1.cmake)
include(CheckCSourceCompiles)

# check_type_exists_bool
# small wrapper around check_c_source_compiles to get "1" or "0" as result
function(check_type_exists_bool type var)
  if(DEFINED HAVE_${var})
    if(HAVE_${var})
      set(${var} 1 PARENT_SCOPE)
    else()
      set(${var} 0 PARENT_SCOPE)
    endif()
    return()
  endif()

  set(builtin 0)
  if("BUILTIN_TYPES_ONLY" IN_LIST ARGN)
    set(builtin 1)
  endif()

  if(NOT CMAKE_REQUIRED_QUIET)
    message(STATUS "Check type ${type}")
  endif()

  set(src "")
  if(NOT builtin)
    foreach(ii IN LISTS CMAKE_EXTRA_INCLUDE_FILES)
      string(APPEND src "#include <${ii}>\n")
    endforeach()
  endif()
  string(APPEND src "int main() { ${type} x; (void)x; return 0; }")

  set(CMAKE_REQUIRED_QUIET_save ${CMAKE_REQUIRED_QUIET})
  set(CMAKE_REQUIRED_QUIET 1)
  check_c_source_compiles("${src}" HAVE_${var})
  set(CMAKE_REQUIRED_QUIET ${CMAKE_REQUIRED_QUIET_save})

  if(HAVE_${var})
    set(${var} 1)
  else()
    set(${var} 0)
  endif()

  if(NOT CMAKE_REQUIRED_QUIET)
    set(_msg " - ${var}=${${var}} HAVE_${var}=${HAVE_${var}}")
    if(NOT ${var})
      message(STATUS "Check type ${type} - failed${_msg}")
    else()
      message(STATUS "Check type ${type} - done${_msg}")
    endif()
  endif()

  set(${var} "${${var}}" PARENT_SCOPE)

endfunction()
