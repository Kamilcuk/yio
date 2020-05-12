project(yio_m4_tests)

list(APPEND CMAKE_MODULE_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
include(test_extract_properties_from_file)
include(m4)

m4_add_options(-P lib.m4)
m4_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
m4_get_command(m4cmd)

file(GLOB srcs ${CMAKE_CURRENT_SOURCE_DIR}/*.m4)
foreach(file IN LISTS srcs)
	get_filename_component(name ${file} NAME_WE)
	string(PREPEND name "yio_m4_")
	
	add_test(NAME ${name}
		COMMAND ${m4cmd} ${file}
	)
	set_tests_properties(${name} PROPERTIES TIMEOUT 1)
	test_extract_properties_from_file(${name} ${file})	
endforeach()