cmake_minimum_required(VERSION 3.11)

############################################################################
# Functions

set(name "m4_cmake")

macro(usage)
	message(STATUS [=[
Usage: cmake -D VAR=VAL [options] -P ${CMAKE_CURRENT_LIST_DIR}

Following variables are used:
- M4 - The path to m4 executable.
- M4_FLAGS - List of semicolon separated flags to pass to m4 executable.
- OUTPUTFILE - Output filename. With no output, write to standard output.
- DEPFILE - Dependency filename.
- DEPTARGET - Dependency target name.

Written by Kamil Cukrowski 2019
Licensed under GPL 3.0
]=])
endmacro()

macro(fatal)
	message(FATAL_ERROR "${name}:" ${ARGN})
endmacro()

############################################################################

foreach(i IN ITEMS
	M4
	M4_FLAGS
	OUTPUTFILE
)
	if(NOT DEFINED ${i})
		usage()
		fatal("${i} not defined")
	endif()
endforeach()

get_filename_component(outputdir ${OUTPUTFILE} DIRECTORY)
get_filename_component(outputfilename ${OUTPUTFILE} NAME)
set(tempdir ${outputdir}/tmp)
set(tempfile ${tempdir}/${outputfilename})

file(MAKE_DIRECTORY "${tempdir}")

# Execute m4 process.
execute_process(
	COMMAND ${M4} -dp ${M4_FLAGS}
	RESULT_VARIABLE result
	# We generate output into temporary file
	OUTPUT_FILE "${tempfile}"
	ERROR_VARIABLE errors	
)
# Copy temporary file to output dir with only read-only permissions.
file(
	COPY "${tempfile}"
	DESTINATION "${outputdir}"
	FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
)
# Remove temporary directory.
file(REMOVE_RECURSE "${tempdir}")

# Regex to match m4 debug output from -dp option.
set(deprgx "^m4debug: path search for `.*' found `(.*)'$")

# Parse errors into two parts:
# - dependency output from -dp option,
# - and real errors. 
set(erroutput)
set(depfiles)
string(REGEX REPLACE "\n" ";" errors "${errors}")
foreach(line IN LISTS errors)
	if("${line}" MATCHES "${deprgx}")
		string(REGEX REPLACE "${deprgx}" "\\1" line "${line}")
		file(RELATIVE_PATH line "${CMAKE_CURRENT_LIST_DIR}" "${line}")
		list(APPEND depfiles "${line}")
	else()
		list(APPEND erroutput "${line}")
	endif()
endforeach()

if(NOT result EQUAL 0)
	message("m4 exited with ${result}")
	string(REGEX REPLACE ";" "\n" erroutput "${erroutput}")
	message("${erroutput}")
	message(FATAL_ERROR)
endif()

if(DEFINED DEPFILE)
	if(NOT DEFINED DEPTARGET)
		fatal("DEPFILE defined but DEPTARGET not defined")
	endif()
	string(REGEX REPLACE ";" " \\\\\n" depfiles "${depfiles}")
	file(WRITE "${DEPFILE}" "${DEPTARGET}: \\\n${depfiles}")
endif()


