cmake_minimum_required(VERSION 3.10)
project(YioGccPlugin CXX C)

enable_testing()

# Assert C compilers are GCC/G++
if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "This plugin requires GCC for testing. Found: ${CMAKE_C_COMPILER_ID}")
endif()
if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "This plugin requires G++ for building. Found: ${CMAKE_CXX_COMPILER_ID}")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fno-rtti -fno-exceptions -Wno-literal-suffix")

execute_process(
    COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=plugin
    OUTPUT_VARIABLE GCC_PLUGIN_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "GCC Plugin directory: ${GCC_PLUGIN_DIR}")

set(GCC_PLUGIN_INCLUDE_DIR "${GCC_PLUGIN_DIR}/include")
include_directories(${GCC_PLUGIN_INCLUDE_DIR})

# Add multiarch include paths for GMP and others
execute_process(
    COMMAND ${CMAKE_C_COMPILER} -print-multiarch
    OUTPUT_VARIABLE GCC_MULTIARCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(GCC_MULTIARCH)
    include_directories(/usr/include/${GCC_MULTIARCH})
endif()
include_directories(/usr/include/x86_64-linux-gnu)

add_library(yio_plugin MODULE src/yio_plugin.cc)
set_target_properties(yio_plugin PROPERTIES PREFIX "")
add_dependencies(yio_plugin yio_gen)

# Include the test extraction tool
include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/k/test_extract_properties_from_file.cmake)

if(NOT TARGET yio)
    set(YIO_DEV OFF CACHE BOOL "" FORCE)
    set(YIO_BUILD_TESTING OFF CACHE BOOL "" FORCE)
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
    add_subdirectory(../../ ${CMAKE_CURRENT_BINARY_DIR}/yio EXCLUDE_FROM_ALL)
endif()

# Find include dir
set(YIO_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/yio/gen)

function(add_yio_test name file)
    set(comp_test "${name}_comp")
    add_test(NAME ${comp_test}
        COMMAND ${CMAKE_C_COMPILER} -fplugin=$<TARGET_FILE:yio_plugin> 
            -I ${YIO_GEN_DIR}
            -I ${YIO_GEN_DIR}/yio
            -I ${CMAKE_CURRENT_SOURCE_DIR}/test 
            -c ${CMAKE_CURRENT_SOURCE_DIR}/${file} -o /dev/null
    )

    # Extract properties like PASS_REGULAR_EXPRESSION from the C file
    test_extract_properties_from_file(yio_plugin ${comp_test} ${CMAKE_CURRENT_SOURCE_DIR}/${file})

    # Check if the test should also be run
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/${file}" content)
    if(NOT content MATCHES "WILL_FAIL" AND NOT name MATCHES "too_few|too_many|validation")
        set(exec_name "${name}_exe")
        add_executable(${exec_name} ${file} test/conftest.c)
        add_dependencies(${exec_name} yio_gen)
        target_include_directories(${exec_name} PRIVATE 
            ${YIO_GEN_DIR}
            ${YIO_GEN_DIR}/yio
            ${CMAKE_CURRENT_SOURCE_DIR}/test 
        )
        set_target_properties(${exec_name} PROPERTIES C_STANDARD 11)
        target_compile_options(${exec_name} PRIVATE "-fplugin=$<TARGET_FILE:yio_plugin>")
        
        add_test(NAME ${name} COMMAND ${exec_name})
        test_extract_properties_from_file(${exec_name} ${name} ${CMAKE_CURRENT_SOURCE_DIR}/${file} PREFIX "^[ \t]*//+[ \t]*test_")
    endif()
endfunction()

add_yio_test(test_too_few test/test_too_few_args.c)
add_yio_test(test_too_many test/test_too_many_args.c)
add_yio_test(test_validation test/test_validation.c)

# Reuse testprogram logic for the plugin
set_from_env(TESTPROGRAM)
if(DEFINED TESTPROGRAM AND NOT TESTPROGRAM STREQUAL "")
    set(snippet_c "${CMAKE_CURRENT_BINARY_DIR}/yio_plugin_testprogram.c")
    if(TESTPROGRAM MATCHES "main[ \t\r\n]*\\(")
        set(source "#include <yio.h>\n#include <stdio.h>\n${TESTPROGRAM}\n")
    else()
        set(source "#include <yio.h>\n#include <stdio.h>\nint main() {\n  ${TESTPROGRAM}\n  return 0;\n}\n")
    endif()
    file(WRITE "${snippet_c}" "${source}")
    add_executable(yio_plugin_testprogram EXCLUDE_FROM_ALL "${snippet_c}")
    add_dependencies(yio_plugin_testprogram yio_plugin yio_gen)
    target_link_libraries(yio_plugin_testprogram PRIVATE yio)
    target_include_directories(yio_plugin_testprogram PRIVATE 
        ${YIO_GEN_DIR}
        ${YIO_GEN_DIR}/yio
    )
    target_compile_options(yio_plugin_testprogram PRIVATE "-fplugin=$<TARGET_FILE:yio_plugin>")
endif()
