file(GLOB_RECURSE srcs RELATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR} *.c)
foreach(src IN LISTS srcs)
  jinja_generate(
    SOURCE ${src}
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${src}
  )
  yio_add_test(reprocessed/${src} ${CMAKE_CURRENT_BINARY_DIR}/${src} ${src})
endforeach()

