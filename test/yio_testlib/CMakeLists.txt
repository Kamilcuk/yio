project(yio_testlib LANGUAGES C)
set(yio_test_gen ${CMAKE_CURRENT_BINARY_DIR}/gen/yio_test_float.h)
jinja_generate(
	SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/yio_test_float.h
	OUTPUT "${yio_test_gen}"
)
add_custom_target(yio_testlib_gen
	COMMENT "yio_testlib generated"
	DEPENDS "${yio_test_gen}"
)
add_library(${PROJECT_NAME}
	yio_test.c
	yio_test.h
	yio_test_private.h
	"${yio_test_gen}"
)
add_dependencies(${PROJECT_NAME} yio_testlib_gen)
target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_BINARY_DIR}/gen
	${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME} PUBLIC
	yio
	m
)
find_library(DFP dfp)
if(DFP)
	target_link_libraries(${PROJECT_NAME} PUBLIC
		${DFP}
	)
#else()
	#target_link_libraries(${PROJECT_NAME} PUBLIC
		#yio_compat
	#)
endif()
