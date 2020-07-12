
include_guard()
message(STATUS "Configuring with arm-none-eabi-gcc toolchain")
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(_common_flags " -ffunction-sections -fdata-sections ")
set(CMAKE_C_FLAGS_INIT " ${_common_flags} ")
set(CMAKE_EXE_LINKER_FLAGS_INIT " ${_common_flags} ")
string(APPEND CMAKE_EXE_LINKER_FLAGS_INIT 
	" -Wl,--gc-sections --specs=rdimon.specs -u_printf_float -u_scanf_float ")
unset(_common_flags)

set(CMAKE_C_FLAGS_DEBUG_INIT " -ggdb3")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "")

set(CMAKE_C_FLAGS_RELEASE_INIT " -Os")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "")

find_program(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
find_program(CMAKE_C_COMPILER    arm-none-eabi-gcc)
find_program(CMAKE_CXX_COMPILER  arm-none-eabi-g++)

include(${CMAKE_CURRENT_LIST_DIR}/crosscompiling.cmake)
