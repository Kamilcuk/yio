
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

file(GLOB_RECURSE srcs RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.c *.h)
if(NOT YYIO_HAS_STDFIX_TYPES)
	#list(FILTER srcs EXCLUDE REGEX "stdfix/")
endif()
if(NOT YYIO_HAS_COMPLEX)
	list(FILTER srcs EXCLUDE REGEX "complex/")
endif()
list(FILTER srcs EXCLUDE REGEX "gen/")


add_custom_target(yio_templated_gen
	COMMENT "yio/tests/templated generated"
)

foreach(src IN LISTS srcs)
	set(genout ${CMAKE_CURRENT_BINARY_DIR}/gen/${src})
	jinja_generate(
		TARGET yio_templated_gen
		SOURCE ${src}
		OUTPUT ${genout}
	)
	yio_add_test(${genout} NAME yio/${src} REALFILE ${src} GETNAME testname LABELS yio)
	add_dependencies(${testname} yio_templated_gen)
	target_link_libraries(${testname} PRIVATE tiny-regex-c)
endforeach()

