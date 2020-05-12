
# @def test_extract_properties_from_file(testname file [PREFIX prefix])
# Extract test properties from the filename.
# In the test file you should have lines like:
# // PASS_REGULAR_EXPRESSION "blabla"
# The string "blabla" would be extracted from the file and 
#    add_tests_properties(..... PASS_REGULAR_EXPRESSION "blabla")
# would be executed. The purpouse is to store the regular expressions
# inside the file.
function(test_extract_properties_from_file testname file)
	set(prefix "[ \t]*// ")
	foreach(i IN ITEMS
		PASS_REGULAR_EXPRESSION
		FAIL_REGULAR_EXPRESSION
		SKIP_REGULAR_EXPRESSION
		WILL_FAIL
	)
		file(STRINGS ${file} expr REGEX "^${prefix}${i} ")
		if(expr)
			string(REGEX REPLACE "^${prefix}${i} " "" expr "${expr}")
			if("${expr}" MATCHES  ";${prefix}${i} " AND ("${expr}" MATCHES ".^" OR "${expr}" MATCHES "$."))
				message(FATAL_ERROR "Problem with item ${i} in ${file}")
			endif()
			string(REGEX REPLACE ";${prefix}${i} " "[^\n]*\n" expr "${expr}")
			set_tests_properties(${testname} PROPERTIES ${i} "${expr}")
		endif()
	endforeach()
endfunction()
