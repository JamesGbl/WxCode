/////////////////////////////////////////////////////////////////////////////
// Name:        regexp.h
// Purpose:     Regular expression functionality
// Author:      See regexp.cpp
// Modified by:
// Created:
// RCS-ID:      $Id: regexp.h,v 1.1.1.1 2003-12-28 21:08:21 wyo Exp $
// Copyright:   See regexp.cpp
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#ifndef _FREEPHONE_REGEXP_H_
#define _FREEPHONE_REGEXP_H_

/*
 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.
 */
#define NSUBEXP  10
typedef struct regexp {
	char *startp[NSUBEXP];
	char *endp[NSUBEXP];
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	int regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} regexp;

extern regexp *regcomp(const char* exp);
extern int regexec(register const regexp *prog, register const char *string);
extern void regsub(const regexp *prog, const char *source, char *dest);
extern void regerror(const char *);

#endif
    /* _FREEPHONE_REGEXP_H_ */

