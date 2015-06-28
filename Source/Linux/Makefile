###############################################################################
#                                                               ZED for Linux #
###############################################################################
ifeq ($(strip $(ZEDPATH)),)
$(error "Please set ZEDPATH in your environment.  export ZEDPATH=/path/to/ZEDOut")
endif

ifndef TOPDIR
export TOPDIR	:= $(shell cd ../../ && pwd)
export TOPSRC	:= $(TOPDIR)/Source
export BUILDDIR	:= $(shell pwd)
export PLATFORM	:= $(notdir $(CURDIR))
export BUILD_PLATFORM	:= LINUX
export BUILD_ARCH		:= X86
export ARCH			:= x86
export BITSIZE			:= 32
export BUILD_TYPE		:= debug

# Get the machine type so, hopefully, the programmer building this will not
# receive a bunch of errors upon compilation
export UNAME			:= $(shell uname)
export UNAME_MACHINE	:= $(shell uname -m)

export GCCVER := $(shell $(CPP) -dumpversion)
export GCCVER_MAJOR := $(shell echo $(GCCVER) | cut -f1 -d.)
export GCCVER_MINOR := $(shell echo $(GCCVER) | cut -f2 -d.)
export CPP0XFLAG := $(shell [ $(GCCVER_MAJOR) -gt 4 -o \( $(GCCVER_MAJOR) -eq 4 -a $(GCCVER_MINOR) -ge 3 \) ] && echo true)
export CPP11FLAG := $(shell [ $(GCCVER_MAJOR) -gt 4 -o \( $(GCCVER_MAJOR) -eq 4 -a $(GCCVER_MINOR) -ge 7 \) ] && echo true)
export CPP := g++

32BIT	?= False
64BIT	?= False

ifeq ($(UNAME), Linux)
	export CPPFLAGS_EXT	= -ffriend-injection
ifeq ($(CPP11FLAG),true)
	CPPFLAGS_EXT += -std=c++11
else
ifeq ($(CPP0XFLAG),true)
	CPPFLAGS_EXT += -std=c++0x
endif
endif
	export SYSIPATH		= -I/usr/include/xorg
	export SYSLIBS			= -lX11 -lGL -lrt
	export LINKFLAGS		=
##### Check for forced 32-bit or 64-bit builds
ifneq ($(64BIT),False)
	CPPFLAGS_EXT	+= -m64
	BUILD_ARCH		:= X86
	ARCH			:= x86
	BITSIZE			:= 64
else
ifneq ($(32BIT),False)
	CPPFLAGS_EXT	+= -m32
	BUILD_ARCH		:= X86
	ARCH			:= x86
	BITSIZE			:= 32
else
##### Nothing forced.  Use what the machine reports ###########################
ifeq ($(UNAME_MACHINE), x86_64)
	CPPFLAGS_EXT	+= -m64
	BUILD_ARCH		:= X86
	ARCH			:= x86
	BITSIZE			:= 64
endif
ifeq ($(UNAME_MACHINE), i686)
	CPPFLAGS_EXT	+= -m32
	BUILD_ARCH		:= X86
	ARCH			:= x86
	BITSIZE			:= 32
endif
endif
endif
endif
endif

ifndef TARGETDIR
export TARGETDIR = $(TOPDIR)/Lib/$(PLATFORM)/$(ARCH)/$(BITSIZE)
export SOURCEDIR = .
export HEADERDIR = Headers
endif

COPYFILES ?= True

SUBDIRS			:= `find Source -type d | egrep -v '/\..*$$|^.$$'`
COMMONSUBDIRS	:= `find $(TOPSRC)/Common/Source -type d | egrep -v '/\..*$$|^.$$'`

debug: BUILD = debug
debug: ZED
ifeq ($(COPYFILES), True)
debug: copyfiles
endif

release: BUILD = release
release: ZED
ifeq ($(COPYFILES), True)
release: copyfiles
endif

profile: BUILD = profile
profile: ZED
ifeq ($(COPYFILES), True)
profile: copyfiles
endif

