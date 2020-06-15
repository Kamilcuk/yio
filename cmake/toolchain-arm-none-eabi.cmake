set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_FLAGS_INIT
	" -ffunction-sections -fdata-sections "
	CACHE INTERNAL "")
set(CMAKE_EXE_LINKER_FLAGS_INIT
	" -Wl,--gc-sections --specs=rdimon.specs -u_printf_float -u_scanf_float " 
	CACHE INTERNAL "")

set(CMAKE_C_FLAGS_DEBUG_INIT "-Ogdb3 -g" CACHE INTERNAL "")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "" CACHE INTERNAL "")

set(CMAKE_C_FLAGS_RELEASE_INIT "-Os" CACHE INTERNAL "")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "" CACHE INTERNAL "")

find_program(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
find_program(CMAKE_C_COMPILER    arm-none-eabi-gcc)
find_program(CMAKE_CXX_COMPILER  arm-none-eabi-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)


