
# define common variables use through out the build process
include ../global.mak
# define the subdirectories required by webserver module
# 1. Websocket Library
# 2. CJSON Library
# 3. Webserver Library
# 4. Webserver Daemon
SUBDIRS = $(ROOTDIR)/lib3p/libwebsockets $(ROOTDIR)/lib3p/libcjson webserver daemon

.PHONY: $(MAKECMDGOALS) $(SUBDIRS)

.NOTPARALLEL:

$(MAKECMDGOALS) : $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)