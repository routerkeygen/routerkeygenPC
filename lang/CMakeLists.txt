#
# RouterKeygen build system
# Authors: Rui Araújo <ruka.araujo@gmail.com>
#

## RCC file
set(TS_QRC ${CMAKE_BINARY_DIR}/lang/resources-lang.qrc)

if(Qt5LinguistTools_FOUND OR CMAKE_CROSS_COMPILING)
    set(QT_LUPDATE_EXECUTABLE lupdate)
    set(QT_LRELEASE_EXECUTABLE lrelease)
endif(Qt5LinguistTools_FOUND OR CMAKE_CROSS_COMPILING)

if(NOT QT_LUPDATE_EXECUTABLE )
    message(WARNING "Could not find lupdate. You won't be able to update translations.")
endif(NOT QT_LUPDATE_EXECUTABLE)

if(NOT QT_LRELEASE_EXECUTABLE)
    message(WARNING "Could not find lrelease. Your build won't contain translations.")
    ## Create an empty RCC file
    file(WRITE ${TS_QRC} "<RCC></RCC>")
endif(NOT QT_LRELEASE_EXECUTABLE)

## Wraps the supplied .ts files in lrelease commands
macro(QT_ADD_TRANSLATIONS outfiles)
    file(WRITE ${TS_QRC} "<RCC><qresource prefix=\"lang\">\n")
    FOREACH (it ${ARGN})
        get_filename_component(it ${it} ABSOLUTE)
        get_filename_component(outfile ${it} NAME_WE)

        file(APPEND ${TS_QRC} "<file>${outfile}.qm</file>\n")

        set(outfile ${CMAKE_BINARY_DIR}/lang/${outfile}.qm)

        add_custom_command(
            OUTPUT ${outfile}
            COMMAND ${QT_LRELEASE_EXECUTABLE}
            ARGS -compress -silent -nounfinished ${it} -qm ${outfile}
            MAIN_DEPENDENCY ${it}
        )

        set(${outfiles} ${${outfiles}} ${outfile})
    endforeach(it)
    file(APPEND ${TS_QRC} "</qresource></RCC>")
endmacro(QT_ADD_TRANSLATIONS)

## set available translations file ids here
option(LANGUAGES "Using translations")
set(ALL_LANGUAGES
        en
        pt
	nl
        es # add here more lang tokens like jp, ro etc. as per ts files
   )

## Check if user has provided specific LANGS option
if(NOT DEFINED LANGS)
    set(LANGUAGES ${ALL_LANGUAGES} CACHE STRING "Using translations" FORCE)
else(NOT DEFINED LANGS)
    if(NOT LANGS)
        set(LANGUAGES "" CACHE STRING "Using translations" FORCE)
    elseif(LANGS STREQUAL *)
        set(LANGUAGES ${ALL_LANGUAGES} CACHE STRING "Using translations" FORCE)
    else(NOT LANGS)
        STRING(REGEX MATCHALL [a-zA-Z_]+
               langs1 ${LANGS})
        set(LANGUAGES ${langs1} CACHE STRING "Using translations" FORCE)
    endif(NOT LANGS)
endif(NOT DEFINED LANGS)

## Display what translations files will be processed
message(STATUS "Translations: ${LANGUAGES}")

## Collect all translations files that are to be processed
foreach(LANGUAGE ${LANGUAGES})
    file(GLOB temp_TS ${CMAKE_SOURCE_DIR}/lang/routerkeygen_*${LANGUAGE}*.ts)
    list(APPEND routerkeygen_TS ${temp_TS})
endforeach()

if(QT_LRELEASE_EXECUTABLE)
    ## Wraps the collected translations
    QT_ADD_TRANSLATIONS(routerkeygen_QMS ${routerkeygen_TS})

    ## Create a target that runs lrelease for all the .ts files
    ## "ALL" means that it will be run by default.
    add_custom_target(translations ALL DEPENDS ${routerkeygen_QMS})
endif(QT_LRELEASE_EXECUTABLE)

if(QT_LUPDATE_EXECUTABLE)
    ## Search for all translatable strings in the sources directory
    file(GLOB_RECURSE translate_SRCS ${CMAKE_SOURCE_DIR}/src/*.cpp
                                 ${CMAKE_SOURCE_DIR}/src/*.hpp)
    file(GLOB_RECURSE translate_HDRS ${CMAKE_SOURCE_DIR}/src/*.h)
    file(GLOB_RECURSE translate_UIS ${CMAKE_SOURCE_DIR}/src/*.ui)

    set(translate_SRCS ${translate_SRCS} ${translate_HDRS} ${translate_UIS})

    ## Create a target that runs lupdate for all the sources and UI files
    add_custom_target(lupdate
        COMMAND ${QT_LUPDATE_EXECUTABLE} -noobsolete ${translate_SRCS} -ts ${routerkeygen_TS}
    )
endif(QT_LUPDATE_EXECUTABLE)

