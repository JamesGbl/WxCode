# ==============================================================
# A basic "configure.ac" using a WX-based bakefile
#
# Author: Francesco Montorsi
# RCS-ID: $Id$
# ==============================================================

AC_PREREQ(2.59)
AC_INIT(webupdate, 1.0, [frm@sourceforge.net])

# This allows us to use Bakefile, recognizing the system type
# (and sets the AC_CANONICAL_BUILD, AC_CANONICAL_HOST and
# AC_CANONICAL_TARGET variables)
AC_CANONICAL_SYSTEM

# Checks for programs.
AC_PROG_AWK
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_RANLIB
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CXXCPP



# Checks for libraries (these checks are used to set config.h)
# ------------------------------------------------------------
AC_LANG(C++)

# Check for wxWidgets
AM_OPTIONS_WXCONFIG
AM_PATH_WXCONFIG(2.6.0, wxWin=1, wxWin=0)
if test "$wxWin" != 1; then
	AC_MSG_ERROR([
	   wxWidgets must be installed on your system
	   but wx-config script couldn't be found.
	
	   Please check that wx-config is in path, the directory
	   where wxWidgets libraries are installed (returned by
	   'wx-config --libs' command) is in LD_LIBRARY_PATH or
	   equivalent variable and wxWidgets version is 2.5.3 or above.
	])
else
	# do not update CPPFLAGS nor CXXFLAGS otherwise we'll get the
	# same settings repetead three times (since Makefile.in merges these
	# two variables with $WX_CPPFLAGS automatically)
	#CPPFLAGS="$CPPFLAGS"
	CXXFLAGS="$CXXFLAGS"
	CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
	LDFLAGS="$LDFLAGS $WX_LIBS"
fi




# Synch our DEBUG,UNICODE,SHARED variables with wx ones
# -----------------------------------------------------

# these var will be set to '0' when they are FALSE, '1' when they are TRUE
UNICODE_FLAG=$(test ! $($WX_CONFIG_WITH_ARGS --unicode >/dev/null 2>&1))
DEBUG_FLAG=$(test ! $($WX_CONFIG_WITH_ARGS --debug >/dev/null 2>&1))
SHARED_FLAG=$($WX_CONFIG_WITH_ARGS --static >/dev/null 2>&1)

AC_MSG_CHECKING([if wxWidgets was built with UNICODE enabled])
if test "$UNICODE_FLAG" != "0"; then
	AC_MSG_RESULT([yes])
	WX_UNICODE=1
else
	AC_MSG_RESULT([no])
	WX_UNICODE=0
fi


AC_MSG_CHECKING([if wxWidgets was built in DEBUG or RELEASE mode])
if test "$DEBUG_FLAG" != "0"; then
	AC_MSG_RESULT([debug])
	WX_DEBUG=1
else
	AC_MSG_RESULT([release])
	WX_DEBUG=0
fi

AC_MSG_CHECKING([if wxWidgets was built in SHARED or STATIC mode])
if test "$SHARED_FLAG" != "0"; then
	AC_MSG_RESULT([shared])
	WX_SHARED=1
else
	AC_MSG_RESULT([static])
	WX_SHARED=0
fi




## Final macros
## ------------

AC_BAKEFILE([m4_include(autoconf_inc.m4)])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT



# Just a little msg for GNU AUTOtools newbies (like me)
echo
echo " ------------------------------------------------------"
echo "  Successfully configured wxDockIt ${PACKAGE_VERSION}:"
echo "  wxDockIt will be built using the same configuration "
echo "  used to build wxWidgets:"
if [[ "$WX_DEBUG" = "1" ]]; then
	echo "  - DEBUG build"
else
	echo "  - RELEASE build"
fi
if [[ "$WX_UNICODE" = "1" ]]; then
	echo "  - UNICODE enabled"
else
	echo "  - UNICODE disabled"
fi
if [[ "$WX_SHARED" = "1" ]]; then
	echo "  - SHARED mode"
else
	echo "  - STATIC mode"
fi
echo
echo "  Now, just run make."
echo " ------------------------------------------------------"
echo
