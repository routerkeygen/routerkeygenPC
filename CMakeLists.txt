#
# RouterKeygen build system
# Author: Rui Araújo <ruka.araujo AT gmail.com>
#
# Project Info
cmake_minimum_required (VERSION 2.8.9)


set(PROJECT_NAME_SHORT   "RouterKeygen")
string(TOLOWER ${PROJECT_NAME_SHORT} PROJECT_NAME_SHORT_LOWER)
project(${PROJECT_NAME_SHORT_LOWER} CXX)
set(PROJECT_NAME_LONG    "RouterKeygen")
set(PROJECT_DESCRIPTION_SHORT "Generates WPA/WEP keys based on MAC and/or BSSID")
set(PROJECT_DESCRIPTION  "Default WPA/WEP key generator for several routers such as Thomson based routers ( this includes Thomson, SpeedTouch, Orange, Infinitum, BBox, DMax, BigPond, O2Wireless, Otenet, Cyta, TN_private and Blink ), DLink ( only some models ), Pirelli Discus, Eircom, Verizon FiOS ( only some routers supported), Alice AGPF, FASTWEB Pirelli and Telsey, Huawei (some InfinitumXXXX), Wlan_XXXX or Jazztel_XXXX, Wlan_XX ( only some are supported), Ono ( P1XXXXXX0000X ), WlanXXXXXX YacomXXXXXX and WifiXXXXXX, Sky V1 routers, Clubinternet.box v1 and v2 ( TECOM-AH4XXXX ), InfostradaWifi, CONN-X, Megared, EasyBox Arcor and Vodafone, PBS (Austria), MAXCOM, PTV, TeleTu/Tele2, Axtel Axtel-xtremo, Intercable, OTE, Cabovisao Sagem, Alice in Germany and Speedport.")
set(PROJECT_COPYRIGHT    "Copyright (C) 2010-2015 Rui Araújo")
set(PROJECT_CONTACT      "ruka.araujo@gmail.com")
set(PROJECT_VENDOR       "Rui Araújo")
set(ORG_WEBSITE          "https://github.com/routerkeygen/routerkeygenPC")

# Configuring build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
    message("setting build type to '${CMAKE_BUILD_TYPE}'")
ELSE(NOT CMAKE_BUILD_TYPE)
    message("Build type set to '${CMAKE_BUILD_TYPE}'")
endif(NOT CMAKE_BUILD_TYPE)

if(MSYS)
    # Building in release for MSYS
    set(CMAKE_BUILD_TYPE "Release")
    set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE QT_NO_DEBUG)
    # Use mingw's sprintf instead of windows's
    add_definitions( -D__USE_MINGW_ANSI_STDIO=1 )
endif(MSYS)

