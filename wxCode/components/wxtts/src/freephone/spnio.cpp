/////////////////////////////////////////////////////////////////////////////
// Name:        spnio.cpp
// Purpose:     ??
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: spnio.cpp,v 1.1.1.1 2003-12-28 21:08:13 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "t2s.h"

void tsFreePhoneImplementation::put_mbrola_data(CONFIG *config, SPN *ps)
{
	int i;
	int dur;
	int j = 0;	/* pointer to where in targets  */
	char *converted_phon;
	PKEY *ptmp;

	for(i=0;i<ps->p_sz;i++) {
		dur = ps->duration[i]/10.0;
                if((ptmp=pbinary(ps->phons[i],edin2sampa0,config->edin2sampa0_num))!=NULL) {
                        converted_phon = ptmp->keyvalue; 
                } else {
                	converted_phon = "_";
                	fprintf(stderr,"Unrecognized phoneme: %s\n",ps->phons[i]);
                }
		fprintf(config->ofd,"%s\t%d",converted_phon,dur);
		while(i == ps->targ_phon[j]) {
			fprintf(config->ofd,"\t%d %d",ps->pc_targs[j],ps->targ_freq[j]);
			j++;
		}
		fprintf(config->ofd,"\n");
	}
	fprintf(config->ofd,"#\n");
	fflush(config->ofd);
}

void tsFreePhoneImplementation::put_spn_data(CONFIG *config, SPN *ps)
{
	int i;
	int dur;
	int j = 0;	/* pointer to where in targets  */

	for(i=0;i<ps->p_sz;i++) {
		dur = ps->duration[i]/10.0;
		fprintf(config->ofd,"%s\t%d",ps->phons[i],dur);
		while(i == ps->targ_phon[j]) {
			fprintf(config->ofd,"\t(%d,%d)",ps->pc_targs[j],ps->targ_freq[j]);
			j++;
		}
		fprintf(config->ofd,"\n");
	}
}

void tsFreePhoneImplementation::get_spn_data(CONFIG *config, SPN *ps)
{
	char s[100];
	int lineno=1;

	ps->p_sz = 0;  /* actually set in the mallocing too  */
	ps->t_sz = 0;

	while(fgets(s,100,config->ifd)) {
		s[99] = '\0';
		if(!strcmp(s,"\n")) {
			return;
		}
		if(strlen(s) == 99) {
			fprintf(stderr,"Possibly malformed .spn input file, line %d\n",lineno);
		}

		transmogrify(config,s,ps);

		if(ps->p_sz>=ps->p_max) {
			ps_realloc(ps->p_max*2,ps->t_max,ps);
		}
		if(ps->t_sz>=ps->t_max) {
			ps_realloc(ps->p_max,ps->t_max*2,ps);
		}
		lineno++;
	}
	/* perhaps we should close the file  ??  */
}

void tsFreePhoneImplementation::transmogrify(CONFIG *config, char *s, SPN *ps)
{
	char *phon;
	char *durword;
	float durfloat;
	char *targ_spec;
	int freq,perc;

	phon = strtok(s," \t");
	strcpy(ps->phons[ps->p_sz],phon);

	durword = strtok(NULL," \t");
	sscanf(durword,"%f",&durfloat);
	if(((durfloat < 20) || ( durfloat > 400)) && strcmp(phon,"#")) {
		fprintf(stderr,"Unusual duration %dmS for phoneme %s\n",(int)durfloat,phon);
	}
	ps->duration[ps->p_sz] = (int)((config->sr/1000)*durfloat);	/* durations are stored as samples  */

	while((targ_spec = strtok(NULL," \t"))!=NULL) {
		sscanf(targ_spec," %*c%d%*c%d%*c",&perc,&freq);
	
		ps->pc_targs[ps->t_sz] = perc;
		ps->targ_freq[ps->t_sz] = freq;
		ps->targ_phon[ps->t_sz] = ps->p_sz;
	
		ps->t_sz++;
	}

	ps->p_sz++;	/* must come after copying of freq. info  */

	if(!ps->t_sz)
		fprintf(stderr,"No frequency specified, using default\n");
}

