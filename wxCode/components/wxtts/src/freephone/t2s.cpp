/////////////////////////////////////////////////////////////////////////////
// Name:        t2s.cpp
// Purpose:     ??
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: t2s.cpp,v 1.1.1.1 2003-12-28 21:08:11 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "t2s.h"

void tsFreePhoneImplementation::process_sentence(CONFIG *config, BUFFER *buffer, LING_LIST *ling_list, SENT *sent, SPROSOD_LIST *spl, SPN *ps, ACOUSTIC *as)
{
	high_level(config,buffer,ling_list,sent,spl,ps);

	low_level(config,ps,as);

	fflush(config->ofd);
}

void tsFreePhoneImplementation::produce_spn_file(CONFIG *config, BUFFER *buffer, LING_LIST *ling_list, SENT *sent, SPROSOD_LIST *spl, SPN *ps)
{

	high_level(config,buffer,ling_list,sent,spl,ps);

	if(config->type == _SPN_) {
		put_spn_data(config,ps);
	} else if(config->type == _MBROLA_) {
		put_mbrola_data(config,ps);
	}

	fflush(config->ofd);
}

void tsFreePhoneImplementation::process_spn_file(CONFIG *config, SPN *ps, ACOUSTIC *as)
{
	get_spn_data(config,ps);

	low_level(config,ps,as);

	fflush(config->ofd);
}

void tsFreePhoneImplementation::high_level(CONFIG *config, BUFFER *buffer, LING_LIST *ling_list, SENT *sent, SPROSOD_LIST *spl, SPN *ps)
{
	tags(config,buffer,ling_list); 

	grammar(ling_list);

	transcribe(config,ling_list);

	conv1(config,ling_list,sent);
	phon_rules(sent);
	conv2(config,sent,spl);

	durpros(config,spl);

	prosody(spl,ps);
}

void tsFreePhoneImplementation::low_level(CONFIG *config, SPN *ps, ACOUSTIC *as)
{
        phonstoframes(ps,as);

        durations(config,ps,as);       

        calc_pitch(config,ps,as);      

        make_wave(config,as);
                                        
}
