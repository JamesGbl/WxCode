# ==============================================================
# A basic "configure.ac" using a WX-based bakefile
#
# Author: Francesco Montorsi
# RCS-ID: $Id$
# ==============================================================


AC_PREREQ([2.57])
AC_INIT([wxxml2], [1.5], [frm@sourceforge.net])
AC_LANG(C++)


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



# check for the libxml2 library
# AM_PATH_XML([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
# Test for XML, and define XML_CPPFLAGS and XML_LIBS
AM_PATH_XML2([2.6.1], [],
	[AC_MSG_ERROR([Cannot found the libxml2 library or your libxml2 is too old (libxml2 > 2.6.1 is required); download it from http://xmlsoft.org])])

# add all XML stuff that we've found...
CXXFLAGS="$CXXFLAGS $XML_CPPFLAGS"
CFLAGS="$CFLAGS $XML_CPPFLAGS"
LDFLAGS="$LDFLAGS $WX_LIBS $XML_LIBS"



# check for wxWidgets library 
# and initialization of WX_* variables
AM_WXPRESETS([2.4.0])



# final macros
AC_BAKEFILE([m4_include(autoconf_inc.m4)])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT



# Just a little msg for GNU AUTOtools newbies (like me)
echo
echo " ------------------------------------------------------"
AM_WXPRESETS_MSG
echo
echo "  Now, just run make."
echo " ------------------------------------------------------"
echo
