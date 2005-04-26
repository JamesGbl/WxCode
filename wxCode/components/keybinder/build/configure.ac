# ======================================================
# A basic "configure.ac" using a WXBASE-based bakefile
# ======================================================

AC_PREREQ(2.57)
AC_INIT(keybinder, 1.0, [frm@sourceforge.net])


# Some standard checks are required...
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_RANLIB
AC_PROG_MAKE_SET


# This allows us to use Bakefile, recognizing the system type
# (and sets the AC_CANONICAL_BUILD, AC_CANONICAL_HOST and
# AC_CANONICAL_TARGET variables)
AC_CANONICAL_SYSTEM

AM_OPTIONS_WXCONFIG
AM_PATH_WXCONFIG(2.5.3, wxWin=1)
if test "$wxWin" != 1; then
     AC_MSG_ERROR([
              wxWindows must be installed on your system
              but wx-config script couldn't be found.

              Please check that wx-config is in path, the directory
              where wxWindows libraries are installed (returned by
              'wx-config --libs' command) is in LD_LIBRARY_PATH or
              equivalent variable and wxWindows version is 2.5.3 or above.
     ])
fi

CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
LDFLAGS="$LDFLAGS $WX_LIBS"



# The final lines of the configure script
AC_BAKEFILE
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
