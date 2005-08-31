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

MY_ARG_WITH([httpengine], [Enables the proxy and authentication support], [yes])

if test "$with_httpengine" = "yes"; then
   USE_HTTPENGINE=1
   AC_MSG_RESULT([Checking for the --with-httpengine option... yes])
else
   USE_HTTPENGINE=0
   AC_MSG_RESULT([Checking for the --with-httpengine option... no])
fi



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
