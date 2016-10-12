MESON = meson
MESONCONF = mesonconf
NINJA = ninja
INSTALL = install

BUILD_DIR = build
BUILD_NINJAFILE = $(BUILD_DIR)/build.ninja
BUILD_CMD = $(NINJA) -C $(BUILD_DIR)

all: $(BUILD_NINJAFILE)
	$(BUILD_CMD)

clean:
	$(BUILD_CMD) clean

distclean: remove-builddir

remove-builddir:
	rm -rf $(BUILD_DIR)

builddir: $(BUILD_DIR)

builddir-ninja: $(BUILD_NINJAFILE)

$(BUILD_DIR):
	$(INSTALL) -d $(BUILD_DIR)

$(BUILD_NINJAFILE):
	$(MAKE) remove-builddir
	$(MAKE) $(BUILD_DIR)
	meson $(BUILD_DIR)

.PHONY: clean distclean remove-builddir builddir
