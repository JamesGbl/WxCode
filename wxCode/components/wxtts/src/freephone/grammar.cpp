/////////////////////////////////////////////////////////////////////////////
// Name:        grammar.cpp
// Purpose:     ??
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: grammar.cpp,v 1.1.1.1 2003-12-28 21:08:14 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include "t2s.h"

void tsFreePhoneImplementation::grammar(LING_LIST *ling_list)
{
	int i;
	int sum = 0;
	int count = ling_list->sz;

	for(i=0;i<count-1;i++) {
		/* count number and location of boundaries  */
		if(ling_list->text[i]->type==CW && ling_list->text[i+1]->type==FW) {
			sum++;
		}
	}
	if((sum+count) >= ling_list->max ) {
		ling_list_realloc(2*(ling_list->max),ling_list);
	} 

	ling_list->sz = sum+count;
	
	for(i=count-1;i>0;i--) {
		if(!sum)
			break;
		ling_list->text[i+sum]->word = ling_list->text[i]->word;
		ling_list->text[i+sum]->type = ling_list->text[i]->type;
		ling_list->text[i+sum]->transcription = NULL;
		if(ling_list->text[i]->type==FW && ling_list->text[i-1]->type==CW) {
			sum--;
			ling_list->text[i+sum]->word = strdup(""); 
			ling_list->text[i+sum]->type = PUNCT; 
			ling_list->text[i+sum]->transcription = NULL; 
		}
	}
}

