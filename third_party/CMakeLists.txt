
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