psdocs:	BUILD = psdocs
psdocs:	ZED

VERSIONINFO:
	@printf "%s" "------------------------- Generating Version Information ---"
	@printf "%s\n" "-------------------"
	@mkdir -p Headers
	@sh ./GitVersion.sh $(TOPSRC)/Common/Headers/System/GitVersion.hpp ZED
	@printf "%s" "------------------------------------------------------------"
	@printf "%s\n" "-------------------"

ZED: VERSIONINFO
	@for i in $(COMMONSUBDIRS); do if test -e \
	$$i/Makefile ; then $(MAKE) --no-print-directory -C $$i $(BUILD) \
	|| { exit 1; } fi; done;
	@for i in $(SUBDIRS); do if test -e \
	$$i/Makefile ; then $(MAKE) --no-print-directory -C $$i $(BUILD) \
	|| { exit 1; } fi; done;

.PHONY: clean
clean:
	@printf "Removing files from: /Obj ... "
	@rm -rf $(TOPDIR)/Obj
	@printf "[OK]\n"
	@printf "Removing files from: /Lib ... "
	@rm -rf $(TOPDIR)/Lib
	@printf "[OK]\n"

#####################
ifeq ($(COPYFILES), True)
.PHONY: copyfiles
$(ZEDPATH)/include/$(PLATFORM):
	@printf "%s" "------------------------------------------------------------"
	@printf "%s\n" "-------------------"
	@printf "Creating directory: $(ZEDPATH)/include/$(PLATFORM) ... "
	@mkdir -p $(ZEDPATH)/include/$(PLATFORM)
	@printf "[OK]\n"
	@printf "%s" "------------------------------------------------------------"
	@printf "%s\n\n" "-------------------"

$(ZEDPATH)/lib/$(PLATFORM)/$(ARCH)/$(BITSIZE):
	@printf "%s" "------------------------------------------------------------"
	@printf "%s\n" "-------------------"
	@printf "Creating directory: $(ZEDPATH)/lib/$(PLATFORM)/$(ARCH)/$(BITSIZE) ... "
	@mkdir -p $(ZEDPATH)/lib/$(PLATFORM)/$(ARCH)/$(BITSIZE)
	@printf "[OK]\n"
	@printf "%s" "------------------------------------------------------------"
	@printf "%s\n\n" "-------------------"

copyfiles: $(ZEDPATH)/include/$(PLATFORM) $(ZEDPATH)/lib/$(PLATFORM)/$(ARCH)/$(BITSIZE)
	@printf "%s" "------------------------------------------------------------"
	@printf "%s\n" "-------------------"
	@printf "Copying local header files to: $(ZEDPATH)/include/$(PLATFORM) ..."
	@printf " "
	@cp -rf $(BUILDDIR)/Headers/* $(ZEDPATH)/include/$(PLATFORM)/
	@printf "[OK]\n"
	@printf "Copying common header files to: $(ZEDPATH)/include/$(PLATFORM) .."
	@printf ". "
	@cp -rf $(TOPSRC)/Common/Headers/* $(ZEDPATH)/include/$(PLATFORM)/
	@printf "[OK]\n"
	@printf "Copying library files to $(ZEDPATH)/lib/$(PLATFORM)/$(ARCH)/$(BITSIZE) ... "
	@files=$$(ls $(TARGETDIR)/*.a 2> /dev/null | wc -l);\
	if [ "$$files" != "0" ] ; then cp -f $(TARGETDIR)/*.a \
	$(ZEDPATH)/lib/$(PLATFORM)/$(ARCH)/$(BITSIZE)/; fi
	@files=$$(ls $(TARGETDIR)/*.so 2> /dev/null | wc -l);\
	if [ "$$files" != "0" ] ; then cp -f $(TARGETDIR)/*.so \
	$(ZEDPATH)/lib/$(PLATFORM)/$(ARCH)/$(BITSIZE)/; fi
	@printf "[OK]\n"
	@printf "%s" "------------------------------------------------------------"
	@printf "%s\n" "-------------------"
endif
#####################
