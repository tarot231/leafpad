SUBDIRS = icons

desktopdir       = $(datadir)/applications
desktop_in_files = leafpad.desktop.in
desktop_DATA     = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

pixmapsdir       = $(datadir)/pixmaps
pixmaps_DATA     = leafpad.png leafpad.xpm

dist_man_MANS = leafpad.1

EXTRA_DIST = \
	$(desktop_DATA) $(desktop_in_files) \
	$(pixmaps_DATA) $(dist_man_MANS)
