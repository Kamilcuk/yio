
# Iterate over count items at a time.
# @def foreach_count_items(loopvariable internal_state_variable variables...)
# @param internal_state_variable The name of the loop variables used in the foreach loop.
#               The varibale will be set to ON or OFF depending if user code should continue.
# @param internal_state_variable An internal name of the state variable used to keep track of call number.
# @param variables... Variable names that will be assigned the loop argumnets.
# Example below with tests.
#
# cmake-lint: disable=C0103
function(foreach_count_items fci_foreach_count_items_loopvar_name
         fci_foreach_count_items_statevar_name)
  if("${ARGC}" LESS 1)
    message(FATAL_ERROR "foreach_count_items: wrong number of arguments")
  endif()

  if(NOT DEFINED ${fci_foreach_count_items_statevar_name})
    set(fci_foreach_count_items_statevar 1)
  else()
    set(fci_foreach_count_items_statevar
        "${${fci_foreach_count_items_statevar_name}}")
  endif()
  set(fci_foreach_count_items_loopvar
      "${${fci_foreach_count_items_loopvar_name}}")
  # message(STATUS "fci_foreach_count_items_loopvar=${fci_foreach_count_items_loopvar}")
  # message(STATUS "fci_foreach_count_items_statevar=${fci_foreach_count_items_statevar}")

  math(EXPR count "${ARGC} - 2")
  math(EXPR argc_less_one "${ARGC} - 1")
  math(EXPR argc_less_two "${ARGC} - 2")

  math(EXPR lastidx "${ARGC} - 1")
  # message(STATUS "set(${ARGV${lastidx}} ${fci_foreach_count_items_loopvar}
  # PARENT_SCOPE)")
  set("${ARGV${lastidx}}" "${fci_foreach_count_items_loopvar}")
  set("${ARGV${lastidx}}"
      "${fci_foreach_count_items_loopvar}"
      PARENT_SCOPE)
  # message(STATUS "lastidx=${lastidx}") message(STATUS
  # "ARGV${lastidx}=${ARGV${lastidx}}=${${ARGV${lastidx}}}")

  if(fci_foreach_count_items_statevar LESS count)
    foreach(i RANGE 2 ${argc_less_two} 1)
      math(EXPR j "${i} + 1")
      # message(STATUS "${i} ${j} | ${ARGV${i}}=${${ARGV${i}}}
      # ${ARGV${j}}=${${ARGV${j}}}")
      if(DEFINED ${ARGV${j}})
        set("${ARGV${i}}" "${${ARGV${j}}}")
        set("${ARGV${i}}" "${${ARGV${j}}}" PARENT_SCOPE)
      endif()
    endforeach()

    # foreach(i RANGE ${argc_less_one}) message(STATUS "ARGV${i} = ${ARGV${i}} =
    # ${${ARGV${i}}}") ndforeach()

    math(EXPR fci_foreach_count_items_statevar
         "${fci_foreach_count_items_statevar} + 1")
    set(fci_foreach_count_items_loopvar YES)
  else()
    set(fci_foreach_count_items_statevar 1)
    set(fci_foreach_count_items_loopvar NO)
  endif()

  set("${fci_foreach_count_items_statevar_name}" "${fci_foreach_count_items_statevar}" PARENT_SCOPE)
  set("${fci_foreach_count_items_loopvar_name}" "${fci_foreach_count_items_loopvar}" PARENT_SCOPE)
endfunction()

if(KCMAKELIB_TEST)
  add_test(
    NAME foreach_count_items
    COMMAND ${CMAKE_COMMAND} -D KCMAKELIB_TEST_1=1 -P ${CMAKE_CURRENT_LIST_FILE}
  )
  set_tests_properties(foreach_count_items
     PROPERTIES PASS_REGULAR_EXPRESSION [=[
-- thing=car color=blue
-- thing=cat color=yellow
-- thing=chair color=brown
]=])
endif()
if(KCMAKELIB_TEST_1)
  foreach(ii IN ITEMS
      "car" "blue"
      "cat" "yellow"
      "chair" "brown"
  )
    foreach_count_items(ii statevariable thing color)
    if(ii)
      continue()
    endif()
    message(STATUS "thing=${thing} color=${color}")
  endforeach()
endif()

