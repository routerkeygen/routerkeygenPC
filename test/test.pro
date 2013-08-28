QT  += core \
    testlib
QT -= gui
TARGET = RouterKeygenTest
TEMPLATE = app
SOURCES += \
    ../src/algorithms/*.cpp \
    ../src/WirelessMatcher.cpp \
    ../src/sha/sha256.cpp\
    ../src/config/*.cpp \
    ../src/wifi/QScanResult.cpp \
    AlgorithmsTest.cpp

HEADERS += ../src/algorithms/*.h \
    ../src/config/*.h \
    ../src/sha/sha256.h \
    ../src/wifi/QScanResult.h \
    ../src/WirelessMatcher.h

INCLUDEPATH += ../src/
LIBS += -lcrypto

RESOURCES += ../resources/resources.qrc

