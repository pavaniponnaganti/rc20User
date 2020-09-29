QT += core
QT -= gui
CONFIG += c++11
TARGET = webserver
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

QMAKE_CFLAGS_DEPS += -MM -H

# declare path to required libraries and headers
NWS_SOURCE_PATH = $$PWD/../../daemon

# specify where to search the headers
INCLUDEPATH += $$NWS_SOURCE_PATH

# specify defines
DEFINES += _CRT_SECURE_NO_WARNINGS

SOURCES += $$files($$NWS_SOURCE_PATH/*.c, true)

HEADERS += $$files($$NWS_SOURCE_PATH/*.h, true)

include ($$PWD/../libwebserver/libwebserver.pri)

