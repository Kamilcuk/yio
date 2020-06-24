
# https://stackoverflow.com/questions/61499646/cmake-set-variable-readonly-protect-from-override
# Analogue for 'set' command which defines readonly variable.
# Usage:
# set_readonly(FOO value)
macro(readonly VAR)
	if(ARGC GREATER 1)
 		# Set the variable itself
 		set("${VAR}" "${ARGN}")
 	endif()
	# Store the variable's value for restore it upon modifications.
	set("_${VAR}_readonly_val" "${ARGN}")
	# Register a watcher for a variable
	variable_watch("${VAR}" readonly_guard)
endmacro()

# Watcher for a variable which emulates readonly property.
macro(readonly_guard VAR access value current_list_file stack)
	if("${access}" STREQUAL "MODIFIED_ACCESS")
		message(FATAL_ERROR "Attempt to change readonly variable '${VAR}'!")
		# Restore a value of the variable to the initial one.
		set(${VAR} "${_${VAR}_readonly_val}")
	endif()
endmacro()