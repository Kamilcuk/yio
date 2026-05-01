
include_guard()

# Global variable to store the current counter accross all invocations.
set(_COUNTEROFFSET_N 0 CACHE INTERNAL "")

# cmake-lint: disable=C0103
function(add_source_files_counter_offset)
  cmake_parse_arguments(A "" "INCREMENT;MACRONAME" "" ${ARGN})
  if(NOT DEFINED A_MACRONAME)
    set(A_MACRONAME "COUNTER")
  endif()
  set(ii "${_COUNTEROFFSET_N}")
  foreach(ff IN LISTS A_UNPARSED_ARGUMENTS)
    set_source_files_properties(${ff} PROPERTIES COMPILE_DEFINITIONS
      "${A_MACRONAME}=(__COUNTER__+${ii})"
    )
    if(NOT A_INCREMENT)
      # Get length dynamically by searching the string "COUNTER" in the source file.
      file(STRINGS ${ff} count REGEX "${A_MACRONAME}")
      list(LENGTH count incr)
    else()
      set(incr ${A_INCREMENT})
    endif()
    math(EXPR ii "${ii} + ${incr}")
  endforeach()
  set(_COUNTEROFFSET_N "${ii}" CACHE INTERNAL "")
endfunction()

if(KCMAKELIB_TEST)
  set(D ${CMAKE_CURRENT_BINARY_DIR}/counteroffset)
  file(MAKE_DIRECTORY ${D})
  file(WRITE ${D}/CMakeLists.txt [=[
cmake_minimum_required(VERSION 3.1)
project(test)
include(${THIS_FILE})
add_source_files_counter_offset(file1.c main.c)
add_source_files_counter_offset(file2.c)
add_executable(main file1.c main.c file2.c)
]=])
  file(WRITE ${D}/file1.c [=[
int x = COUNTER;
int y = COUNTER;
]=])
  file(WRITE ${D}/file2.c [=[
int a = COUNTER;
int b = COUNTER;
]=])
  file(WRITE ${D}/main.c [=[
#include <stdio.h>
#include <assert.h>
extern int a, b, x, y;
int main() {
  assert(x == 0);
  assert(y == 1);
  assert(a == 2);
  assert(b == 3);
  printf("%d %d %d %d\n", x, y, a, b);
}
]=])
  file(WRITE ${D}/script.cmake [=[
macro(EXEC_CHECK)
  execute_process(COMMAND ${ARGN} RESULT_VARIABLE CMD_RESULT)
    if(CMD_RESULT)
        message(FATAL_ERROR "Error running ${CMD}")
    endif()
endmacro()
EXEC_CHECK(${CMAKE_COMMAND} -S . -B _build -D THIS_FILE=${THIS_FILE})
EXEC_CHECK(${CMAKE_COMMAND} --build _build --verbose)
EXEC_CHECK(_build/main)
]=])
  add_test(
    NAME counteroffset
    COMMAND ${CMAKE_COMMAND} -D THIS_FILE=${CMAKE_CURRENT_LIST_FILE} -P ${D}/script.cmake
    WORKING_DIRECTORY ${D}
  )
endif()

