ifeq ($(strip $(ZEDPATH)),)
$(error "Please set ZEDPATH in your environment.  export ZEDPATH=/path/to/ZEDOut")
endif


ifndef TOPDIR
export TOPDIR := $(shell cd ../../ && pwd)
endif

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
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE) \
	--no-print-directory -C $$i $(BUILD) || { exit 1; } fi; done;

.PHONY: clean
clean:
	@printf "Removing files from: $(TOPDIR)/Obj ... "
	@rm -rf $(TOPDIR)/Obj
	@printf "[OK]\n"
	@printf "Removing files from: $(TOPDIR)/Lib ... "
	@rm -rf $(TOPDIR)/Lib
	@printf "[OK]\n"

