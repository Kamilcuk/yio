# Test to check how CMake regex treats \b
set(text "slash\\back")

# Test 1: Literal match with single backslash in regex
if(text MATCHES "slash\\back")
    message(STATUS "RESULT 1: 'slash\\\\back' MATCHES 'slash\\back' (regex saw \\b)")
else()
    message(STATUS "RESULT 1: 'slash\\\\back' DOES NOT MATCH 'slash\\back'")
endif()

# Test 2: Check if \b is a word boundary
set(word_text "hello world")
if(word_text MATCHES "\\bworld")
    message(STATUS "RESULT 2: '\\bworld' MATCHES 'world' (word boundary supported)")
else()
    message(STATUS "RESULT 2: '\\bworld' DOES NOT MATCH 'world'")
endif()

# Test 3: Check if \b matches literal 'b'
if("ab" MATCHES "a\\b")
    message(STATUS "RESULT 3: 'a\\b' MATCHES 'ab'")
else()
    message(STATUS "RESULT 3: 'a\\b' DOES NOT MATCH 'ab'")
endif()
