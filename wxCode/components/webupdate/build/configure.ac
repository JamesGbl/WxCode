# ======================================================
# A basic "configure.ac" using a WXBASE-based bakefile
# ======================================================

AC_PREREQ(2.57)
AC_INIT(wxxml2, 2.0, [frm@sourceforge.net])


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

# Two little custom macros which define the ENABLE/WITH configure arguments.
# Macro arguments:
# $1 = the name of the --enable- feature
# $2 = the description of that feature
# $3 = the default value for that feature
#
# NOTE: the space between the two hyphens (-) are required since in XML
#       they are forbidden... remove them before using this file
#
AC_DEFUN([MY_ARG_ENABLE], [AC_ARG_ENABLE($1, 
			AC_HELP_STRING([--enable-$1],
			[$2 (default is $3)]),, [enable_$1=$3])])
AC_DEFUN([MY_ARG_WITH], [AC_ARG_WITH($1, 
			AC_HELP_STRING([--with-$1],
			[$2 (default is $3)]),, [with_$1=$3])])

MY_ARG_ENABLE([shared], [Builds shared libraries instead of static libraries], [no])
MY_ARG_ENABLE([debug], [Builds in debug mode], [yes])
MY_ARG_ENABLE([unicode], [Builds in UNICODE mode], [no])

if test "$enable_shared" = "yes"; then
   SHARED=1
   AC_MSG_RESULT([Checking for the SHARED option... yes])
else
   SHARED=0
   AC_MSG_RESULT([Checking for the SHARED option... no])
fi

if test "$enable_debug" = "yes"; then
   BUILD=debug
   AC_MSG_RESULT([Checking for the DEBUG option... yes])
else
   BUILD=release
   AC_MSG_RESULT([Checking for the DEBUG option... no])
fi

if test "$enable_unicode" = "yes"; then
   UNICODE=1
   AC_MSG_RESULT([Checking for the UNICODE option... yes])
else
   UNICODE=0
   AC_MSG_RESULT([Checking for the UNICODE option... no])
fi


# check for the libxml2 library
# AM_PATH_XML([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
# Test for XML, and define XML_CPPFLAGS and XML_LIBS
AM_PATH_XML2([2.6.1], [],
	[AC_MSG_ERROR([Cannot found the libxml2 library right version (> 2.6.1); download it from http://xmlsoft.org])])

AM_PATH_WXCONFIG(2.3.4, wxWin=1)
if test "$wxWin" != 1; then
    AC_MSG_ERROR([
       wxWindows must be installed on your system
       but wx-config script couldn't be found.

       Please check that wx-config is in path, the directory
       where wxWindows libraries are installed (returned by
       'wx-config --libs' command) is in LD_LIBRARY_PATH or
        equivalent variable and wxWindows version is 2.3.4 or above.
    ])
fi

# we won't use the WX_CXXFLAGS and WX_CFLAGS variables here because
# as written in the wxbase.bkl template, we are going to use wx-config
# to retrieve all flags
CXXFLAGS="$CXXFLAGS $XML_CPPFLAGS"
CFLAGS="$CFLAGS $XML_CPPFLAGS"
LDFLAGS="$LDFLAGS $WX_LIBS $XML_LIBS"


# The final lines of the configure script
AC_BAKEFILE
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
