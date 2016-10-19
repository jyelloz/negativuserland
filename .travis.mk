PREFIX = $(PWD)/pfx
BINDIR = $(PREFIX)/bin
LIBDIR = $(PREFIX)/lib
SOURCE_DIR = $(PWD)/src
WORK_DIR = $(PWD)/work

CURL = curl
INSTALL = install
TAR = bsdtar

MAKEFLAGS = -j8

GLIB_VERSION_MAJOR = 2.50
GLIB_VERSION_MINOR = 1
GLIB_VERSION = $(GLIB_VERSION_MAJOR).$(GLIB_VERSION_MINOR)

LIBGEE_VERSION_MAJOR = 0.18
LIBGEE_VERSION_MINOR = 1
LIBGEE_VERSION = $(LIBGEE_VERSION_MAJOR).$(LIBGEE_VERSION_MINOR)

GTK_VERSION_MAJOR = 3.22
GTK_VERSION_MINOR = 1
GTK_VERSION = $(GTK_VERSION_MAJOR).$(GTK_VERSION_MINOR)

NINJA_VERSION = 1.7.1
MESON_VERSION = 0.35.1

GLIB_BASENAME = glib-$(GLIB_VERSION).tar.xz
LIBGEE_BASENAME = libgee-$(LIBGEE_VERSION).tar.xz
GTK_BASENAME = gtk+-$(GTK_VERSION).tar.xz
NINJA_BASENAME = ninja-linux.zip
MESON_BASENAME = meson-$(MESON_VERSION).tar.gz

GLIB_SOURCE = $(SOURCE_DIR)/$(GLIB_BASENAME)
LIBGEE_SOURCE = $(SOURCE_DIR)/$(LIBGEE_BASENAME)
GTK_SOURCE = $(SOURCE_DIR)/$(GTK_BASENAME)
NINJA_SOURCE = $(SOURCE_DIR)/$(NINJA_BASENAME)
MESON_SOURCE = $(SOURCE_DIR)/$(MESON_BASENAME)

GNOME_MIRROR = http://ftp.gnome.org/pub/GNOME/sources

GLIB_URL = $(GNOME_MIRROR)/glib/$(GLIB_VERSION_MAJOR)/$(GLIB_BASENAME)
LIBGEE_URL = $(GNOME_MIRROR)/libgee/$(LIBGEE_VERSION_MAJOR)/$(LIBGEE_BASENAME)
GTK_URL = $(GNOME_MIRROR)/gtk+/$(GTK_VERSION_MAJOR)/$(GTK_BASENAME)
NINJA_URL = https://github.com/ninja-build/ninja/releases/download/v$(NINJA_VERSION)/$(NINJA_BASENAME)
MESON_URL = https://github.com/mesonbuild/meson/releases/download/$(MESON_VERSION)/$(MESON_BASENAME)

GLIB_INSTALL_STAMP = $(LIBDIR)/libglib-2.0.so
GTK_INSTALL_STAMP = $(LIBDIR)/libgtk-3.so
LIBGEE_INSTALL_STAMP = $(LIBDIR)/libgee-0.8.so
NINJA_INSTALL_STAMP = $(BINDIR)/ninja
MESON_INSTALL_STAMP = $(BINDIR)/meson.py

all: builddeps rundeps

builddeps: meson ninja
rundeps: glib libgee gtk

glib: $(GLIB_INSTALL_STAMP)
gtk: $(GTK_INSTALL_STAMP)
libgee: $(LIBGEE_INSTALL_STAMP)

ninja: $(NINJA_INSTALL_STAMP)
meson: $(MESON_INSTALL_STAMP)

$(SOURCE_DIR):
	$(INSTALL) -d $(SOURCE_DIR)

$(WORK_DIR):
	$(INSTALL) -d $(WORK_DIR)

$(PREFIX):
	$(INSTALL) -d $(PREFIX)

$(BINDIR): $(PREFIX)
	$(INSTALL) -d $(BINDIR)

$(GLIB_SOURCE): $(SOURCE_DIR)
	$(CURL) -L -o $(GLIB_SOURCE) $(GLIB_URL)

$(GTK_SOURCE): $(SOURCE_DIR)
	$(CURL) -L -o $(GTK_SOURCE) $(GTK_URL)

$(LIBGEE_SOURCE): $(SOURCE_DIR)
	$(CURL) -L -o $(LIBGEE_SOURCE) $(LIBGEE_URL)

$(NINJA_SOURCE): $(SOURCE_DIR)
	$(CURL) -L -o $(NINJA_SOURCE) $(NINJA_URL)

$(MESON_SOURCE): $(SOURCE_DIR)
	$(CURL) -L -o $(MESON_SOURCE) $(MESON_URL)

$(GLIB_INSTALL_STAMP): $(GLIB_SOURCE) $(WORK_DIR)
	$(TAR) xf $(GLIB_SOURCE) -C $(WORK_DIR)
	cd $(WORK_DIR)/glib-$(GLIB_VERSION) && \
		./configure --prefix=$(PREFIX) --disable-introspection --disable-libmount --with-pcre=internal && \
		$(MAKE) $(MAKEFLAGS) \
		&& $(MAKE) install

$(GTK_INSTALL_STAMP): $(GTK_SOURCE) $(WORK_DIR) $(GLIB_INSTALL_STAMP)
	$(TAR) xf $(GTK_SOURCE) -C $(WORK_DIR)
	cd $(WORK_DIR)/gtk+-$(GTK_VERSION) && \
		./configure --prefix=$(PREFIX) --disable-introspection && \
		$(MAKE) $(MAKEFLAGS) && \
		$(MAKE) install

$(LIBGEE_INSTALL_STAMP): $(LIBGEE_SOURCE) $(WORK_DIR) $(GLIB_INSTALL_STAMP)
	$(TAR) xf $(LIBGEE_SOURCE) -C $(WORK_DIR)
	cd $(WORK_DIR)/libgee-$(LIBGEE_VERSION) && \
		./configure --prefix=$(PREFIX) --disable-introspection && \
		$(MAKE) $(MAKEFLAGS) && \
		$(MAKE) install

$(NINJA_INSTALL_STAMP): $(NINJA_SOURCE) $(WORK_DIR)
	$(TAR) xf $(NINJA_SOURCE) -C $(WORK_DIR)
	$(INSTALL) -m 0755 $(WORK_DIR)/ninja $(BINDIR)

$(MESON_INSTALL_STAMP): ninja $(MESON_SOURCE) $(WORK_DIR) $(BINDIR)
	$(TAR) xf $(MESON_SOURCE) -C $(WORK_DIR)
	ln -svnf $(WORK_DIR)/meson-$(MESON_VERSION)/meson.py $(BINDIR)
	ln -svnf $(WORK_DIR)/meson-$(MESON_VERSION)/mesonconf.py $(BINDIR)

.PHONY: all meson ninja gtk libgee glib builddeps rundeps
