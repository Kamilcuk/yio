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
set(COMMON_FLAGS -Werror)

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

add_library(fstring_plugin SHARED src/fstring_plugin.cc)
target_compile_options(fstring_plugin PRIVATE ${COMMON_FLAGS} -fPIC -fno-rtti -fno-exceptions -Wno-literal-suffix)
set_target_properties(fstring_plugin PROPERTIES PREFIX "")
add_dependencies(fstring_plugin yio_gen)

# Proxy file to enforce dependency for Ninja
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fstring_plugin_ready
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/fstring_plugin_ready
    DEPENDS fstring_plugin
)
add_custom_target(fstring_plugin_ready_target DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/fstring_plugin_ready)

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
    set(cmd ${CMAKE_C_COMPILER} ${COMMON_FLAGS} -fplugin=$<TARGET_FILE:fstring_plugin> 
            -I ${YIO_GEN_DIR}
            -I ${YIO_GEN_DIR}/yio
            -I ${CMAKE_CURRENT_SOURCE_DIR}/test 
            -c ${CMAKE_CURRENT_SOURCE_DIR}/${file} -o /dev/null)
    if(file MATCHES "noncompilable/")
        add_test(NAME ${comp_test} COMMAND sh -c "! \"$@\"" -- ${cmd})
    else()
        add_test(NAME ${comp_test} COMMAND ${cmd})
    endif()

    # Extract properties like PASS_REGULAR_EXPRESSION from the C file
    test_extract_properties_from_file(fstring_plugin ${comp_test} ${CMAKE_CURRENT_SOURCE_DIR}/${file} PREFIX "//// COMPILATION")

    # Check if the test should also be run
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/${file}" content)
    if(NOT content MATCHES "WILL_FAIL" AND NOT file MATCHES "noncompilable/|too_few|too_many")
        set(exec_name "${name}_exe")
        add_executable(${exec_name} ${file} test/conftest.c)
        add_dependencies(${exec_name} yio_gen fstring_plugin_ready_target)
        set_source_files_properties(${file} test/conftest.c PROPERTIES OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/fstring_plugin_ready)
        target_include_directories(${exec_name} PRIVATE 
            ${YIO_GEN_DIR}
            ${YIO_GEN_DIR}/yio
            ${CMAKE_CURRENT_SOURCE_DIR}/test 
        )
        set_target_properties(${exec_name} PROPERTIES C_STANDARD 11)
        target_link_libraries(${exec_name} PRIVATE yio)
        target_compile_options(${exec_name} PRIVATE ${COMMON_FLAGS} "-fplugin=$<TARGET_FILE:fstring_plugin>")
        
        add_test(NAME ${name} COMMAND ${exec_name})
        test_extract_properties_from_file(${exec_name} ${name} ${CMAKE_CURRENT_SOURCE_DIR}/${file} PREFIX "//// RUN")
    endif()
endfunction()

file(GLOB_RECURSE TEST_FILES "test/test_*.c" "test/**/test_*.c")
foreach(test_path ${TEST_FILES})
    file(RELATIVE_PATH rel_path ${CMAKE_CURRENT_SOURCE_DIR} ${test_path})
    string(REPLACE "/" "_" test_name ${rel_path})
    string(REPLACE ".c" "" test_name ${test_name})
    add_yio_test(${test_name} ${rel_path})
endforeach()

add_test(NAME test_plugin_flag_disable
    COMMAND ${CMAKE_C_COMPILER} ${COMMON_FLAGS} -fplugin=$<TARGET_FILE:fstring_plugin> 
        -fplugin-arg-fstring_plugin-Wfstring-format=0
        -I ${YIO_GEN_DIR}
        -I ${YIO_GEN_DIR}/yio
        -I ${CMAKE_CURRENT_SOURCE_DIR}/test 
        -c ${CMAKE_CURRENT_SOURCE_DIR}/test/plugin_flags.c -o /dev/null
)

add_test(NAME test_plugin_flag_error
    COMMAND sh -c "! \"$@\"" -- ${CMAKE_C_COMPILER} -fplugin=$<TARGET_FILE:fstring_plugin> 
        -fplugin-arg-fstring_plugin-Werror-fstring-format=1
        -I ${YIO_GEN_DIR}
        -I ${YIO_GEN_DIR}/yio
        -I ${CMAKE_CURRENT_SOURCE_DIR}/test 
        -c ${CMAKE_CURRENT_SOURCE_DIR}/test/plugin_flags.c -o /dev/null
)
test_extract_properties_from_file(fstring_plugin test_plugin_flag_error ${CMAKE_CURRENT_SOURCE_DIR}/test/plugin_flags.c PREFIX "//// COMPILATION")

# Reuse testprogram logic for the plugin
set_from_env(TESTPROGRAM)
if(DEFINED TESTPROGRAM AND NOT TESTPROGRAM STREQUAL "")
    set(snippet_c "${CMAKE_CURRENT_BINARY_DIR}/fstring_plugin_testprogram.c")
    if(TESTPROGRAM MATCHES "main[ \t\r\n]*\\(")
        set(source "#include <yio.h>\n#include <stdio.h>\n${TESTPROGRAM}\n")
    else()
        set(source "#include <yio.h>\n#include <stdio.h>\nint main() {\n  ${TESTPROGRAM}\n  return 0;\n}\n")
    endif()
    file(WRITE "${snippet_c}" "${source}")
    add_executable(fstring_plugin_testprogram EXCLUDE_FROM_ALL "${snippet_c}")
    add_dependencies(fstring_plugin_testprogram fstring_plugin yio_gen)
    target_link_libraries(fstring_plugin_testprogram PRIVATE yio)
    target_include_directories(fstring_plugin_testprogram PRIVATE 
        ${YIO_GEN_DIR}
        ${YIO_GEN_DIR}/yio
    )
    target_compile_options(fstring_plugin_testprogram PRIVATE ${COMMON_FLAGS} "-fplugin=$<TARGET_FILE:fstring_plugin>")
endif()
