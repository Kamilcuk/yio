
set(CPACK_PACKAGE_VENDOR "Kamil Cukrowski")
set(CPACK_PACKAGE_DESCRIPTION "Yio Input Output C Library that aims in type-safe printf replacement")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://gitlab.com/kamcuk/yio")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_LIST_DIR}/../LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_LIST_DIR}/../README.md")
set(CPACK_STRIP_FILES TRUE)
set(CPACK_PACKAGE_CONTACT "Kamil Cukrowski <kamilcukrowski@gmail.com>")
set(CPACK_PACKAGE_VENDOR "Kamil Cukrowski <kamilcukrowski@gmail.com>")

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6")
set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "libunistring2")

set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_RPM_PACKAGE_LICENSE "GPL3.0")
set(CPACK_RPM_PACKAGE_URL "${CPACK_PACKAGE_HOMEPAGE_URL}")
set(CPACK_RPM_PACKAGE_REQUIRES "libunistring")
set(CPACK_RPM_PACKAGE_SUGGESTS "libunistring")
set(CPACK_RPM_PACKAGE_PROVIDES "yio")

include(CPack)

