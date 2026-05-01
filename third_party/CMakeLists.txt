
if(NOT YIO_SDCC)
  # skip tiny-regex-c for sdcc
  add_library(tiny-regex-c EXCLUDE_FROM_ALL
    ./tiny-regex-c/re.c
    ./tiny-regex-c/re.h
  )
  target_include_directories(tiny-regex-c PUBLIC
    ./tiny-regex-c
  )
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
	  target_compile_options(tiny-regex-c PUBLIC
		  -Wno-missing-prototypes
	  )
  endif()
	target_compile_definitions(tiny-regex-c INTERFACE YIO_HAS_TINY_REGEX_C=1)

  if(YIO_HAS_FLOATf OR YIO_HAS_FLOATd)
    if(NOT TARGET yio_ryu)
      add_library(yio_ryu OBJECT EXCLUDE_FROM_ALL
        ./ryu/ryu/f2s.c
        ./ryu/ryu/d2s.c
        ./ryu/ryu/d2fixed.c
        ./ryu/ryu/s2d.c
        ./ryu/ryu/s2f.c
      )
      target_include_directories(yio_ryu PUBLIC
        ./ryu
      )

      if(YYIO_HAS_INT128)
        target_sources(yio_ryu PRIVATE ./ryu/ryu/generic_128.c)
      endif()
      if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(yio_ryu PRIVATE -Wno-analyzer-possible-null-dereference)
      endif()
    endif()
  endif()
else()
  add_library(tiny-regex-c INTERFACE)
endif()
