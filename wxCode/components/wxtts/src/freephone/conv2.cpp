/////////////////////////////////////////////////////////////////////////////
// Name:        conv2.cpp
// Purpose:     ??
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: conv2.cpp,v 1.1.1.1 2003-12-28 21:08:12 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

/* working note...
   This will now work in 2 stages, with the second being writing the
   utterance in one long string plus annotations and converting it
   into spl format.
*/

#ifdef _WINDOWS
#include <windows.h>
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "t2s.h"


void tsFreePhoneImplementation::conv2(CONFIG *config, SENT *sent, SPROSOD_LIST *spl)
{
  int syll = 0;
  int vref = -1;	/* will be a reference for the last vowel found  */
  int j = 0;  	/* phoneme counter  */
  int i,k;
  char **elem;
  char *shape = "";
  float factor = 0.0;
  int durn;

  i = -1;
  j = 0;
  k = 0;
  elem = split(buffer_text(&(sent->list)));
  while(elem[++i]!=NULL) {
    if(!strcmp(elem[i],"##")) {
      syll++;
      shape = sent->sil[k].shape;
      factor = sent->sil[k].factor;
      durn = sent->sil[k].durn;
      k++;
      spl_cpy(j++,syll++,"#",durn,"",factor,spl);
      spl_cat(vref,shape,factor,spl);
      shape = "";
      factor = 0.0;
    } else if(!strcmp(elem[i],"*") || !strcmp(elem[i],"~")) {
      shape = "H*";
      factor = 0.5;
    } else if(!strcmp(elem[i],"#")) {
      shape = sent->sil[k].shape;
      factor = sent->sil[k].factor;
      durn = sent->sil[k].durn;
      syll++;
      k++;
      spl_cat(vref,shape,factor,spl);
      shape = "";
      factor = 0.0;
    } else if(!strcmp(elem[i],".") || !strcmp(elem[i],"-") || !strcmp(elem[i],"|") || !strcmp(elem[i],"+")) {
      syll++;
    } else if(islower(elem[i][0]) || (elem[i][0]=='_')) {
      /* */
      if(vowel(elem[i])) {
	vref = j;
      }
      spl_cpy(j++,syll,elem[i],0,shape,factor,spl);
      shape = "";
      factor = 0.0;
    } else if(elem[i][0]=='@') {
      /* schwa was disguised in the rules so as not to
	 be treated like a vowel  */
      elem[i][0] = 'a';
      spl_cpy(j++,syll,elem[i],0,shape,factor,spl);
      shape = "";
      factor = 0.0;
    } else {
      fprintf(stderr,"Conv2: %s???\n",elem[i]);
      exit(1);
    }

  }
  tidy_split(elem);

  spl->sz = j;
}

/* starting  */
void tsFreePhoneImplementation::spl_cpy(int index,int syll, char *phon, int dur, char *type, float strength, SPROSOD_LIST *spl)
{
	if(index == spl->max) 
		spl_realloc(2*(spl->max),spl);
	strcpy(spl->phoneme[index]->phoneme,phon);
	spl->phoneme[index]->syll = syll;
	spl->phoneme[index]->dur = dur;
	spl->phoneme[index]->sprosod1 = type;
	spl->phoneme[index]->strength1 = strength;
	spl->phoneme[index]->sprosod2 = "";
	spl->phoneme[index]->strength2 = 0.0;
}

/* adding to  */
void tsFreePhoneImplementation::spl_cat(int index,char *type, float strength, SPROSOD_LIST *spl)
{
	if(index>=spl->max) {
		(void)fprintf(stderr,"Spl problem in spl_cat\n");
	}
	if(index < 0 ) {
		return;	/* forget it  */
	}
	spl->phoneme[index]->sprosod2 = type;
	spl->phoneme[index]->strength2 = strength;
}

int tsFreePhoneImplementation::vowel(char *ph)
{
	switch(ph[0]) {
		case 'a' : case 'e' : case 'i' : case 'o' : case 'u' :
			return(1);
		default : 
			return(0);
	}
}
