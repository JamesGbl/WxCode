# ======================================================================================
# Author: Francesco Montorsi
# RCS-ID: $Id$
#
# A basic "configure.ac" for a wxCode component.
# See 
#    http://www.gnu.org/software/autoconf/manual/autoconf-2.57/html_mono/autoconf.html
# for more info about autoconf and the autoconf macros.
# ======================================================================================


# ENABLES/DISABLES THE DEBUG MODE FOR THIS CONFIGURE SCRIPT
#DBG_CONFIGURE=1

# Init this configure script with basic info about the component
AC_INIT([wxXml2], [1.5], [frm@sourceforge.net])
m4_include(../../../build/bakefiles/wxcode.m4)
AM_WXCODE_INIT




## CONFIGURE OPTIONS
##
## Before starting checks, declare the options of this configure script
## Here you should use the AC_ARG_ENABLE and AC_ARG_WITH macros
## to add to this configure script the --enable-XXX and/or --with-XXX
## options required. If you did not use any <option> tag in your
## component's bakefile, then you can leave this section as is
#####################################################################

AM_OPTIONS_WXCONFIG
AM_OPTIONS_WXPRESETS

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

MY_ARG_WITH([python], [Builds with Python support], [yes])
MY_ARG_WITH([lua], [Builds with LUA support], [yes])
MY_ARG_WITH([cint], [Builds with CINT support], [no])
MY_ARG_WITH([underc], [Builds with UnderC support], [no])
MY_ARG_WITH([pythonver], [The Python version to use], [2_4])

if test "$with_python" = "yes"; then
   USE_PYTHON=1
   AC_MSG_RESULT([Checking for the PYTHON option... yes])
else
   USE_PYTHON=0
   AC_MSG_RESULT([Checking for the PYTHON option... no])
fi

if test "$with_lua" = "yes"; then
   USE_LUA=1
   AC_MSG_RESULT([Checking for the LUA option... yes])
else
   USE_LUA=0
   AC_MSG_RESULT([Checking for the LUA option... no])
fi

if test "$with_underc" = "yes"; then
   USE_UNDERC=1
   AC_MSG_RESULT([Checking for the UNDERC option... yes])
else
   USE_UNDERC=0
   AC_MSG_RESULT([Checking for the UNDERC option... no])
fi

if test "$with_cint" = "yes"; then
   USE_CINT=1
   AC_MSG_RESULT([Checking for the CINT option... yes])
else
   USE_CINT=0
   AC_MSG_RESULT([Checking for the CINT option... no])
fi

if test "$with_pythonver" = "2_2"; then
   PYTHON_VER=2_2
elif test "$with_pythonver" = "2_3"; then
   PYTHON_VER=2_3
elif test "$with_pythonver" = "2_4"; then
   PYTHON_VER=2_4
elif test "$with_pythonver" = "2_5"; then
   PYTHON_VER=2_3
elif test "$with_pythonver" = "2_6"; then
   PYTHON_VER=2_3
else
   PYTHON_VER=2_4
fi
AC_MSG_RESULT([Checking for the Python version to use... $PYTHON_VER])

#AM_PATH_PYTHON(2.2.2)







## CONFIGURE CHECKS
##
## Here you should use the AC_CHECK_LIB, AC_COMPILE_IFELSE, 
## AC_LINK_IFELSE, etc macros to check that the libraries required
## by your component exist on the host machine and match your
## required options (version, build settings, etc)
#####################################################################

AM_WXCODE_CHECKS([2.4.0])       # 2.4.0 is the minimum required version of wx

if test "$USE_LUA" = "1" ; then
   
   AC_CHECK_HEADER([lua.h], [hasLUA=1], [hasLUA=0])
   AC_CHECK_HEADER([tolua.h], [hasTOLUA=1], [hasTOLUA=0])

   AC_MSG_CHECKING([for the LUA & TOLUA presence... ])   
   if test "$hasLUA" != 1; then
       AC_MSG_ERROR([
         Could not found LUA headers in system path; please 
         install latest version from www.lua.org or specify
         the --without-lua option
                   ])
   elif test "$hasTOLUA" != 1; then
       AC_MSG_ERROR([
         Could not found TOLUA headers in system path; please 
         install latest version from www.lua.org or specify
         the --without-lua option
                   ])
   fi

   AC_MSG_RESULT([yes])
fi




# end in a nice way the configure script
AM_WXCODE_END
