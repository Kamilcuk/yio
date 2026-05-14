
if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
	return()
endif()

add_executable(correct_function_is_chosen correct_function_is_chosen.c)

set(backends strfrom ryu printf naive)
set(reprs B16 B32 B64 B80 B128 D32 D64 D128)

foreach(bb IN LISTS backends)
	foreach(rr IN LISTS reprs)
		target_link_options(correct_function_is_chosen PRIVATE
			-Wl,--wrap=YIO_float_astrfrom_${bb}_${rr}
		)
	endforeach()
endforeach()

target_link_libraries(correct_function_is_chosen PRIVATE
	yio
	yio_testlib
)
add_test(NAME correct_function_is_chosen COMMAND correct_function_is_chosen)
