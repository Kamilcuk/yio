# test/cpp/CMakeLists.txt

if(CMAKE_CXX_COMPILER_ID)
	add_executable(test_cpp_compilation test_cpp_compilation.cpp)
	target_link_libraries(test_cpp_compilation PRIVATE yio m)
	add_test(NAME test_cpp_compilation COMMAND test_cpp_compilation)
	set_tests_properties(test_cpp_compilation PROPERTIES LABELS "cpp")
endif()
