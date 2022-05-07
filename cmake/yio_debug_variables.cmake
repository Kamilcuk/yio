include_guard()
include(k/set_from_env)

# @def yio_debug_variables(PREFIX) Outputs all variables passed to it with
# values.
function(yio_debug_variables)
  if(NOT YIO_DEV)
    return()
  endif()

  set_from_env(YIO_DEBUG_VARIABLES_ALWAYS)
  if(NOT YIO_DEBUG_VARIABLES_ALWAYS)
    string(MD5 varname "${ARGV}")
    string(PREPEND varname "YIO_DEBUG_VARIABLES_")
    if(${varname})
      return()
    endif()
    set(${varname}
        TRUE
        CACHE
          INTERNAL
          "Internal variable used to make yio_debug_variable print only once")
  endif()

  set(llist "${ARGV}")

  # list(SORT llist) list(REMOVE_DUPLICATES llist)

  foreach(ii IN LISTS llist)
	if(ii MATCHES "DIRECTORY:")
	  string(REGEX REPLACE "DIRECTORY:" "" ii ${ii})
      get_directory_property(vval ${ii})
      set(ii "DIRECTORY:${ii}=")
    elseif(DEFINED ${ii})
      set(vval "=${${ii}}")
    else()
      set(vval " undefined")
    endif()
    message(STATUS "yio: ${ii}${vval}")
  endforeach()
endfunction()
