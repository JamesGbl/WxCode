/////////////////////////////////////////////////////////////////////////////
// Name:        interface.cpp
// Purpose:     ??
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: interface.cpp,v 1.1.1.1 2003-12-28 21:08:15 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "t2s.h"

#define MAX_LENGTH 128

static int Char, Char1, Char2, Char3;

static int offset;
static char *inptr;

char stringC[1024];

void tsFreePhoneImplementation::outstring(char* string)
{
	char stringCX[1024];
	char *string2 = stringCX;

	nrl_edin_conv(string,string2);

	strcat(stringC,string2);
}

int tsFreePhoneImplementation::makeupper(int character)
	{
	if (islower(character))
		return toupper(character);
	else
		return character;
	}

int tsFreePhoneImplementation::Xgetc()
{
	return((int)inptr[offset++]);
}

char tsFreePhoneImplementation::new_char()
	{
	/*
	If the cache is full of newline, time to prime the look-ahead
	again.  If an '\0' is found, fill the remainder of the queue with
	'\0''s.
	*/
	if (Char == '\n'  && Char1 == '\n' && Char2 == '\n' && Char3 == '\n')
		{	/* prime the pump again */
		Char = Xgetc();
		if (Char == '\0')
			{
			Char1 = '\0';
			Char2 = '\0';
			Char3 = '\0';
			return Char;
			}
		if (Char == '\n')
			return Char;

		Char1 = Xgetc();
		if (Char1 == '\0')
			{
			Char2 = '\0';
			Char3 = '\0';
			return Char;
			}
		if (Char1 == '\n')
			return Char;

		Char2 = Xgetc();
		if (Char2 == '\0')
			{
			Char3 = '\0';
			return Char;
			}
		if (Char2 == '\n')
			return Char;

		Char3 = Xgetc();
		}
	else
		{
		/*
		Buffer not full of newline, shuffle the characters and
		either get a new one or propagate a newline or '\0'.
		*/
		Char = Char1;
		Char1 = Char2;
		Char2 = Char3;
		if (Char3 != '\n' && Char3 != '\0')
			Char3 = Xgetc();
		}
	return Char;
	}

void tsFreePhoneImplementation::xlate_string(char* is) /* short for input string  */
	{
	/* Prime the queue */
	offset = 0;
	inptr = is;
	stringC[0] = '\0';
	Char = '\n';
	Char1 = '\n';
	Char2 = '\n';
	Char3 = '\n';
	new_char();	/* Fill Char, Char1, Char2 and Char3 */

	while (Char != '\0')	/* All of the words in the file */
		{
		if (isdigit(Char))
			have_number();
		else
		if (isalpha(Char) || Char == '\'')
			have_letter();
		else
		if (Char == '$' && isdigit(Char1))
			have_dollars();
		else
			have_special();
		}
	}

void tsFreePhoneImplementation::have_dollars()
	{
	long int value;

	value = 0L;
	for (new_char() ; isdigit(Char) || Char == ',' ; new_char())
		{
		if (Char != ',')
			value = 10 * value + (Char-'0');
		}

	say_cardinal(value);	/* Say number of whole dollars */

	/* Found a character that is a non-digit and non-comma */

	/* Check for no decimal or no cents digits */
	if (Char != '.' || !isdigit(Char1))
		{
		if (value == 1L)
			outstring("dAAlER ");
		else
			outstring("dAAlAArz ");
		return;
		}

	/* We have '.' followed by a digit */

	new_char();	/* Skip the period */

	/* If it is ".dd " say as " DOLLARS AND n CENTS " */
	if (isdigit(Char1) && !isdigit(Char2))
		{
		if (value == 1L)
			outstring("dAAlER ");
		else
			outstring("dAAlAArz ");
		if (Char == '0' && Char1 == '0')
			{
			new_char();	/* Skip tens digit */
			new_char();	/* Skip units digit */
			return;
			}

		outstring("AAnd ");
		value = (Char-'0')*10 + Char1-'0';
		say_cardinal(value);

		if (value == 1L)
			outstring("sEHnt ");
		else
			outstring("sEHnts ");
		new_char();	/* Used Char (tens digit) */
		new_char();	/* Used Char1 (units digit) */
		return;
		}

	/* Otherwise say as "n POINT ddd DOLLARS " */

	outstring("pOYnt ");
	for ( ; isdigit(Char) ; new_char())
		{
		say_ascii(Char);
		}

	outstring("dAAlAArz ");

	return;
	}

