/////////////////////////////////////////////////////////////////////////////
// Name:        transcribe.cpp
// Purpose:     ??
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: transcribe.cpp,v 1.1.1.1 2003-12-28 21:08:18 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <limits.h>

#include "MFDict.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "t2s.h"

/* Why is nrl_rules an unresolved symbol if in interface.cpp? */
extern char stringC[];
char *tsFreePhoneImplementation::nrl_rules(char* in)
{
	xlate_string(in);

	stringC[strlen(stringC)-2] = '\0';

	return(stringC);
}


void tsFreePhoneImplementation::transcribe(CONFIG *config, LING_LIST *ling_list)
{
	int i;
	int count = ling_list->sz;
	char *tr;
	char *word;

	for(i=0;i<count;i++) {
		if(ling_list->text[i]->type == PUNCT) {
			ling_list->text[i]->transcription = strdup("");
		} else if((tr = lookup_db(ling_list->text[i]->word,config))) {
			tr = strdup(tr);
			tr = syllabify(tr,config);
			ling_list->text[i]->transcription = tr;
		} else {
			word = strdup(ling_list->text[i]->word);
			ToLower(word);
			if((tr = lookup_db(word,config))) {
				tr = strdup(tr);
				tr = syllabify(tr,config);
				ling_list->text[i]->transcription = tr;
			} else {
				tr = nrl_rules(word);
				tr = strdup(tr);
				tr = syllabify(tr,config);
				tr = stress(tr);
				ling_list->text[i]->transcription = tr;
			}
			free(word);
		}
		/* modify transcriptions..  */
	}
}

/* JACS: new lookup_db using MFFindData instead of Unix-specific
 * database functions.
 */

char *tsFreePhoneImplementation::lookup_db(char *word, CONFIG *config)
{
	if (config->phoneme_db)
    {
		char* s = MFFindData(config->phoneme_db, word);
        return s;
    }
	else
		return NULL;
}

void tsFreePhoneImplementation::ToLower(char *word)
{
	while(*word) {
		*word = tolower(*word);
		word++;
	}
}
