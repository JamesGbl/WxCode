# ======================================================================================
# Author: Francesco Montorsi
# RCS-ID: $Id: configure.ac,v 1.8 2007/03/04 08:57:20 frm Exp $
#
# A basic "configure.ac" for a wxCode component.
# See 
#    http://www.gnu.org/software/autoconf/manual/autoconf-2.57/html_mono/autoconf.html
# for more info about autoconf and the autoconf macros.
# ======================================================================================


# Init this configure script with basic info about the component
AC_INIT([wxAvahi], [0.2.0], [emilien.kia@free.fr])
AC_CONFIG_AUX_DIR(build)

# ENABLES/DISABLES THE DEBUG MODE FOR THIS CONFIGURE SCRIPT
WX_DEBUG_CONFIGURE=1

m4_include(../../../build/autoconf/wxcode.m4)
WXCODE_INIT


PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([AVAHICLIENT], [avahi-client])
LIBS="$AVAHICLIENT_LIBS $LIBS"
CFLAGS="$AVAHICLIENT_CFLAGS $CFLAGS"


## CONFIGURE OPTIONS
##
## Before starting checks, declare the options of this configure script
## Here you should use the AC_ARG_ENABLE and AC_ARG_WITH macros,
## or the wxCode-specific AM_WXCODE_ARG_ENABLE & AM_WXCODE_ARG_WITH macros,
## to add to this configure script the --enable-XXX and/or --with-XXX
## options required. If you did not use any <option> tag in your
## component's bakefile, then you can leave this section as is
#####################################################################

WXCODE_OPTIONS([debug,unicode,shared,toolkit,wxshared,wxversion])


# Check for Plugins
AC_ARG_ENABLE([wxavahigui],
	AC_HELP_STRING([--disable-wxavahigui],[disable wxAvahi GUI classes (like wxAvahiServiceTypeListCtrl)]),
	[case "${enableval}" in
	  yes) with_wxavahigui="yes" ;;
	  no)  with_wxavahigui="no" ;;
	  *) AC_MSG_ERROR(bad value ${enableval} for --disable-wxavahigui) ;;
	esac],
	[with_wxavahigui="yes"])
if test "$with_wxavahigui" = "yes" ; then
  USE_GUI="1"
else
  USE_GUI="0"
fi
AC_SUBST(USE_GUI)


## CONFIGURE CHECKS
##
## Here you should use the AC_CHECK_LIB, AC_COMPILE_IFELSE, 
## AC_LINK_IFELSE, etc macros to check that the libraries required
## by your component exist on the host machine and match your
## required options (version, build settings, etc)
#####################################################################

WXCODE_CHECKS([2.8.0], [core,base,net])



# end in a nice way the configure script
WXCODE_END
