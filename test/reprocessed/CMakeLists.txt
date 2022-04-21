file(GLOB_RECURSE srcs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.c)
foreach(src IN LISTS srcs)
	set(out ${CMAKE_CURRENT_BINARY_DIR}/gen/${src})
	jinja_generate(
		SOURCE ${src}
		OUTPUT ${out}
	)
	yio_add_test(${out} NAME yio_reprocessed_${src} REALFILE ${src})
endforeach()

