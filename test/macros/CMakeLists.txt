
if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
	return()
endif()

# Little hacky include paths finding, but works as-is

get_target_property(yioincludes yio INTERFACE_INCLUDE_DIRECTORIES)
set(includeargs)
foreach(i IN LISTS yioincludes)
	list(APPEND includeargs -I ${i})
endforeach()

file(GLOB srcs *.pp)

foreach(f IN LISTS srcs)
	get_filename_component(name ${f} NAME)
	add_test(
		NAME ${name}
		COMMAND ${CMAKE_C_COMPILER} -xc -E 
			${includeargs}
			-I ${CMAKE_CURRENT_SOURCE_DIR}/../yio_testlib
			-include yio_test_private.h ${f}
	)
	test_extract_properties_from_file(${name} ${f})
	set_tests_properties(${name} PROPERTIES LABELS nomemcheck)
endforeach()   