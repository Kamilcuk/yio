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
add_library(${PROJECT_NAME} OBJECT
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
)
if(NOT CMAKE_C_COMPILER_ID STREQUAL "SDCC")
	target_link_libraries(${PROJECT_NAME} PUBLIC m sstest)
endif()
find_library(DFP
	NAMES dfp
	DOC "Path to decimal floating point library"
)
target_link_libraries(${PROJECT_NAME} PUBLIC
	yio_compat
)
target_compile_definitions(${PROJECT_NAME} INTERFACE EXIT_SKIP=77)
if(DFP OR TARGET dfp)
  target_compile_definitions(${PROJECT_NAME} PUBLIC YIO_HAS_LIBDFP=1)
  if(DFP)
    target_link_libraries(${PROJECT_NAME} INTERFACE
		  ${DFP}
	  )
  endif()
endif()
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|IntelLLVM")
	target_compile_options(${PROJECT_NAME} PUBLIC -Wno-missing-field-initializers)
endif()
