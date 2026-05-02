# SDCC-C.cmake
include(Compiler/CMakeCommonCompilerMacros)

set(CMAKE_C_VERBOSE_FLAG "-V")

set(CMAKE_C90_STANDARD_COMPILE_OPTION "--std-c89")
set(CMAKE_C90_EXTENSION_COMPILE_OPTION "--std-sdcc89")

set(CMAKE_C99_STANDARD_COMPILE_OPTION "--std-c99")
set(CMAKE_C99_EXTENSION_COMPILE_OPTION "--std-sdcc99")

set(CMAKE_C11_STANDARD_COMPILE_OPTION "--std-c11")
set(CMAKE_C11_EXTENSION_COMPILE_OPTION "--std-sdcc11")

set(CMAKE_C17_STANDARD_COMPILE_OPTION "--std-c11")
set(CMAKE_C17_EXTENSION_COMPILE_OPTION "--std-sdcc11")

set(CMAKE_C23_STANDARD_COMPILE_OPTION "--std-c23")
set(CMAKE_C23_EXTENSION_COMPILE_OPTION "--std-sdcc23")

set(CMAKE_C_STANDARD_DEFAULT "11")

set(CMAKE_C_COMPILE_FEATURES
  c_std_90
  c_std_99
  c_std_11
  c_std_17
  c_std_23
  c_function_prototypes
  c_static_assert
  c_variadic_macros
)
set(CMAKE_C90_COMPILE_FEATURES c_std_90)
set(CMAKE_C99_COMPILE_FEATURES c_std_99 c_function_prototypes c_variadic_macros)
set(CMAKE_C11_COMPILE_FEATURES c_std_11 c_static_assert)
set(CMAKE_C17_COMPILE_FEATURES c_std_11 c_static_assert)
set(CMAKE_C23_COMPILE_FEATURES c_std_23)

# Dependency resolving
set(CMAKE_DEPFILE_FLAGS_C "-Wp,-MMD,-MT<OBJECT>,-MF<DEPFILE>")
set(CMAKE_C_DEPENDS_USE_COMPILER TRUE)
