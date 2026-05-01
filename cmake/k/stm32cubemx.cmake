
function(cubemx_make_get_variable result_var variable cubemx_dir)
  execute_process(
    COMMAND /bin/bash -c "make -s -f Makefile -f <(echo -e 'printvar-%: ; $(info $($*))') printvar-${variable}"
    WORKING_DIRECTORY "${cubemx_dir}"
    RESULT_VARIABLE ret
    OUTPUT_VARIABLE temp
  )
  if(ret)
    message(FATAL_ERROR "cubemx_make_get_variable make returned ${ret}")
  endif()
  set(${result_var} "${temp}" PARENT_SCOPE)
endfunction()

function(cubemx_get_asm_sources result_var cubemx_dir)
  file(GLOB temp ${cubemx_dir}/*.s)
  set(${result_var} "${temp}" PARENT_SCOPE)
endfunction()

function(cubemx_get_c_sources result_var cubemx_dir halrepo_dir)
  cubemx_make_get_variable(mx_c_src C_SOURCES "${cubemx_dir}")
  string(REPLACE " " ";" mx_c_src "${mx_c_src}")
  set(tmp "")
  foreach(ii ${mx_c_src})
    if (${ii} MATCHES ".*/STM32Cube_FW_*")
      string(REGEX REPLACE ".*/STM32Cube_FW_[^/]+/" "${halrepo_dir}/" ii "${ii}")
      set(ii "${CMAKE_SOURCE_DIR}/${ii}")
    else()
      set(ii "${cubemx_dir}/${ii}")
    endif()
    string(REGEX REPLACE "\n$" "" ii "${ii}")
    if (NOT EXISTS ${ii})
      MESSAGE(FATAL_ERROR "${ii} does not exists")
    endif()
    list(APPEND tmp ${ii})
  endforeach()
  set(${result_var} "${tmp}" PARENT_SCOPE)
endfunction()

function(cubemx_get_c_includes result_var cubemx_dir halrepo_dir)
  cubemx_make_get_variable(mx_c_inc C_INCLUDES "${cubemx_dir}")
  string(REPLACE "-I" "" mx_c_inc "${mx_c_inc}")
  string(REPLACE " " ";" mx_c_inc "${mx_c_inc}")
  list(REVERSE mx_c_inc)
  set(tmp "")
  foreach(ii ${mx_c_inc})
    if (${ii} MATCHES ".*/STM32Cube_FW_*")
      string(REGEX REPLACE ".*/STM32Cube_FW_[^/]+" "${halrepo_dir}/" ii "${ii}")
      string(REGEX REPLACE "\n$" "" ii "${ii}")
      set(i "${CMAKE_SOURCE_DIR}/${ii}")
      if (NOT EXISTS ${ii})
        MESSAGE(FATAL_ERROR ${ii} does not exists)
      endif()
      list(APPEND tmp "${ii}")
    endif()
  endforeach()
  set(${result_var} "${tmp}" PARENT_SCOPE)
endfunction()

function(cubemx_get_linker_script result_var cubemx_dir)
  cubemx_make_get_variable(temp LDSCRIPT "${cubemx_dir}")
  string(REGEX REPLACE "\n$" "" temp "${temp}")
  set(${result_var} "${cubemx_dir}/${temp}" PARENT_SCOPE)
endfunction()

function(cubemx_add_hal_library target type c_inc_output_var cubemx_dir halrepo_dir)
  cubemx_get_asm_sources(mx_asm_src ${cubemx_dir})
  cubemx_get_c_sources(mx_c_src ${cubemx_dir} ${halrepo_dir})
  cubemx_get_c_includes(mx_c_inc ${cubemx_dir} ${halrepo_dir})
  list(APPEND mx_c_inc "${cubemx_dir}/Inc")
  add_library(${target} ${type} ${mx_asm_src} ${mx_c_src})
  target_include_directories(${target} PUBLIC ${mx_c_inc})
  set(${c_inc_output_var} "${mx_c_inc}" PARENT_SCOPE)
endfunction()

function(cubemx_add_gen_include_header_target target result_file cubemx_dir)
  file(GLOB mx_headers "${cubemx_dir}/Inc/*.h")
  get_filename_component(result_file_dir "${result_file}" DIRECTORY)
  file(GLOB mx_headers_relative RELATIVE "${result_file_dir}" "${cubemx_dir}/Inc/*.h")
  add_custom_command(
    OUTPUT "${result_file}"
    COMMAND echo "/* this file is autogonerated by ${CMAKE_CURRENT_LIST_FILE} cmake script */" > "${result_file}"
    COMMAND printf "#include \"%s\"\\n" ${mx_headers_relative} >> "${result_file}"
    DEPENDS ${mx_headers}
    COMMENT "Generate ${result_file} from cubemx headers"
    VERBATIM
  )
  add_custom_target(${target}
    COMMENT "Generate ${result_file} from cubemx headers"
    DEPENDS "${result_file}")
endfunction()

function(cubemx_add_gen_cubemx_header_target target result_file cubemx_dir)
  file(GLOB cubemx_c_src ${cubemx_dir}/Src/*.c)
  get_filename_component(result_dir "${result_file}" DIRECTORY)
  add_custom_command(
    OUTPUT "${result_file}"
    COMMAND mkdir -p "${result_dir}"
    COMMAND printf "%s\\n"
    "/**"
    " * This file was generated automatically by ${CMAKE_CURRENT_LIST_FILE} cmake script."
    " */"
    ""
    "/* handles and functions extracted from cubemx files */"
    > "${result_file}"
    COMMAND
    grep -h -a "^[^[:space:]]*_HandleTypeDef \\|^void MX_[^[:space:]]*_Init(void)" -- ${cubemx_c_src} |
    sort -u |
    sed
    -e "s/\\r//" # remove windows newlines
    -e "/_HandleTypeDef/s/^/extern /" # prepend HandleTypeDef variables with extern
    -e "/^void MX_/s/$/;/" # append missing semicolon to functions
    >> "${result_file}"
    COMMAND printf "%s\\n"
    ""
    >> "${result_file}"
    DEPENDS ${cubemx_c_src}
    COMMENT "Generate ${result_file} from cubemx headers"
    VERBATIM
  )
  add_custom_target(${target}
    COMMENT "Generate ${result_file} from cubemx headers"
    DEPENDS "${result_file}"
  )
  set(${result_var} ${res} PARENT_SCOPE)
endfunction()

