/////////////////////////////////////////////////////////////////////////////
// Name:        tsdefs.h
// Purpose:     Various type definitions for FreePhone
// Author:      Julian Smart
// Modified by:
// Created:     29/01/98
// RCS-ID:      $Id: tsdefs.h,v 1.1.1.1 2003-12-28 21:08:24 wyo Exp $
// Copyright:
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef _TSDEFS_H_
#define _TSDEFS_H_

#define TSDLLEXPORT
#define TSNOT_FOUND (-1)

/// the type for various indexes (string, arrays, ...)
typedef unsigned int    uint;

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

// VC++ 4.0 is 1000.

// Add more tests here for compilers that don't already define bool.
#if ( defined(_MSC_VER) && (_MSC_VER <= 1000) ) || (defined(__BORLANDC__) && (__BORLANDC__ < 0x500))
typedef unsigned int bool;
#endif

#if ( defined(_MSC_VER) && (_MSC_VER <= 800) ) || defined(__GNUWIN32__)
#define byte unsigned char
#endif

#define TSUNUSED(identifier) /* identifier */

#endif
  // _TSDEFS_H_
