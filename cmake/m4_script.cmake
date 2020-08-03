cmake_minimum_required(VERSION 3.1)

# Escape string for makefile
macro(escape str)
	string(REGEX REPLACE "([ \t\n#])" "\\\\1" "${str}" "${${str}}")
endmacro()

foreach(i IN ITEMS
		M4_COMMAND
		OUTPUT
		M4_ARGS
)
	if(NOT DEFINED ${i})
		message(FATAL_ERROR "m4_script.cmake: ${i} is not defined")
	endif()
endforeach()

###############################################################

if(CMAKE_HOST_UNIX)
	execute_process(
		COMMAND chmod ugo+w "${OUTPUT}"
		RESULT_VARIABLE _
		OUTPUT_VARIABLE _
		ERROR_VARIABLE _
	)
	unset(_)
endif()
get_filename_component(outputdir "${OUTPUT}" DIRECTORY)
file(MAKE_DIRECTORY "${outputdir}")
execute_process(
	COMMAND "${M4_COMMAND}" --debug=i ${M4_ARGS}
	RESULT_VARIABLE m4result
	OUTPUT_FILE "${OUTPUT}"
	ERROR_VARIABLE error_raw
)
if(CMAKE_HOST_UNIX)
	execute_process(
		COMMAND chmod ugo-w "${OUTPUT}"
		RESULT_VARIABLE _
		OUTPUT_VARIABLE _
		ERROR_VARIABLE _
	)
	unset(_)
endif()

###############################################################

# From stderr output from m4
# Extract and ignore debug lines
# And extra debug lines about input read and extract from them
# the filename of files that were read
set(error)
set(deps)
string(REGEX REPLACE "\n" ";" error_raw "${error_raw}")
foreach(line IN LISTS error_raw)
	if(line STREQUAL "")
		continue()
	endif()
	if(line MATCHES "^m4debug: ")
		if(line MATCHES "^m4debug: input read from ([^\n]*)")
			string(REGEX REPLACE "m4debug: input read from ([^\n]*)" "\\1" line "${line}")
			if(line STREQUAL "stdin")
				continue()
			endif()
			get_filename_component(line "${line}" ABSOLUTE)
			escape(line)
			string(APPEND deps " ${line}")
		endif()
	else()
		# Remove executable name from error line
		# So that my eclipse picks up source file
		# Then add something on the end of line, so I know it's m4
		string(REGEX REPLACE "^${M4_COMMAND}:" "" line "${line} (m4)")
		string(APPEND error "\n${line}")
	endif()
endforeach()
string(REGEX REPLACE "^\n" "" error "${error}")

###############################################################

if(NOT m4result EQUAL 0)
	string(REGEX REPLACE ";" " " M4_ARGS "${M4_ARGS}")
	message("+ ${M4_COMMAND} ${M4_ARGS}")
	message("${error}")
	message(FATAL_ERROR "m4 returned: ${m4result}")
endif()

###############################################################

# Generate depdency file
if(DEFINED DEPFILE)
	if(NOT DEFINED DEPNAME)
		set(DEPNAME ${OUTPUT})
	endif()
	escape(DEPNAME)
	file(WRITE ${DEPFILE} "${DEPNAME}:${deps}\n")
endif()


