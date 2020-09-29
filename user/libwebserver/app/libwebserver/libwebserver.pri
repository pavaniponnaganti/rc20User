OUT_LIBWEBSERVER = $$OUT_PWD/../libwebserver

# specify the webserver header path
INCLUDEPATH += $$PWD/../../webserver \
               $$NWS_DBSCHEMA_INC_PATH \
               $$NWS_DEP_INC_PATH

DEPENDPATH += $$PWD/../../webserver \
              $$NWS_DBSCHEMA_INC_PATH \
              $$NWS_DEP_INC_PATH \
              $$NWS_DEP_INC_PATH/libwebsockets \
              $$NWS_DEP_INC_PATH/libxml2 \
               $NWS_DEP_INC_PATH/cjson

win32:!win32-g++ {
    PRE_TARGETDEPS += $$NWS_DEP_LIB_PATH/xml2.lib \
                  $$NWS_DEP_LIB_PATH/cjson.lib \
                  $$NWS_DEP_LIB_PATH/websockets.dll.lib
} else:win32-g++ {
    PRE_TARGETDEPS += $$NWS_DEP_LIB_PATH/libxml2.a \
                  $$NWS_DEP_LIB_PATH/libcjson.a \
                  $$NWS_DEP_LIB_PATH/libwebsockets.dll.a
}

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_LIBWEBSERVER/release/libwebserver.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_LIBWEBSERVER/debug/libwebserver.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_LIBWEBSERVER/release/webserver.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OOUT_LIBWEBSERVER/debug/webserver.lib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_LIBWEBSERVER/release/
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_LIBWEBSERVER/debug/
else:  LIBS += -L$$OUT_LIBWEBSERVER

# specify dependencies path
LIBS += -L$$NWS_DEP_LIB_PATH

#link to the libwebserver
LIBS += -lwebserver

# Linked all libwebserver dependencies
LIBS += -lwebsockets -lcjson -lxml2

# load libxml2 linker flags
QMAKE_CFLAGS=`xml2-config --cflags`
LIBS +=`xml2-config --libs`

# install the mount point
mount.path += $$OUT_PWD
mount.files += $$PWD/../../mount
mount.depends += FORCE

INSTALLS += mount
