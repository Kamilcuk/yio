message(STATUS "Configuring for ARM platform, GNU-C compiler, ${CMAKE_SYSTEM_PROCESSOR} processor")
string(APPEND ARM_PROCESSOR_FLAGS " -mcpu=${CMAKE_SYSTEM_PROCESSOR} -mthumb")

