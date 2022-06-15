project(yio_testlib LANGUAGES C)
set(yio_test_gen ${CMAKE_CURRENT_BINARY_DIR}/gen/yio_test_float.h)
add_custom_target(yio_testlib_gen
	COMMENT "yio_testlib generated"
)
jinja_generate(
	TARGET yio_testlib_gen
	SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/yio_test_float.h
	OUTPUT "${yio_test_gen}"
)
add_library(${PROJECT_NAME} INTERFACE
	yio_test.h
	yio_test_private.h
	"${yio_test_gen}"
)
add_dependencies(${PROJECT_NAME} yio_testlib_gen)
target_include_directories(${PROJECT_NAME} INTERFACE
	${CMAKE_CURRENT_BINARY_DIR}/gen
	${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME} INTERFACE
	yio
	m
	tiny-regex-c
  sstest
)
find_library(DFP
	NAMES dfp
	DOC "Path to decimal floating point library"
)
if(DFP)
  target_link_libraries(${PROJECT_NAME} INTERFACE
		${DFP}
	)
#else()
	#target_link_libraries(${PROJECT_NAME} PUBLIC
		#yio_compat
	#)
endif()
if(CMAKE_COMPILER_IS_GNUCC)
  target_compile_options(${PROJECT_NAME} INTERFACE
    -Wno-missing-field-initializers
  )
endif()
