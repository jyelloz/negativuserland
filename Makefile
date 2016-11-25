MESON = meson
MESONCONF = mesonconf
NINJA = ninja
INSTALL = install

NINJA_FLAGS ?=

ifeq ($(V),1)
NINJA_FLAGS += -v
endif

BUILD_DIR = build
BUILD_NINJAFILE = $(BUILD_DIR)/build.ninja
BUILD_CMD = $(NINJA) $(NINJA_FLAGS) -C $(BUILD_DIR)

all: configure
	$(BUILD_CMD)

help: configure
	$(BUILD_CMD) -t list

configure: builddir-ninja

check: all
	$(BUILD_CMD) test

clean: builddir-ninja
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
	$(MESON) $(BUILD_DIR)

.DEFAULT: configure
	$(BUILD_CMD) -t $@

.PHONY: clean help configure check distclean remove-builddir builddir
