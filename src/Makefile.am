## Process this file with automake to produce Makefile.in

## Created by Anjuta

uidir = $(datadir)/flashbulb/ui
ui_DATA = flashbulb.ui

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(FLASHBULB_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = flashbulb

flashbulb_SOURCES = \
	callbacks.c \
	callbacks.h \
	main.c     \
	flashbulb-card.h     \
	flashbulb-card.c

flashbulb_LDFLAGS = \
	-Wl,--export-dynamic

flashbulb_LDADD = $(FLASHBULB_LIBS)

EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(datadir)/flashbulb
