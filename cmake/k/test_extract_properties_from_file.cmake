include_guard()
cmake_policy(SET CMP0057 NEW)

# @def test_extract_properties_from_file(exename testname file [PREFIX prefix])
# Extract test properties from the filename.
# In the test file you should have lines like:
# //// PASS_REGULAR_EXPRESSION "blabla"
#
# QUOTING AND ESCAPES:
# If the value is NOT enclosed in double quotes:
#   - Taken as a literal string. Leading/trailing whitespace is stripped.
#   - NO escape sequences are processed.
#
# If the value IS enclosed in double quotes:
#   - Surrounding quotes are stripped.
#   - The following escapes are processed:
#     \n -> literal newline, \t -> literal tab, \r -> literal carriage return, \" -> literal quote.
#   - To prevent processing an escape, double the backslash (e.g., \\n stays as \n).
#   - \\ becomes \ (useful for regex literals like \\s).
#
# PROPERTY CATEGORIES:
# 1. Non-appendable (e.g., TIMEOUT, WILL_FAIL): Setting these twice triggers a FATAL_ERROR.
# 2. List-appendable (e.g., LABELS): Multiple lines are appended as a semicolon-separated list.
# 3. Regex-appendable (*_REGULAR_EXPRESSION): Multiple lines are joined with '.*' to ensure
#    they match sequentially in the output.
#
# MULTI-LINE REGEX AND BOUNDARIES:
# - Sequential appending with '.*' only works if the previous regex does not end with '$'
#   and the new regex does not start with '^'. Violating this triggers a FATAL_ERROR.
# - Since '.' matches newlines in CMake, '.*' effectively bridges lines.
# - For complex multi-line matches, use a single quoted string with '\n' embedded.
#
# CMAKE CONFLICTS:
# - Using WILL_FAIL and any *_REGULAR_EXPRESSION together triggers a WARNING because
#   CMake ignores regex checks when WILL_FAIL is set.
function(test_extract_properties_from_file exename testname file)
  cmake_parse_arguments(T "" "PREFIX" "" ${ARGN})
  if(T_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "test_extract_properties_from_file: Unparsed arguments: ${T_UNPARSED_ARGUMENTS}")
  endif()

  # Parse prefix
  if(T_PREFIX)
    set(prefix "^[ \t]*${T_PREFIX}[ \t]+")
  else()
    if(file MATCHES "\\.cmake$")
      set(prefix "^[ \t]*##+[ \t]+")
    else()
      set(prefix "^[ \t]*//+[ \t]+")
    endif()
  endif()

  set(testprops COMPILATION RUN SKIP_RETURN_CODE WILL_FAIL TIMEOUT EXPECTED_EXIT_CODE)
  set(testprops_regex PASS_REGULAR_EXPRESSION FAIL_REGULAR_EXPRESSION SKIP_REGULAR_EXPRESSION)
  set(testprops_append LABELS)
  set(exeprops C_STANDARD C_STANDARD_REQUIRED C_EXTENSIONS)
  set(allprops ${testprops} ${testprops_regex} ${testprops_append} ${exeprops})

  foreach(prop IN LISTS allprops)
    unset(P_${prop})
  endforeach()

  file(STRINGS "${file}" contents REGEX "${prefix}")

  foreach(line IN LISTS contents)
    if(line MATCHES "${prefix}([^ \t]+)[ \t]+(.*)")
      set(prop "${CMAKE_MATCH_1}")
      set(value "${CMAKE_MATCH_2}")

      if(NOT prop IN_LIST allprops)
        continue()
      endif()

      string(STRIP "${value}" value)

      # Handle quotes and escapes
      if(value MATCHES "^\"(.*)\"$")
        set(value "${CMAKE_MATCH_1}")
        # Protect escaped backslashes by replacing \\ with a temporary marker
        string(REPLACE "\\\\" "[[BACKSLASH_MARKER]]" value "${value}")

        # Process escapes
        string(REPLACE "\\n" "\n" value "${value}")
        string(REPLACE "\\t" "\t" value "${value}")
        string(REPLACE "\\r" "\r" value "${value}")
        string(REPLACE "\\\"" "\"" value "${value}")

        # Restore backslashes (one level of de-escaping: \\ becomes \)
        string(REPLACE "[[BACKSLASH_MARKER]]" "\\" value "${value}")
      endif()

      if(prop IN_LIST testprops OR prop IN_LIST exeprops)
        if(DEFINED P_${prop})
          message(FATAL_ERROR "test_extract_properties_from_file: Property ${prop} set twice in ${file}. "
                              "This property does not support multiple values and setting it again would unexpectedly overwrite the previous one. "
                              "If you intended to add more values, check if an appendable property (like LABELS or REGULAR_EXPRESSIONS) should be used instead.")
        endif()
        set("P_${prop}" "${value}")
      elseif(prop IN_LIST testprops_regex)
        if(DEFINED "P_${prop}")
          if("${P_${prop}}" MATCHES "\\$$")
            message(FATAL_ERROR "test_extract_properties_from_file: Previous ${prop} ends with '$' in ${file}. "
                                "Appending '.*' after '$' will result in a regex that never matches across lines as intended. "
                                "For intricate multi-line matches, use a single ${prop} with '\\n' for newlines.")
          endif()
          if("${value}" MATCHES "^\\^")
            message(FATAL_ERROR "test_extract_properties_from_file: New ${prop} starts with '^' in ${file}. "
                                "Appending '^' after '.*' will result in a regex that never matches across lines as intended. "
                                "For intricate multi-line matches, use a single ${prop} with '\\n' for newlines.")
          endif()
          set("P_${prop}" "${P_${prop}}.*${value}")
        else()
          set("P_${prop}" "${value}")
        endif()
      elseif(prop IN_LIST testprops_append)
        list(APPEND "P_${prop}" "${value}")
      endif()
    if(DEFINED P_WILL_FAIL)
      foreach(rprop IN LISTS testprops_regex)
        if(DEFINED "P_${rprop}")
          message(WARNING "test_extract_properties_from_file: WILL_FAIL and ${rprop} are together in ${file}. "
                          "CMake ignores regular expression properties when WILL_FAIL is set because it only checks for a non-zero exit code. "
                          "To validate output for a failing test, use a shell wrapper like 'sh -c \"! \\\"$@\\\"\"' to invert the exit code instead of using WILL_FAIL.")
        endif()
      endforeach()
    endif()
    endif()
  endforeach()

  foreach(prop IN LISTS allprops)
    if(NOT DEFINED "P_${prop}")
      continue()
    endif()
    if(prop IN_LIST testprops OR prop IN_LIST testprops_regex)
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
