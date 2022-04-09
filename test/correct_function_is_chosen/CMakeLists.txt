
if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
	return()
endif()

add_executable(correct_function_is_chosen correct_function_is_chosen.c)
foreach(i IN ITEMS
	_yIO_print_float_strfromf
	_yIO_print_float_strfromd
	_yIO_print_float_strfroml
	_yIO_print_float_printff
	_yIO_print_float_printfd
	_yIO_print_float_printfl
	_yIO_print_float_customf
	_yIO_print_float_customd
	_yIO_print_float_customl
)
	target_link_options(correct_function_is_chosen PRIVATE
		-Wl,--wrap=${i}
	)
endforeach()
target_link_libraries(correct_function_is_chosen PRIVATE
	yio
	yio_testlib
)
add_test(NAME correct_function_is_chosen COMMAND correct_function_is_chosen)
