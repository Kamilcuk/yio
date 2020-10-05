
include_guard()
message(STATUS "Configuring with sdcc pic16 toolchain")
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR PIC16)

set(_common_flags " -mpic16 --use-non-free -Dstderr=stdout")
set(CMAKE_C_FLAGS " ${_common_flags} ")
set(CMAKE_EXE_LINKER_FLAGS " ${_common_flags} ")
unset(_common_flags)

set(CMAKE_C_FLAGS_DEBUG_INIT "")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "")

set(CMAKE_C_FLAGS_RELEASE_INIT "")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "")

find_program(CMAKE_ASM_COMPILER  sdcc)
find_program(CMAKE_C_COMPILER    sdcc)

include(${CMAKE_CURRENT_LIST_DIR}/crosscompiling.cmake)

set(_yIO_HAS_WCHAR_H 0 CACHE BOOL "")

