include_guard()

set(TEMPLATE_DIR ${CMAKE_CURRENT_LIST_DIR})

macro(yio_template_in i relative)
	get_filename_component(i "${i}" ABSOLUTE)
	get_filename_component(i_name "${i}" NAME)
	file(RELATIVE_PATH i_rel ${relative} ${i})
	set(out yio/${name}/${i_rel})
	
	if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${i_rel} AND EXISTS ${TEMPLATE_DIR}/${i_rel})
		message(FATAL_ERROR 
			"yio_template.cmake: ERROR: Duplicate template files found"
			" in both the template/ and y*io directory. Only one can exists."
			" Files: "
			"${CMAKE_CURRENT_LIST_DIR}/${i_rel}"
			" and "
			"${TEMPLATE_DIR}/${i_rel}\n"
		)
	endif()
	
	add_custom_command(
		COMMENT "yio_template: Generating ${out} from ${i}"
		OUTPUT
			${CMAKE_CURRENT_BINARY_DIR}/template/${out}
		DEPENDS
			${TEMPLATE_DIR}/yio_template_replace.cmake
			${i}
			${CMAKE_CURRENT_LIST_FILE}
		COMMAND ${CMAKE_COMMAND}
			-D INPUT=${i}
			-D OUTPUT=${CMAKE_CURRENT_BINARY_DIR}/template/${out}
			-D REPLACEMENT=${REPLACEMENT}
			-P ${TEMPLATE_DIR}/yio_template_replace.cmake
	)
	
	file(RELATIVE_PATH i_rel_srcdir ${SRCDIR} ${i})
	m4_add_command(
		OUTPUT
			${GENDIR}/${out}
		SOURCE
			${CMAKE_CURRENT_BINARY_DIR}/template/${out}
		OPTIONS
			-D m4_SOURCE=${i}
			-D m4_TEMPLATE_SOURCE=${i_rel_srcdir}
			-D m4_CONFIG=yio/${name}/template.m4
	)
	list(APPEND srcs ${out})
endmacro()

function(yio_template)
	# yio or ywio or yuio
	get_filename_component(name ${CMAKE_CURRENT_LIST_DIR} NAME_WE)
	
	if(NOT "${name}" MATCHES "^y.*io$")
		message(FATAL_ERROR "NOT REGEX ${name} ${rgx}")
	endif()
	string(REGEX REPLACE "^y(.*)io$" "\\1" REPLACEMENT "${name}")
	if(NOT (REPLACEMENT STREQUAL "" OR
			REPLACEMENT STREQUAL "w" OR
			REPLACEMENT STREQUAL "c16" OR 
			REPLACEMENT STREQUAL "u") )
		message(FATAL_ERROR "NOT LETTER")
	endif()
	
	set(srcs)
	file(GLOB_RECURSE tmp ${TEMPLATE_DIR}/*.c ${TEMPLATE_DIR}/*.h)
	foreach(i IN LISTS tmp)
		yio_template_in(${i} ${TEMPLATE_DIR})
	endforeach()
	file(GLOB_RECURSE tmp ${CMAKE_CURRENT_LIST_DIR}/*.c ${CMAKE_CURRENT_LIST_DIR}/*.h)
	foreach(i IN LISTS tmp)
		yio_template_in(${i} ${CMAKE_CURRENT_LIST_DIR})
	endforeach()
	
	set(${name}_gensrcs "${srcs}" PARENT_SCOPE)
endfunction()
