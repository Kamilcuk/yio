
include_guard()
message(STATUS "Configuring with arm-none-eabi-gcc toolchain")
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

if(IS_DIRECTORY /usr/arm-none-eabi)
  set(CMAKE_SYSTEM_PREFIX_PATH  /usr/arm-none-eabi)
endif()

if(IS_DIRECTORY CMAKE_SYSTEM_PREFIX_PATH)
  set(CMAKE_SYSTEM_LIBRARY_PATH ${CMAKE_SYSTEM_PREFIX_PATH}/lib)
  set(CMAKE_SYSTEM_INCLUDE_PATH ${CMAKE_SYSTEM_PREFIX_PATH}/include)
  set(CMAKE_SYSTEM_PROGRAM_PATH ${CMAKE_SYSTEM_PREFIX_PATH}/bin)
endif()

set(_common_flags " -ffunction-sections -fdata-sections")
set(CMAKE_C_FLAGS_INIT " ${_common_flags}")
set(CMAKE_EXE_LINKER_FLAGS_INIT " ${_common_flags}")
unset(_common_flags)
string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT " -Wl,--gc-sections --specs=rdimon.specs")

set(CMAKE_C_DEBUG_FLAGS_INIT " -Og")
set(CMAKE_C_RELEASE_FLAGS_INIT " -Os")

find_program(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
find_program(CMAKE_C_COMPILER    arm-none-eabi-gcc)
find_program(CMAKE_CXX_COMPILER  arm-none-eabi-g++)

include(${CMAKE_CURRENT_LIST_DIR}/crosscompiling.cmake)

get_filename_component(CMAKE_CROSSCOMPILING_EMULATOR
  ${KCMAKELIB_SCRIPTS_DIR}/cmake_crosscompiling_emulator_arm_none_gdb.sh
  ABSOLUTE)

if(ARM_NONE_EABI_GCC_PRINT_FLOAT)
  string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT " -u_printf_float -u_scanf_float")
endif()
if(ARM_NONE_EABI_GCC_RDIMON)
  string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT " --specs=rdimon.specs")
endif()
if(NOT ARM_NONE_EABI_GCC_NO_NEWLIB_NANO)
  string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT " --specs=nano.specs")
endif()


