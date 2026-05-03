include_guard()
message(STATUS "Configuring with arm-none-eabi-gcc toolchain")
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

find_program(CMAKE_C_COMPILER arm-none-eabi-gcc REQUIRED)
find_program(CMAKE_CXX_COMPILER arm-none-eabi-g++ REQUIRED)

message(STATUS ${CMAKE_C_COMPILER})
# 1. Get the raw path from the compiler
execute_process(
    COMMAND ${CMAKE_C_COMPILER} -print-sysroot
    OUTPUT_VARIABLE PRINT_SYSROOT
    RESULT_VARIABLE PRINT_SYSROOT_RESULT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# 2. Comprehensive check
if(NOT PRINT_SYSROOT_RESULT EQUAL 0)
    message(FATAL_ERROR "Command failed with error code: ${PRINT_SYSROOT_RESULT}")
endif()

if(PRINT_SYSROOT STREQUAL "")
    set(PRINT_SYSROOT "/usr/lib/arm-none-eabi")
endif()

# 3. Convert and validate directory
file(TO_CMAKE_PATH "${PRINT_SYSROOT}" PRINT_SYSROOT)
if(IS_DIRECTORY "${PRINT_SYSROOT}")
    set(CMAKE_SYSROOT "${PRINT_SYSROOT}")
    list(APPEND CMAKE_SYSTEM_PREFIX_PATH "${PRINT_SYSROOT}")
    message(STATUS "Found ARM Sysroot: ${PRINT_SYSROOT}")
    # Tell CMake to search for programs in the host but libs/headers in the sysroot
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
else()
    # If it's a valid string but not a directory, this is a critical configuration error
    message(FATAL_ERROR "The determined sysroot path is not a directory: ${PRINT_SYSROOT}")
endif()

if(IS_DIRECTORY CMAKE_SYSTEM_PREFIX_PATH)
  set(CMAKE_SYSTEM_LIBRARY_PATH ${CMAKE_SYSTEM_PREFIX_PATH}/lib)
  set(CMAKE_SYSTEM_INCLUDE_PATH ${CMAKE_SYSTEM_PREFIX_PATH}/include)
  set(CMAKE_SYSTEM_PROGRAM_PATH ${CMAKE_SYSTEM_PREFIX_PATH}/bin)
endif()

set(CMAKE_C_DEBUG_FLAGS_INIT " -Og -g")
set(CMAKE_C_RELEASE_FLAGS_INIT " -Os")

find_program(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
find_program(CMAKE_C_COMPILER    arm-none-eabi-gcc)
find_program(CMAKE_CXX_COMPILER  arm-none-eabi-g++)

include(${CMAKE_CURRENT_LIST_DIR}/crosscompiling.cmake)

# Change this part
find_program(QEMU_EXECUTABLE qemu-system-arm)

if(NOT CMAKE_CROSSCOMPILING_EMULATOR)
    get_filename_component(CMAKE_CROSSCOMPILING_EMULATOR
      ${KCMAKELIB_SCRIPTS_DIR}/cmake_crosscompiling_emulator_arm_none_gdb.sh
      ABSOLUTE)
endif()
