/////////////////////////////////////////////////////////////////////////////
// Name:        tags.cpp
// Purpose:     ??
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: tags.cpp,v 1.1.1.1 2003-12-28 21:08:12 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "t2s.h"

void tsFreePhoneImplementation::tags(CONFIG *config,BUFFER *buffer, LING_LIST *ling_list)
{
  char *lbuf = buffer_text(buffer);
  int count = 0;
  int i;
  char * ref;

  if((ref=strtok(lbuf," ")) != NULL) {
    ling_list->text[count]->word = strdup(ref);
    count++;
    while((ref=strtok(NULL," "))!=NULL) {
      ling_list->text[count]->word = strdup(ref);
      count++;
      if(count==ling_list->max) {
	ling_list_realloc(2*(ling_list->max),ling_list);
      }
    }
  }
  ling_list->sz = count;
  for(i=0;i<count;i++) {
    if(in_fw_list(ling_list->text[i]->word,config)) {
      ling_list->text[i]->type = FW;
      /* now if at least one letter  */
    } else if (something_to_say(ling_list->text[i]->word)) {
      ling_list->text[i]->type = CW;
    } else /* assume punct  */ {
      ling_list->text[i]->type = PUNCT;
    }
  }
		
}

int tsFreePhoneImplementation::in_fw_list(char *ascii,CONFIG *config)
{
	if(binary(ascii,fw,config->fw_num))
		return(1);
	else 
		return(0);
}

int tsFreePhoneImplementation::something_to_say(char *ascii)
{
	int i;

	for(i=0;i<strlen(ascii);i++) {
		if(ascii[i]>='0' && ascii[i] <='9')
			return(1);
		if(ascii[i]>='a' && ascii[i] <='z')
			return(1);
		if(ascii[i]>='A' && ascii[i] <='Z')
			return(1);
	}
	return(0);
}

