# test/CMakeLists.txt
cmake_minimum_required(VERSION 3.12)

if(NOT "${PROJECT_NAME}" STREQUAL "yio")
	message(FATAL_ERROR "DO NOT INCLUDE THIS FILE BY ITS OWN: PROJECT_NAME=${PROJECT_NAME}")
endif()

include(subdirlist)
include(test_extract_properties_from_file)

# m4 ###################################################################
# Initialize m4 preprocessor the same way as in ../src/CMakeLists.txt
include(m4)
m4_add_options(lib.m4)
m4_add_include_directories(${SRCDIR})

# Compiler options #######################################################
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
			-O0
		)
	else()
		add_compile_definitions(
			_FORTIFY_SOURCE=2
		)
	endif()
endif()

#####################################################################

add_custom_target(build_tests)

# Detecting sources #################################################
# List all directories in current testing directory
subdirlist(dirs ${CMAKE_CURRENT_SOURCE_DIR})

# omit cmake_example, it's for cmake project installation testing
list(FILTER dirs EXCLUDE REGEX "cmake_example")

# Create a list of sources
set(srcs "")
foreach(d IN ITEMS "." ${dirs})

	# If the directory is not current directory and it contains CMakeLists.txt file
	if(NOT "${d}" STREQUAL "." AND
			 EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${d}/CMakeLists.txt)
		# Add it as a subdirectory.
		add_subdirectory(${d})
	else()
		# Files in current directory are processed here too.
		file(GLOB tmp ${d}/*.c)
		foreach(source IN LISTS	tmp)
			# Create path relative to current source dir that is relative to binary dir.
			file(RELATIVE_PATH sourcenice ${CMAKE_CURRENT_SOURCE_DIR} ${source})
			set(output ${CMAKE_CURRENT_BINARY_DIR}/${sourcenice})

			if(${source} MATCHES "\\.m4\\.c$")
				# Files that are .m4.c are preprocessed by m4 preprocessor.
				# Note: this is _before_ detecting PASS_REGULAR_EXPRESSION* in files.
				# So it has to be done on configuration stage
				if(NOT EXISTS ${output} OR ${source} IS_NEWER_THAN ${output})
					m4_execute_process(${output} ${source})
				endif()
			else()
				# *All* files are copied to CMAKE_CURRENT_BINARY_DIR.
				get_filename_component(output_dir ${output} DIRECTORY)
				file(COPY ${source} DESTINATION ${output_dir})
			endif()

			list(APPEND srcs ${output})
		endforeach()
	endif()
endforeach()

list(REMOVE_DUPLICATES srcs)
list(FILTER srcs EXCLUDE REGEX "ywio")
# list(FILTER srcs EXCLUDE REGEX "scan")
# list(FILTER srcs INCLUDE REGEX "examples")
# list(FILTER srcs INCLUDE REGEX "simple/")
# list(FILTER srcs INCLUDE REGEX "scan.*08")

######################################################################################
# Create tests
foreach(file IN LISTS srcs)
	file(RELATIVE_PATH name ${CMAKE_CURRENT_BINARY_DIR} ${file})
	string(REPLACE "/" "_" name "${name}")
	string(REPLACE "\\" "_" name "${name}")
	string(REGEX REPLACE "\\..*$" "" name "${name}")

	# message(STATUS "Adding test ${name} ${file}")
	add_executable(${name} ${file})
	target_link_libraries(${name} PRIVATE _yio_testlib m)
	add_dependencies(build_tests ${name})

	file(STRINGS ${file} input REGEX "^// INPUT ")
	if(NOT input)
		add_test(NAME ${name} COMMAND ${name})
		set_tests_properties(${name} PROPERTIES LABELS normal)
	else()
		string(REGEX REPLACE "^[ \t]*// INPUT " "" input "${input}")
		string(REGEX REPLACE ";[ \t]*// INPUT " "\n" input "${input}")
		add_test(NAME ${name}
			# COMMAND bash --noprofile --norc -c "exec \"$2\" <<<\"$1\"" _ ${input} $<TARGET_FILE:${name}>
			COMMAND ${CMAKE_CURRENT_BINARY_DIR}/redirectstdin/src/redirectstdin-build/redirectstdin
					${input} ${CMAKE_CROSSCOMPILING_EMULATOR} $<TARGET_FILE:${name}>
		)
		# set_property(TEST ${name} APPEND PROPERTY LABELS nomemcheck)
		set_property(TEST ${name} APPEND PROPERTY FIXTURES_REQUIRED redirectstdin)
		set_property(TEST ${name} APPEND PROPERTY LABELS redirectedstdin)
	endif()
	set_property(TEST ${name} APPEND PROPERTY TIMEOUT 5)

	test_extract_properties_from_file(${name} ${file})

	if($ENV{YIO_BUILD_TESTING})
		set_property(TARGET ${name} PROPERTY EXCLUDE_FROM_ALL TRUE)

		add_test(NAME build_${name}
			COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${name}
		)
		set_property(TEST build_${name} PROPERTY FIXTURES_SETUP build_${name})
		set_property(TEST build_${name} APPEND PROPERTY LABELS nomemcheck)
		set_property(TEST build_${name} APPEND PROPERTY LABELS compile)

		set_property(TEST ${name} APPEND PROPERTY FIXTURES_REQUIRED build_${name})
	endif()

endforeach()

# test cmake too!
add_test(NAME yio_test_cmake
	COMMAND ${CMAKE_COMMAND} -P test/test.cmake
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
)
set_property(TEST yio_test_cmake APPEND PROPERTY LABELS nomemcheck)

# test if there are any absolute includes in the source tree
add_test(NAME find_absolute_includes
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/absolute_includes_find.sh ${GENDIR}
)


