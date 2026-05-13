include_guard()
cmake_policy(SET CMP0057 NEW)

# @def test_extract_properties_from_file(testname file [PREFIX prefix])
# Extract test properties from the filename.
# In the test file you should have lines like:
# //// PASS_REGULAR_EXPRESSION "blabla"
# The string "blabla" would be extracted from the file and
#    add_tests_properties(..... PASS_REGULAR_EXPRESSION "blabla")
# would be executed. The purpouse is to store the regular expressions
# inside the file.
function(test_extract_properties_from_file exename testname file)
  cmake_parse_arguments(T "" "PREFIX" "" ${ARGN})

  # Parse prefix
  if(T_PREFIX)
    set(prefix ${T_PREFIX})
  else()
    if(file MATCHES ".cmake$")
      set(prefix "^[ \t]*##+[ \t]+")
    else()
      set(prefix "^[ \t]*//+[ \t]+")
    endif()
  endif()

  set(testprops SKIP_RETURN_CODE PASS_REGULAR_EXPRESSION FAIL_REGULAR_EXPRESSION SKIP_REGULAR_EXPRESSION WILL_FAIL
                TIMEOUT EXPECTED_EXIT_CODE
  )
  set(testprops_append LABELS)
  set(exeprops C_STANDARD C_STANDARD_REQUIRED C_EXTENSIONS)
  set(allprops ${testprops} ${testprops_append} ${exeprops})

  foreach(prop IN LISTS allprops)
    unset(P_${prop})
  endforeach()

  file(READ "${file}" contents)
  # https://cmake.org/pipermail/cmake/2007-May/014222.html
  string(REGEX REPLACE ";" "\\\\;" contents "${contents}")
  string(REGEX REPLACE "\n" ";" contents "${contents}")
  foreach(line IN LISTS contents)
    if(NOT line MATCHES "${prefix}")
      continue()
    endif()
	# Och CMake. Fun fact: REGEX REPLACE is repeated!
	string(REGEX MATCH "${prefix}" matchedprefix "${line}")
	string(LENGTH "${matchedprefix}" matchedprefixlen)
	string(SUBSTRING "${line}" "${matchedprefixlen}" -1 line)
	string(REGEX MATCH "^[^ \t]+" prop "${line}")
	string(LENGTH "${prop}" proplen)
	string(SUBSTRING "${line}" "${proplen}" -1 line)
	string(REGEX REPLACE "^[ \t]" "" line "${line}")
	string(STRIP "${line}" line)
    if(NOT prop IN_LIST allprops)
      continue()
    endif()
    list(APPEND "P_${prop}" "${line}")
  endforeach()

  foreach(prop IN LISTS allprops)
    if(NOT DEFINED "P_${prop}")
      continue()
    endif()
    if(prop IN_LIST testprops)
      set_property(TEST ${testname} PROPERTY "${prop}" "${P_${prop}}")
    elseif(prop IN_LIST testprops_append)
      set_property(
        TEST ${testname}
        APPEND
        PROPERTY "${prop}" "${P_${prop}}"
      )
    elseif(prop IN_LIST exeprops)
      set_property(TARGET "${exename}" PROPERTY "${prop}" "${P_${prop}}")
    else()
      message(FATAL_ERROR "internal error")
    endif()
  endforeach()
endfunction()
