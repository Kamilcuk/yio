
file(GLOB srcs *.c)
foreach(n IN LISTS srcs)
	get_filename_component(n "${n}" NAME_WE)
	add_executable(${n} EXCLUDE_FROM_ALL ${n}.c)
	target_link_libraries(${n} PRIVATE _yio_testlib)
	add_test(
		NAME ${n} 
		COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${n}
	)
	if(NOT DEFINED CMAKE_BUILD_TYPE_UPPER)
		message(FATAL_ERROR "CMAKE_BUILD_TYPE_UPPER not defined")
	endif()
	if(CMAKE_BUILD_TYPE_UPPER STREQUAL "RELEASE")
		set_property(TEST ${n} PROPERTY WILL_FAIL TRUE)
	endif()
	set_property(TEST ${n} APPEND PROPERTY LABELS compile)
	set_property(TEST ${n} APPEND PROPERTY LABELS nomemcheck)
endforeach()
