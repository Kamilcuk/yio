file(GLOB srcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c)
foreach(src IN LISTS srcs)
  string(REGEX REPLACE "\\.c$" "" name "${src}")
  string(PREPEND name "yio_example_")
  add_executable(${name} ${src})
  target_link_libraries(${name} PRIVATE yio)
  add_test(NAME ${name} COMMAND ${name})
  set_tests_properties(${name} PROPERTIES LABELS normal)
endforeach()
