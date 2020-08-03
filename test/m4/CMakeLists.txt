
if(CMAKE_CROSSCOMPILING)
	return()
endif()

list(APPEND CMAKE_MODULE_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
include(test_extract_properties_from_file)
include(m4)

m4_add_options()
m4_add_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB srcs ${CMAKE_CURRENT_SOURCE_DIR}/../../m4/lib/*.m4)
foreach(file IN LISTS srcs)
	get_filename_component(name_we ${file} NAME_WE)
	set(name "${name_we}")
	string(PREPEND name "yio_m4_")
	
	add_test(NAME ${name}
		COMMAND
			${M4_COMMAND}
			${_M4_OPTIONS_INIT}
			-D "m4_SOURCE=${file}"
			${_M4_FILES_INIT}
			-D "m4_TESTFILE=${file}"
			${CMAKE_CURRENT_SOURCE_DIR}/testit.m4
	)
	set_property(TEST ${name} PROPERTY TIMEOUT 1)
	test_extract_properties_from_file(${name} ${file})
	set_property(TEST ${name} APPEND PROPERTY LABELS nomemcheck)
	set_property(TEST ${name} APPEND PROPERTY LABELS m4)
endforeach()
