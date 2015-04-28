#
# NSIS builder
#

# Output file name
set(NSIS_OUTPUT_FILE "${PROJECT_NAME}_installer.exe")

# NSIS is only available for Win32 builds (native or cross compilation)
if(MSYS)
    set(NSIS_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/")
    # Check contribs existence
    if(EXISTS "${CMAKE_FIND_ROOT_PATH}/")
        message(STATUS "${CMAKE_FIND_ROOT_PATH} found!")
        # Copy necessary libraries required at runtime
        file(COPY "${CMAKE_FIND_ROOT_PATH}/bin/"
             DESTINATION ${NSIS_OUTPUT_DIR}
             FILES_MATCHING PATTERN "*.dll")
        message(STATUS "Configuring NSIS")
        # Copy and configure the nsis script file
        configure_file(installer.nsi "${NSIS_OUTPUT_DIR}/installer.nsi" @ONLY)
        configure_file(FileAssociation.nsh "${NSIS_OUTPUT_DIR}/FileAssociation.nsh" COPYONLY)
        add_custom_target(installer
            ${CMAKE_COMMAND} -E remove "${NSIS_OUTPUT_DIR}${NSIS_OUTPUT_FILE}"
            COMMAND makensis "${NSIS_OUTPUT_DIR}/installer.nsi"
            WORKING_DIRECTORY "${NSIS_OUTPUT_DIR}"
            )
        add_dependencies(installer routerkeygen)
    else(EXISTS "${CMAKE_FIND_ROOT_PATH}/")
        message(WARNING "${CMAKE_FIND_ROOT_PATH} not available, you won't be able to create an installer. Read INSTALL.win32 for details.")
    endif(EXISTS "${CMAKE_FIND_ROOT_PATH}/")
endif(MSYS)
