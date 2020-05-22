
include(${CMAKE_CURRENT_LIST_DIR}/debug_variables.cmake)

macro(set_default)
	if(NOT DEFINED "${ARGV0}")
		set("${ARGV0}" "${ARGV1}")
	endif()
endmacro()

macro(log)
	message(STATUS ">>>>>>>>>> dashboard: ${ARGV}")
endmacro()
macro(logln)
	message("")
	log(${ARGV})
endmacro()

macro(handle_ret _handle_ret_func _handle_ret_ret)
	if(NOT WITH_SUBMIT OR (WITH_SUBMIT AND "${_handle_ret_func}" STREQUAL "ctest_submit"))
		if(NOT "${${_handle_ret_ret}}" EQUAL 0)
			message(FATAL_ERROR ">>>>>>>>>> dashboard_lib: ${_handle_ret_func} failed")
		endif()
	endif()
endmacro()

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
		ctest_update(${ARGV} CAPTURE_CMAKE_ERROR ret)
		handle_ret(ctest_update ret)
	endif()
endmacro()

macro(lib_ctest_configure configure_flags c_flags)
	logln("ctest_configure(OPTIONS ${configure_flags};-DCMAKE_C_FLAGS=${c_flags} ${ARGN})")
	ctest_configure(OPTIONS "${configure_flags};-DCMAKE_C_FLAGS=${c_flags};-DCMAKE_VERBOSE_MAKEFILE=1" ${ARGN}
		CAPTURE_CMAKE_ERROR ret)
	handle_ret(ctest_configure ret)
endmacro()

macro(lib_ctest_build)
	logln("ctest_build(${ARGV})")
	ctest_build(${ARGV} CAPTURE_CMAKE_ERROR ret)
	handle_ret(ctest_build ret)
endmacro()

macro(lib_ctest_test)
	logln("ctest_test(${ARGV})")
	ctest_test(${ARGV} CAPTURE_CMAKE_ERROR ret)
	handle_ret(ctest_test ret)
endmacro()

macro(lib_ctest_memcheck)
	if(WITH_MEMCHECK)
		logln("ctest_memcheck(${ARGV})")
		ctest_memcheck(${ARGV} CAPTURE_CMAKE_ERROR ret)
		handle_ret(ctest_coverage ret)
	endif()
endmacro()

macro(lib_ctest_coverage)
	logln("ctest_coverage(${ARGV})")
	ctest_coverage(${ARGV} CAPTURE_CMAKE_ERROR ret)
	handle_ret(ctest_coverage ret)
endmacro()

macro(lib_ctest_submit)
	if(WITH_SUBMIT)
		logln("ctest_submit(${ARGV} ...)")
		ctest_submit(${ARGV} ${LIB_SUBMIT_FLAGS} RETURN_VALUE ret)
		handle_ret(ctest_submit ret)
	else()
		logln("without ctest_submit")
	endif()
endmacro()










