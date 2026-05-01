# redirectstdin.cmake
# cmake-lint: disable=C0103

# Extract positional arguments from script invocation
set(command)
set(idx 0)
math(EXPR argv_one_less "${CMAKE_ARGC} - 1")
foreach(ii RANGE 1 ${argv_one_less} 1)
  #message(STATUS "AAA ${CMAKE_ARGV${ii}}")
  if(CMAKE_ARGV${ii} STREQUAL -P)
    set(idx 1)
  elseif(idx EQUAL 1)
    set(idx 2)
  elseif(idx EQUAL 2)
    set(idx 3)
    set(INPUT "${CMAKE_ARGV${ii}}")
  elseif(idx EQUAL 3)
    list(APPEND command "${CMAKE_ARGV${ii}}")
  endif()
endforeach()

#message(STATUS "CMD ${command}")
if(INPUT STREQUAL "")
  message(FATAL_ERROR "INPUT is empty")
endif()
list(LENGTH command args)
if(args EQUAL 0)
  message(FATAL_ERROR "Missing command")
endif()

# Execute the process
execute_process(
  COMMAND ${CMAKE_COMMAND} -E echo "${INPUT}"
  COMMAND "${command}"
  ECHO_OUTPUT_VARIABLE
  ECHO_ERROR_VARIABLE
  COMMAND_ERROR_IS_FATAL ANY
)

