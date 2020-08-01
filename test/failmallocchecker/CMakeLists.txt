
#[[
This is a small program that first counts all possible calls to
malloc of a particular function. Afte the count is done, then 
the library makes every single each call to malloc fail while 
looping over a function. It is meant to be used to test every possible
allocation failure.
]]

if(CMAKE_CROSSCOMPILING)
	return()
endif()
if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
	return()
endif()

add_library(failmallocchecker OBJECT lib/failmallocchecker.c)
target_include_directories(failmallocchecker PUBLIC lib)
target_link_options(failmallocchecker PUBLIC -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc)
add_library(fmc INTERFACE)
add_dependencies(fmc INTERFACE failmallocchecker)

file(GLOB srcs *.c)
foreach(src IN LISTS srcs)
	get_filename_component(name ${src} NAME_WE)
	add_executable(${name} ${src})
	target_link_libraries(${name} PRIVATE failmallocchecker yio _yio_testlib)
	add_test(NAME ${name} COMMAND ${name})
	set_tests_properties(${name} PROPERTIES LABELS normal)
	set_tests_properties(${name} PROPERTIES LABELS nomemcheck)
endforeach()
