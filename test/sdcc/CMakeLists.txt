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
	add_test(NAME sdcc_${test_name} COMMAND python3 ${CMAKE_SOURCE_DIR}/scripts/s51_simulator.py -t 30 $<TARGET_FILE:sdcc_${test_name}>)
	set_tests_properties(sdcc_${test_name} PROPERTIES TIMEOUT 40)
	add_dependencies(yio_build_tests sdcc_${test_name})
endforeach()
