cmake_minimum_required(VERSION 3.12)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
include(CMakeDependentOption)
include(m4)
include(list_filter)
include(var_to_0_or_1)

function(log)
	get_filename_component(_log_list_filename ${CMAKE_CURRENT_LIST_FILE} NAME)
	message(STATUS "yio: ${_log_list_filename}: ${ARGN}")
endfunction()

# 

set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})
set(GENDIR ${CMAKE_CURRENT_BINARY_DIR}/gen)
get_filename_component(GENDIR "${GENDIR}" ABSOLUTE)

##########################################################################################
# Environment checks
include(environment_checks.cmake)

# Configuration
include(configuration.cmake)

###########################################################################################
# generate config.m4
configure_file(
	${SRCDIR}/m4/yio_config.m4.in
	${GENDIR}/m4/yio_config.m4
	@ONLY
)

# generate yio_config.h
configure_file(
	${SRCDIR}/yio/yio_config.h.in
	${GENDIR}/yio/yio_config.h
	@ONLY
)

# generate .m4 files ##################################################
m4_add_options(lib.m4)
m4_add_include_directories(${SRCDIR} ${GENDIR}/m4)
m4_add_file_dependencies(${GENDIR}/m4/yio_config.m4)
file(GLOB_RECURSE m4srcs *.c *.h)
foreach(source IN LISTS m4srcs)
	file(RELATIVE_PATH sourcenice ${SRCDIR} ${source})
	set(output ${GENDIR}/${sourcenice})
	# dbg("m4_add_command ${output} ${source}")
	m4_add_command(${output} ${source})
	list(APPEND m4_outputs ${output})
endforeach()

# generate error messages ####################
add_custom_command(
	OUTPUT
		${GENDIR}/yio/yio_error_messages.h
	DEPENDS
		${SRCDIR}/yio/yio_error.h
		${SRCDIR}/yio/yio_error_messages.cmake
	COMMAND ${CMAKE_COMMAND}
		-D OUTPUT=${GENDIR}/yio/yio_error_messages.h
		-D INPUT=${SRCDIR}/yio/yio_error.h
		-P ${CMAKE_CURRENT_SOURCE_DIR}/yio/yio_error_messages.cmake
	COMMENT "Generate yio/yio_error_messages.h file"
)

# Generate all files target ###########################################
set(gensrcs
	${m4_outputs}
	${GENDIR}/yio/yio_config.h
	${GENDIR}/yio/yio_config_gen.h
	${GENDIR}/yio/yio_error_messages.h
	${GENDIR}/m4/yio_config.m4
	${GENDIR}/m4/floatlist.m4
)
add_custom_target(_yio_gen DEPENDS ${gensrcs})

# Remove files in GENDIR not in gensrcs ###############################
file(GLOB_RECURSE files ${GENDIR}/*.c ${GENDIR}/*.h ${GENDIR}/*.m4)
function(list_exclude result listin listexclude)
	set(tmp)
	foreach(i IN LISTS ${listin})
		list(FIND "${listexclude}" "${i}" index)
		if (${index} EQUAL -1)
			list(APPEND tmp "${i}")
		endif()
	endforeach()
	set(${result} "${tmp}" PARENT_SCOPE)
endfunction()
list_exclude(files_not_in_gensrcs files gensrcs)
if(files_not_in_gensrcs)
	foreach(i IN LISTS files_not_in_gensrcs)
		message(STATUS "Yio:Removing: ${i}")
	endforeach()
	file(REMOVE ${files_not_in_gensrcs})
endif()

# Compile options #######################################################

# yio_add_library(name sources)
# adds the library with the name <name>
# that has the sources <sources>
# enables warnings if needed
# generates install target for the library
function(yio_add_library name)
	set(argn ${ARGN})
	list(SORT argn)

	# create list of sources by appending GENDIR
	set(srcs)
	foreach(i IN LISTS argn)
		list(APPEND srcs "${GENDIR}/${i}")
	endforeach()


	# extract list of headers
	set(headers "${argn}")
	list(FILTER headers INCLUDE REGEX "\\.h$")
	
	# add the library
	add_library(${name} ${srcs})
	set_property(TARGET ${name} PROPERTY
		C_EXTENSIONS YES
		C_STANDARD 11
		C_STANDARD_REQUIRED NO
		$<$<NOT:$<CONFIG:DEBUG>>:INTERPROCEDURAL_OPTIMIZATION TRUE>	
		DEBUG_POSTFIX "d"
	)
	target_include_directories(${name} PUBLIC
		$<BUILD_INTERFACE:${GENDIR}>
	)
	
	# compile options
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
			CMAKE_C_COMPILER_ID STREQUAL "Clang")
		target_compile_options(${name} PRIVATE
			$<$<CONFIG:Debug>:-ggdb3>
			-Wall
			-Wextra
			-Wwrite-strings
			-Wno-unused-function
			-Wno-unused-argument
			-Wno-unused-parameter
		)
	endif()
	if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
		target_compile_options(${name} PRIVATE
			-Wno-varargs
		)
		target_compile_options(${name} PUBLIC
			-Wno-array-bounds
			-Wno-sizeof-array-decay
		)
	endif()
	
	add_dependencies(${name} _yio_gen)
	target_link_libraries(${name} PUBLIC m)
	if (NOT ${name} STREQUAL "yio")
		target_link_libraries(${name} PUBLIC yio)
	endif()

	# install the library
	install(TARGETS ${name}
		EXPORT yio
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		INCLUDES DESTINATION include
	)

	# install the headers
	foreach(h IN LISTS headers)
        	get_filename_component(dir ${h} DIRECTORY)
        	install(FILES "${GENDIR}/${h}" DESTINATION include/${dir}/)
	endforeach()

endfunction()

# file(GLOB) but results in files with 
# substituted ${SRCDIR} for ${GENDIR}
function(file_glob_relative_to_gendir glob var)
	file(${glob} tmp ${ARGN})
	set(tmp2)
	foreach(i IN LISTS tmp)
		# message("${var} ${i}")
		file(RELATIVE_PATH j ${SRCDIR} ${i})
		list(APPEND tmp2 ${j})
	endforeach()
	set(${var} "${tmp2}" PARENT_SCOPE)
endfunction()

# add yio executable ##############################################
file_glob_relative_to_gendir(GLOB auto_stupiddfp_srcs yio/stupiddfp/*.h yio/stupiddfp/*.c)
yio_add_library(yio_stupiddfp 
	${auto_stupiddfp_srcs}	
	yio/yio_config.h
	yio/yio_config_gen.h
)

file_glob_relative_to_gendir(GLOB auto_generic_srcs yio/*.h yio/*.c)
file_glob_relative_to_gendir(GLOB_RECURSE auto_yio_srcs yio/yio/*.h yio/yio/*.c)
file_glob_relative_to_gendir(GLOB_RECURSE auto_yio_bits_srcs yio/bits/*.h yio/bits/*.c)
yio_add_library(yio
	${auto_yio_bits_srcs}
	${auto_generic_srcs}
	yio/yio_config.h
	yio/yio_config_gen.h
	yio/yio_error_messages.h
	
	yio.h
	${auto_yio_srcs}
)

# install config.cmakecmake 
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/yioConfig.cmake
	DESTINATION lib/yio/
)

# and install the targets
install(EXPORT yio
	FILE yioTargets.cmake
	NAMESPACE yio::
	DESTINATION lib/yio
)

add_custom_target(
	yio_uninstall 
	COMMAND sh -c "xargs rm -f -v < install_manifest.txt"
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	VERBATIM
)
