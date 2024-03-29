SET(CPACK_GENERATOR "TGZ")
SET(CPACK_PACKAGE_VERSION "latest")

IF(BUILD_DEB)
    SET(CPACK_GENERATOR "TGZ;DEB")
    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "root@localhost")
    SET(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
    SET(CPACK_PACKAGE_VERSION "0.0.0")
ENDIF()

# CPackDeb: Debian package versioning ([<epoch>:]<version>[-<release>])
# should confirm to "^([0-9]+:)?[0-9][A-Za-z0-9.+~-]*$" regex in order to


INCLUDE(CPack)
