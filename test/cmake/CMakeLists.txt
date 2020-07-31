
# test cmake too!
add_test(NAME yio_test_cmake
	COMMAND ${CMAKE_COMMAND} 
		-D CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}
		-P ${CMAKE_CURRENT_SOURCE_DIR}/test.cmake
)
set_property(TEST yio_test_cmake APPEND PROPERTY LABELS nomemcheck)
