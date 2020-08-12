
#[[
Files from this directory are first preprocessed with cmake
and π and Ω are substituted for "", "w"/"W" or "u"/"U".
Then the files are processed with m4 with template configuration
loaded. So each one of this files is compiled 3 times and results
in 3 separate tests.
]]

execute_process(
COMMAND ${CMAKE_COMMAND} -E create_symlink
	${CMAKE_CURRENT_BINARY_DIR}/gen
	${CMAKE_CURRENT_SOURCE_DIR}/gen
)

if(_yIO_HAS_WCHAR_H)
	list(APPEND replacements "w")
endif()
if(_yIO_HAS_UCHAR_H)
	list(APPEND replacements "c16" "u")
endif()

file(GLOB_RECURSE srcs *.c *.h)
if(NOT _yIO_HAS_STDFIX_TYPES)
	list(FILTER srcs EXCLUDE REGEX "stdfix/")
endif()
if(NOT _yIO_HAS_COMPLEX)
	list(FILTER srcs EXCLUDE REGEX "complex/")
endif()
list(FILTER srcs EXCLUDE REGEX "gen/")
foreach(src IN LISTS srcs)

	# Print an erro mesasge if the file doesn't contain neither pi nor omega
	file(READ "${src}" content)
	if(NOT content MATCHES "π" AND NOT content MATCHES "Ω")
		message(FATAL_ERROR
			"File ${src} doesn't contain pi nor omega characters."
		)
	endif()
	set(content)


	foreach(replacement IN ITEMS "" ${replacements})
		file(RELATIVE_PATH src_rel_source ${CMAKE_CURRENT_SOURCE_DIR} ${src})
		set(path ${CMAKE_CURRENT_BINARY_DIR}/gen/y${replacement}io/${src_rel_source})

		# test_print_float_on_strto* have a depency, setup here
		get_filename_component(src_name "${src}" NAME)
		set(adddepends)
		if("${src_name}" MATCHES "test_print_float_on_strto*"
				AND NOT "${src_name}" STREQUAL "test_print_float_on_strto.c")
			get_filename_component(path_dir "${path}" DIRECTORY)
			list(APPEND adddepends DEPENDS "${path_dir}/test_print_float_on_strto.c")
		endif()

		yio_add_template_command(
			INPUT ${src}
			INTERMEDIATE ${path}.m4
			OUTPUT ${path}
			REPLACEMENT "_${replacement}"
			${adddepends}
		)

		file(RELATIVE_PATH name ${CMAKE_CURRENT_BINARY_DIR}/gen ${path})
		string(REPLACE "/" "_" name "${name}")
		string(REPLACE "\\" "_" name "${name}")
		string(REGEX REPLACE "\\..*$" "" name "${name}")

		yio_add_test(${name} ${path} ${src})
		set_property(TEST ${name} APPEND PROPERTY LABELS y${replacement}io)

	endforeach()
endforeach()

