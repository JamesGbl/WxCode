/////////////////////////////////////////////////////////////////////////////
// Name:        durations.cpp
// Purpose:     ??
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: durations.cpp,v 1.1.1.1 2003-12-28 21:08:13 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "t2s.h"

static int min(int a, int b);
static float fmax(float a, float b);

void tsFreePhoneImplementation::durations(CONFIG *config, SPN *ps, ACOUSTIC *as)
{
	int durdist;
	int interdist;
	float multiplier_i;
	float proportion;
	int i;
	int j;
	
	for(i=0;i<ps->p_sz;i++) 
		ps->scale[i] = (float)ps->duration[i] /
		(float)((ps->pb[i+1]-ps->pb[i])*config->fr_sz);
	
	ps->cum_dur[0] = 0;  /* do cumulative at same time  */
	for(i=0,j=0;i<as->f_sz;i++) {
		if(i == ps->pb[j]) {
			if(j != 0) {
				ps->cum_dur[j] = ps->duration[j-1] + ps->cum_dur[j-1];
			}
			as->duration[i] = config->fr_sz;
			ps->duration[j] = config->fr_sz; /* saves adding later  */
			j++;
		} else {
			durdist = min(i-ps->pb[j-1],ps->pb[j]-i);
			interdist = ps->pb[j] - ps->pb[j-1];
			proportion = (float)durdist/(float)interdist;
			multiplier_i = fmax(0.01,4.0*proportion*(ps->scale[j-1]-1.0)+1.0);
			as->duration[i] = config->fr_sz*multiplier_i;
			ps->duration[j-1] += as->duration[i];
		}
	}
}

static int min(int a, int b)
{
	return((a<b)?a:b);
}

static float fmax(float a, float b)
{
	return((a>b)?a:b);
}

