# SDCC.cmake

include_guard()
message(STATUS "Configuring for SDCC-C")
include(Compiler/CMakeCommonCompilerMacros)

set(CMAKE_C_VERBOSE_FLAG "-V") 

string(APPEND CMAKE_C_FLAGS_MINSIZEREL_INIT " -DNDEBUG")
string(APPEND CMAKE_C_FLAGS_RELEASE_INIT " -DNDEBUG")
string(APPEND CMAKE_C_FLAGS_RELWITHDEBINFO_INIT " -DNDEBUG")

set(CMAKE_C90_STANDARD_COMPILE_OPTION "--std-c89")
set(CMAKE_C90_EXTENSION_COMPILE_OPTION "--std-sdcc89")
set(CMAKE_C90_STANDARD__HAS_FULL_SUPPORT ON)

set(CMAKE_C99_STANDARD_COMPILE_OPTION "--std-c99")
set(CMAKE_C99_EXTENSION_COMPILE_OPTION "--std-sdcc99")
set(CMAKE_C99_STANDARD__HAS_FULL_SUPPORT ON)

set(CMAKE_C11_STANDARD_COMPILE_OPTION "--std-c11")
set(CMAKE_C11_EXTENSION_COMPILE_OPTION "--std-sdcc11")
set(CMAKE_C11_STANDARD__HAS_FULL_SUPPORT ON)

set(CMAKE_C_STANDARD_DEFAULT "11")
