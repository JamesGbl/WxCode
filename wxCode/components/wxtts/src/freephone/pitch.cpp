/////////////////////////////////////////////////////////////////////////////
// Name:        pitch.cpp
// Purpose:     ??
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: pitch.cpp,v 1.1.1.1 2003-12-28 21:08:14 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "t2s.h"

void tsFreePhoneImplementation::calc_pitch(CONFIG *config, SPN *ps, ACOUSTIC *as)
{
	int j,k;
	int y;
	int l = 0;
	int k_old = 0;
	int k_fine = 0;
	int x = 0;

	for(j=0;j<ps->t_sz;j++) 
		ps->abs_targ[j] = ps->cum_dur[ps->targ_phon[j]] + 
		ps->pc_targs[j]*ps->duration[ps->targ_phon[j]]/100.0;

	for(k=0;k<ps->cum_dur[ps->p_sz];k+=100) {
		y = interpolated_freq(k,ps);
		x += 100*y;
		while(x>config->sr) {
			k_fine = k + interpolate(x-100*y,0,x,100,config->sr);
			x -= config->sr;
			as->pitch[l++] = k_fine-k_old;
			if(l == as->p_max) {
				as_realloc(as->f_max,as->p_max*2,as);
			}
			k_old = k_fine;
                }
	}
	as->p_sz = l; /* I THINK THIS SHOULD BE +1  */
	as->pitch[0] += config->fr_sz/2;  /* to compensate for mismatch  */
}

int tsFreePhoneImplementation::interpolated_freq(int k, SPN *ps)
{
	int i;
	int freq;

	if(!ps->t_sz)
		return(DEF_F0);
	else if(k<ps->abs_targ[0]) 
		return(ps->abs_targ[0]);
	else if(k>=ps->abs_targ[ps->t_sz-1])
		return(ps->abs_targ[ps->t_sz-1]);
	for(i=1;i<ps->t_sz;i++) {
		if((k<ps->abs_targ[i]) && (k>=ps->abs_targ[i-1])) {
			freq = interpolate(ps->abs_targ[i-1],
					   ps->targ_freq[i-1],
					   ps->abs_targ[i],
					   ps->targ_freq[i],k);
			return(freq);
		} 
	}
	return(-1); /* should never arrive here  */
}

int tsFreePhoneImplementation::interpolate(int a,int b,int c,int d,int e)
{
	int f;

	f = (c*b + d*e - e*b -a*d)/(c-a);

	return(f);
}
