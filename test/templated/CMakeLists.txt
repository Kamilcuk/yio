
execute_process(
COMMAND ${CMAKE_COMMAND} -E create_symlink
	${CMAKE_CURRENT_BINARY_DIR}/gen
	${CMAKE_CURRENT_SOURCE_DIR}/gen
)

if(YIO_DEV)
  # Target for clangd
  file(GLOB_RECURSE srcs *.c *.h)
  list(FILTER srcs EXCLUDE REGEX "^gen/.*")
  yio_cdt_parser(TEMPLATED_CDT_PARSER ${srcs})
  target_link_libraries(TEMPLATED_CDT_PARSER PUBLIC yio_testlib)
endif()


set(modes yio)
if(_yIO_HAS_WCHAR_H)
	list(APPEND modes "ywio")
endif()
if(_yIO_HAS_UCHAR_H)
	list(APPEND modes "yc16io" "yuio")
endif()

file(GLOB_RECURSE srcs RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.c *.h)
if(NOT _yIO_HAS_STDFIX_TYPES)
	list(FILTER srcs EXCLUDE REGEX "stdfix/")
endif()
if(NOT _yIO_HAS_COMPLEX)
	list(FILTER srcs EXCLUDE REGEX "complex/")
endif()
list(FILTER srcs EXCLUDE REGEX "gen/")


foreach(src IN LISTS srcs)

	# Print an erro mesasge if the file doesn't contain neither pi nor omega
	file(READ "${src}" _content)
	if(NOT _content MATCHES "π" AND NOT _content MATCHES "Ω")
		message(FATAL_ERROR "File ${src} doesn't contain pi nor omega characters.")
	endif()

	foreach(mode IN ITEMS ${modes})
		set(genout ${CMAKE_CURRENT_BINARY_DIR}/gen/${mode}/${src})
		jinja_generate(
			SOURCE ${src}
			OUTPUT ${genout}
			--mode ${mode}
		)
		yio_add_test(${genout} NAME ${mode}/${src} REALFILE ${src} GETNAME testname LABELS ${mode})
		if(mode STREQUAL "ywio" AND src MATCHES "simple/yio_test_other.c")
			set_tests_properties(${testname} PROPERTIES LABELS nomemcheck)
		endif()
	endforeach()
endforeach()

