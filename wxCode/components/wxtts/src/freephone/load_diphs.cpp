/////////////////////////////////////////////////////////////////////////////
// Name:        load_diphs.cpp
// Purpose:     ??
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: load_diphs.cpp,v 1.1.1.1 2003-12-28 21:08:19 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "t2s.h"

void tsFreePhoneImplementation::load_speech(CONFIG *config)
{
	if(strcmp(config->diphone_file,"-")) {
		load_index(config);  /* in alphabetical order  */
		load_diphs(config);
	}
}

void tsFreePhoneImplementation::load_index(CONFIG *config)
{
	char s[100];
	char *index_file;
	int i;

	index_file = (char*) malloc(strlen(config->diphone_file)+10);
	strcpy(index_file,config->diphone_file);
	strcat(index_file,".idx");

	if((config->xfd=fopen(index_file,"r")) == NULL) {
		(void)fprintf(stderr,"Can't open file %s\n",index_file);
		exit(1);
	}

	for(i=0;(fgets(s,100,config->xfd) != NULL) && (i < NDIPHS);i++) {
		sscanf(s,"%s %d %d %d",indx[i].diph,&indx[i].beg,&indx[i].mid,&indx[i].end);
	}
	nindex = i;

	fclose(config->xfd);

	free(index_file);
}

void tsFreePhoneImplementation::load_diphs(CONFIG *config)
{
	int i;
	int j;
	char *diphone_file;
	int type;
	int calc_mlp = 1;
	int tot_fr = 0;
	int tot_short = 0;
	short *malloc_ptr;

	diphone_file = (char*) malloc(strlen(config->diphone_file)+10);
	strcpy(diphone_file,config->diphone_file);
	strcat(diphone_file,".dat");

	if((config->dfd=fopen(diphone_file,"r")) == NULL) {
		(void)fprintf(stderr,"Can't open file %s\n",diphone_file);
		exit(1);
	}

	type = read_header(config);
	if(config->norm) {
		calc_mlp = 0;
	}

	malloc_ptr = (short *) malloc((sizeof(short )*config->fr_data)*NFRAMES);
  	for(j=0;j<NFRAMES;j++) {
		dico[j].frame = (short *)(malloc_ptr + config->fr_data*j);
	}

	/* zero the first one... */
	for(i=0;i<config->fr_data;i++) {
		dico[0].frame[i] = 0;
	}

	/* note the use of 1 to tie in with indexing  */
	tot_fr = 0;
	tot_short = 0;
	if((tot_short=fread((short *)(dico[1].frame),sizeof(short),config->fr_data*(NFRAMES-1),config->dfd))!=0) {
		if(FT_LITTLE_ENDIAN) {
			for(i=1;i<=tot_short;i++) {
				*(malloc_ptr+i) = SWAPSHORT(*(malloc_ptr+i));
			}
		}
	}
	tot_fr = tot_short/config->fr_data;
	

	for(i=1;i<=tot_fr;i++) {
		if(calc_mlp) {
			if(dico[i].frame[0] > config->norm) {
				config->norm = dico[i].frame[0];
			}
		}
	}
	config->fr_sz = (int)dico[1].frame[2];
	dico[0].frame[2] = config->fr_sz;
#ifdef DEBUG
	for(i=1;i<=tot_fr;i++) {
		if(config->fr_sz != (int)dico[i].frame[2]) {
			(void)fprintf(stderr,"New frame size (frame %d): %d\n",i,(int)dico[i].frame[2]);
		}
	}
#endif

	fclose(config->dfd);

	free(diphone_file);
}

int tsFreePhoneImplementation::lookup(char *diph)
{
	int low, high, mid;


	low = 0; 
	high = nindex-1;
	while(low <= high) {
		mid = (low+high) / 2;
		if(strcmp(diph,indx[mid].diph)<0) 
			high = mid-1;
		else if(strcmp(diph,indx[mid].diph)>0) 
			low = mid+1;
		else
			return(mid);
	}
	return(-1);
}

void tsFreePhoneImplementation::phonstoframes(SPN *ps, ACOUSTIC *as)
{
	int i,j;
	int ref;

	as->f_sz = 0;
	
	for(i=0;i<ps->p_sz-1;i++) 
		sprintf(ps->diphs[i],"%s-%s",ps->phons[i],ps->phons[i+1]);

	ps->pb[0] = 0;	/* Gets treated a little bit specially  */

	/* insert zero frame  */
	as->mcebuf[as->f_sz++] = &dico[0];

	for(i=0;i<ps->p_sz-1;i++) {
		ref = lookup(ps->diphs[i]);	/* gives back the reference no.  */
		if(ref == -1) {
			(void)fprintf(stderr,"Diphone not found -  %s\n",ps->diphs[i]);
			ref = 0;
		}
		if(as->f_sz+50 > as->f_max) {
			as_realloc(as->f_max*2,as->p_max,as);
		}
		for(j=indx[ref].beg;j<=indx[ref].end;j++) {
			if(j==indx[ref].mid)
				ps->pb[i+1] = as->f_sz;
			as->mcebuf[as->f_sz++] = &dico[j];
		}
	}
	as->mcebuf[as->f_sz++] = &dico[0];
	as->mcebuf[as->f_sz++] = &dico[0];
	as->mcebuf[as->f_sz++] = &dico[0];

	ps->pb[ps->p_sz] = as->f_sz-1;

}

int tsFreePhoneImplementation::read_header(CONFIG *config)
{
	char thdr[256]; /* under certain circumstances may not be enuf  */
	short thdr_s[100];
	int type = 0;
	int i = 0;

	/* read header and go back (and point at the beginning of the DATA if necessary)  */

	if(fread(thdr,1,8,config->dfd) == 0) {
		(void)fprintf(stderr,"Problem with file %s\n",config->diphone_file);
	}
	while(thdr[i++] == 'G') {
		type += 1;
	}

	rewind(config->dfd);
	
	/* interesting info except for frame size  */
	/* and number of frames total  */
	if (type == 2) {
		fread((short *)thdr_s,2,8,config->dfd);
		if(FT_LITTLE_ENDIAN) {
			for(i=0;i<8;i++) {
				*(thdr+i) = SWAPSHORT(*(thdr+i));
			}
		}
		config->fr_data = thdr_s[4];
		config->ncoeffs = thdr_s[5];
		config->sr = thdr_s[6];
		config->norm = thdr_s[7];
		fread((short *)thdr_s,2,config->fr_data-8,config->dfd);
	} else {
		(void)fprintf(stderr,"Problem with file %s\n",config->diphone_file);
		exit(2);
	}
	return(type);
}


void tsFreePhoneImplementation::unload_diphs(CONFIG *config) /* for easier changes later  */
{
        int j;

  	for(j=0;j<NFRAMES;j++) {
		free(dico[0].frame);
	}
}
