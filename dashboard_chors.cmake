 # dashboard_chors.cmake
include(${CTEST_SOURCE_DIRECTORY}/cmake/dashboard_lib.cmake RESULT_VARIABLE ret)
if(NOT ret)
	message(FATAL_ERROR "include dashboard_lib.cmake failed ret=${ret}")
endif()

cmake_host_system_information(RESULT CTEST_SITE QUERY HOSTNAME)

set(CTEST_CONFIGURATION_TYPE Release)

set(CTEST_CMAKE_GENERATOR "Ninja")

cmake_host_system_information(RESULT NUMBER_OF_LOGICAL_CORES QUERY NUMBER_OF_LOGICAL_CORES)
set(CTEST_PARALLEL_LEVEL ${NUMBER_OF_LOGICAL_CORES})

# Setup configure flags
list(APPEND CONFIGURE_FLAGS "-DBUILD_TESTING=1")
list(APPEND CONFIGURE_FLAGS "-DYIO_BUILD_TESTING=1")
foreach(i IN ITEMS CMAKE_TOOLCHAIN_FILE CMAKE_CROSSCOMPILING_EMULATOR)
	if(DEFINED ${i})
		list(APPEND CONFIGURE_FLAGS "-D${i}=${${i}}")
	endif()
endforeach()

find_program(CTEST_UPDATE_COMMAND NAMES git)
find_program(CTEST_COVERAGE_COMMAND NAMES gcov)
find_program(CTEST_MEMORYCHECK_COMMAND NAMES valgrind)
	
# main ####################################################

set_default(MODEL "Experimental")
set_default(SYSTEM "Linux-x86-64-$ENV{CC}")
set_default(WITH_SUBMIT OFF)
set_default(WITH_UPDATE OFF)
if(SYSTEM MATCHES "armv4t")
	set_default(WITH_MEMCHECK OFF)
	set_default(WITH_COVERAGE OFF)
	set_default(WITH_SANITIZE ON)
else()
	set_default(WITH_MEMCHECK ON)
	set_default(WITH_COVERAGE ON)
	set_default(WITH_SANITIZE ON)
endif()
if(CMAKE_CROSSCOMPILING_EMULATOR)
	set(WITH_MEMCHECK OFF)
endif()

# Debug variables ##############################

debug_variables("dashboard:"
	CTEST_SOURCE_DIRECTORY
	CTEST_BINARY_DIRECTORY
	CTEST_CONFIGURATION_TYPE
	CTEST_CMAKE_GENERATOR
	CTEST_SITE
	CTEST_CONFIGURE_COMMAND
	CTEST_COVERAGE_COMMAND
	CTEST_MEMORYCHECK_COMMAND
	CMAKE_TOOLCHAIN_FILE
	CMAKE_CROSSCOMPILING_EMULATOR
	CTEST_PARALLEL_LEVEL
	MODEL
	SYSTEM
	CONFIGURE_FLAGS
	WITH_UPDATE
	WITH_MEMCHECK
	WITH_COVERAGE
	WITH_SANITIZE
	WITH_SUBMIT
)

# Main ##############################################

if(WITH_SUBMIT)
	if(DEFINED ENV{CDASH_KARTA_DYZIO_PL_PASSWORD})
		log("submit: Using Authorization: Bearer")
		set(LIB_SUBMIT_FLAGS HTTPHEADER 
			"Authorization: Bearer $ENV{CDASH_KARTA_DYZIO_PL_PASSWORD}")
	endif()
endif()

set(CTEST_BUILD_NAME "${SYSTEM}")	
lib_ctest_empty_binary_directory(${CTEST_BINARY_DIRECTORY})
lib_ctest_start(${MODEL})
lib_ctest_update()
lib_ctest_configure("${CONFIGURE_FLAGS}" "")
lib_ctest_build()
lib_ctest_test()	
lib_ctest_memcheck(EXCLUDE_LABEL nomemcheck)
lib_ctest_submit(PARTS Start Update Configure Build Test Memcheck)

# Coverage results are appended to 
if(WITH_COVERAGE)
	lib_ctest_start(APPEND)	
	lib_ctest_configure("${CONFIGURE_FLAGS}" "-g --coverage" APPEND)
	lib_ctest_build(APPEND)
	lib_ctest_test(APPEND)
	lib_ctest_coverage(APPEND)
	lib_ctest_submit(PARTS Coverage)
endif()
	
if(WITH_SANITIZE)
	set(CTEST_BUILD_NAME "${SYSTEM}-sanitize")
	lib_ctest_empty_binary_directory(${CTEST_BINARY_DIRECTORY})	
	lib_ctest_start(${MODEL} GROUP Sanitize)	
	set(sanitize_flags "-fsanitize=address -fsanitize=undefined -fsanitize=leak -fsanitize=pointer-subtract -fsanitize=pointer-compare -fno-omit-frame-pointer -fstack-protector-all -fstack-clash-protection -fcf-protection")
	lib_ctest_configure("${CONFIGURE_FLAGS}" "${sanitize_flags}")
	lib_ctest_build()
	lib_ctest_test()
	lib_ctest_submit()
endif()
