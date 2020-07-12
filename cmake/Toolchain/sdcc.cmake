
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR default)
set(CMAKE_CROSSCOMPILING TRUE)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(_common_flags " -mpic16 --use-non-free --std-sdcc2x -Dstderr=stdout")
set(CMAKE_C_FLAGS
	" ${_common_flags} "
	CACHE INTERNAL "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS
	" ${_common_flags} "
	CACHE INTERNAL "" FORCE)
unset(_common_flags)

set(CMAKE_C_FLAGS_DEBUG_INIT "" CACHE INTERNAL "")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "" CACHE INTERNAL "")

set(CMAKE_C_FLAGS_RELEASE_INIT "" CACHE INTERNAL "")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "" CACHE INTERNAL "")

find_program(CMAKE_ASM_COMPILER  sdcc)
find_program(CMAKE_C_COMPILER    sdcc)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_IGNORE_PATH /bin /lib /include)

set(CMAKE_C_STANDARD_DEFAULT "")

set(CMAKE_C90_STANDARD_COMPILE_OPTION "--std-c89")
set(CMAKE_C90_EXTENSION_COMPILE_OPTION "--std-sdcc89")
set(CMAKE_C90_STANDARD__HAS_FULL_SUPPORT ON)

set(CMAKE_C99_STANDARD_COMPILE_OPTION "--std-c99")
set(CMAKE_C99_EXTENSION_COMPILE_OPTION "--std-sdcc99")
set(CMAKE_C99_STANDARD__HAS_FULL_SUPPORT ON)

set(CMAKE_C11_STANDARD_COMPILE_OPTION "--std-c11")
set(CMAKE_C11_EXTENSION_COMPILE_OPTION "--std-sdcc11")
set(CMAKE_C11_STANDARD__HAS_FULL_SUPPORT ON)

include(Compiler/CMakeCommonCompilerMacros)

#CMake Warning (dev) at /usr/share/cmake-3.17/Modules/GNUInstallDirs.cmake:225 (message):
#  Unable to determine default CMAKE_INSTALL_LIBDIR directory because no
#  target architecture is known.  Please enable at least one language before
#  including GNUInstallDirs.
#Call Stack (most recent call first):
#  src/CMakeLists.txt:145 (include)
#This warning is for project developers.  Use -Wno-dev to suppress it.
set(CMAKE_INSTALL_LIBDIR "lib")

