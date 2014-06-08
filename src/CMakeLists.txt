#
# RouterKeygen build system
# Author: Rui Araújo <ruka.araujo AT gmail.com>
#

#Put here CPPs

file(GLOB ROUTERKEYGEN_SRCS
    "algorithms/*.cpp"
    "config/*.cpp"
    "qcmdlineparser/*.cpp"
    "dialog/*.cpp"
    "*.cpp"
)

list( APPEND ROUTERKEYGEN_SRCS
    wifi/QScanResult.cpp
    wifi/QWifiManagerPrivate.cpp
    wifi/QWifiManager.cpp
)

file(GLOB ROUTERKEYGEN_HDRS
    "algorithms/*.h"
    "config/*.h"
    "dialog/*.h"
    "qcmdlineparser/*.h"
    "*.h"
)

list( APPEND ROUTERKEYGEN_HDRS
    wifi/QWifiManagerPrivate.h
    wifi/QWifiManager.h
    wifi/QScanResult.h
)

#Put here QRC files to be Rcc-ed
set(ROUTERKEYGEN_RCC
    ../resources/resources.qrc
   ${CMAKE_CURRENT_BINARY_DIR}/../lang/resources-lang.qrc
)

if(Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()


#include paths for routerkeygen
include_directories(
  ${OPENSSL_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  .
)

configure_file(${CMAKE_SOURCE_DIR}/cmake/version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/version.h)


#Put here UI files
set(ROUTERKEYGEN_UIS
    forms/routerkeygen.ui
    forms/welcome_dialog.ui
    forms/aboutdialog.ui
    forms/UpdateDialog.ui
)

if (UNIX AND NOT APPLE )
    list( APPEND ROUTERKEYGEN_SRCS wifi/QWifiManagerPrivateUnix.cpp )
    list( APPEND ROUTERKEYGEN_HDRS wifi/QWifiManagerPrivateUnix.h )
    include_directories(/usr/include/NetworkManager)
    if(Qt5Core_FOUND)
         find_package(Qt5DBus)
        set(ROUTERKEYGEN_LIBS ${ROUTERKEYGEN_LIBS} ${Qt5DBus_LIBRARIES})
    else(Qt5Core_FOUND)
        set(ROUTERKEYGEN_LIBS ${ROUTERKEYGEN_LIBS} ${QT_QTDBUS_LIBRARY})
    endif(Qt5Core_FOUND)
endif(UNIX AND NOT APPLE )


if( MINGW )
    # resource compilation for MinGW
    add_custom_command( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/icon.o
        COMMAND ${CMAKE_RC_COMPILER} -I${CMAKE_SOURCE_DIR} -i${CMAKE_SOURCE_DIR}/cmake/icon.rc
        -o ${CMAKE_CURRENT_BINARY_DIR}/icon.o )
    set(ROUTERKEYGEN_SRCS ${ROUTERKEYGEN_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/icon.o)
    set(LINK_FLAGS -Wl,-subsystem,windows)
endif( MINGW )

if(WIN32)
    enable_language(ASM)
    list( APPEND ROUTERKEYGEN_SRCS wifi/QWifiManagerPrivateWin.cpp )
    list( APPEND ROUTERKEYGEN_HDRS wifi/QWifiManagerPrivateWin.h )
    set(GUI_TYPE WIN32)
endif(WIN32)

if(APPLE)
    list( APPEND ROUTERKEYGEN_SRCS wifi/QWifiManagerPrivateMac.cpp mac/macloginitemsmanager.mm)
    list( APPEND ROUTERKEYGEN_HDRS wifi/QWifiManagerPrivateMac.h mac/macloginitemsmanager.h )
    set(GUI_TYPE MACOSX_BUNDLE)
    set(ROUTERKEYGEN_SRCS ${ROUTERKEYGEN_SRCS} ../share/routerkeygen.icns)
    find_library(COCOA_LIBRARY Cocoa)
    find_path(COCOA_INCLUDE_DIR Cocoa/Cocoa.h)
    set(ROUTERKEYGEN_LIBS ${ROUTERKEYGEN_LIBS} ${COCOA_LIBRARY})
endif(APPLE)

set(CMAKE_AUTOMOC ON)
if (Qt5Core_FOUND)
    qt5_wrap_ui(ROUTERKEYGEN_UIS_H ${ROUTERKEYGEN_UIS})
    qt5_add_resources(ROUTERKEYGEN_RCC_SRCS ${ROUTERKEYGEN_RCC})
    add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x000000) #handle deprecated methods
else(Qt5Core_FOUND)
    qt4_wrap_ui(ROUTERKEYGEN_UIS_H ${ROUTERKEYGEN_UIS})
    qt4_add_resources(ROUTERKEYGEN_RCC_SRCS ${ROUTERKEYGEN_RCC})
endif(Qt5Core_FOUND)


add_executable(routerkeygen ${GUI_TYPE} ${ROUTERKEYGEN_SRCS} ${ROUTERKEYGEN_HDRS} ${ROUTERKEYGEN_UIS_H} ${ROUTERKEYGEN_RCC_SRCS} )

# Add translations
add_dependencies(routerkeygen translations)

# Link the target against the Qt libraries
target_link_libraries(routerkeygen ${ROUTERKEYGEN_LIBS} )


# Now we are done.
install(TARGETS routerkeygen
        BUNDLE  DESTINATION ${ROUTERKEYGEN_BIN_DIR}
        RUNTIME DESTINATION ${ROUTERKEYGEN_BIN_DIR})

add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/bin/routerkeygen
    COMMAND ${CMAKE_COMMAND} copy ${CMAKE_CURRENT_SOURCE_DIR}/routerkeygen ${CMAKE_SOURCE_DIR}/bin/routerkeygen
    DEPENDS ${CMAKE_BINARY_DIR}/src/routerkeygen
)

if(APPLE)
    set_target_properties(routerkeygen PROPERTIES MACOSX_BUNDLE_INFO_PLIST ../share/MacOSXBundleInfo.plist.in)
    set_source_files_properties(share/routerkeygen.icns PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources)
    set(MACOSX_BUNDLE_ICON_FILE routerkeygen.icns)
endif(APPLE)

add_definitions(-W -Wall -D_FORTIFY_SOURCE=2  )

if (APPLE)
    #Stop Apple from complaining that OpenSSL is deprecated
    #add_definitions(-DMAC_OS_X_VERSION_MIN_REQUIRED=MAC_OS_X_VERSION_10_8﻿)
endif(APPLE)
