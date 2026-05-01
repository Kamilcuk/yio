
include_guard()

# Provide target_link_targets implementation.
# See k_target_link_targets below.
function(target_link_targets)
  k_target_link_targets(${ARGN})
endfunction()

# Takes the same arguments as target_link_libraries, but all libraries
# have to be CMake targets. Because of circular inclusion of possibility of linking
# with static libraries before they are defined in CMake, list of targets has to be
# checked _on the end_ of CMake file.
# From CMake 3.19.0 this is done with cmake_lanaguge(DEFER call. If using CMake before
# 3.19.0 you have to call k_target_link_targets_check manually on the end of your CMake
# file.
#
# @param target The target to link targets to.
# @param NODEFER Do not defer the execution and require all targets to be defined NOW.
# @param INTERFACE/PUBLIC/PRIVATE <library> See CMake documentation for target_link_libraries.
function(k_target_link_targets target)
  if(NOT TARGET "${target}")
    message(FATAL_ERROR
      "Cannot specify link targets for target \"${target}\" which is not built by this project."
    )
  endif()
  set(types PRIVATE INTERFACE PUBLIC)
  cmake_parse_arguments(K "NODEFER" "" "${types}" ${ARGN})
  list(APPEND K_PUBLIC ${K_UNPARSED_ARGUMENTS})
  # cmake-lint: disable=C0301
  # message(STATUS "target=${target} K_NODEFER=${K_NODEFER} deferargs=${deferargs} PUBLIC=${K_PUBLIC} PRIVATE=${K_PRIVATE}")
  set(deferargs)
  foreach(tt IN LISTS types)
    foreach(lib IN LISTS K_${tt})
      if(TARGET ${lib})
        target_link_libraries(${target} ${tt} ${lib})
      else()
        if(K_NODEFER)
          message(FATAL_ERROR
            "Cannot link target \"${target}\" with target \"${lib}\". Target \"${lib}\" is not built by this project."
          )
        else()
          list(APPEND deferargs ${tt} ${lib})
        endif()
      endif()
    endforeach()
  endforeach()
  if(deferargs)
    # From version 3.19.0 use cmake_language(DEFER to call check automatically.
    if(NOT K_TARGET_LINK_TARGETS_DEFERRED AND "${CMAKE_VERSION}" VERSION_GREATER_EQUAL "3.19.0")
      cmake_language(DEFER DIRECTORY ${CMAKE_SOURCE_DIR} CALL k_target_link_targets_check)
    endif()
    # Append deferred arguments to our cache variable.
    set(_K_TARGET_LINK_TARGETS_ARGS
      # ; on the end results in one empty element in the list separating "groups" of elements.
      "${_K_TARGET_LINK_TARGETS_ARGS};${target};${deferargs};"
      CACHE INTERNAL "Arguments for k_target_link_targets_check call"
    )
    # message(STATUS "DEFER=${_K_TARGET_LINK_TARGETS_ARGS}")
  endif()
endfunction()

# Checks if all the remainding non-target libraries from k_target_link_targets calls
# are actual targets.
# If at the moment of calling k_target_link_targets a library is not a target it is
# added to a cache entry. This function takes the elements from the cache and checks
# if they are actual targets.
function(k_target_link_targets_check)
  # message(STATUS "_K_=${_K_TARGET_LINK_TARGETS_ARGS}")
  # Use a temporary local variable in case we fail in the middle.
  set(llist "${_K_TARGET_LINK_TARGETS_ARGS}")
  unset(_K_TARGET_LINK_TARGETS_ARGS CACHE)
  # Iterate over arguments and on empty element flush arguments to target_link_targets.
  set(largs)
  foreach(ii IN LISTS llist)
    if(NOT ${ii} STREQUAL "")
      list(APPEND largs "${ii}")
    else()
      if(largs)
        k_target_link_targets(${largs} NODEFER)
      endif()
      set(largs)
    endif()
  endforeach()
endfunction()

###############################################################################

if(KCMAKELIB_TEST)
  set(dir ${CMAKE_CURRENT_BINARY_DIR}/ktarget_link_targets)
  file(MAKE_DIRECTORY ${dir})
  file(WRITE ${dir}/a.c [=[
    void a() {}
  ]=])
  file(WRITE ${dir}/CMakeLists.txt [=[
    cmake_minimum_required(VERSION 3.1)
    include(${THIS_FILE})
    project(test LANGUAGES C)
    if(NOT NO_A_LIB)
      add_library(a a.c)
    endif()
    target_link_targets(a b)
    target_link_targets(a PUBLIC c PRIVATE d)
    if(NOT NO_B_LIB)
      add_library(b a.c)
      target_link_targets(b a c)
    endif()
    add_library(c a.c)
    target_link_libraries(c PUBLIC a b INTERFACE d)
    add_library(d INTERFACE)
    target_link_libraries(d INTERFACE a b c)
    k_target_link_targets_check()
  ]=])

  add_test(
    NAME target_link_targets_1
    COMMAND cmake
      -S ${dir}
      -B ${dir}/build1
      -D THIS_FILE=${CMAKE_CURRENT_LIST_FILE}
  )

  file(WRITE ${dir}/test.cmake [=[
    execute_process(
      COMMAND ${CMAKE_COMMAND}
        -S .
        -B ${BUILDDIR}
        -D THIS_FILE=${THIS_FILE}
        -D NO_A_LIB=${NO_A_LIB}
        -D NO_B_LIB=${NO_B_LIB}
      RESULT_VARIABLE ret
      OUTPUT_VARIABLE out
      ERROR_VARIABLE err
    )
    message("${out}")
    message("${err}")
    if(NOT ret)
      message(FATAL_ERROR "")
    endif()
    if(NOT err MATCHES "${PASS_REGULAR_EXPRESSION}")
      message(FATAL_ERROR "\"${err}\" NOT MATCHES \"${PASS_REGULAR_EXPRESSION}\"")
    endif()
  ]=])
  add_test(
    NAME target_link_targets_2
    COMMAND ${CMAKE_COMMAND}
      -D BUILDDIR=build2
      -D THIS_FILE=${CMAKE_CURRENT_LIST_FILE}
      -D NO_B_LIB=1
      -D "PASS_REGULAR_EXPRESSION=.*Cannot link target.*"
      -P test.cmake
    WORKING_DIRECTORY ${dir}
  )
  add_test(
    NAME target_link_targets_3
    COMMAND ${CMAKE_COMMAND}
      -D BUILDDIR=build3
      -D THIS_FILE=${CMAKE_CURRENT_LIST_FILE}
      -D NO_A_LIB=1
      -D "PASS_REGULAR_EXPRESSION=.*Cannot specify link targets.*"
      -P test.cmake
    WORKING_DIRECTORY ${dir}
  )

endif()



