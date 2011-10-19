ifeq ($(strip $(GLINCPATH)),)
$(error "Please set GLINCPATH in your environment.  export GLINCPATH=/path/to/OpenGL/include")
endif
ifeq ($(strip $(ZEDPATH)),)
$(error "Please set ZEDPATH in your environment.  export ZEDPATH=/path/to/ZEDOut")
endif

release: BUILD = release
release: ZED

debug: BUILD = debug
debug: ZED

profile: BUILD = profile
profile: ZED

clean: BUILD = clean
clean: ZED

ZED: FORCE
	cd System && $(MAKE) $(BUILD)
	cd Arithmetic && $(MAKE) $(BUILD)
	cd Renderer && $(MAKE) $(BUILD)

FORCE:

