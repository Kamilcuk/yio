if(NOT (CMAKE_C_COMPILER_ID MATCHES "GNU" AND CMAKE_MAKE_PROGRAM MATCHES "ninja") )
	return()
endif()

add_executable(yio_count_err_exe EXCLUDE_FROM_ALL count_err.c)
target_link_libraries(yio_count_err_exe yio_testlib)
add_custom_target(yio_count_err
	COMMAND sh -c "ninja yio_count_err_exe | tee /dev/stderr | grep -Ec ': error:|: warning:|: note:' > yio_error_count.txt || true"
	COMMAND sh -c "echo -n 'ERROR COUNT: '; cat yio_error_count.txt"
	DEPENDS yio_testlib
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	VERBATIM
)
set(MAX 10)
add_test(
	NAME err_count_less_than_${MAX}
	COMMAND sh -c "ninja yio_count_err && test $(cat yio_error_count.txt) -le ${MAX} || echo 'error count is not less or equal ${MAX}'"
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
set_property(TEST err_count_less_than_${MAX} PROPERTY LABELS nomemcheck)
