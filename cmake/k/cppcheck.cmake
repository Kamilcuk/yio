
find_program(CMAKE_C_CPPCHECK NAMES cppcheck)
if (CMAKE_C_CPPCHECK)
  message(STATUS "Found cppcheck: ${CMAKE_C_CPPCHECK}")
  list(APPEND CMAKE_C_CPPCHECK
    "--inconclusive"
    "--inline-suppr"
    "--template={file}:{line}:{column}: info: cppcheck: {severity}: {id}: {message}"
  )
  set(CMAKE_CXX_CPPCHECK "${CMAKE_C_CPPCHECK}")
endif()
