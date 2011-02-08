release: BUILD = release
release: ZED

debug: BUILD = debug
debug: ZED

profile: BUILD = profile
profile: ZED

clean: BUILD = clean
clean: ZED

ZED: FORCE
	cd Arithmetic && $(MAKE) $(BUILD)
	cd Debugging && $(MAKE) $(BUILD)
	cd Renderer && $(MAKE) $(BUILD)
#cd System && $(MAKE) $(BUILD)

FORCE:

