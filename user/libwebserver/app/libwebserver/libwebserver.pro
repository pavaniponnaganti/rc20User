QT       -= gui
TARGET = webserver
TEMPLATE = lib
CONFIG += static
# declare path to required libraries and headers
NWS_SOURCE_PATH = $$PWD/../../webserver

# specify where to search the headers
INCLUDEPATH += $$NWS_SOURCE_PATH/subprotocols \
               $$NWS_SOURCE_PATH \
               $$NWS_DEP_INC_PATH \
               $$NWS_DEP_INC_PATH/libwebsockets \
               $$NWS_DEP_INC_PATH/libxml2 \
               $$NWS_DEP_INC_PATH/cjson \
               $$NWS_USER_INC_PATH \
               $$NWS_DBSCHEMA_INC_PATH

DEPENDPATH += $$NWS_SOURCE_PATH \
              $$NWS_DBSCHEMA_INC_PATH \
              $$NWS_DEP_INC_PATH

win32:!win32-g++ {
    PRE_TARGETDEPS += $$NWS_DEP_LIB_PATH/xml2.lib \
                  $$NWS_DEP_LIB_PATH/cjson.lib \
                  $$NWS_DEP_LIB_PATH/websockets.dll.lib
} else:win32-g++ {
    PRE_TARGETDEPS += $$NWS_DEP_LIB_PATH/libxml2.a \
                  $$NWS_DEP_LIB_PATH/libcjson.a \
                  $$NWS_DEP_LIB_PATH/libwebsockets.dll.a
}

# specify dependent libraries location
LIBS += -L$$NWS_DEP_LIB_PATH

#specify required libraries
LIBS += -lcjson -lxml2 -lwebsockets

# specify defines
DEFINES += _CRT_SECURE_NO_WARNINGS

SOURCES += $$files($$NWS_SOURCE_PATH/*.c, true)

HEADERS += $$files($$NWS_SOURCE_PATH/*.h, true)
