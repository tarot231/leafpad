bin_PROGRAMS = leafpad

leafpad_SOURCES = \
	leafpad.h main.c \
	window.h window.c \
	menu.h menu.c \
	callback.h callback.c \
	view.h view.c \
	undo.h undo.c \
	font.h font.c \
	linenum.h linenum.c \
	indent.h indent.c \
	hlight.h hlight.c \
	selector.h selector.c \
	file.h file.c \
	encoding.h encoding.c \
	search.h search.c \
	dialog.h dialog.c \
	gtkprint.h gtkprint.c \
	gnomeprint.h gnomeprint.c \
	about.h about.c \
	utils.h utils.c \
	emacs.h emacs.c \
	gtksourceiter.h gtksourceiter.c \
	i18n.h

leafpad_LDADD    = $(GTK_LIBS) $(GNOMEPRINT_LIBS) $(INTLLIBS)
leafpad_CFLAGS   = $(GTK_CFLAGS) $(GNOMEPRINT_CFLAGS)
leafpad_CPPFLAGS = -DICONDIR=\"$(datadir)/pixmaps\"
