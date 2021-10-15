include_guard()

set(YIO_TEMPLATE_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "")

function(yio_add_template_command)
	cmake_parse_arguments(ARG
			""
			"REPLACEMENT;INPUT;OUTPUT;INTERMEDIATE"
			"OPTIONS;DEPENDS"
			${ARGV}
	)
	foreach(i IN ITEMS REPLACEMENT INPUT OUTPUT INTERMEDIATE)
		set(i "ARG_${i}")
		if(NOT DEFINED "${i}")
			message(FATAL_ERROR "Wrong argument to yio_add_template_command - ${i}=${${i}} is unset")
		endif()
	endforeach()
	if(NOT DEFINED YIO_TEMPLATE_DIR)
		message(FATAL_ERROR "YIO_TEMPLATE_DIR not defined")
	endif()
	if(NOT EXISTS "${YIO_TEMPLATE_DIR}")
		message(FATAL_ERROR "YIO_TEMPLATE_DIR=${YIO_TEMPLATE_DIR} does not exists")
	endif()

	get_filename_component(outfile "${ARG_OUTPUT}" ABSOLUTE)
	get_filename_component(infile "${ARG_INPUT}" ABSOLUTE)
	get_filename_component(intermediate "${ARG_INTERMEDIATE}" ABSOLUTE)
	set(REPLACEMENT "${ARG_REPLACEMENT}")
	# a little trick for allowing to pass zero arguments - add "_" in front of it
	string(REGEX REPLACE "_" "" REPLACEMENT "${REPLACEMENT}")

	set(config ${SRCDIR}/yio/y${REPLACEMENT}io/template.m4)

	if(NOT EXISTS "${config}")
		message(FATAL_ERROR "${config} file does not exists")
	endif()

	file(RELATIVE_PATH outfile_rel_bindir "${CMAKE_CURRENT_BINARY_DIR}" "${outfile}")
	file(RELATIVE_PATH infile_rel_srcdir "${CMAKE_CURRENT_SOURCE_DIR}" "${infile}")

	file(RELATIVE_PATH intermediate_rel_bindir "${CMAKE_CURRENT_BINARY_DIR}" "${intermediate}")

	#message(DEBUG "--\ninfile=\t${infile}\nintermediate=\t${intermediate}\noutfile=\t${outfile}")

	string(TOUPPER "${REPLACEMENT}" REPLACEMENT_UPPER)
	m4_add_command(
		OUTPUT
			${intermediate}
		SOURCE
			${infile}
		OPTIONS
			-D m4_SOURCE=${infile}
			-D m4_TEMPLATE_SOURCE=${infile}
			-D m4_CONFIG=${config}
			${ARG_OPTIONS}
	)

	add_custom_command(
		OUTPUT
			${outfile}
		DEPENDS
			${intermediate}
			${CMAKE_CURRENT_LIST_FILE}
			${YIO_TEMPLATE_DIR}/yio_template_replace.cmake
			${ARG_DEPENDS}
		COMMAND ${CMAKE_COMMAND}
			-D INPUT=${intermediate}
			-D OUTPUT=${outfile}
			-D REPLACEMENT=${REPLACEMENT}
			-P ${YIO_TEMPLATE_DIR}/yio_template_replace.cmake
		COMMENT "yio_template: Templating with Ω=${REPLACEMENT_UPPER} ${infile_rel_srcdir} from ${intermediate_rel_bindir}"
	)

endfunction()

macro(yio_template_in infile relative)
	get_filename_component(infile_abs "${infile}" ABSOLUTE)
	file(RELATIVE_PATH infile_rel ${relative} ${infile})
	set(out "yio/${name}/${infile_rel}")

	if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${infile_rel} AND EXISTS ${YIO_TEMPLATE_DIR}/${infile_rel})
		message(FATAL_ERROR
			"yio_template.cmake: ERROR: Duplicate template files found"
			" in both the template/ and y*io directory. Only one can exists."
			" Files: "
			"${CMAKE_CURRENT_LIST_DIR}/${infile_rel}"
			" and "
			"${YIO_TEMPLATE_DIR}/${infile_rel}\n"
		)
	endif()

	if (infile MATCHES "\.c$")
		# GNU ar static library archiver does not distinguishes from file paths.
		# It only sees file names.
		# Make sure that _source_ files have distnct names - by adding a prefix to them,
		# so that GNU ar sees different source files.
		get_filename_component(out_name "${infile}" NAME)
		get_filename_component(out_dir "${out}" DIRECTORY)
		set(out "${out_dir}/${REPLACEMENT}_${out_name}")
	endif()

	yio_add_template_command(
		REPLACEMENT "_${REPLACEMENT}"
		INPUT "${infile}"
		INTERMEDIATE "${CMAKE_CURRENT_BINARY_DIR}/template/${out}"
		OUTPUT "${GENDIR}/${out}"
	)

	list(APPEND srcs ${out})
endmacro()

macro(_yio_handle_float_in infile relative)
	get_filename_component(infile_abs "${infile}" ABSOLUTE)
	file(RELATIVE_PATH infile_rel ${relative} ${infile})
	get_filename_component(infile_rel_dir ${infile_rel} DIRECTORY)
	get_filename_component(out_name "${infile}" NAME)
	foreach(ss IN LISTS YIO_FLOAT_SUFFIXES)
		# Flaot is promoted to double
		#if (ss STREQUAL "f")
			#continue()
		#endif()
		set(out "yio/${name}/${infile_rel_dir}/${REPLACEMENT}_${ss}_${out_name}.c")
		yio_add_template_command(
			REPLACEMENT "_${REPLACEMENT}"
			INPUT "${infile}"
			INTERMEDIATE "${CMAKE_CURRENT_BINARY_DIR}/template/${out}"
			OUTPUT "${GENDIR}/${out}"
			OPTIONS -D m4_FORFLOAT=${ss}
		)
		list(APPEND srcs ${out})
	endforeach()
endmacro()

function(yio_template)
	# yio or ywio or yuio
	get_filename_component(name ${CMAKE_CURRENT_LIST_DIR} NAME_WE)

	if(NOT "${name}" MATCHES "^y.*io$")
		message(FATAL_ERROR "NOT REGEX ${name} ${rgx}")
	endif()
	string(REGEX REPLACE "^y(.*)io$" "\\1" REPLACEMENT "${name}")

	set(srcs)
	file(GLOB_RECURSE tmp ${YIO_TEMPLATE_DIR}/*.c ${YIO_TEMPLATE_DIR}/*.h)
	foreach(ii IN LISTS tmp)
		yio_template_in(${ii} ${YIO_TEMPLATE_DIR})
	endforeach()
	file(GLOB_RECURSE tmp ${CMAKE_CURRENT_LIST_DIR}/*.c ${CMAKE_CURRENT_LIST_DIR}/*.h)
	foreach(ii IN LISTS tmp)
		yio_template_in(${ii} ${CMAKE_CURRENT_LIST_DIR})
	endforeach()
	_yio_handle_float_in(
		${YIO_TEMPLATE_DIR}/manip/print_float.c.in
		${YIO_TEMPLATE_DIR}
	)

	set(${name}_gensrcs "${srcs}" PARENT_SCOPE)
endfunction()

