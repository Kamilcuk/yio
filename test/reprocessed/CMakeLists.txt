file(GLOB_RECURSE srcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c)
add_custom_target(yio_reprocessed_gen
	COMMENT "yio/test/reprocessed sources generated"
)
foreach(src IN LISTS srcs)
	set(out ${CMAKE_CURRENT_BINARY_DIR}/gen/${src})
	jinja_generate(
		TARGET yio_reprocessed_gen
		SOURCE ${src}
		OUTPUT ${out}
	)
	yio_add_test(${out} NAME yio_reprocessed_${src} REALFILE ${src} GETNAME testname)
	add_dependencies(${testname} yio_reprocessed_gen)
endforeach()

