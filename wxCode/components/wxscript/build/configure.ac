# ======================================================
# A basic "configure.ac" using a WXBASE-based bakefile
# ======================================================

AC_PREREQ(2.57)
AC_INIT(wxscript, 1.0, [frm@sourceforge.net])

# This allows us to use Bakefile, recognizing the system type
# (and sets the AC_CANONICAL_BUILD, AC_CANONICAL_HOST and
# AC_CANONICAL_TARGET variables)
AC_CANONICAL_SYSTEM


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
			AC_HELP_STRING([--enable-$1], 				# Remove the space between the two hyphens
			[$2 (default is $3)]),, [enable_$1=$3])])
AC_DEFUN([MY_ARG_WITH], [AC_ARG_WITH($1, 
			AC_HELP_STRING([--with-$1],  				# Remove the space between the two hyphens
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

# The final lines of the configure script
AC_BAKEFILE
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
