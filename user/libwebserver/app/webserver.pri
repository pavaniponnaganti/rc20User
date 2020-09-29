# select appropriate dependency path
# Note when copying this project into a different folder, please make sure the paths are all
# correct. If not, the linker will not find the correct libraries
ios {
    NWS_DEP_LIB_PATH =
    NWS_DEP_INC_PATH =
} else: android {
    NWS_DEP_LIB_PATH =
    NWS_DEP_INC_PATH =
} else: win32 {
    NWS_DEP_LIB_PATH = c:/msys64/mingw32/lib
    NWS_DEP_INC_PATH = c:/msys64/mingw32/include
}

NWS_USER_INC_PATH = $$PWD/../../include
NWS_USER_LIB_PATH = $$PWD/../../lib-host
NWS_DBSCHEMA_INC_PATH = $$PWD/../../../dbschema/include/dbschema

# export requied path
cache(NWS_DEP_LIB_PATH, set, NWS_DEP_LIB_PATH)
cache(NWS_DEP_INC_PATH, set, NWS_DEP_INC_PATH)
cache(NWS_USER_INC_PATH, set, NWS_USER_INC_PATH)
cache(NWS_USER_LIB_PATH, set, NWS_USER_LIB_PATH)
cache(NWS_DBSCHEMA_INC_PATH, set, NWS_DBSCHEMA_INC_PATH)