# Project Version
set(${PROJECT_NAME_SHORT_LOWER}_VERSION_MAJOR 1)
set(${PROJECT_NAME_SHORT_LOWER}_VERSION_MINOR 1)
set(${PROJECT_NAME_SHORT_LOWER}_VERSION_PATCH 0)
set(PROJECT_VERSION ${${PROJECT_NAME_SHORT_LOWER}_VERSION_MAJOR}.${${PROJECT_NAME_SHORT_LOWER}_VERSION_MINOR}.${${PROJECT_NAME_SHORT_LOWER}_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

message(STATUS "Version: ${PROJECT_VERSION}")


# System on which project is built
site_name(HOSTNAME)
set(SYSNAME ${CMAKE_SYSTEM}-${CMAKE_SYSTEM_PROCESSOR})

# CMake settings
cmake_minimum_required(VERSION 2.8.9)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)

# set path to additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES x86_64 )
endif(APPLE)

if (UNIX)
    set(WITH_PROFILING FALSE CACHE BOOL "Build with profiling support" )
    set(CMAKE_INSTALL_PREFIX /usr)
endif(UNIX)

# Search and includeQt parameters
find_package(OpenSSL REQUIRED )
find_package(Qt5Core QUIET)
if (Qt5Core_FOUND)
    message("Using Qt${Qt5Core_VERSION_STRING}")
    find_package(Qt5Widgets REQUIRED)
    find_package(Qt5Network REQUIRED)
    find_package(Qt5Script REQUIRED)
    find_package(Qt5LinguistTools QUIET)
    set(ROUTERKEYGEN_LIBS ${Qt5Script_LIBRARIES}  ${Qt5Network_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${QtCore_LIBRARIES} ${OPENSSL_LIBRARIES} )
    include_directories(${Qt5Script_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS})
    # Use the compile definitions defined in the Qt 5 modules
    add_definitions(${Qt5Script_DEFINITIONS} ${Qt5Network_DEFINITIONS} ${Qt5Widgets_DEFINITIONS} ${Qt5Core_DEFINITIONS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Script_EXECUTABLE_COMPILE_FLAGS} ${Qt5Network_EXECUTABLE_COMPILE_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
else(Qt5Core_FOUND)
    message("Using Qt4")
    find_package(Qt4 4.8.0 COMPONENTS QtCore QtGui QtNetwork QtScript REQUIRED )
    set(ROUTERKEYGEN_LIBS ${QT_QTSCRIPT_LIBRARY} ${QT_QTNETWORK_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY} ${OPENSSL_LIBRARIES} ) # Add here any other QT module or other library
    include(${QT_USE_FILE})
#FindQt4.cmake is so brain-dead when Xcompiling
    add_definitions(${QT_DEFINITIONS})
endif(Qt5Core_FOUND)

# MSVC specific configuration (for compiling on windows)
if(MSVC)
    set(ROUTERKEYGEN_LIBS ${ROUTERKEYGEN_LIBS} ${QT_QTMAIN_LIBRARY})
endif(MSVC)

if(MSYS AND CMAKE_CROSS_COMPILING)
    set (ROUTERKEYGEN_LIBS ${ROUTERKEYGEN_LIBS} crypt32 jpeg png tiff mng lzma lcms2 z imm32 winmm ws2_32 )
endif(MSYS AND CMAKE_CROSS_COMPILING)

# Layout of CMakeQt installation
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set common dirs for MAC and UNIX
set(DEFAULT_BIN_SUBDIR     bin)
set(DEFAULT_LIB_SUBDIR     lib)
set(DEFAULT_DATA_SUBDIR    share)
set(DEFAULT_SRC_SUBDIR     src)
set(DEFAULT_INCLUDE_SUBDIR include)

# set ROUTERKEYGEN path params
set(ROUTERKEYGEN_BIN_SUBDIR     "${DEFAULT_BIN_SUBDIR}"        CACHE STRING "Subdirectory where executables will be installed")
set(ROUTERKEYGEN_LIB_SUBDIR     "${DEFAULT_LIB_SUBDIR}"        CACHE STRING "Subdirectory where libraries will be installed")
set(ROUTERKEYGEN_DATA_SUBDIR    "${DEFAULT_DATA_SUBDIR}"       CACHE STRING "Subdirectory where ROUTERKEYGEN's data will be installed")
set(ROUTERKEYGEN_SRC_SUBDIR     "${DEFAULT_SRC_SUBDIR}"        CACHE STRING "Subdirectory where ROUTERKEYGEN's source is")
set(ROUTERKEYGEN_INCLUDE_SUBDIR "${DEFAULT_INCLUDE_SUBDIR}"    CACHE STRING "Subdirectory where header files will be installed")

# Full paths for the installation
set(ROUTERKEYGEN_BIN_DIR     ${CMAKE_INSTALL_PREFIX}/${ROUTERKEYGEN_BIN_SUBDIR})
set(ROUTERKEYGEN_LIB_DIR     ${CMAKE_INSTALL_PREFIX}/${ROUTERKEYGEN_LIB_SUBDIR})
set(ROUTERKEYGEN_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/${ROUTERKEYGEN_INCLUDE_SUBDIR})
set(ROUTERKEYGEN_DATA_DIR    ${CMAKE_INSTALL_PREFIX}/${ROUTERKEYGEN_DATA_SUBDIR}/routerkeygen/)
set(ROUTERKEYGEN_DOC_DIR     ${CMAKE_INSTALL_PREFIX}/${ROUTERKEYGEN_DATA_SUBDIR}/doc/routerkeygen/)
set(ROUTERKEYGEN_EFFECTS_DIR ${CMAKE_INSTALL_PREFIX}/${ROUTERKEYGEN_LIB_SUBDIR}/routerkeygen/effects/)
set(ROUTERKEYGEN_DESKTOP_DIR ${CMAKE_INSTALL_PREFIX}/${ROUTERKEYGEN_DATA_SUBDIR}/applications/)
set(ROUTERKEYGEN_PIXMAPS_DIR ${CMAKE_INSTALL_PREFIX}/${ROUTERKEYGEN_DATA_SUBDIR}/pixmaps/)

# Manual page - makes sense only on unix systems
if(UNIX)
    set(DEFAULT_MANUAL_SUBDIR  ${ROUTERKEYGEN_DATA_SUBDIR}/man)
    set(ROUTERKEYGEN_MANUAL_SUBDIR  "${DEFAULT_MANUAL_SUBDIR}"  CACHE STRING "Subdirectory where manual files will be installed")
    mark_as_advanced(ROUTERKEYGEN_MANUAL_SUBDIR)
    set(ROUTERKEYGEN_MANUAL_DIR  ${CMAKE_INSTALL_PREFIX}/${ROUTERKEYGEN_MANUAL_SUBDIR})
endif(UNIX)

# Recursively add sub dirs
add_subdirectory(lang)
add_subdirectory(src)
add_subdirectory(nsis)


# Copy stuff to doc subdirectory
INSTALL (FILES debian/copyright AUTHORS INSTALL NEWS README.md doc/routerkeygen.1
         DESTINATION ${ROUTERKEYGEN_DOC_DIR})

# Copy .desktop file
INSTALL (FILES share/routerkeygen.desktop DESTINATION ${ROUTERKEYGEN_DESKTOP_DIR})

# Copy the ROUTERKEYGEN pixmap
INSTALL (FILES share/routerkeygen.png DESTINATION ${ROUTERKEYGEN_PIXMAPS_DIR})

#############################################################
# Enable packaging

include(InstallRequiredSystemLibraries)

set(CPACK_STRIP_FILES "TRUE")
set(CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME})
set(CPACK_PACKAGE_NAME ${PROJECT_NAME_SHORT})
set(CPACK_PACKAGE_DESCRIPTION "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION_SHORT} \n\\ ${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_VENDOR ${PROJECT_VENDOR})
set(CPACK_PACKAGE_CONTACT ${PROJECT_CONTACT})
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
set(CPACK_PACKAGE_CHANGELOG_FILE)

# Force Package Name
set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${CPACK_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR})

# NSIS Windows installer
if(MSYS AND NOT UNIX)
    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backslashes.
    set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/share\\\\routerkeygen.png")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "\\\\routerkeygen.exe")
    set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} ROUTERKEYGEN")
    set(CPACK_NSIS_HELP_LINK "https:\\\\\\\\github.com/routerkeygen/routerkeygenPC/")
    set(CPACK_NSIS_URL_INFO_ABOUT "https:\\\\\\\\github.com/routerkeygen/routerkeygenPC/")
    set(CPACK_NSIS_CONTACT "ruka.araujo AT gmail.com")
    set(CPACK_NSIS_MODifY_PATH ON)
endif(MSYS AND NOT UNIX)

# RPM packages
include(cmake/RpmBuild.cmake)
if(RPMBUILD_FOUND AND NOT MSYS)
    set(CPACK_GENERATOR "RPM")
    set(CPACK_RPM_PACKAGE_SUMMARY ${CPACK_PACKAGE_DESCRIPTION_SUMMARY})
    set(CPACK_RPM_PACKAGE_NAME ${CPACK_PACKAGE_NAME})
    set(CPACK_RPM_PACKAGE_VERSION ${PROJECT_VERSION})
    set(CPACK_RPM_PACKAGE_LICENSE "GPLv3+ License")
    set(CPACK_RPM_PACKAGE_GROUP ${PROJECT_VENDOR})
    set(CPACK_RPM_PACKAGE_VENDOR ${PROJECT_VENDOR})
    set(CPACK_RPM_PACKAGE_SUMMARY ${PROJECT_DESCRIPTION})
    set(CPACK_RPM_PACKAGE_DEPENDS "libc, qt ( >= 4.6 ), qt-x11 ( >= 4.6 ), NetworkManager ( >= 0.8 )") # Specify dependencies here
    set(CPACK_set_DESTDIR TRUE)
endif(RPMBUILD_FOUND AND NOT MSYS)

# Debian packages
include(cmake/DpkgBuild.cmake)
if(DPKG_FOUND AND NOT MSYS)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_NAME ${CPACK_PACKAGE_NAME})
    set(CPACK_DEBIAN_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION})
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION ${CPACK_PACKAGE_DESCRIPTION_SUMMARY})
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR} <${CPACK_PACKAGE_CONTACT}>")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    set(CPACK_DEBIAN_PACKAGE_DEBUG ON)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS ${PACKAGE_REQUIRES})

    find_package (UnixCommands REQUIRED)
    add_custom_target (compress_changelog ALL COMMAND ${GZIP} -9 -c ${PROJECT_SOURCE_DIR}/NEWS > changelog.gz)
    install (FILES ${PROJECT_BINARY_DIR}/changelog.gz DESTINATION share/doc/${PROJECT_NAME_SHORT_LOWER} COMPONENT app)

    if(Qt5Core_FOUND)
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), qtbase5-dev, network-manager ( >= 0.8 )") # Specify dependencies here
    else(Qt5Core_FOUND)
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libqt4-gui ( >= 4.6 ), libqt4-network ( >= 4.6 ), network-manager ( >= 0.8 )") # Specify dependencies here
    endif(Qt5Core_FOUND)
        set(CPACK_set_DESTDIR TRUE)
    endif(DPKG_FOUND AND NOT MSYS)

set(CPACK_PACKAGE_EXECUTABLES ${PROJECT_NAME_SHORT_LOWER} "ROUTERKEYGEN")
INCLUDE(CPack)

#For debug purposes, print all variables
#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}\n")
#endforeach()
