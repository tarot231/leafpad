AC_INIT(leafpad, 0.8.19, tarot@sdf.org)
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS(config.h)

AC_PROG_INTLTOOL(0.31, no-xml)
AC_PROG_CC
if test "$GCC" = "yes"; then
	CFLAGS="$CFLAGS -Wall"
fi
AM_PROG_CC_C_O

PKG_CHECK_MODULES(GTK, gtk+-2.0)

AC_ARG_ENABLE(chooser,
	AC_HELP_STRING(--disable-chooser, [force to use GtkFileSelector]))
if test "$enable_chooser" != "no"; then
	AC_DEFINE(ENABLE_CHOOSER, 1, [Define if GtkFileChooser is enabled.])
fi

AC_ARG_ENABLE(print,
	AC_HELP_STRING(--disable-print, [force to disable print feature]))
if test "$enable_print" != "no"; then
	AC_DEFINE(ENABLE_PRINT, 1, [Define if print feature is enabled.])
fi

AC_ARG_ENABLE(emacs,
	AC_HELP_STRING(--enable-emacs, [implement Emacs key theme (experimental)]),
	AC_DEFINE(ENABLE_EMACS, 1, [Define if Emacs key theme is enabled.])
)

GETTEXT_PACKAGE="$PACKAGE"
AC_SUBST(GETTEXT_PACKAGE)
AM_GLIB_GNU_GETTEXT
AM_GLIB_DEFINE_LOCALEDIR(LOCALEDIR)

AC_OUTPUT([
	Makefile
	src/Makefile
	data/Makefile
	data/icons/Makefile
	data/icons/16x16/Makefile
	data/icons/22x22/Makefile
	data/icons/24x24/Makefile
	data/icons/32x32/Makefile
	data/icons/scalable/Makefile
	po/Makefile.in
])
