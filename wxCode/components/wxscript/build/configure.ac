# ======================================================
# A basic "configure.ac" using a WXBASE-based bakefile
# ======================================================

AC_PREREQ(2.57)
AC_INIT(wxscript, 1.0, [frm@sourceforge.net])


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
	
# This adds some standard wxWidgets options to the configure script
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
	
# check for wxWin version & presence
AM_PATH_WXCONFIG([2.4.2], [wxWin=1], [wxWin=0])
if test "$wxWin" != 1; then
    AC_MSG_ERROR([
               wxWindows must be installed on your system
               but wx-config script couldn't be found.
               Please check that wx-config is in path, the directory
               where wxWindows libraries are installed (returned by
               'wx-config - -libs' command) is in LD_LIBRARY_PATH or
               equivalent variable and wxWindows version is 2.3.4 or above.
       ])
fi



# wxScript-specific tests & options
###################################

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

   AC_MSG_RESULT([Checking for the LUA & TOLUA presence... ])   
   AC_CHECK_HEADER([lua.h], [hasLUA=1], [hasLUA=0])
   AC_CHECK_HEADER([tolua.h], [hasTOLUA=1], [hasTOLUA=0])
   if test "$hasLUA" != 1; then
       AC_MSG_ERROR([Could not found LUA headers in system path; please 
                     install latest version from www.lua.org or specify
                     the --without-lua option])
   elif test "$hasTOLUA" != 1; then
       AC_MSG_ERROR([Could not found TOLUA headers in system path; please 
                     install latest version from www.lua.org or specify
                     the --without-lua option])
   fi
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


# The final lines of the configure script
AC_BAKEFILE
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
