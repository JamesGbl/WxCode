/////////////////////////////////////////////////////////////////////////////
// Name:        regerror.cpp
// Purpose:     Regexp error handler
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: regerror.cpp,v 1.1.1.1 2003-12-28 21:08:14 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include "./regexp.h"
#include <stdio.h>

void
regerror(const char *s)
{
#ifdef ERRAVAIL
	error("regexp: %s", s);
#else
/*
	fprintf(stderr, "regexp(3): %s\n", s);
	exit(1);
*/
	return;	  /* let std. egrep handle errors */
#endif
	/* NOTREACHED */
}
