
include(debug_variables)
include(assert_is_valid_bool_only)

## @def set_default(var value)
## If the variable _var_ is not set, then
## sets the variable _var_ to value _value_.
macro(set_ifunset _var)
	if(NOT DEFINED "${_var}")
		set("${_var}" ${ARGN})
	endif()
endmacro()

## @def set_default(var value)
## @see set_ifunset
macro(set_default)
	set_ifunset(${ARGN})
endmacro()

macro(log)
	message(STATUS ">>>>>>>>>> dashboard: ${ARGV}")
endmacro()

macro(logln)
	message("")
	log(${ARGV})
endmacro()

## @def handle_ret(function_name return_variable_with_exit_status)
## Specially handle error from ctest_* functions.
## If we are not submitting, each error is a fatal error.
## If we are submitting, then only error from ctest_submit is a fatal error.
macro(handle_ret _handle_ret_func _handle_ret_ret)
	if(NOT WITH_SUBMIT OR (WITH_SUBMIT AND "${_handle_ret_func}" STREQUAL "ctest_submit"))
		if(NOT "${${_handle_ret_ret}}" EQUAL 0)
			message(FATAL_ERROR ">>>>>>>>>> dashboard_lib: ${_handle_ret_func} failed")
		endif()
	endif()
endmacro()

###################################################################################################
# God help me

macro(lib_ctest_empty_binary_directory)
	log("ctest_empty_binary_directory(${ARGN})")
	ctest_empty_binary_directory(${ARGN})
endmacro()

macro(lib_ctest_start)
	debug_variables("lib_ctest_start" CTEST_BUILD_NAME)
	logln("ctest_start(${ARGV})")
	ctest_start(${ARGV})
endmacro()

macro(lib_ctest_update)
	if(WITH_UPDATE)
		logln("ctest_update(${ARGV})")
		ctest_update(${ARGV} CAPTURE_CMAKE_ERROR _lib_ret)
		handle_ret(ctest_update _lib_ret)
	endif()
endmacro()

macro(lib_ctest_configure configure_flags c_flags)
	set(_lib_ctest_configure_flags "${configure_flags}")	
	string(APPEND _lib_ctest_configure_flags ";-DCMAKE_VERBOSE_MAKEFILE=1")
	string(APPEND _lib_ctest_configure_flags ";-DCMAKE_C_FLAGS=${c_flags}")
	logln("ctest_configure(OPTIONS ${_lib_ctest_configure_flags} ${ARGN})")
	ctest_configure(OPTIONS "${_lib_ctest_configure_flags}" ${ARGN}
		CAPTURE_CMAKE_ERROR _lib_ret)
	handle_ret(ctest_configure _lib_ret)
endmacro()

macro(lib_ctest_build)
	logln("ctest_build(${ARGV})")
	ctest_build(${ARGV} CAPTURE_CMAKE_ERROR _lib_ret)
	handle_ret(ctest_build _lib_ret)
endmacro()

macro(lib_ctest_test)
	logln("ctest_test(${ARGV})")
	ctest_test(${ARGV} CAPTURE_CMAKE_ERROR _lib_ret)
	handle_ret(ctest_test _lib_ret)
endmacro()

macro(lib_ctest_memcheck)
	if(WITH_MEMCHECK)
		logln("ctest_memcheck(${ARGV})")
		ctest_memcheck(${ARGV} CAPTURE_CMAKE_ERROR _lib_ret)
		handle_ret(ctest_coverage _lib_ret)
	endif()
endmacro()

macro(lib_ctest_coverage)
	logln("ctest_coverage(${ARGV})")
	ctest_coverage(${ARGV} CAPTURE_CMAKE_ERROR _lib_ret)
	handle_ret(ctest_coverage _lib_ret)
endmacro()

macro(lib_ctest_submit)
	if(WITH_SUBMIT)
		logln("ctest_submit(${ARGV} ***)")
		ctest_submit(${ARGV} ${LIB_SUBMIT_FLAGS} RETURN_VALUE _lib_ret)
		handle_ret(ctest_submit _lib_ret)
	else()
		logln("without ctest_submit")
	endif()
endmacro()










