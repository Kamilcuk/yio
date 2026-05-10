# test/sdcc/CMakeLists.txt

add_library(sdcc_conftest STATIC conftest.c)
target_include_directories(sdcc_conftest PUBLIC .)
target_link_libraries(sdcc_conftest PUBLIC yio)

if(NOT YIO_SDCC)
	target_link_libraries(sdcc_conftest PUBLIC sstest)
endif()

file(GLOB tests RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} test_*.c)

foreach(test_source IN LISTS tests)
	get_filename_component(test_name ${test_source} NAME_WE)
	add_executable(sdcc_${test_name} ${test_source})
	target_link_libraries(sdcc_${test_name} PRIVATE sdcc_conftest)
	add_test(NAME sdcc_${test_name} COMMAND sdcc_${test_name})
	add_dependencies(yio_build_tests sdcc_${test_name})
endforeach()
