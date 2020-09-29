# Include the file which creates VER_xxx environmental variables if they don't
# already exist.
# This is a NOJA makefile for integrating with the NOJA build scripts
include ../../user/global.mak
include $(NOJACORE)/nojacore.mak

CFLAGS := $(subst,-Wall,,$(CFLAGS))
CFLAGS := $(subst,-Werror,,$(CFLAGS))

# Variables
NAME = cjson

MAKE = /usr/bin/make
RM = /bin/rm -rf
MKDIR = /bin/mkdir
CP = /bin/cp

BUILD_PATH = ./bin

UNITTEST_CSRC =  $(wildcard test/*.c)

# Targets
all: freescale

# when developing don't clean the host environment
clean : freescale-clean
# clean : freescale-clean host-clean


# Freescale targets
freescale: freescale-setup
	$(MAKE) -C $(BUILD_PATH)/$@
	@test -d $(ROOTDIR)/user/lib || $(MKDIR) $(ROOTDIR)/user/lib
	$(CP) $(BUILD_PATH)/$@/lib$(NAME).a $(ROOTDIR)/user/lib

freescale-clean:
	$(RM) $(BUILD_PATH)/freescale


freescale-setup:
	./setup_freescale.sh

# Host targets
host: host-setup
	$(MAKE) -C $(BUILD_PATH)/$@
	@test -d $(ROOTDIR)/user/lib-host || $(MKDIR) $(ROOTDIR)/user/lib-host
	$(CP) $(BUILD_PATH)/$@/lib$(NAME).a $(ROOTDIR)/user/lib-host

host-clean:
	$(RM) $(BUILD_PATH)/host

host-setup:
	./setup_host.sh


check:
	@echo "lib$(NAME) unit tests not implemented, yet!"

pre_copy:

doc:
