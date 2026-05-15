
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
        "${CMAKE_CURRENT_SOURCE_DIR}/ryu"
      )

      if(YIO_HAS_INT128)
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

# Check if any decimal float types are supported
set(YIO_HAS_ANY_DECIMAL_FLOAT FALSE)
foreach(suffix IN ITEMS d32 d64 d128 d32x d64x d128x)
    if(YIO_HAS_FLOAT${suffix})
        set(YIO_HAS_ANY_DECIMAL_FLOAT TRUE)
        break()
    endif()
endforeach()

if(YIO_HAS_ANY_DECIMAL_FLOAT)
    set(EXTRA_PATHS /usr/lib /lib)
    if(DEFINED CMAKE_LIBRARY_ARCHITECTURE AND CMAKE_LIBRARY_ARCHITECTURE)
        list(APPEND EXTRA_PATHS "/usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}" "/lib/${CMAKE_LIBRARY_ARCHITECTURE}")
    endif()
    find_library(dfp_LIBRARY NAMES dfp PATHS ${EXTRA_PATHS})

    if(dfp_LIBRARY)
        set(dfp_FOUND TRUE)
        find_path(dfp_INCLUDE_DIR NAMES dfp.h)
        message(STATUS "Found libdfp include dir: ${dfp_INCLUDE_DIR}")
        add_library(dfp INTERFACE)
        target_link_libraries(dfp INTERFACE ${dfp_LIBRARY})
        if(dfp_INCLUDE_DIR)
            target_include_directories(dfp INTERFACE ${dfp_INCLUDE_DIR})
        endif()
        message(STATUS "Found system libdfp at ${dfp_LIBRARY}")
    endif()

    if(NOT dfp_FOUND)
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libdfp/configure")
            message(STATUS "Building libdfp from source via ExternalProject.")
            include(ExternalProject)
            set(LIBDFP_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/libdfp_install")
            file(MAKE_DIRECTORY "${LIBDFP_INSTALL_DIR}/include")
            
            ExternalProject_Add(libdfp_ext
                SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libdfp"
                PREFIX "${CMAKE_CURRENT_BINARY_DIR}/libdfp_build"
                INSTALL_DIR "${LIBDFP_INSTALL_DIR}"
                CONFIGURE_COMMAND "${CMAKE_COMMAND}" -E env "CC=${CMAKE_C_COMPILER}" "AR=${CMAKE_AR}" "RANLIB=${CMAKE_RANLIB}" "${CMAKE_CURRENT_SOURCE_DIR}/libdfp/configure" "--prefix=${LIBDFP_INSTALL_DIR}" "--enable-static" "--disable-shared"
                BUILD_COMMAND "${CMAKE_COMMAND}" -E env "AR=${CMAKE_AR}" "RANLIB=${CMAKE_RANLIB}" make
                INSTALL_COMMAND "${CMAKE_COMMAND}" -E env "AR=${CMAKE_AR}" "RANLIB=${CMAKE_RANLIB}" make install-headers install-doc
                COMMAND "${CMAKE_COMMAND}" -E make_directory "${LIBDFP_INSTALL_DIR}/lib"
                COMMAND "${CMAKE_COMMAND}" -E copy "libdfp.a" "${LIBDFP_INSTALL_DIR}/lib/"
                COMMAND "${CMAKE_COMMAND}" -E copy "libdecnumber/libdecnumber.a" "${LIBDFP_INSTALL_DIR}/lib/"
                BUILD_BYPRODUCTS "${LIBDFP_INSTALL_DIR}/lib/libdfp.a"
            )
            
            add_library(dfp STATIC IMPORTED GLOBAL)
            add_dependencies(dfp libdfp_ext)
            set_target_properties(dfp PROPERTIES
                IMPORTED_LOCATION "${LIBDFP_INSTALL_DIR}/lib/libdfp.a"
                INTERFACE_INCLUDE_DIRECTORIES "${LIBDFP_INSTALL_DIR}/include"
                INTERFACE_LINK_LIBRARIES "${LIBDFP_INSTALL_DIR}/lib/libdecnumber.a"
            )
            set(dfp_FOUND TRUE)
        else()
            message(WARNING "Decimal floating point library (libdfp) not found. _Decimal formatting might fail to link.")
        endif()
    endif()
endif()
