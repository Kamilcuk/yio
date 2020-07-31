
foreach(i IN ITEMS
		REPLACEMENT
		INPUT
		OUTPUT
)
	if(NOT DEFINED ${i})
		message(FATAL_ERROR "${i} not defined")
	endif()
endforeach()

string(TOLOWER "${REPLACEMENT}" PI)
string(TOUPPER "${REPLACEMENT}" OMEGA)

file(READ "${INPUT}" tmp)
string(REPLACE "π" "${PI}" tmp "${tmp}")
string(REPLACE "Ω" "${OMEGA}" tmp "${tmp}")
get_filename_component(dir "${OUTPUT}" DIRECTORY)
file(MAKE_DIRECTORY "${dir}")
file(WRITE "${OUTPUT}" "${tmp}") 