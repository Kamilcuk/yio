

execute_process(
COMMAND ${CMAKE_COMMAND} -E create_symlink
	${CMAKE_CURRENT_BINARY_DIR}/gen
	${CMAKE_CURRENT_SOURCE_DIR}/gen
)
		
file(GLOB_RECURSE srcs *.c *.h)
foreach(REPLACEMENT IN ITEMS "" "w")
	foreach(src IN LISTS srcs)
	
		file(RELATIVE_PATH src_rel_source ${CMAKE_CURRENT_SOURCE_DIR} ${src})
		set(path ${CMAKE_CURRENT_BINARY_DIR}/gen/y${REPLACEMENT}io/${src_rel_source})
		add_custom_command(
			OUTPUT ${path}.m4
			DEPENDS 
				${src}
				${CMAKE_CURRENT_SOURCE_DIR}/../../src/yio/template/yio_template_replace.cmake
				${CMAKE_CURRENT_LIST_FILE}
			COMMAND ${CMAKE_COMMAND}
				-D OUTPUT=${path}.m4
				-D INPUT=${src}
				-D REPLACEMENT=${REPLACEMENT}
				-P ${CMAKE_CURRENT_SOURCE_DIR}/../../src/yio/template/yio_template_replace.cmake
		)
		if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../src/yio/y${REPLACEMENT}io/template.m4)
			message(FATAL_ERROR "")
		endif()
		m4_add_command(
			OUTPUT ${path}
			SOURCE
				${CMAKE_CURRENT_SOURCE_DIR}/../../src/yio/y${REPLACEMENT}io/template.m4
				${path}.m4
			OPTIONS
				-D m4_SOURCE=${src}
		)
			
		file(RELATIVE_PATH name ${CMAKE_CURRENT_BINARY_DIR}/gen ${path})
		string(REPLACE "/" "_" name "${name}")
		string(REPLACE "\\" "_" name "${name}")
		string(REGEX REPLACE "\\..*$" "" name "${name}")
	
		yio_add_test(${name} ${path} ${src})
	endforeach()
endforeach()
				