
set(_JINJA2_PYTHON_SCRIPT  "${CMAKE_CURRENT_LIST_DIR}/jinja2_script.py")

set(_JINJA2_SEARCHPATH)
macro(JINJA2_ADD_SEARCHPATH)
  list(APPEND _JINJA2_SEARCHPATH ${ARGN})
endmacro()

# Preprocesss the file with python with jinja2.
function(jinja2_render)
  set(jinja_env_options
    block_start_string
    block_end_string
    variable_start_string
    variable_end_string
    comment_start_string
    comment_end_string
    line_statement_prefix
    line_comment_prefix
    trim_blocks
    lstrip_blocks
    newline_sequence
    keep_trailing_newline
    extensions
    optimized
    #undefined
    #finalize
    #autoescape
    #loader
    cache_size
    auto_reload
    #bytecode_cache
    enable_async
  )
  set(one_arg_options
    INPUT
    OUTPUT
    ${jinja_env_options}
    render_options
  )
  set(multi_arg_options
    SEARCHPATH
    DEPENDS
  )
  cmake_parse_arguments(A "" "${one_arg_options}" "${multi_arg_options}" "${ARGN}")

  set(args)
  if(A_SEARCHPATH)
    foreach(ii IN LISTS A_SEARCHPATH)
      list(APPEND args -s "${A_SEARCHPATH}")
    endforeach()
  endif()
  if(A_render_options)
    list(APPEND args -r "${A_render_options}")
  endif()
  foreach(op IN LISTS jinja_env_options)
    if(A_${op})
      #message(${op})
      list(APPEND args -e "${op}=${A_${op}}")
    endif()
  endforeach()
  if(NOT A_INPUT AND NOT A_OUTPUT AND A_UNPARSED_ARGUMENTS)
    if(NOT A_UNPARSED_ARGUMENTS)
      message(FATAL_ERROR "jinja2_render: Missing OUTPUT and INPUT arguments")
    endif()
    list(POP_FRONT A_UNPARSED_ARGUMENTS A_INPUT A_OUTPUT)
  endif()
  if(A_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "jinja2_render: Invalid arguments: ${A_UNPARSED_ARGUMENTS}")
  endif()

  find_program(PYTHON3 NAMES python3 python)
  add_custom_command(
    OUTPUT "${A_OUTPUT}"
    COMMAND "${PYTHON3}" "${_JINJA2_PYTHON_SCRIPT}" ${args} -- "${A_INPUT}" "${A_OUTPUT}"
    DEPENDS "${_JINJA2_PYTHON_SCRIPT}" "${A_INPUT}" ${A_DEPENDS}
    COMMENT "Generating ${A_OUTPUT} from jinja2 processing of ${A_INPUT}"
    VERBATIM
  )
endfunction()

if(KCMAKELIB_TEST)
  set(bindir ${CMAKE_CURRENT_BINARY_DIR}/jinja2)
  file(MAKE_DIRECTORY ${bindir})
  file(WRITE ${bindir}/in.txt [=[
{% set i = 1 %}
{{i}}
]=])
  file(WRITE ${bindir}/CMakeLists.txt [=[
    cmake_minimum_required(VERSION 3.1)
    project(test_jinja2 LANGUAGES)
    include(${THIS_FILE})
    jinja2_render(
      ${CMAKE_CURRENT_SOURCE_DIR}/in.txt
      ${CMAKE_CURRENT_BINARY_DIR}/out.txt
      trim_blocks True
      lstrip_blocks True
    )
    add_custom_target(build ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/out.txt)
  ]=])
  file(WRITE ${bindir}/test.cmake [=[
    if(EXISTS _build/out.txt)
      file(REMOVE _build/out.txt)
    endif()
    execute_process(
      COMMAND ${CMAKE_COMMAND} -S . -B _build -D THIS_FILE=${THIS_FILE}
      RESULT_VARIABLE res
      OUTPUT_VARIABLE out
      ERROR_VARIABLE err
      WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )
    message("${out}")
    message("${err}")
    if(res)
      message(FATAL_ERROR "Configure failed: ${res}")
    endif()
    execute_process(
      COMMAND ${CMAKE_COMMAND} --build _build --verbose
      RESULT_VARIABLE res
      OUTPUT_VARIABLE out
      ERROR_VARIABLE err
      WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    )
    message("${out}")
    message("${err}")
    if(res)
      message(FATAL_ERROR "Build failed: ${res}")
    endif()
    file(READ _build/out.txt output)
    if(NOT "${output}" STREQUAL "1\n")
      message(FATAL_ERROR "${output} is not 1")
    endif()
  ]=])
  add_test(
    NAME jinja2_render
    COMMAND ${CMAKE_COMMAND} -D THIS_FILE=${CMAKE_CURRENT_LIST_FILE} -P ${bindir}/test.cmake
    WORKING_DIRECTORY ${bindir}
  )
endif()


