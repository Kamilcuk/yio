add_executable(yio_godbolt_example default.c)
target_link_libraries(yio_godbolt_example PRIVATE yio)
add_test(NAME godbolt_example COMMAND yio_godbolt_example)
set_tests_properties(godbolt_example PROPERTIES PASS_REGULAR_EXPRESSION "Point\\(x=10, y=20\\)")
