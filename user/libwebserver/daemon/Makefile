# Include the file which creates VER_xxx environmental variables if they don't
# already exist.
include ../../global.mak
include $(NOJACORE)/nojacore.mak
include ../webserver/noja_webserver.mak

NAME = webserver

WORKDIR =  $(ROOTDIR)/user/libwebserver
# build location
BUILD_PATH = $(WORKDIR)/bin
# host-mode build location
HOST_BUILD_PATH = $(WORKDIR)/bin-host
TOP_DIR = $(ROOTDIR)/user
# source directory
SOURCE =  $(WORKDIR)/daemon
# header directory
INCLUDE = $(WORKDIR)/daemon
# common library dir path
ifeq ($(MAKECMDGOALS),host)
LIB = $(ROOTDIR)/user/lib-host
else
LIB = $(ROOTDIR)/user/lib
endif

# create the compile flags
CFLAGS += -O3
# add the headers path
CFLAGS += $(addprefix -I, $(INCLUDE))
# add the common headers
CFLAGS += -I$(ROOTDIR)/user -I$(ROOTDIR)/user/include -I$(ROOTDIR)/dbschema/include \
		  -I$(ROOTDIR)/dbschema/include/dbschema -I$(ROOTDIR)/user/$(NAME) \
		  -I$(ROOTDIR)/user -I$(ROOTDIR)/user/dbapi -I$(ROOTDIR)/user -I$(ROOTDIR)/user/libsmp
# add any pre-process macros required
CFLAGS +=
CFLAGS += -DNWS_IGNORE_SMP_ERROR

# Add to the list of libraries to link against
LDLIBS += -ldbapi -lnojacore -llog -lutils -lpowersystems -lm -lrt -lsmp -lchannel -lwebsockets  -lssl -lcrypto $(LD_DATAREF)
# Add to the library path
LDFLAGS = -L$(LIB)

# Specify the source files.  One approach is to simply accept that all .c and .s
# files in the project directory are source files to be compiled into the project.
# If that will work, then specify the source files as:
CSRC := $(wildcard $(SOURCE)/*.c)

########################################################################################
# The following should not need to be modified.

# Specify the tool chain
#GCC_PREFIX  = m68k-uclinux
#CC          = $(GCC_PREFIX)-gcc -mcpu=5329 -DCONFIG_COLDFIRE
#LD          = $(GCC_PREFIX)-ld
#AR          = $(GCC_PREFIX)-ar
#AS          = $(GCC_PREFIX)-gcc
#GASP        = $(GCC_PREFIX)-gasp
#NM          = $(GCC_PREFIX)-nm
#OBJCOPY     = $(GCC_PREFIX)-objcopy
#OBJDUMP     = $(GCC_PREFIX)-objdump
#RANLIB      = $(GCC_PREFIX)-ranlib
#STRIP       = $(GCC_PREFIX)-strip
#SIZE        = $(GCC_PREFIX)-size
#READELF     = $(GCC_PREFIX)-readelf
CP          = cp -p
RM          = rm -f
MV          = mv
MK          = mkdir -p
#PERL		= perl

# Define the object files based on the source files
COBJS  := $(addprefix $(BUILD_PATH)/,$(notdir $(patsubst %.c,%.o,$(CSRC))))
AOBJS  := $(addprefix $(BUILD_PATH)/,$(notdir $(patsubst %.s,%.o,$(ASRC))))
# Define the dependency files for each source file
CDEPS  := $(patsubst %.o,%.d,$(COBJS))
ADEPS  := $(patsubst %.o,%.d,$(AOBJS))


.PHONY: all doc clean pre_copy

.NOTPARALLEL:

all : pre_copy clean
	$(MAKE) $(BUILD_PATH)/$(NAME)

$(BUILD_PATH)/$(NAME) : $(AOBJS) $(COBJS) Makefile
	$(CC) -o $@ $(AOBJS) $(COBJS) $(LDFLAGS) $(LDLIBS)

# copy and generate the include files
pre_copy:

$(NAME).lst : $(NAME).a
	$(OBJDUMP) -dStl $^ >$@

host: host-clean host-app

#install-doc :
#	../update_docmake.sh -o $(NAME) -s clips -i

doc :

clean :
	$(RM) $(COBJS) $(AOBJS) $(CDEPS) $(ADEPS) $(BUILD_PATH)/$(NAME)
	$(RM) $(NAME).lst $(NAME).map $(wildcard *.stackdump)

$(BUILD_PATH)/%.o: $(SOURCE)/%.c
	test -d $(BUILD_PATH) || mkdir $(BUILD_PATH)
	$(CC) -g -c -o $@ $< $(CFLAGS)

$(BUILD_PATH)/%.d: $(SOURCE)/%.c
	test -d $(BUILD_PATH) || mkdir $(BUILD_PATH)
	$(CC) -MM -MT $(CFLAGS) $< > $@

# include the dependency files
-include $(CDEPS)
-include $(ADEPS)


include ../../host-app.mak