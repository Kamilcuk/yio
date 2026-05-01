
macro(KCUK_GETNAME name)
  get_filename_component(${name} ${CMAKE_CURRENT_LIST_DIR} NAME)
  string(PREPEND ${name} "kcuk_")
endmacro()

macro(KCUK_SRC_COMMON)

  kcuk_getname(name)

  file(GLOB_RECURSE ${name}_src ${CMAKE_CURRENT_LIST_DIR}/*.c)
  set(${name}_src ${${name}_src} PARENT_SCOPE)
  add_library(${name}_objects OBJECT ${${name}_src})
  target_include_directories(${name}_objects PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
  target_include_directories(${name}_objects PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  get_property(${name}_incs TARGET ${name}_objects PROPERTY INCLUDE_DIRECTORIES)
  set(${name}_incs ${${name}_incs} PARENT_SCOPE)

  add_library(${name}_static STATIC $<TARGET_OBJECTS:${name}_objects>)
  target_include_directories(${name}_static PUBLIC ${${name}_incs})

endmacro()

function(_kcuk_test_common_in target srcs)
  add_executable(${target} ${srcs})
  target_link_libraries(${target} kcuk_lib_static)
  target_link_libraries(${target} ${name}_static)
  add_test(${target} ${target})
endfunction()

function(kcuk_tests_common)

  kcuk_getname(name)

  include(k/subdirlist)

  subdirlist(dirs ${CMAKE_CURRENT_LIST_DIR})
  foreach(dir ${dirs})
    file(GLOB srcs ${dir}/*.c)
    _kcuk_test_common_in(${name}_test_${dir} "${srcs}")
  endforeach()

  file(GLOB files ${CMAKE_CURRENT_LIST_DIR}/*.c)
  foreach(file ${files})
    get_filename_component(n ${file} NAME_WE)
    _kcuk_test_common_in(${name}_test_${n}_c ${file})
  endforeach()

endfunction()
