/////////////////////////////////////////////////////////////////////////////
// Name:        saynum.cpp
// Purpose:     Integer to readable ASCII conversion routine.
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: saynum.cpp,v 1.1.1.1 2003-12-28 21:08:17 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "t2s.h"

/*
**              Integer to Readable ASCII Conversion Routine.
**
** Synopsis:
**
**      say_cardinal(value)
**      	long int     value;          -- The number to output
**
**	The number is translated into a string of phonemes
**
*/

static char *Cardinals[] = 
	{
	"zIHrOW ",	"wAHn ",	"tUW ",		"THrIY ",
	"fAOr ",	"fAYv ",	"sIHks ",	"sEHvAXn ",
	"EYt ",		"nAYn ",		
	"tEHn ",	"IYlEHvAXn ",	"twEHlv ",	"THERtIYn ",
	"fAOtIYn ",	"fIHftIYn ", 	"sIHkstIYn ",	"sEHvEHntIYn ",
	"EYtIYn ",	"nAYntIYn "
	} ;

static char *Twenties[] = 
	{
	"twEHntIY ",	"THERtIY ",	"fAOtIY ",	"fIHftIY ",
	"sIHkstIY ",	"sEHvEHntIY ",	"EYtIY ",	"nAYntIY "
	} ;

static char *Ordinals[] = 
	{
	"zIHrOWEHTH ",	"fERst ",	"sEHkAHnd ",	"THERd ",
	"fAOTH ",	"fIHfTH ",	"sIHksTH ",	"sEHvEHnTH ",
	"EYtTH ",	"nAYnTH ",		
	"tEHnTH ",	"IYlEHvEHnTH ",	"twEHlvTH ",	"THERtIYnTH ",
	"fAOtIYnTH ",	"fIHftIYnTH ", 	"sIHkstIYnTH ",	"sEHvEHntIYnTH ",
	"EYtIYnTH ",	"nAYntIYnTH "
	} ;

static char *Ord_twenties[] = 
	{
	"twEHntIYEHTH ","THERtIYEHTH ",	"fAOtIYEHTH ",	"fIHftIYEHTH ",
	"sIHkstIYEHTH ","sEHvEHntIYEHTH ","EYtIYEHTH ",	"nAYntIYEHTH "
	} ;


/*
** Translate a number to phonemes.  This version is for CARDINAL numbers.
**	 Note: this is recursive.
*/
void tsFreePhoneImplementation::say_cardinal(long int value)
	{
	if (value < 0)
		{
		outstring("mAYnAHs ");
		value = (-value);
		if (value < 0)	/* Overflow!  -32768 */
			{
			outstring("IHnfIHnIHtIY ");
			return;
			}
		}

	if (value >= 1000000000L)	/* Billions */
		{
		say_cardinal(value/1000000000L);
		outstring("bIHlIYAXn ");
		value = value % 1000000000;
		if (value == 0)
			return;		/* Even billion */
		if (value < 100)	/* as in THREE BILLION AND FIVE */
			outstring("AEnd ");
		}

	if (value >= 1000000L)	/* Millions */
		{
		say_cardinal(value/1000000L);
		outstring("mIHlIYAXn ");
		value = value % 1000000L;
		if (value == 0)
			return;		/* Even million */
		if (value < 100)	/* as in THREE MILLION AND FIVE */
			outstring("AEnd ");
		}

	/* Thousands 1000..1099 2000..99999 */
	/* 1100 to 1999 is eleven-hunderd to ninteen-hunderd */
	if ((value >= 1000L && value <= 1099L) || value >= 2000L)
		{
		say_cardinal(value/1000L);
		outstring("THAWzAEnd ");
		value = value % 1000L;
		if (value == 0)
			return;		/* Even thousand */
		if (value < 100)	/* as in THREE THOUSAND AND FIVE */
			outstring("AEnd ");
		}

	if (value >= 100L)
		{
		outstring(Cardinals[value/100]);
		outstring("HHAHndrEHd ");
		value = value % 100;
		if (value == 0)
			return;		/* Even hundred */
		}

	if (value >= 20)
		{
		outstring(Twenties[(value-20)/ 10]);
		value = value % 10;
		if (value == 0)
			return;		/* Even ten */
		}

	outstring(Cardinals[value]);
	return;
	} 


/*
** Translate a number to phonemes.  This version is for ORDINAL numbers.
**	 Note: this is recursive.
*/
void tsFreePhoneImplementation::say_ordinal(long int value)
	{

	if (value < 0)
		{
		outstring("mAHnAXs ");
		value = (-value);
		if (value < 0)	/* Overflow!  -32768 */
			{
			outstring("IHnfIHnIHtIY ");
			return;
			}
		}

	if (value >= 1000000000L)	/* Billions */
		{
		say_cardinal(value/1000000000L);
		value = value % 1000000000;
		if (value == 0)
			{
			outstring("bIHlIYAXnTH ");
			return;		/* Even billion */
			}
		outstring("bIHlIYAXn ");
		if (value < 100)	/* as in THREE BILLION AND FIVE */
			outstring("AEnd ");
		}

	if (value >= 1000000L)	/* Millions */
		{
		say_cardinal(value/1000000L);
		value = value % 1000000L;
		if (value == 0)
			{
			outstring("mIHlIYAXnTH ");
			return;		/* Even million */
			}
		outstring("mIHlIYAXn ");
		if (value < 100)	/* as in THREE MILLION AND FIVE */
			outstring("AEnd ");
		}

	/* Thousands 1000..1099 2000..99999 */
	/* 1100 to 1999 is eleven-hunderd to ninteen-hunderd */
	if ((value >= 1000L && value <= 1099L) || value >= 2000L)
		{
		say_cardinal(value/1000L);
		value = value % 1000L;
		if (value == 0)
			{
			outstring("THAWzAEndTH ");
			return;		/* Even thousand */
			}
		outstring("THAWzAEnd ");
		if (value < 100)	/* as in THREE THOUSAND AND FIVE */
			outstring("AEnd ");
		}

	if (value >= 100L)
		{
		outstring(Cardinals[value/100]);
		value = value % 100;
		if (value == 0)
			{
			outstring("HHAHndrEHdTH ");
			return;		/* Even hundred */
			}
		outstring("HHAHndrEHd ");
		}

	if (value >= 20)
		{
		if ((value%10) == 0)
			{
			outstring(Ord_twenties[(value-20)/ 10]);
			return;		/* Even ten */
			}
		outstring(Twenties[(value-20)/ 10]);
		value = value % 10;
		}

	outstring(Ordinals[value]);
	return;
	} 
