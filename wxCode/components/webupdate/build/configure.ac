# ==============================================================
# A basic "configure.ac" using a WX-based bakefile
#
# Author: Francesco Montorsi
# RCS-ID: $Id$
# ==============================================================

AC_PREREQ(2.59)
AC_INIT([webupdate], [1.0], [frm@sourceforge.net])

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
AC_LANG(C++)



# check for wxWidgets library 
# and initialization of WX_* variables
AM_WXPRESETS([2.6.1])



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
