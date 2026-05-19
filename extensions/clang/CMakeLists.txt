cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0075 NEW)
project(fstring_clang_plugin)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

enable_testing()

add_subdirectory(../common common EXCLUDE_FROM_ALL)

add_library(fstring_plugin MODULE src/fstring_plugin.cpp)
target_include_directories(fstring_plugin PRIVATE ${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS} ../common/src)
target_compile_definitions(fstring_plugin PRIVATE ${LLVM_DEFINITIONS})
set_target_properties(fstring_plugin PROPERTIES PREFIX "")
if(APPLE)
	set_target_properties(fstring_plugin PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif()

target_link_libraries(
	fstring_plugin
	PRIVATE clangAST
					clangBasic
					clangFrontend
					clangLex
					clangSema
					clangRewrite
					extensions_common
					yio
)

add_library(fstring_plugin_test_base INTERFACE)
target_compile_options(
	fstring_plugin_test_base
	INTERFACE "SHELL:-Xclang -load -Xclang $<TARGET_FILE:fstring_plugin> -Xclang -add-plugin -Xclang fstring_plugin"
						"-Wno-unused-command-line-argument" "-Werror"
)

add_library(fstring_plugin_unopt INTERFACE)
target_link_libraries(fstring_plugin_unopt INTERFACE fstring_plugin_test_base)
target_compile_options(fstring_plugin_unopt INTERFACE "-fplugin-arg-fstring_plugin-ffstring-optimizations=0")

add_library(fstring_plugin_opt INTERFACE)
target_link_libraries(fstring_plugin_opt INTERFACE fstring_plugin_test_base)
target_compile_options(fstring_plugin_opt INTERFACE "-fplugin-arg-fstring_plugin-ffstring-optimizations=1")

add_fstring_plugin_test_dir("${CMAKE_CURRENT_SOURCE_DIR}/../common/test")

add_fstring_testprogram_from_env()
