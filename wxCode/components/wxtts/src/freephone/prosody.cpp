/////////////////////////////////////////////////////////////////////////////
// Name:        prosody.cpp
// Purpose:     ??
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: prosody.cpp,v 1.1.1.1 2003-12-28 21:08:15 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include "t2s.h"

void tsFreePhoneImplementation::prosody(SPROSOD_LIST *spl, SPN *ps)
{
	int i, count;
	int sos, eos, curr_sn, prev_sn;

	count = spl->sz;

	ps->t_sz = 0;
	sos = 0;
	curr_sn = 0;
	prev_sn = -1;
	for(i=0;i<count;i++) {
		if(!i) {
			set_target(i,0,125,ps);
		}
		if((i+1) >=ps->p_max) {
			ps_realloc(ps->p_max*2,ps->t_max,ps);
		}
		strcpy(ps->phons[i],spl->phoneme[i]->phoneme);
		ps->duration[i] = spl->phoneme[i]->dur * 10;

		prev_sn = curr_sn;
		curr_sn = spl->phoneme[i]->syll;
		if(prev_sn != curr_sn)
			sos = i;
		eos = i;
		while(eos<count && spl->phoneme[eos]->syll==curr_sn)
			eos++;
		eos--;

		if(!strcmp(spl->phoneme[i]->sprosod1,"") &&
			!strcmp(spl->phoneme[i]->sprosod2,"")) {
			;
		} else if(!strcmp(spl->phoneme[i]->sprosod1,"H*") &&
			!strcmp(spl->phoneme[i]->sprosod2,"")) {
			set_target(sos,0,100,ps);
			set_target(i,30,130,ps);
		} else if(!strcmp(spl->phoneme[i]->sprosod1,"H[") &&
			!strcmp(spl->phoneme[i]->sprosod2,"")) {
			; /* nothing  */
		} else if(!strcmp(spl->phoneme[i]->sprosod2,"L-H]") &&
			!strcmp(spl->phoneme[i]->sprosod1,"")) {
			set_target(eos,90,100,ps);
		} else if(!strcmp(spl->phoneme[i]->sprosod2,"L-L]") &&
			!strcmp(spl->phoneme[i]->sprosod1,"")) {
			set_target(eos,99,80,ps);
		} else if(!strcmp(spl->phoneme[i]->sprosod1,"H*") &&
			!strcmp(spl->phoneme[i]->sprosod2,"L-H]")) {
			set_target(sos,0,100,ps);
			set_target(i,30,130,ps);
			set_target(i,80,100,ps);
			set_target(eos,99,120,ps);
		} else if(!strcmp(spl->phoneme[i]->sprosod1,"H*") &&
			!strcmp(spl->phoneme[i]->sprosod2,"L-L]")) {
			set_target(sos,0,100,ps);
			set_target(i,30,130,ps);
			set_target(i,80,90,ps);
			set_target(eos,99,80,ps);
		} else {
			(void)fprintf(stderr,"f0 pattern not recognized: %s %s\n",
				spl->phoneme[i]->sprosod1,spl->phoneme[i]->sprosod2);
		}
		if(i==count-1) {
			set_target(i,99,70,ps);
		}
	}
	ps->p_sz = count;
}

void tsFreePhoneImplementation::set_target(int i, int perc, int freq, SPN *ps)
{
        ps->pc_targs[ps->t_sz] = perc;
        ps->targ_freq[ps->t_sz] = freq;
        ps->targ_phon[ps->t_sz] = i;

        ps->t_sz++;
	if(ps->t_sz == ps->t_max) {
		ps_realloc(ps->p_max,ps->t_max*2,ps);
	}
}
