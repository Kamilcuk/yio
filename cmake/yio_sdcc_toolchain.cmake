set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_C_COMPILER sdcc)
# SDCC compiles .c to .rel
set(CMAKE_C_OUTPUT_EXTENSION ".rel")
set(CMAKE_EXECUTABLE_SUFFIX ".ihx")

if(CMAKE_C_FLAGS MATCHES "-mstm8")
    set(CMAKE_CROSSCOMPILING_EMULATOR ucsim_stm8 -Sout=- -G)
elseif(CMAKE_C_FLAGS MATCHES "-mz80")
    set(CMAKE_CROSSCOMPILING_EMULATOR ucsim_z80 -Sout=- -G)
elseif(CMAKE_C_FLAGS MATCHES "-mmos6502")
    set(CMAKE_CROSSCOMPILING_EMULATOR ucsim_mos6502 -Sout=- -G)
else()
    set(CMAKE_CROSSCOMPILING_EMULATOR s51 -Sout=- -I "if=xram[0xffff]" -G)
endif()

# Tell CMake we don't have standard stuff to avoid checking host features
set(CMAKE_C_COMPILER_WORKS 1)
