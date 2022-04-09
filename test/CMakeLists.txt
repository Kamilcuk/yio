# test/CMakeLists.txt
cmake_minimum_required(VERSION 3.12)

if(NOT "${PROJECT_NAME}" STREQUAL "yio")
	message(FATAL_ERROR "DO NOT INCLUDE THIS FILE BY ITS OWN: PROJECT_NAME=${PROJECT_NAME}")
endif()

include(k/subdirlist)
include(k/test_extract_properties_from_file)

set(ENV{LC_ALL} "en_US.utf-8")

if(YIO_DEV)
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		add_compile_options(
			-Wno-missing-prototypes
			-Wno-error
		)
	endif()
endif()

####################################################################
# yio_add_test

set(YIO_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# @def yio_add_test(name file [propertiesfile])
# add_executable from file
# With the name name
# then also add the name name
# Then read properties of the test from propertiesfile
function(yio_add_test name file)

	# unicode tests need unicode support
	if(NOT _yIO_HAS_UCHAR_H AND (file MATCHES "yui" OR file MATCHES "yc16io"))
    return()
	endif()
	# stdfix are run only if we have stdfix
	if(NOT _yIO_HAS_STDFIX_TYPES AND file MATCHES "stdfix")
    return()
	endif()

  string(REPLACE "/" "_" name "${name}")
  string(REPLACE "\\" "_" name "${name}")
  string(REGEX REPLACE "\\..*$" "" name "${name}")

	# message(STATUS "Adding test ${name} ${file}")
	add_executable(${name} ${file})
	target_link_libraries(${name} PRIVATE yio_testlib m)
	add_dependencies(build_tests ${name})

	set(realfile "${file}")
	if(ARGC GREATER_EQUAL 3)
		set(realfile "${ARGV2}")
	endif()

	if(NOT realfile STREQUAL "")
		add_test(NAME ${name} COMMAND ${name})
		set_tests_properties(${name} PROPERTIES LABELS normal)
		set_property(TEST ${name} APPEND PROPERTY TIMEOUT 10)
		test_extract_properties_from_file(${name} ${name} ${realfile})
	else()
		add_test(NAME ${name} COMMAND ${name})
		set_tests_properties(${name} PROPERTIES LABELS normal)
	endif()

	set_from_env(YIO_STEST)
	if(YIO_STEST)
		set_property(TARGET ${name} PROPERTY EXCLUDE_FROM_ALL TRUE)

		add_test(NAME build_${name}
			COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${name}
		)
		set_property(TEST build_${name} PROPERTY FIXTURES_SETUP build_${name})
		set_property(TEST build_${name} APPEND PROPERTY LABELS nomemcheck)
		set_property(TEST build_${name} APPEND PROPERTY LABELS compile)
		get_property(tmp TEST ${name} PROPERTY DISABLED)
		set_property(TEST build_${name} PROPERTY DISABLED ${tmp})

		set_property(TEST ${name} APPEND PROPERTY FIXTURES_REQUIRED build_${name})
	endif()
endfunction()

#####################################################################

add_custom_target(
	build_tests
	COMMENT "Yio tests build"
)

# Detecting sources #################################################
# List all directories in current testing directory
subdirlist(dirs ${CMAKE_CURRENT_SOURCE_DIR})

# omit cmake_example, it's for cmake project installation testing
list(FILTER dirs EXCLUDE REGEX "cmake_example")

add_subdirectory(yio_testlib)
list(FILTER dirs EXCLUDE REGEX "yio_testlib")

# Create a list of sources
set(_sources "")
foreach(dir IN ITEMS "." ${dirs})
	# If the directory is not current directory and it contains CMakeLists.txt file
	if(NOT "${dir}" STREQUAL "." AND
			 EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/CMakeLists.txt)
		# Add it as a subdirectory.
		add_subdirectory(${dir})
	else()
		# Files in current directory are test sources
		file(GLOB tmp ${dir}/*.c)
		foreach(ii IN LISTS tmp)
			get_filename_component(ii "${ii}" ABSOLUTE)
			list(APPEND _sources "${ii}")
		endforeach()
	endif()
endforeach()

######################################################################################
# Create tests

list(REMOVE_DUPLICATES _sources)
list(SORT _sources)
list(FILTER _sources EXCLUDE REGEX "ywio") # these are not working yet
# list(FILTER _sources INCLUDE REGEX "examples")
# list(FILTER _sources INCLUDE REGEX "simple/")

foreach(source IN LISTS _sources)
	file(RELATIVE_PATH sourcenice "${CMAKE_CURRENT_SOURCE_DIR}" "${source}")
	yio_add_test("${sourcenice}" "${source}")
endforeach()

######################################################################################

# test if there are any absolute includes in the source tree
add_test(NAME find_absolute_includes
	COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/absolute_includes_find.sh ${GENDIR}
)
set_property(TEST find_absolute_includes APPEND PROPERTY LABELS nomemcheck)


