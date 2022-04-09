
if(CMAKE_CROSSCOMPILING OR NOT UNIX)
	return()
endif()

add_library(failmallocchecker OBJECT lib/failmallocchecker.c)
target_include_directories(failmallocchecker PUBLIC lib)
target_compile_definitions(failmallocchecker PRIVATE
	FMC_USE_METHOD=FMC_METHOD_WRAP_MALLOC
)
target_link_options(failmallocchecker PUBLIC
	-Wl,--wrap=malloc
	-Wl,--wrap=calloc
	-Wl,--wrap=realloc
)
add_library(fmc INTERFACE)
add_dependencies(fmc INTERFACE failmallocchecker)

file(GLOB srcs *.c)
foreach(src IN LISTS srcs)
	get_filename_component(name ${src} NAME_WE)
	add_executable(${name} ${src})
	target_link_libraries(${name} PRIVATE failmallocchecker yio yio_testlib)
	add_test(NAME ${name} COMMAND ${name})
	set_tests_properties(${name} PROPERTIES LABELS normal)
	set_tests_properties(${name} PROPERTIES LABELS nomemcheck)
endforeach()
