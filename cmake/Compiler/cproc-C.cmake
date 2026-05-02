include_guard()
message(STATUS "Configuring for cproc-C")
include(Compiler/CMakeCommonCompilerMacros)

set(CMAKE_C_VERBOSE_FLAG "-V")
string(APPEND CMAKE_C_FLAGS_INIT " -D__CPROC__=1")
string(APPEND CMAKE_C_FLAGS_MINSIZEREL_INIT " -DNDEBUG")
string(APPEND CMAKE_C_FLAGS_RELEASE_INIT " -DNDEBUG")
string(APPEND CMAKE_C_FLAGS_RELWITHDEBINFO_INIT " -DNDEBUG")

set(CMAKE_C11_STANDARD_COMPILE_OPTION "")
set(CMAKE_C11_EXTENSION_COMPILE_OPTION "")
set(CMAKE_C11_STANDARD__HAS_FULL_SUPPORT ON)

set(CMAKE_C_STANDARD_DEFAULT "11")

set(CMAKE_C_COMPILE_FEATURES
  c_std_11
  c_function_prototypes
  c_restrict
  c_static_assert
  c_variadic_macros
)
