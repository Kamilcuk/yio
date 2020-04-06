cmake_minimum_required(VERSION 3.0)
list(APPEND CMAKE_MODULE_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
project(yio_tests)

include(CTest)
include(subdirlist)

if(CMAKE_COMPILER_IS_GNUCC)
	add_compile_options(
		$<$<CONFIG:DEBUG>:-Os>
		$<$<CONFIG:DEBUG>:-ggdb3>
		-Wall
		-Wextra
		-Wattribute-warning
	)
	if(0)
		add_compile_options(
			-pedantic
			-error-pedantic
			-Werror
		)
	else()
		add_compile_definitions(
			_yIO_COMPILE_PEDANTIC=0
		)
	endif()
	if(0)
		add_compile_options(
			-O0
		)
	else()
		add_compile_definitions(
			_FORTIFY_SOURCE=2
		)
	endif()
endif(CMAKE_COMPILER_IS_GNUCC)

foreach(i IN ITEMS CMAKE_BUILD_TYPE BUILD_TESTING CMAKE_C_FLAGS)
	message(STATUS "${i}=${${i}}")
endforeach()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. yio)

file(GLOB srcs *.c)

subdirlist(dirs ${CMAKE_CURRENT_SOURCE_DIR})
foreach(d IN LISTS dirs)
	if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${d}/CMakeLists.txt)
		add_subdirectory(${d})
	else()
		file(GLOB tmp ${d}/*.c)
		list(APPEND srcs ${tmp})
	endif()
endforeach()

list(REMOVE_DUPLICATES srcs)
list(FILTER srcs EXCLUDE REGEX "ywio")
# list(FILTER srcs EXCLUDE REGEX "scan")
# list(FILTER srcs INCLUDE REGEX "examples")
# list(FILTER srcs INCLUDE REGEX "simple/")
# list(FILTER srcs INCLUDE REGEX "scan.*08")

foreach(file IN LISTS srcs)
	file(RELATIVE_PATH name ${CMAKE_CURRENT_SOURCE_DIR} ${file})
	string(REPLACE "/" "_" name "${name}")
	string(REPLACE "\\" "_" name "${name}")
	string(REGEX REPLACE "\\..*$" "" name "${name}")
	string(PREPEND name "yio_")
			 
	# message(STATUS "Adding test ${name} ${file}")
	add_executable(${name} ${file})
	target_link_libraries(${name} PUBLIC yio)
	
	file(STRINGS ${file} input REGEX "^// INPUT ")
	if(NOT input)
		set(command ${name})
	else()
		string(REGEX REPLACE "^[ \t]*// INPUT " "" input "${input}")
		string(REGEX REPLACE ";[ \t]*// INPUT " "\n" input "${input}")
		set(command bash --noprofile --norc -c "exec \"$2\" <<<\"$1\"" _ ${input} $<TARGET_FILE:${name}>) 
	endif()
	add_test(NAME ${name} COMMAND ${command})

	set_tests_properties(${name} PROPERTIES TIMEOUT 5)
	
	foreach(i IN ITEMS
		PASS_REGULAR_EXPRESSION
		FAIL_REGULAR_EXPRESSION
		SKIP_REGULAR_EXPRESSION
		WILL_FAIL
	)
		file(STRINGS ${file} expr REGEX "^// ${i} ")
		if(expr)
			string(REGEX REPLACE "^[ \t]*// ${i} " "" expr "${expr}")
			if("${expr}" MATCHES  ";[ \t]*// ${i} " AND ("${expr}" MATCHES ".^" OR "${expr}" MATCHES "$."))
				message(FATAL_ERROR "Problem with item ${i} in ${file}")
			endif()
			string(REGEX REPLACE ";// ${i} " "[^\n]*\n" expr "${expr}")
			set_tests_properties(${name} PROPERTIES ${i} "${expr}")
		endif()
	endforeach()

endforeach()

 
