function(k_add_test)
  set(flags COMMAND_EXPAND_LIST)
  set(oneargs
      NAME
      CONFIGURATIONS
      WORKING_DIRECTORY
      #
      ATTACHED_FILES
      ATTACHED_FILES_ON_FAIL
      COST
      DISABLED
      ENVIRONMENT
      ENVIRONMENT_MODIFICATION
      FAIL_REGULAR_EXPRESSION
      MEASUREMENT
      PASS_REGULAR_EXPRESSION
      PROCESSOR_AFFINITY
      RESOURCE_LOCK
      RUN_SERIAL
      SKIP_REGULAR_EXPRESSION
      SKIP_RETURN_CODE
      TIMEOUT
      TIMEOUT_AFTER_MATCH
      WILL_FAIL
  )
  set(margs
      COMMAND
      #
      DEPENDS
      FIXTURES_CLEANUP
      FIXTURES_REQUIRED
      FIXTURES_SETUP
      LABELS
      PROCESSORS
      REQUIRED_FILES
      RESOURCE_GROUPS
  )
  cmake_parse_arguments(A "${flags}" "${oneargs}" "${margs}" ${ARGV})
  set(test_properties
      ATTACHED_FILES
      ATTACHED_FILES_ON_FAIL
      COST
      DEPENDS
      DISABLED
      ENVIRONMENT
      ENVIRONMENT_MODIFICATION
      FAIL_REGULAR_EXPRESSION
      FIXTURES_CLEANUP
      FIXTURES_REQUIRED
      FIXTURES_SETUP
      LABELS
      MEASUREMENT
      PASS_REGULAR_EXPRESSION
      PROCESSOR_AFFINITY
      PROCESSORS
      REQUIRED_FILES
      RESOURCE_GROUPS
      RESOURCE_LOCK
      RUN_SERIAL
      SKIP_REGULAR_EXPRESSION
      SKIP_RETURN_CODE
      TIMEOUT
      TIMEOUT_AFTER_MATCH
      WILL_FAIL
      #WORKING_DIRECTORY
  )
  #
  set(args)
  foreach(ii IN ITEMS NAME COMMAND CONFIGURATIONS WORKING_DIRECTORY)
    if(DEFINED "A_${ii}")
      list(APPEND args "${ii}" "${A_${ii}}")
    endif()
  endforeach()
  if(DEFINED A_COMMAND_EXPAND_LISTS)
    list(APPEND args COMMAND_EXPAND_LIST)
  endif()
  #
  add_test(${args})
  foreach(ii IN LISTS test_properties)
    if(DEFINED A_${ii})
      set_property(TEST "${A_NAME}" PROPERTY "${ii}" "${A_${ii}}")
    endif()
  endforeach()
endfunction()
