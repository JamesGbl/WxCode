/////////////////////////////////////////////////////////////////////////////
// Name:        conv1.cpp
// Purpose:     ??
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: conv1.cpp,v 1.1.1.1 2003-12-28 21:08:15 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

/* working note...
   This will now work in 2 stages, with the first being writing the
   utterance in one long string (plus associated housekeeping)
*/

#include "t2s.h"

void tsFreePhoneImplementation::sil_fill(int j, char *shape, float factor, int durn, SENT *sent)
{
  sent_alloc_sil(sent, j);
  sent->sil[j].shape = shape;        
  sent->sil[j].factor = factor;        
  sent->sil[j].durn = durn;
}

void tsFreePhoneImplementation::conv1(CONFIG *config, LING_LIST *ling_list, SENT *sent)
{
  BUFFER *slist = &(sent->list);
  int count = ling_list->sz;
  int j = 0;  	/* phoneme counter  */
  int i,k;
  char **plist;

  sil_fill(j++,"H[",1.0,50,sent);
  buffer_clear(slist);
  buffer_add_str(slist, "##");
  for(i=0;i<count;i++) {
    if(ling_list->text[i]->type == PUNCT) {
      if(ling_list->text[i]->word[0] == '\0') {
	sil_fill(j++,"L-H]",0.2,0,sent);	
	buffer_add_str(slist," #");
      } else if(ling_list->text[i]->word[0] == '-') {
	sil_fill(j++,"L-H]",0.7,50,sent);
	buffer_add_str(slist," ##");
      } else if(ling_list->text[i]->word[0] == ':') {
	sil_fill(j++,"L-H]",0.7,750,sent);
	buffer_add_str(slist," ##");
      } else if(ling_list->text[i]->word[0] == ';') {
	sil_fill(j++,"L-L]",0.7,750,sent);
	buffer_add_str(slist," ##");
      } else if(ling_list->text[i]->word[0] == '.') {
	sil_fill(j++,"L-L]",1.0,1200,sent);
	buffer_add_str(slist," ##");
      } else if(ling_list->text[i]->word[0] == '!') {
	sil_fill(j++,"L-L]",1.0,1200,sent);
	buffer_add_str(slist," ##");
      } else if(ling_list->text[i]->word[0] == '?') {
	sil_fill(j++,"L-H]",1.0,1200,sent);
	buffer_add_str(slist," ##");
      } else {
	sil_fill(j++,"L-H]",0.7,200,sent);
	buffer_add_str(slist," ##");
      } /* more of same to follow  */
    } else {
      /* CW and FW  */
      plist = split(ling_list->text[i]->transcription);
      /* the glue ('|')  */
      if (buffer_last_char(slist) != '#') {
	buffer_add_str(slist," |");
      }
      k = 0;
      while(plist[k] != NULL) {
	if(islower(plist[k][0])) {  /* phoneme  */
	  buffer_add_str(slist," ");
	  buffer_add_str(slist,plist[k]);
	} else if(plist[k][0] == '.' || plist[k][0] == '-' || plist[k][0] == '+' || plist[k][0] == '|') {
	  buffer_add_str(slist," ");
	  buffer_add_str(slist,plist[k]);
	} else if(plist[k][0] == '*') {
	  if(ling_list->text[i]->type == CW) {
	    buffer_add_str(slist," *");
	  }
	} else if(plist[k][0] == '~') {
	  if(ling_list->text[i]->type == CW) {
	    buffer_add_str(slist," ~");
	  } 
	}
	k++;
      }
      tidy_split(plist);
    }
  }
  if(buffer_last_char(slist) != '#') {
    sil_fill(j++,"",0.0,1200,sent);
    buffer_add_str(slist," ##");
  }
  sent->sil_sz = j;
}

