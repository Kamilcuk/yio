project(_yio_testlib LANGUAGES C)
set(yio_test_gen ${CMAKE_CURRENT_BINARY_DIR}/gen/yio_test_float.h)
set(yio_test_gen "${yio_test_gen}" PARENT_SCOPE)
m4_add_command(
	SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/yio_test_float.h
	OUTPUT "${yio_test_gen}"
)
add_library(${PROJECT_NAME} 
	yio_test.c
	yio_test.h
	yio_test_private.h
	"${yio_test_gen}"
)
target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_BINARY_DIR}/gen
	${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME} PUBLIC
	yio
)
if(UNISTRING_LIB)
	target_link_libraries(${PROJECT_NAME} PUBLIC
		yuio
	)
endif()
