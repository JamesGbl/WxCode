/////////////////////////////////////////////////////////////////////////////
// Name:        durpros.cpp
// Purpose:     Assignment of intrinsic durations.
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: durpros.cpp,v 1.1.1.1 2003-12-28 21:08:13 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "t2s.h"

/* assignment of intrinsic durations  */
/* note that the section dealing with diacritics is rather poorly done  */

void tsFreePhoneImplementation::durpros(CONFIG *config, SPROSOD_LIST *spl)
{
	int i;
	SPROSOD *splp;
	char tph[5];
	int syll_ref = -1;
        KEY *tmp;

	for(i=spl->sz-1;i>=0;i--) {
		splp = spl->phoneme[i];
		if(strcmp(splp->phoneme,"#")) {
		        if(splp->phoneme[0]!='_') {
			        sscanf(splp->phoneme,"%[a-z]",tph);
			} else {
			        sscanf(&(splp->phoneme[1]),"%[a-z]",tph);
		        }
			if((tmp=binary(tph,dur0,config->dur0_num))!=NULL) {
				splp->dur = tmp->keycount; 
			} else {
				splp->dur = 100;
                                fprintf(stderr,"Unrecognized phoneme: %s\n",tph);
			}
			if((syll_ref-splp->syll)==1) {
			        splp->dur *=1.3;  /* phrase final lengthening  */
			}
		} else {
		        syll_ref = splp->syll;
		}
	}
}

