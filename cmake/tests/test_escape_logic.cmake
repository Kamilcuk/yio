# Test to check how the parser handles \t vs \\t in quoted strings
set(value "\\t") # This is a literal backslash followed by 't' in the variable

# This mimics the logic in the function:
# string(REPLACE "\\t" "\t" value "${value}")
# In CMake, the first argument to REPLACE is the literal string to find.
# To find a backslash-t pair, we use "\\t".

string(REPLACE "\\t" "TAB_CHAR" result "${value}")
message(STATUS "REPLACING '\\\\t': [${value}] -> [${result}]")

set(value2 "\t") # This is an actual tab character (if the editor/shell allows) or just \t
string(REPLACE "\\t" "TAB_CHAR" result2 "${value2}")
message(STATUS "REPLACING '\t': [${value2}] -> [${result2}]")
