# Test to check how CMake regex treats \a
set(text "abc")

if(text MATCHES "\\a")
    message(STATUS "RESULT 1: '\\a' MATCHES 'a' (or matches something in 'abc')")
else()
    message(STATUS "RESULT 1: '\\a' DOES NOT MATCH 'a'")
endif()

if("a" MATCHES "\\a")
    message(STATUS "RESULT 2: 'a' MATCHES '\\a'")
else()
    message(STATUS "RESULT 2: 'a' DOES NOT MATCH '\\a'")
endif()

string(ASCII 7 BELL)
if("${BELL}" MATCHES "\\a")
    message(STATUS "RESULT 3: BELL MATCHES '\\a'")
else()
    message(STATUS "RESULT 3: BELL DOES NOT MATCH '\\a'")
endif()
