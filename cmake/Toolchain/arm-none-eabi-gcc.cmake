
include_guard()
message(STATUS "Configuring with arm-none-eabi-gcc toolchain")
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)
set(CMAKE_SYSTEM_PREFIX_PATH  /usr/arm-none-eabi)
set(CMAKE_SYSTEM_LIBRARY_PATH /usr/arm-none-eabi/lib)
set(CMAKE_SYSTEM_INCLUDE_PATH /usr/arm-none-eabi/include)
set(CMAKE_SYSTEM_PROGRAM_PATH /usr/arm-none-eabi/bin)

set(_common_flags " -ffunction-sections -fdata-sections")
set(CMAKE_C_FLAGS_INIT " ${_common_flags}")
set(CMAKE_EXE_LINKER_FLAGS_INIT " ${_common_flags}")
unset(_common_flags)
string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT
	" -Wl,--gc-sections --specs=rdimon.specs -u_printf_float -u_scanf_float")

string(APPEND CMAKE_C_FLAGS " -Os")

find_program(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
find_program(CMAKE_C_COMPILER    arm-none-eabi-gcc)
find_program(CMAKE_CXX_COMPILER  arm-none-eabi-g++)

include(${CMAKE_CURRENT_LIST_DIR}/crosscompiling.cmake)

get_filename_component(CMAKE_CROSSCOMPILING_EMULATOR
	${CMAKE_CURRENT_LIST_DIR}/../../scripts/cmake_crosscompiling_emulator_arm_none_gdb.sh ABSOLUTE)


