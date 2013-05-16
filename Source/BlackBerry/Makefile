# Check for environment variables that should be set
ifeq ($(strip $(QNX_TARGET)),)
$(error Please run: $$ source /path/to/bbndk/bbndk-env.sh to set up your \
environment variables )
endif

ifeq ($(strip $(QNX_HOST)),)
$(error Please run: $$ source /path/to/bbndk/bbndk-env.sh to set up your \
environment variables )
endif

ifeq ($(strip $(QNX_CONFIGURATION)),)
$(error Please run: $$ source /path/to/bbndk/bbndk-env.sh to set up your \
environment variables )
endif

ifeq ($(strip $(ZEDPATH)),)
$(error "Please set ZEDPATH in your environment.  export ZEDPATH=/path/to/ZEDOut")
endif

ifndef TOPDIR
export TOPDIR := $(shell cd ../../ && pwd)
export TOPSRC = $(TOPDIR)/Source
endif

COPYFILES ?= True

SUBDIRS := `ls | egrep -v '^(CVS)$$'`

debug: BUILD = debug
debug: ZED

release: BUILD = release
release: ZED

profile: BUILD = profile
profile: ZED

psdocs:	BUILD = psdocs
psdocs:	ZED

ZED:
	@for i in $(SUBDIRS); do if test -e \
	$$i/Makefile ; then $(MAKE) --no-print-directory -C $$i $(BUILD) \
	|| { exit 1; } fi; done;

.PHONY: clean
clean:
	@printf "Removing files from: $(TOPDIR)/Obj ... "
	@rm -rf $(TOPDIR)/Obj
	@printf "[OK]\n"
	@printf "Removing files from: $(TOPDIR)/Lib ... "
	@rm -rf $(TOPDIR)/Lib
	@printf "[OK]\n"

