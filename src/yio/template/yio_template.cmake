include_guard()

set(TEMPLATE_DIR ${CMAKE_CURRENT_LIST_DIR})

macro(yio_template_in i relative)
	get_filename_component(i "${i}" ABSOLUTE)
	get_filename_component(i_name "${i}" NAME)
	file(RELATIVE_PATH i_rel ${relative} ${i})
	set(out yio/${name}/${i_rel})
	
	if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/${i_rel} AND EXISTS ${TEMPLATE_DIR}/${i_rel})
		message(FATAL_ERROR 
			"duplicate template files"
			"${CMAKE_CURRENT_LIST_DIR}/${i_rel}"
			"${TEMPLATE_DIR}/${i_rel}"
		)
	endif()
	
	add_custom_command(
		OUTPUT
			${CMAKE_CURRENT_BINARY_DIR}/template/${out}
		DEPENDS
			${TEMPLATE_DIR}/yio_template_replace.cmake
			${i}
		COMMAND ${CMAKE_COMMAND}
			-D INPUT=${i}
			-D OUTPUT=${CMAKE_CURRENT_BINARY_DIR}/template/${out}
			-D PI=${PI}
			-D OMEGA=${OMEGA}
			-P ${TEMPLATE_DIR}/yio_template_replace.cmake
	)
	
	file(RELATIVE_PATH i_rel_srcdir ${SRCDIR} ${i})
	m4_add_command(
		${GENDIR}/${out}
		${CMAKE_CURRENT_BINARY_DIR}/template/${out}
		-D m4_SOURCE=${i}
		-D m4_TEMPLATE_SOURCE=${i_rel_srcdir}
		-D m4_CONFIG=yio/${name}/template.m4
	)
	list(APPEND srcs ${GENDIR}/${out})
endmacro()

function(yio_template)
	# yio or ywio or yuio
	get_filename_component(name ${CMAKE_CURRENT_LIST_DIR} NAME_WE)
	
	if(NOT "${name}" MATCHES "^y.*io$")
		message(FATAL_ERROR "NOT REGEX ${name} ${rgx}")
	endif()
	string(REGEX REPLACE "^y(.*)io$" "\\1" letter "${name}")
	string(TOUPPER "${letter}" OMEGA)  # "" or "w" or "u"
	string(TOLOWER "${letter}" PI)     # "" or "W" or "U"
	if(NOT (letter STREQUAL "" OR letter STREQUAL "u" OR letter STREQUAL "w") )
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
