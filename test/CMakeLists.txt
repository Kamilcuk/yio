# test/CMakeLists.txt
cmake_minimum_required(VERSION 3.12)

if(NOT "${PROJECT_NAME}" STREQUAL "yio")
	message(FATAL_ERROR "DO NOT INCLUDE THIS FILE BY ITS OWN: PROJECT_NAME=${PROJECT_NAME}")
endif()

include(subdirlist)
include(test_extract_properties_from_file)

set(ENV{LC_ALL} "en_US.utf-8")

####################################################################
# yio_add_test

set(YIO_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# @def yio_add_test(name file [propertiesfile])
# add_executable from file
# With the name name
# then also add the name name
# Then read properties of the test from propertiesfile
function(yio_add_test name file)
	# message(STATUS "Adding test ${name} ${file}")
	add_executable(${name} ${file})
	target_link_libraries(${name} PRIVATE _yio_testlib m)
	add_dependencies(build_tests ${name})

	set(realfile "${file}")
	if(ARGC GREATER_EQUAL 3)
		set(realfile "${ARGV2}")
	endif()

	if(NOT realfile STREQUAL "")
		file(STRINGS ${realfile} input REGEX "^// INPUT ")
		
		if(NOT input)
			add_test(NAME ${name} COMMAND ${name})
			set_tests_properties(${name} PROPERTIES LABELS normal)
		else()
			string(REGEX REPLACE "^[ \t]*// INPUT " "" input "${input}")
			string(REGEX REPLACE ";[ \t]*// INPUT " "\n" input "${input}")
			add_test(NAME ${name}
				COMMAND ${CMAKE_CURRENT_BINARY_DIR}/redirectstdin/src/redirectstdin-build/redirectstdin
						${input} ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:${name}>
			)
			set_property(TEST ${name} APPEND PROPERTY FIXTURES_REQUIRED redirectstdin)
			set_property(TEST ${name} APPEND PROPERTY LABELS redirectedstdin)
		endif()
		set_property(TEST ${name} APPEND PROPERTY TIMEOUT 5)
		
		test_extract_properties_from_file(${name} ${realfile})
	else()
		add_test(NAME ${name} COMMAND ${name})
		set_tests_properties(${name} PROPERTIES LABELS normal)
	endif()

	if($ENV{YIO_TEST_BUILDING})
		set_property(TARGET ${name} PROPERTY EXCLUDE_FROM_ALL TRUE)

		add_test(NAME build_${name}
			COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${name}
		)
		set_property(TEST build_${name} PROPERTY FIXTURES_SETUP build_${name})
		set_property(TEST build_${name} APPEND PROPERTY LABELS nomemcheck)
		set_property(TEST build_${name} APPEND PROPERTY LABELS compile)

		set_property(TEST ${name} APPEND PROPERTY FIXTURES_REQUIRED build_${name})
	endif()
endfunction()

#####################################################################

add_custom_target(build_tests)

# Detecting sources #################################################
# List all directories in current testing directory
subdirlist(dirs ${CMAKE_CURRENT_SOURCE_DIR})

# omit cmake_example, it's for cmake project installation testing
list(FILTER dirs EXCLUDE REGEX "cmake_example")

# Create a list of sources
set(sources "")
foreach(dir IN ITEMS "." ${dirs})
	# If the directory is not current directory and it contains CMakeLists.txt file
	if(NOT "${dir}" STREQUAL "." AND
			 EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/CMakeLists.txt)
		# Add it as a subdirectory.
		add_subdirectory(${dir})
	else()
		# Files in current directory are test sources
		file(GLOB tmp ${dir}/*.c)
		foreach(i IN LISTS tmp)
			get_filename_component(i "${i}" ABSOLUTE)
			list(APPEND sources "${i}")
		endforeach()
	endif()
endforeach()

######################################################################################
# Create tests

list(REMOVE_DUPLICATES sources)
list(SORT sources)
list(FILTER sources EXCLUDE REGEX "ywio")
# list(FILTER sources EXCLUDE REGEX "scan")
# list(FILTER sources INCLUDE REGEX "examples")
# list(FILTER sources INCLUDE REGEX "simple/")
# list(FILTER sources INCLUDE REGEX "scan.*08")

foreach(source IN LISTS sources)
	file(RELATIVE_PATH sourcenice "${CMAKE_CURRENT_SOURCE_DIR}" "${source}")
	
	m4_add_command(
		OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${sourcenice}
		SOURCE ${source}
	)
	
	set(name "${sourcenice}")
	string(REPLACE "/" "_" name "${name}")
	string(REPLACE "\\" "_" name "${name}")
	string(REGEX REPLACE "\\..*$" "" name "${name}")

	yio_add_test("${name}" "${CMAKE_CURRENT_BINARY_DIR}/${sourcenice}" "${source}")
endforeach()

######################################################################################

# test if there are any absolute includes in the source tree
add_test(NAME find_absolute_includes
	COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/absolute_includes_find.sh ${GENDIR}
)
set_property(TEST find_absolute_includes APPEND PROPERTY LABELS nomemcheck)


