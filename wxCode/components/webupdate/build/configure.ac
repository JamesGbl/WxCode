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
AC_INIT([WebUpdate], [1.2], [frm@sourceforge.net])
AC_CONFIG_AUX_DIR(build)

# you need the wxCode/build/bakefiles in your local repository to generate a configure
# script from this configure.ac file since wxcode.m4 is required....
m4_include(../../../build/autoconf/wxcode.m4)
WXCODE_INIT




## CONFIGURE OPTIONS
##
## Before starting checks, declare the options of this configure script
## Here you should use the AC_ARG_ENABLE and AC_ARG_WITH macros
## to add to this configure script the --enable-XXX and/or --with-XXX
## options required. If you did not use any <option> tag in your
## component's bakefile, then you can leave this section as is
#####################################################################

WXCODE_OPTIONS([debug,unicode,shared,toolkit,wxshared,wxversion])

WXCODE_ARG_ENABLE([httpengine],
                  [Enables the proxy and authentication support; requires the wxHttpEngine component], [yes])

AC_MSG_CHECKING([for the --with-httpengine option])
if test "$with_httpengine" = "yes"; then
    USE_HTTPENGINE=1
    AC_MSG_RESULT([yes])
else
    USE_HTTPENGINE=0
    AC_MSG_RESULT([no])
fi





## CONFIGURE CHECKS
##
## Here you should use the AC_CHECK_LIB, AC_COMPILE_IFELSE,
## AC_LINK_IFELSE, etc macros to check that the libraries required
## by your component exist on the host machine and match your
## required options (version, build settings, etc)
#####################################################################

AC_PROG_CXX

# unfortunately wx2.6 has a  bug with net support under unix and thus is unsupported
WXCODE_CHECKS([2.8.0], [xrc,xml,net,core,base])
if test "$USE_HTTPENGINE" = "1"; then

    # check for wxHTTPEngine presence, eventually disabling it if the
    # test program cannot be linked...
    WXCODE_CHECKFOR_COMPONENT([wxHTTPEngine], [wx/httpbuilder.h],
                               [httpengine], [wxHTTPBuilder test;],
                               [USE_HTTPENGINE=1],     # action if found
                               [USE_HTTPENGINE=0])     # action if not found
fi


# end in a nice way the configure script
WXCODE_END_PART1
if test "$USE_HTTPENGINE" = "1"; then
echo "  - wxHTTPEngine enabled"
else
echo "  - wxHTTPEngine disabled"
fi
WXCODE_END_PART2

