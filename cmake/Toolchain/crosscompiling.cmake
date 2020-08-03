
set(CMAKE_CROSSCOMPILING TRUE)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
unset(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE)

set(CMAKE_IGNORE_PATH /bin /lib /include)

#CMake Warning (dev) at /usr/share/cmake-3.17/Modules/GNUInstallDirs.cmake:225 (message):
#  Unable to determine default CMAKE_INSTALL_LIBDIR directory because no
#  target architecture is known.  Please enable at least one language before
#  including GNUInstallDirs.
#Call Stack (most recent call first):
#  src/CMakeLists.txt:145 (include)
#This warning is for project developers.  Use -Wno-dev to suppress it.
set(CMAKE_INSTALL_LIBDIR "lib")
