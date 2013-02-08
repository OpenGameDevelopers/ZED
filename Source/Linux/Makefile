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

clean: BUILD = clean
clean: ZED

ZED:
	@for i in $(SUBDIRS); do if test -e $$i/Makefile ; then $(MAKE) -C $$i \
	$(BUILD) || { exit 1; } fi; done;

