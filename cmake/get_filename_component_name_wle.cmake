
if(${CMAKE_VERSION} VERSION_LESS "3.14.0")

function(get_filename_component_name_wle var filename)
	get_filename_component(${var} ${filename} NAME)
	string(REGEX REPLACE "\.[^.]*$" "" ${var} ${${var}})
	set(${var} ${${var}} PARENT_SCOPE)
endfunction()

else()

function(get_filename_component_name_wle var filename)
	get_filename_component(${var} ${filename} NAME_WLE)
	set(${var} ${${var}} PARENT_SCOPE)
endfunction()

endif()
