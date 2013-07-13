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
endif

COPYFILES ?= True

SUBDIRS			:= `find Source -type d | egrep -v '/\..*$$|^.$$'`
COMMONSUBDIRS	:= `find $(TOPSRC)/Common/Source -type d | egrep -v '/\..*$$|^.$$'`

debug: BUILD = debug
debug: ZED

release: BUILD = release
release: ZED

profile: BUILD = profile
profile: ZED

psdocs:	BUILD = psdocs
psdocs:	ZED

ZED:
	@for i in $(COMMONSUBDIRS); do if test -e \
	$$i/Makefile ; then $(MAKE) --no-print-directory -C $$i $(BUILD) COPYFILES=False \
	|| { exit 1; } fi; done;
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

