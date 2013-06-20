###############################################################################
#                                                             ZED for Pandora #
###############################################################################
ifeq ($(strip $(ZEDPATH)),)
$(error "Please set ZEDPATH in your environment.  export ZEDPATH=/path/to/ZED")
endif

define \n


endef

ifeq ($(strip $(PNDSDK)),)
$(error $(\n)Please set PNDSDK in your environment:$(\n)$$ export PNDSDK=/path/to/PNDSDK)
endif

ifndef TOPDIR
export TOPDIR := $(shell cd ../../ && pwd)
export TOPSRC := $(TOPDIR)/Source
export BUILDDIR	:= $(shell pwd)
endif

COPYFILES ?= True

SUBDIRS := `ls | egrep -v '^(CVS)$$'`
COMMONSUBDIRS	:= `ls $(TOPSRC)/Common/Source | egrep -v '^(CVS)$$'`

debug: BUILD = debug
debug: ZED

release: BUILD = release
release: ZED

profile: BUILD = profile
profile: ZED

clean: BUILD = clean
clean: ZED

ZED:
	@for i in $(TOPSRC)/Common/Source/$(COMMONSUBDIRS); do if test -e \
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

