SUBDIRS = lib
ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = m4

debug:
	$(MAKE) all "CFLAGS=-g -DDEBUG " $(AM_MAKEFLAGS)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = bios.pc

# Doxygen
if USE_DOXYGEN

doxygen:
	cd doc && doxygen Doxyfile

clean-local:
	rm -rf $(top_srcdir)/doc/html



.PHONY: doxygen clean-local
endif
