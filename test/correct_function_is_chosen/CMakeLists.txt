
if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
	return()
endif()

add_executable(correct_function_is_chosen correct_function_is_chosen.c)
foreach(i IN ITEMS
	YYIO_print_float_strfromf
	YYIO_print_float_strfromd
	YYIO_print_float_strfroml
	YYIO_print_float_printff
	YYIO_print_float_printfd
	YYIO_print_float_printfl
	YYIO_print_float_customf
	YYIO_print_float_customd
	YYIO_print_float_customl
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
