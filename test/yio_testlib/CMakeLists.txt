project(_yio_testlib LANGUAGES C)
add_library(${PROJECT_NAME} 
	yio_test.c
	yio_test.h
	yio_test_private.h
)
target_include_directories(${PROJECT_NAME} PUBLIC
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
