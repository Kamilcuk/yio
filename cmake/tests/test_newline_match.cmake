# Test to check if '\n' in regex matches newline or 'n'
set(text "line1\nline2")

# Case 1: Regex has a literal newline
set(regex1 "line1\nline2")
if(text MATCHES "${regex1}")
    message(STATUS "RESULT 1: literal newline MATCHES")
else()
    message(STATUS "RESULT 1: literal newline DOES NOT MATCH")
endif()

# Case 2: Regex has backslash-n string
set(regex2 "line1\\nline2")
if(text MATCHES "${regex2}")
    message(STATUS "RESULT 2: '\\n' string MATCHES")
else()
    message(STATUS "RESULT 2: '\\n' string DOES NOT MATCH")
endif()
