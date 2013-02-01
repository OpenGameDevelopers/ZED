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

debug: BUILD = debug
debug: ZED

release: BUILD = release
release: ZED

profile: BUILD = profile
profile: ZED

clean: BUILD = clean
clean: ZED

ZED: FORCE
	cd System && $(MAKE) $(BUILD)
	cd Arithmetic && $(MAKE) $(BUILD)
	cd Renderer && $(MAKE) $(BUILD)

FORCE:

