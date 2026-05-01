
## @def assert_is_valid_bool_only([PREFIX prefix] var...)
## Checks if variables are valid bools only.
## If they are not, FATAL_ERROR is issued
function(assert_is_valid_bool_only)

  cmake_parse_arguments(arg "" "PREFIX" "" ${ARGN})
  set(arg_PREFIX "${arg_PREFIX}assert_is_valid_bool_only")

  foreach(aa IN LISTS ARG_UNPARSED_ARGUMENTS)
    if(NOT DEFINED ${aa})
      message(FATAL_ERROR "${arg_PREFIX}: ${aa} is not defined")
    endif()

    set(valid FALSE)
    foreach(bb IN ITEMS
        1 ON YES TRUE Y
        0 OFF NO FALSE N IGNORE NOTFOUND
    )
      if(${aa} STREQUAL bb)
        set(valid TRUE)
        break()
      endif()
    endforeach()
    if(NOT valid)
      if(${aa} MATCHES "^[0-9]+$" OR ${aa} MATCHES "^.*-NOTFOUND$")
        set(valid TRUE)
      endif()
    endif()
    if(NOT valid)
      message(FATAL_ERROR "${arg_PREFIX}: Variable ${aa} is not ON nor OFF: ${aa}=${${aa}}")
    endif()
  endforeach()
endfunction()


## @def assert_is_ON_or_OFF_only([PREFIX prefix] var...)
## @see assert_is_valid_bool_only
## Like assert_is_valid_bool_only but only checks for OFF and ON
function(assert_is_ON_or_OFF_only)

  cmake_parse_arguments(arg "" "PREFIX" "" ${ARGN})
  set(arg_PREFIX "${arg_PREFIX}assert_is_ON_or_OFF_only")

  foreach(ii IN LISTS ARG_UNPARSED_ARGUMENTS)
    if(NOT DEFINED ${ii})
      message(FATAL_ERROR "${arg_PREFIX}: ${ii} is not defined")
    endif()

    set(valid FALSE)
    foreach(jj IN ITEMS ON OFF)
      if(${ii} STREQUAL jj)
        set(valid TRUE)
        break()
      endif()
    endforeach()
    if(NOT valid)
      message(FATAL_ERROR "${arg_PREFIX}: Variable ${ii} is not ON nor OFF: ${ii}=${${ii}}")
    endif()

  endforeach()
endfunction()

