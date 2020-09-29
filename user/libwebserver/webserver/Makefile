# Include the file which creates VER_xxx environmental variables if they don't
# already exist.
include ../../global.mak
include $(NOJACORE)/nojacore.mak

NAME = webserver

WORKDIR =  $(ROOTDIR)/user/libwebserver
# build location
BUILD_PATH = $(WORKDIR)/bin
# host-mode build location
HOST_BUILD_PATH = $(WORKDIR)/bin-host
TOP_DIR = $(ROOTDIR)/user
# Test directories
TEST_DIR = $(WORKDIR)/test
# source directory
SOURCE =  $(WORKDIR)/webserver
# header directory
INCLUDE = $(WORKDIR)/webserver  $(WORKDIR)/webserver/subprotocols
# common library dir path
ifeq ($(MAKECMDGOALS),host)
LIB = $(ROOTDIR)/user/lib-host
else
LIB = $(ROOTDIR)/user/lib
endif

LDLIBS += -l$(NAME)

# create the compile flags
CFLAGS += -O3 -fPIC
# add the headers path
CFLAGS += $(addprefix -I$(WORKDIR)/, $(INCLUDE))
# add the common headers
CFLAGS += -I$(ROOTDIR)/user -I$(ROOTDIR)/user/include -I$(ROOTDIR)/dbschema/include -I$(ROOTDIR)/user/dbapi\
		  -I$(ROOTDIR)/dbschema/include/dbschema -I$(ROOTDIR)/user/lib$(NAME) \
		  -I$(ROOTDIR)/lib3p/libwebsockets/include -I$(ROOTDIR)/lib3p/libcjson \
		  -I$(SDKTARGETSYSROOT)/usr/include/libxml2
# add any pre-process macros required
CFLAGS += -DNWS_DEBUG
# Add to the list of libraries to link against
LDLIBS += -ldbapi -lnojacore -llog -lutils -lpowersystems -lm -lrt -lwebsockets -lcjson -lssl -lcrypto -llog -pthread $(LD_DATAREF)
# Add to the library path
LDFLAGS = -L$(LIB)

# Specify the source files.  One approach is to simply accept that all .c and .s
# files in the project directory are source files to be compiled into the project.
# If that will work, then specify the source files as:
CSRC := $(wildcard $(SOURCE)/*.c)
# The source files for unit tests.  Tests are built and run with 'make check'.
UNITTEST_CSRC = $(wildcard $(TEST_DIR)/*.c)

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

all : pre_copy
	$(MAKE) $(BUILD_PATH)/lib$(NAME).a

$(BUILD_PATH)/lib$(NAME).a : $(AOBJS) $(COBJS) Makefile
	$(AR) rcs $@ $(AOBJS) $(COBJS)
	$(MK) $(LIB)
	$(CP) $@ $(LIB)

# copy and generate the include files
pre_copy:

lib$(NAME).lst : lib$(NAME).a
	$(OBJDUMP) -dStl $^ >$@

#install-doc :
#	../update_docmake.sh -o $(NAME) -s clips -i

doc :
	doxygen ../artefacts/Doxyfile

clean :
	$(RM) $(COBJS) $(AOBJS) $(CDEPS) $(ADEPS) $(wildcard $(BUILD_PATH)/lib$(NAME).*)
	$(RM) lib$(NAME).lst lib$(NAME).map $(wildcard *.stackdump)
	$(RM) $(LIB)/lib$(NAME).a pre_copy

$(BUILD_PATH)/%.o: $(SOURCE)/%.c
	test -d $(BUILD_PATH) || mkdir $(BUILD_PATH)
	$(CC) -fPIC -g -c -o $@ $< $(CFLAGS)

$(BUILD_PATH)/%.d: $(SOURCE)/%.c
	test -d $(BUILD_PATH) || mkdir $(BUILD_PATH)
	$(CC) -MM -MT $(CFLAGS) $< > $@

# include the dependency files
-include $(CDEPS)
-include $(ADEPS)


include ../../host-lib.mak