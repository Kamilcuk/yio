include_guard()

macro(set_from_env _var)
	set(${_var})
	if(DEFINED "ENV{${_var}}")
		set(${_var} "$ENV{${_var}}")
	endif()
endmacro()

