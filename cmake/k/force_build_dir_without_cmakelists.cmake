
if(EXISTS "${PROJECT_BINARY_DIR}/CMakeLists.txt")
  message(FATAL_ERROR [=[
In source builds are disabled.
You cannot build in a source directory or any directory with a CMakeLists.txt file.
Feel free to remove CMakeCache.txt and CMakeFiles.
Use a build subdirectory to disable this message.
]=])
endif()

