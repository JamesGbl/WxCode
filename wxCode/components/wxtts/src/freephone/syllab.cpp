/////////////////////////////////////////////////////////////////////////////
// Name:        syllab.cpp
// Purpose:     Syllabification
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: syllab.cpp,v 1.1.1.1 2003-12-28 21:08:12 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "t2s.h"

char *tsFreePhoneImplementation::syllabify(char *string, CONFIG *config) 	/* replace string in situ  */
{
	int look = 0;
	int i = 0;
	int have_bd = 1; /* want nothing prior to first vowel  */
	char **carrier; 
	int marker[1024];
	int c_sz;
	char outlist[4096];


	carrier = split(string);
	free(string);

	while(carrier[i]!= NULL) {
		marker[i++] = 0;
		if(i == 1024) {
			(void)fprintf(stderr,"Truncating syllabification of %s\n",string);
			break;
		}
	}
	c_sz = i;

	/* to produce a list of candidate pts for breaks  */
	/* we start looking once we find a vowel  */
	/* we stop looking if we find an existing syllable boundary  */

	if(b_cat(carrier[c_sz-1],config) == VOWEL) {
		look = 1;
		/* don't need a marker  */
	}
	for(i=c_sz-2;i>=0;i--) {
		if(b_cat(carrier[i],config) == BOUND) {
			look = 0;
		} else if(b_cat(carrier[i],config) == VOWEL) { /* we're off  */
			if(look) {
				marker[i] = 1;
			}
			look = 1;
		} else if(look && (!seq(carrier[i],carrier[i+1]))) {
			marker[i] = 1;
			look = 0;
		} 	/* else do nothing  */
	}
	/* pass in the other direction for the provisionals  */
	/* this code is to eliminate ``multiple'' syllable boundaries  */
	/* and non valid initial sylls.  */

	for(i=0;i<c_sz;i++) {
		if(b_cat(carrier[i],config) == BOUND) {
			have_bd = 1;
		} else if(b_cat(carrier[i],config) == VOWEL) {
			have_bd = 0;
		} else if((marker[i] == 1) && (have_bd == 1)) {
			marker[i] = 0;
		}
	}

	/* and that's it, barring rewriting the string  */

	outlist[0] = '\0';

	for(i=0;i<c_sz;i++) {
		if(i)
			strcat(outlist," ");
		strcat(outlist,carrier[i]);
		if(marker[i]) {
			strcat(outlist," ");
			strcat(outlist,".");
		}
	}
	tidy_split(carrier);
	return(strdup(outlist));
}

char *tsFreePhoneImplementation::stress(char *param) /* no stress allowed initially  */
{
	/* plan -- allow 2 spaces more in result  */
	char **llist;
	int nll;
	int words;
	int look;
	int *marker;
	char *stringout;
	int i;
	int hs;
	int reserve;

	llist = split(param);
	free(param);
	
	nll=0;
	words=0;
	while(llist[nll] != NULL) {
		if(!strcmp(llist[nll],"|"))
			words++;
		nll++;
	}

	marker = (int *)malloc(sizeof(int)*nll);
	for(i=0;i<nll;i++) 
		marker[i] = 0;
	stringout = (char *)malloc(sizeof(char *)*(nll+words*2+2));
	stringout[0] = '\0';

	look=2;
	reserve = -1;
	hs = 0;
	for(i=nll-1;i>=0;i--) {
		if(!strcmp(llist[i],"|")) {
			reserve = -1;
			look = 2 ;  /* ie just started looking  */
		} else if(look==2) {
			hs = heavy(llist[i]);
			look = 1;
		} else if(look==0) {
			;
		}

		if(look==1 && hs) {
			if(vowel(llist[i])) {
				marker[i] = 1;
				look=0;
			}
		} else if(look==1) {
			if(vowel(llist[i]) && reserve == (-1)) {
				reserve = i;
			} else if(vowel(llist[i])) {
				marker[i] = 1;
				reserve = -1;
				look=0;
			}
		}
	}
	if(reserve!=(-1)) {
		marker[reserve] = 1;
	}

	for(i=0;i<nll;i++) {
		if(marker[i]) 
			strcat(stringout,"* ");
		strcat(stringout,llist[i]);
		strcat(stringout," ");
	}
	tidy_split(llist);
	free(marker);

	return(stringout);
}

