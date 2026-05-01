
include(CMakeFindBinUtils)

function(add_bin_target target)
  add_custom_target(${exetarget}.hex ALL
    COMMAND "${CMAKE_OBJCOPY}" -O ihex "$<TARGET_FILE:${exetarget}>" "${exetarget}.hex"
    WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    DEPENDS "${exetarget}"
    COMMENT "Generating ${CMAKE_CURRENT_BINARY_DIR}/${exetarget}.hex"
  )
endfunction()

function(add_hex_target exetarget)
  add_custom_target(${exetarget}.hex ALL
    COMMAND "${CMAKE_OBJCOPY}" -O ihex "$<TARGET_FILE:${exetarget}>" "${exetarget}.hex"
    WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    DEPENDS "${exetarget}"
    COMMENT "Generating ${CMAKE_CURRENT_BINARY_DIR}/${exetarget}.hex"
  )
endfunction()

function(print_executable_size exetarget)
  add_custom_command(TARGET "${exetarget}" POST_BUILD
    COMMAND "${CMAKE_SIZE_UTIL}" -B "$<TARGET_FILE:${exetarget}>"
    COMMENT "Printing ${exetarget} size information"
  )
endfunction()

function(add_print_executable_size_target name exetarget)
  add_custom_target("${name}"
    COMMAND "${CMAKE_SIZE_UTIL}" -B "$<TARGET_FILE:${exetarget}>"
    COMMENT "Printing ${exetarget} size information"
    DEPENDS "${exetarget}"
  )
endfunction()
