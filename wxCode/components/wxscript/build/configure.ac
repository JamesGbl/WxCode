# ======================================================================================
# Author: Francesco Montorsi
# RCS-ID: $Id$
#
# A basic "configure.ac" for a wxCode component.
# See
#    http://www.gnu.org/software/autoconf/manual/autoconf-2.57/html_mono/autoconf.html
# for more info about autoconf and the autoconf macros.
# ======================================================================================


# Init this configure script with basic info about the component
# (DON'T PUT ANYTHING BEFORE AC_INIT, JUST COMMENTS)
AC_INIT([wxScript], [1.5], [frm@sourceforge.net])

# ENABLES/DISABLES THE DEBUG MODE FOR THIS CONFIGURE SCRIPT
#DBG_CONFIGURE=1

m4_include(../../../build/autoconf/wxcode.m4)
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

AM_WXCODE_ARG_WITH([perl], [Builds with Perl support], [yes])
AM_WXCODE_ARG_WITH([python], [Builds with Python support], [yes])
AM_WXCODE_ARG_WITH([pythonver], [The Python version to use], [2_4])
AM_WXCODE_ARG_WITH([lua], [Builds with LUA support], [yes])
AM_WXCODE_ARG_WITH([cint], [Builds with CINT support], [no])
AM_WXCODE_ARG_WITH([underc], [Builds with UnderC support], [no])

if test "$with_perl" = "yes"; then
   USE_PERL=1
   AC_MSG_RESULT([Checking for the --with-perl option... yes])
else
   USE_PERL=0
   AC_MSG_RESULT([Checking for the --with-perl option... no])
fi
if test "$with_python" = "yes"; then
   USE_PYTHON=1
   AC_MSG_RESULT([Checking for the --with-python option... yes])
else
   USE_PYTHON=0
   AC_MSG_RESULT([Checking for the --with-python option... no])
fi

if test "$with_lua" = "yes"; then
   USE_LUA=1
   AC_MSG_RESULT([Checking for the --with-lua option... yes])
else
   USE_LUA=0
   AC_MSG_RESULT([Checking for the --with-lua option... no])
fi

if test "$with_underc" = "yes"; then
   USE_UNDERC=1
   AC_MSG_RESULT([Checking for the --with-underc option... yes])
else
   USE_UNDERC=0
   AC_MSG_RESULT([Checking for the --with-underc option... no])
fi

if test "$with_cint" = "yes"; then
   USE_CINT=1
   AC_MSG_RESULT([Checking for the --with-cint option... yes])
else
   USE_CINT=0
   AC_MSG_RESULT([Checking for the --with-cint option... no])
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

   # we put this message *after* calling AC_CHECK_HEADER
   # since they print a lot of lines and we want to be able to print a "yes"
   # immediately after this message
   AC_MSG_CHECKING([for the LUA & TOLUA presence... ])

   if test "$hasLUA" != 1; then
       AC_MSG_RESULT([no])
       AC_MSG_ERROR([
         Could not found LUA headers in system path; please
         install latest version from www.lua.org...
         disabling LUA support.
                   ])
        USE_LUA=0
   elif test "$hasTOLUA" != 1; then
      AC_MSG_RESULT([no])
       AC_MSG_WARN([
         Could not found TOLUA headers in system path; please
         install latest version from www.lua.org...
         disabling LUA support.
                   ])
       USE_LUA=0
   else
       AC_MSG_RESULT([yes])
   fi
fi

if test "$USE_PYTHON" = "1" ; then

   maj=${PYTHON_VER%%_*}
   min=${PYTHON_VER%%*_}
   AM_PATH_PYTHON([$maj.$min], [hasPython=1], [hasPython=0])
   if test "$hasPython" != 1; then
       AC_MSG_WARN([
         Could not found Python in system paths; please
         install latest version from www.python.org...
         disabling Python support.
                   ])
       USE_PYTHON=0
   else
       AC_MSG_RESULT([yes])
   fi
fi

if test "$USE_PERL" = "1" ; then

   AC_MSG_CHECKING([for the Perl presence... ])

   AC_MSG_RESULT([yes])
fi



# end in a nice way the configure script
AM_WXCODE_END_PART1
if test "$USE_PYTHON" = "1"; then
    echo "  - Python enabled"
else
    echo "  - Python disabled"
fi
if test "$USE_LUA" = "1"; then
    echo "  - Lua enabled"
else
    echo "  - Lua disabled"
fi
if test "$USE_PERL" = "1"; then
    echo "  - Perl enabled"
else
    echo "  - Perl disabled"
fi
AM_WXCODE_END_PART2
