message(STATUS "--- LOADING ARM QEMU TOOLCHAIN ---")
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

find_program(CMAKE_C_COMPILER arm-none-eabi-gcc REQUIRED)
find_program(CMAKE_CXX_COMPILER arm-none-eabi-g++ REQUIRED)

set(ARM_FLAGS "-mcpu=cortex-m3 -mthumb")
set(CMAKE_C_FLAGS_INIT "${ARM_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${ARM_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${ARM_FLAGS} --specs=rdimon.specs -u_printf_float -u_scanf_float -lc -lrdimon")

set(CMAKE_CROSSCOMPILING_EMULATOR qemu-arm -cpu cortex-m3 -L /usr/arm-none-eabi)
