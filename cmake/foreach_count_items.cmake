
# Iterate over count items at a time.
# @def foreach_count_items(loopvariable internal_state_variable variables...)
# @param internal_state_variable The name of the loop variables used in the foreach loop.
#               The varibale will be set to ON or OFF depending if user code should continue.
# @param internal_state_variable An internal name of the state variable used to keep track of call number.
# @param variables... Variable names that will be assigned the loop argumnets.
# 
# Example:
#     foreach(i IN ITEMS
#                    "car" "blue"
#                    "cat" "yellow"
#                    "chair" "brown"
#     )
#            foreach_count_items(i thing color)
#            if(i)
#                 continue()
#            endif()
#            
#            message(STATUS "thing=${thing} color=${color}")
#    endforeach()
# will output:
#    -- thing=car color=blue
#    -- thing=cat color=yellow
#    -- thing=chair color=brown
#
function(foreach_count_items _foreach_count_items_loopvar_name _foreach_count_items_statevar_name)
	if("${ARGC}" LESS 1) 
		message(FATAL_ERROR "foreach_count_items: wrong number of arguments")
	endif()
	
	set(_foreach_count_items_statevar "${${_foreach_count_items_statevar_name}}")
	set(_foreach_count_items_loopvar "${${_foreach_count_items_loopvar_name}}")
	#message(STATUS "_foreach_count_items_loopvar=${_foreach_count_items_loopvar}")
	#message(STATUS "_foreach_count_items_statevar=${_foreach_count_items_statevar}")
	
	if("${_foreach_count_items_statevar}" STREQUAL "")
		set(_foreach_count_items_statevar 1)
	endif()
	
	math(EXPR count "${ARGC} - 2")
	math(EXPR argc_less_one "${ARGC} - 1")
	math(EXPR argc_less_two "${ARGC} - 2")
	
	math(EXPR lastidx "${ARGC} - 1")
	#message(STATUS "set(${ARGV${lastidx}} ${_foreach_count_items_loopvar} PARENT_SCOPE)")
	set("${ARGV${lastidx}}" "${_foreach_count_items_loopvar}")
	set("${ARGV${lastidx}}" "${_foreach_count_items_loopvar}" PARENT_SCOPE)
	#message(STATUS "lastidx=${lastidx}")
	#message(STATUS "ARGV${lastidx}=${ARGV${lastidx}}=${${ARGV${lastidx}}}")		
		
	if(_foreach_count_items_statevar LESS count)
		foreach(i RANGE 2 ${argc_less_two})
			math(EXPR j "${i} + 1")
			#message(STATUS "${i} ${j} | ${ARGV${i}}=${${ARGV${i}}} ${ARGV${j}}=${${ARGV${j}}}")
			set("${ARGV${i}}" "${${ARGV${j}}}")
			set("${ARGV${i}}" "${${ARGV${j}}}" PARENT_SCOPE)
		endforeach()
		
		#foreach(i RANGE ${argc_less_one})
		#	message(STATUS "ARGV${i} = ${ARGV${i}} = ${${ARGV${i}}}")
		#ndforeach()

		math(EXPR _foreach_count_items_statevar "${_foreach_count_items_statevar} + 1")
		set(_foreach_count_items_loopvar YES)
	else()
		set(_foreach_count_items_statevar 1)
		set(_foreach_count_items_loopvar NO)
	endif()
	
	set("${_foreach_count_items_statevar_name}" "${_foreach_count_items_statevar}" PARENT_SCOPE)
	set("${_foreach_count_items_loopvar_name}" "${_foreach_count_items_loopvar}" PARENT_SCOPE)
endfunction()
