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
AC_INIT([wxCurl], [0.3], [ckodonnell@users.sourceforge.net])
AC_CONFIG_AUX_DIR(build)

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





## CONFIGURE CHECKS
##
## Here you should use the AC_CHECK_LIB, AC_COMPILE_IFELSE, 
## AC_LINK_IFELSE, etc macros to check that the libraries required
## by your component exist on the host machine and match your
## required options (version, build settings, etc)
#####################################################################

WXCODE_CHECKS([2.6.0], [xrc,html,adv,xml,core,base])


# check for the libcurl library
LIBCURL_CHECK_CONFIG([yes], [7.15.0], [curlOk=1], [curlOk=0])
if [[ $curlOk = 0 ]]; then
    AC_MSG_ERROR([
    Could not find a valid libCURL installation on your system.

    If you do not have libCURL installed or you have an old version
    then please install the latest version from 
      http://curl.haxx.se/libcurl/

    If you have a valid libCURL installed, then please check the config.log
    file for more details about the failure of this check.
    ])
fi

# add all XML stuff that we've found...
CXXFLAGS="$CXXFLAGS $LIBCURL_CPPFLAGS"
CFLAGS="$CFLAGS $LIBCURL_CPPFLAGS"
LDFLAGS="$LDFLAGS $LIBCURL"


# end in a nice way the configure script
WXCODE_END
