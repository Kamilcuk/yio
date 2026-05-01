
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

if(KCMAKELIB_TEST OR KCMAKELIB_INCLUDE_ALL)
  # Do not run tests in included files - it's handled by testing module.
  unset(KCMAKELIB_TEST CACHE)
  # Do not warn about cmkae_policy in included files.
  cmake_policy(SET CMP0011 NEW)
  file(GLOB_RECURSE files ${CMAKE_CURRENT_LIST_DIR}/k/*.cmake)
  # Filter internal files.
  list(FILTER files EXCLUDE REGEX "m4.*cmake")
  #
  foreach(ff IN LISTS files)
    message(STATUS "Including ${ff}")
    include(${ff})
  endforeach()
endif()