int tsFreePhoneImplementation::b_cat(char *ascii, CONFIG *config)
{
	KEY *key0;

	if((key0 = binary(ascii,broad_cats,config->broad_cats_num)))
		return(key0->keycount);
	else {
		fprintf(stderr,"Something gone wrong with broad_cats\n");
		return(-10);
	}
}

int tsFreePhoneImplementation::heavy(char *phon)	/* according to what I understand by heavy  */
{
	if(!strcmp(phon,"ai")) {
		return(1);
	} else if(!strcmp(phon,"ai")) {
		return(1);
	} else if(!strcmp(phon,"ie")) {
		return(1);
	} else if(!strcmp(phon,"oi")) {
		return(1);
	} else if(!strcmp(phon,"oa")) {
		return(1);
	} else if(!strcmp(phon,"ou")) {
		return(1);
	} else if(!strcmp(phon,"eer")) {
		return(1);
	} else if(!strcmp(phon,"air")) {
		return(1);
	} else if(!strcmp(phon,"oor")) {
		return(1);
	} else if(!vowel(phon)) {
		return(1);
	} else 
		return(0);
}

int tsFreePhoneImplementation::seq(char * prev, char * curr)
{
	/* this is just a list of what can precede what  */
	/* and only for pairs of consonants  */

	if(vowel(prev) || vowel(curr)) {
		return(1);
	}

	if(!strcmp(curr,"r")) {
		if(!strcmp(prev,"p") ||
	 	!strcmp(prev,"t") ||
	 	!strcmp(prev,"k") ||
	 	!strcmp(prev,"b") ||
	 	!strcmp(prev,"d") ||
	 	!strcmp(prev,"g") ||
	 	!strcmp(prev,"th") ||
	 	!strcmp(prev,"sh") ||
	 	!strcmp(prev,"f")) {
			return(1);
		} else {
			return(0);
		}
	} else if(!strcmp(curr,"y")) {
		if(!strcmp(prev,"p") ||
	 	!strcmp(prev,"t") ||
	 	!strcmp(prev,"k") ||
	 	!strcmp(prev,"b") ||
	 	!strcmp(prev,"d") ||
	 	!strcmp(prev,"s") ||
	 	!strcmp(prev,"f")) {
			return(1);
		} else {
			return(0);
		}
	} else if(!strcmp(curr,"l")) {
		if(!strcmp(prev,"p") ||
	 	!strcmp(prev,"k") ||
	 	!strcmp(prev,"b") ||
	 	!strcmp(prev,"g") ||
	 	!strcmp(prev,"s") ||
	 	!strcmp(prev,"f")) {
			return(1);
		} else {
			return(0);
		}
	} else if(!strcmp(curr,"w")) {
		if(!strcmp(prev,"t") ||
	 	!strcmp(prev,"k") ||
	 	!strcmp(prev,"th")) {
			return(1);
		} else {
			return(0);
		}
	} else if(!strcmp(curr,"p")) {
		if(!strcmp(prev,"s")){
			return(1);
		} else {
			return(0);
		}
	} else if(!strcmp(curr,"t")) {
		if(!strcmp(prev,"s")){
			return(1);
		} else {
			return(0);
		}
	} else if(!strcmp(curr,"k")) {
		if(!strcmp(prev,"s")){
			return(1);
		} else {
			return(0);
		}
	} else if(!strcmp(curr,"m")) {
		if(!strcmp(prev,"s")){
			return(1);
		} else {
			return(0);
		}
	} else if(!strcmp(curr,"n")) {
		if(!strcmp(prev,"s")){
			return(1);
		} else {
			return(0);
		}
	}
	return(0);
}
