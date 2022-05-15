# test/CMakeLists.txt

include(k/subdirlist)
include(k/test_extract_properties_from_file)

set(ENV{LC_ALL} "en_US.utf-8")

if(0)
	add_compile_options(
		-Wno-missing-prototypes
		-Wno-error
	)
endif()

####################################################################
# yio_add_test

# @def yio_add_test()
function(yio_add_test T_SOURCE)
	cmake_parse_arguments(T "" "NAME;REALFILE;GETNAME" "LABELS" ${ARGN})

	# unicode tests need unicode support
	if(NOT _yIO_HAS_UCHAR_H AND (T_SOURCE MATCHES "yui" OR T_SOURCE MATCHES "yc16io"))
		return()
	endif()
	# stdfix are run only if we have stdfix
	if(NOT _yIO_HAS_STDFIX_TYPES AND T_SOURCE MATCHES "stdfix")
		return()
	endif()

	if(NOT T_REALFILE)
		set(T_REALFILE ${T_SOURCE})
	endif()
	if(NOT T_NAME)
		set(T_NAME ${T_SOURCE})
	endif()
	string(REPLACE "/" "_" T_NAME "${T_NAME}")
	string(REPLACE "\\" "_" T_NAME "${T_NAME}")
	string(REGEX REPLACE "\\..*$" "" T_NAME "${T_NAME}")

	add_executable(${T_NAME} ${T_SOURCE})
	target_link_libraries(${T_NAME} PRIVATE yio_testlib m)
	add_dependencies(yio_build_tests ${T_NAME})
	add_test(NAME ${T_NAME} COMMAND ${T_NAME})
	list(APPEND T_LABELS "normal")
	set_tests_properties(${T_NAME} PROPERTIES LABELS "${T_LABELS}")
	test_extract_properties_from_file(${T_NAME} ${T_NAME} ${T_REALFILE})

	if(DEFINED T_GETNAME)
		set(${T_GETNAME} ${T_NAME} PARENT_SCOPE)
	endif()
	#set_from_env(YIO_STEST)
	#if(YIO_STEST)
		#set_property(TARGET ${name} PROPERTY EXCLUDE_FROM_ALL TRUE)
		#add_test(NAME build_${name}
			#COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${name}
		#)
		#set_property(TEST build_${name} PROPERTY FIXTURES_SETUP build_${name})
		#set_property(TEST build_${name} APPEND PROPERTY LABELS nomemcheck)
		#set_property(TEST build_${name} APPEND PROPERTY LABELS compile)
		#get_property(tmp TEST ${name} PROPERTY DISABLED)
		#set_property(TEST build_${name} PROPERTY DISABLED ${tmp})
		#set_property(TEST ${name} APPEND PROPERTY FIXTURES_REQUIRED build_${name})
	#endif()
endfunction()

#####################################################################

add_custom_target(
	yio_build_tests
	COMMENT "Yio build all tests"
)

# List all directories in current testing directory
subdirlist(dirs ${CMAKE_CURRENT_SOURCE_DIR})
# Omit cmake_example, it's for cmake project installation testing
list(FILTER dirs EXCLUDE REGEX "cmake_example")
# testlib has our small testing library
add_subdirectory(yio_testlib)
list(FILTER dirs EXCLUDE REGEX "yio_testlib")

foreach(dir IN ITEMS "." ${dirs})
	# If the directory is not current directory and it contains CMakeLists.txt file
	if(NOT "${dir}" STREQUAL "." AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/CMakeLists.txt)
		# Add it as a subdirectory.
		add_subdirectory(${dir})
	else()
		# Files in current directory are test sources
		file(GLOB tmp RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${dir}/*.c)
		foreach(ii IN LISTS tmp)
			yio_add_test("${ii}")
		endforeach()
	endif()
endforeach()

######################################################################################

if(UNIX)
	include(k/k_add_test)
	# test if there are any absolute includes in the source tree
	k_add_test(
		NAME find_absolute_includes
		COMMAND sh -c "$@ $<SEMICOLON> test $? -eq 1" _
			grep -R "^[[:blank:]]*#[[:blank:]]*include[[:blank:]]*<y" "${GENDIR}"
		LABELS nomemcheck
	)

	# Test if {{}} are properly cast to int inside #if
	k_add_test(
		NAME no_jinja2_error_inside_if
		COMMAND sh -c "$@ $<SEMICOLON> test $? -eq 1" _
			grep -ER "#.*if.*(False|True)" "${GENDIR}"
		LABELS nomemcheck
	)

	k_add_test(
		NAME no_omega_nor_pi_in_generated_files
		COMMAND sh -c "$@ $<SEMICOLON> test $? -eq 1" _
			grep -ER "Ω|π" "${GENDIR}"
		LABELS nomemcheck
	)

	k_add_test(
		NAME no_calls_to_dbgln
		COMMAND
      sh -xc "find . -type f -name '*.[hc]' '!' -name 'yio_private.h' | \"$@\"" _
      xargs -t awk "/dbgln/ && !/\\/\\/.*dbgln/{print \"\\n\"FILENAME\":\"FNR\": \" $0 $<SEMICOLON> exit(1)}"
    WORKING_DIRECTORY "${GENDIR}"
		LABELS nomemcheck
	)

endif()

###############################################################################

set_from_env(TESTPROGRAM)
if(DEFINED TESTPROGRAM AND NOT TESTPOGRAM STREQUAL "")
  set(source "\
#include <yio_test.h>
#include <locale.h>
int main(int argc, char *argv[]) {
  ${TESTPROGRAM}
}
")
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/yio_testprogram.c "${source}")
  message(STATUS "COMPILING:
${source}")
  add_executable(yio_testprogram EXCLUDE_FROM_ALL ${CMAKE_CURRENT_BINARY_DIR}/yio_testprogram.c)
  target_link_libraries(yio_testprogram PRIVATE yio_testlib)
  add_test(NAME yio_testprogram COMMAND yio_testprogram)
endif()

