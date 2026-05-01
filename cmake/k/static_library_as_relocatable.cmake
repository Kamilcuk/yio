
# Do you ever have problems with weak functions in embedded environments?
# Don't they work? You must use -Wl,--whole-archive to fix it?
# Fear no more! By creating a relocatable object file from multiple object files
# instead of a static library, all symbols are included in final compilation.

# libstatic.a.o
set(CMAKE_STATIC_LIBRARY_SUFFIX "${CMAKE_STATIC_LIBRARY_SUFFIX}.o")

foreach(ll_lang IN ITEMS C CXX)
  if(NOT CMAKE_${ll_lang}_COMPILER)
    continue()
  endif()
  if(NOT CMAKE_${ll_lang}_COMPILER_ID STREQUAL "GNU")
    message(WARNING "static_library_as_relocatable has been only tested with gcc")
  endif()
  set(CMAKE_${ll_lang}_CREATE_STATIC_LIBRARY "${CMAKE_${ll_lang}_COMPILER} <LINK_FLAGS> -r -o <TARGET> <OBJECTS> ")
  unset(CMAKE_${ll_lang}_ARCHIVE_CREATE)
  unset(CMAKE_${ll_lang}_ARCHIVE_APPEND)
  unset(CMAKE_${ll_lang}_ARCHIVE_FINISH)
endforeach()
unset(ll_lang)

