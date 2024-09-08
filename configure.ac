AC_INIT([leafpad],[0.8.19],[tarot@sdf.org])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS(config.h)

IT_PROG_INTLTOOL([0.31],[no-xml])
AC_PROG_CC
if test "$GCC" = "yes"; then
	CFLAGS="$CFLAGS -Wall"
fi
AM_PROG_CC_C_O

PKG_CHECK_MODULES(GTK, gtk+-2.0)

AC_ARG_ENABLE(chooser,
	AS_HELP_STRING([--disable-chooser],[force to use GtkFileSelector]))
if test "$enable_chooser" != "no"; then
	PKG_CHECK_EXISTS(gtk+-2.0 >= 2.4.0,
		AC_DEFINE(ENABLE_CHOOSER, 1, [Define if GtkFileChooser is enabled.]))
fi

AC_ARG_ENABLE(print,
	AS_HELP_STRING([--disable-print],[force to disable print feature]))
if test "$enable_print" != "no"; then
	PKG_CHECK_EXISTS(gtk+-2.0 >= 2.10.0, have_print=yes, [
		PKG_CHECK_MODULES(GNOMEPRINT, libgnomeprintui-2.2,
			have_print=yes, have_print=no)])
	if test "$have_print" = "yes"; then
		AC_DEFINE(ENABLE_PRINT, 1, [Define if print feature is enabled.])
	fi
fi

AC_ARG_ENABLE(emacs,
	AS_HELP_STRING([--enable-emacs],[implement Emacs key theme (experimental)]),
	AC_DEFINE(ENABLE_EMACS, 1, [Define if Emacs key theme is enabled.])
)

GETTEXT_PACKAGE="$PACKAGE"
AC_SUBST(GETTEXT_PACKAGE)
AM_GLIB_GNU_GETTEXT
AM_GLIB_DEFINE_LOCALEDIR(LOCALEDIR)

AC_CONFIG_FILES([
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
AC_OUTPUT
