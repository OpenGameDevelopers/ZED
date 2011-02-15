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