void tsFreePhoneImplementation::have_special()
	{
	if (Char == '\n')
		(void)fprintf(stdout,"Newlines should be impossible (nrl)!!\n");
	else
	if (!isspace(Char))
		say_ascii(Char);

	new_char();
	return;
	}


void tsFreePhoneImplementation::have_number()
	{
	long int value;
	int lastdigit;

	value = Char - '0';
	lastdigit = Char;

	for (new_char() ; isdigit(Char) ; new_char())
		{
		value = 10 * value + (Char-'0');
		lastdigit = Char;
		}

	/* Recognize ordinals based on last digit of number */
	switch (lastdigit)
		{
	case '1':	/* ST */
		if (makeupper(Char) == 'S' && makeupper(Char1) == 'T' &&
		    !isalpha(Char2) && !isdigit(Char2))
			{
			say_ordinal(value);
			new_char();	/* Used Char */
			new_char();	/* Used Char1 */
			return;
			}
		break;

	case '2':	/* ND */
		if (makeupper(Char) == 'N' && makeupper(Char1) == 'D' &&
		    !isalpha(Char2) && !isdigit(Char2))
			{
			say_ordinal(value);
			new_char();	/* Used Char */
			new_char();	/* Used Char1 */
			return;
			}
		break;

	case '3':	/* RD */
		if (makeupper(Char) == 'R' && makeupper(Char1) == 'D' &&
		    !isalpha(Char2) && !isdigit(Char2))
			{
			say_ordinal(value);
			new_char();	/* Used Char */
			new_char();	/* Used Char1 */
			return;
			}
		break;

	case '0':	/* TH */
	case '4':	/* TH */
	case '5':	/* TH */
	case '6':	/* TH */
	case '7':	/* TH */
	case '8':	/* TH */
	case '9':	/* TH */
		if (makeupper(Char) == 'T' && makeupper(Char1) == 'H' &&
		    !isalpha(Char2) && !isdigit(Char2))
			{
			say_ordinal(value);
			new_char();	/* Used Char */
			new_char();	/* Used Char1 */
			return;
			}
		break;
		}

	say_cardinal(value);

	/* Recognize decimal points */
	if (Char == '.' && isdigit(Char1))
		{
		outstring("pOYnt ");
		for (new_char() ; isdigit(Char) ; new_char())
			{
			say_ascii(Char);
			}
		}

	/* Spell out trailing abbreviations */
	if (isalpha(Char))
		{
		while (isalpha(Char))
			{
			say_ascii(Char);
			new_char();
			}
		}

	return;
	}


void tsFreePhoneImplementation::have_letter()
	{
	char buff[MAX_LENGTH];
	int count;

	count = 0;
	buff[count++] = ' ';	/* Required initial blank */

	buff[count++] = makeupper(Char);

	for (new_char() ; isalpha(Char) || Char == '\'' ; new_char())
		{
		buff[count++] = makeupper(Char);
		if (count > MAX_LENGTH-2)
			{
			buff[count++] = ' ';
			buff[count++] = '\0';
			xlate_word(buff);
			count = 1;
			}
		}

	buff[count++] = ' ';	/* Required terminating blank */
	buff[count++] = '\0';

	/* Check for AAANNN type abbreviations */
	if (isdigit(Char))
		{
		spell_word(buff);
		return;
		}
	else
	if (strlen(buff) == 3)	 /* one character, two spaces */
		say_ascii(buff[1]);
	else
	if (Char == '.')		/* Possible abbreviation */
		abbrev(buff);
	else
		xlate_word(buff);

	if (Char == '-' && isalpha(Char1))
		new_char();	/* Skip hyphens */

	}

/* Handle abbreviations.  Text in buff was followed by '.' */
void tsFreePhoneImplementation::abbrev(char buff[])
	{
	if (strcmp(buff, " DR ") == 0)
		{
		xlate_word(" DOCTOR ");
		new_char();
		}
	else
	if (strcmp(buff, " MR ") == 0)
		{
		xlate_word(" MISTER ");
		new_char();
		}
	else
	if (strcmp(buff, " MRS ") == 0)
		{
		xlate_word(" MISSUS ");
		new_char();
		}
	else
	if (strcmp(buff, " PHD ") == 0)
		{
		spell_word(" PHD ");
		new_char();
		}
	else
		xlate_word(buff);
	}
