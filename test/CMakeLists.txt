cmake_minimum_required(VERSION 3.0)
list(APPEND CMAKE_MODULE_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
project(yio_tests LANGUAGES C)

include(CTest)
include(subdirlist)
include(test_extract_properties_from_file)
include(debug_variables)

# m4 ###################################################################
# Initialize m4 preprocessor the same way as in ../src/CMakeLists.txt
include(m4)
set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/../src)
set(GENDIR ${CMAKE_CURRENT_SOURCE_DIR}/../gen)
m4_add_options(lib.m4)
m4_add_include_directories(${SRCDIR} ${GENDIR}/m4)
m4_add_file_dependencies(${GENDIR}/m4/yio_config.m4)

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
endif(CMAKE_COMPILER_IS_GNUCC)

# Some output
debug_variables("yio_test:" CMAKE_BUILD_TYPE BUILD_TESTING CMAKE_C_FLAGS)

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
endforeach()

# test cmake too!
add_test(NAME yio_test_cmake
	COMMAND ${CMAKE_COMMAND} -P test/test.cmake
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
)


