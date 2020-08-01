
if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
	return()
endif()
return()

# TODO
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
		COMMAND /bin/sh -c "\"$@\" | grep -A9999 \"MARK 75df98e7-3dfb-4e4e-8b79-561eb4001923\"" _
			${CMAKE_C_COMPILER} -xc -E ${includeargs}
			-I ${CMAKE_CURRENT_SOURCE_DIR}/../yio_testlib
			${f}
	)
	test_extract_properties_from_file(${name} ${f})
	set_property(TESTS ${name} APPEND PROPERTY LABELS nomemcheck)
	set_property(TESTS ${name} APPEND PROPERTY LABELS cpp)
endforeach()

