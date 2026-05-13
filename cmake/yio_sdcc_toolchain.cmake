set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER sdcc)
# SDCC compiles .c to .rel
set(CMAKE_C_OUTPUT_EXTENSION ".rel")
set(CMAKE_EXECUTABLE_SUFFIX ".ihx")

# Tell CMake we don't have standard stuff to avoid checking host features
set(CMAKE_C_COMPILER_WORKS 1)
