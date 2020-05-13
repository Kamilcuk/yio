cmake_minimum_required(VERSION 3.1)
project(yio_test LANGUAGES C)
find_package(yio HINTS 
		../../_build/Debug/mytest/ 
		../../_build/Release/mytest/
	REQUIRED
)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PUBLIC yio::yio)
