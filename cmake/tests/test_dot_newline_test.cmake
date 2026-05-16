# Test to check if '.' matches newlines in CMake regex
set(text "line1\nline2")

if(text MATCHES "line1.line2")
    message(STATUS "RESULT: '.' MATCHES NEWLINE")
else()
    message(STATUS "RESULT: '.' DOES NOT MATCH NEWLINE")
endif()

if(text MATCHES "line1.*line2")
    message(STATUS "RESULT: '.*' MATCHES ACROSS NEWLINE")
else()
    message(STATUS "RESULT: '.*' DOES NOT MATCH ACROSS NEWLINE")
endif()
