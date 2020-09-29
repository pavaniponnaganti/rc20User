
# Specify the include directory for CLIPS header file
CFLAGS += -I$(ROOTDIR)/user/libwebserver/webserver

# Library linker flag.
LDLIBS += -lwebserver

# Business rules currently required libdbapi. libdbapi required the
# other libraries included here. libdbapi should create its own include
# make file
LDLIBS += -ldbapi -lnojacore -llog -lutils -lpowersystems -lm -lrt
