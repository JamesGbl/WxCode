/////////////////////////////////////////////////////////////////////////////
// Name:        space.cpp
// Purpose:     This makes the space and has the global data pointers
//              defined too.
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: space.cpp,v 1.1.1.1 2003-12-28 21:08:12 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <limits.h>

#include "MFDict.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "t2s.h"

int ft_endian_loc = 1; /* for deciding if we need to byte-swap  */

ENTRY indx[NDIPHS];
FRAME dico[NFRAMES];
int nindex;

void tsFreePhoneImplementation::init(CONFIG *config, BUFFER *buffer, LING_LIST *ling_list, SENT *sent, SPROSOD_LIST *spl, SPN *ps, ACOUSTIC *as)
{
	config->ofd = NULL;
	config->input_file = NULL;
	config->output_file = NULL;
	config->hash_file = NULL;
	config->db = NULL;
	config->phoneme_db = NULL;

	/* load the diphones including index REMOVED */

	/* initialise input buffer  */

	buffer_init(buffer);


	/* initialise linguistic list  */

	ling_list_malloc(DEF_LING_LIST,ling_list);


    /* initialise sent list  */
    sent_init(sent);


    /* initialise conv list  */

    spl_malloc(DEF_SPL,spl);


	/* initialise conv list  */

	spl_malloc(DEF_SPL,spl);



	/* now the synthesis stuff  */

    ps_malloc(DEF_PHONS,DEF_TARGS,ps);
    as_malloc(DEF_FRAMES,DEF_PM,as);       /* should perhaps use ps??  */



	/* size of fw,clas,dur0 tables (for binary)  */

	config->fw_num=0;
	while(fw[config->fw_num].keyword[0] != '\0')
		config->fw_num++;

	config->broad_cats_num=0;
	while(broad_cats[config->broad_cats_num].keyword[0] != '\0')
		config->broad_cats_num++;

	config->dur0_num=0;
	while(dur0[config->dur0_num].keyword[0] != '\0')
		config->dur0_num++;

	config->edin2sampa0_num=0;
	while(edin2sampa0[config->edin2sampa0_num].keyword[0] != '\0')
		config->edin2sampa0_num++;

	/*	what follows is an example for use as a template  */
	load_context_rules("context_rules");
	/* this goes with the rule engine code...
        for(i=0;i<nrules;i++) {
                rule[i].lc = regcomp(rule[i].left_context);
                rule[i].rc = regcomp(rule[i].right_context);
        }
	*/
 
	phon_rules_init();

/*
	(void)fprintf(stderr,"FreeSpeech (C) 1984,1996 Steve Isard, Alistair Conkie\n");
	(void)fprintf(stderr,"There is ABSOLUTELY NO WARRANTY with this program.\n");

*/
}

void tsFreePhoneImplementation::terminate(CONFIG *config, BUFFER *buffer, LING_LIST *ling_list, SENT *sent, SPROSOD_LIST *spl, SPN *ps, ACOUSTIC *as)
{
/*
	if(config->db != NULL)
#ifdef FBSD_DATABASE
        	(void)(config->db->close)(config->db);
#else
		dbm_close(config->db);
#endif
*/

	output_close(config);

	buffer_free(buffer);
	ling_list_free(ling_list);
	sent_free(sent);
	spl_free(spl);

    ps_free(ps);
    as_free(as);

	unload_diphs(config);
	phon_rules_free();

	if (config->phoneme_db)
    {
		MFCloseDictionaryEx(config->phoneme_db);
        config->phoneme_db = NULL;
    }

	/* also need to free the various other structures  */
}

void tsFreePhoneImplementation::ling_list_malloc(int num, LING_LIST *ling_list)
{
  int i;

  ling_list->max = num;
  ling_list->text = (LING **)malloc(sizeof(LING *)*ling_list->max);
  for(i=0;i<ling_list->max;i++) {
    ling_list->text[i] = (LING *)malloc(sizeof(LING));
  }
  ling_list->sz = 0;
}

void tsFreePhoneImplementation::ling_list_realloc(int num, LING_LIST *ling_list)
{
  int i;
  int rem = ling_list->max;

  ling_list->max = num;
  ling_list->text = (LING **)realloc(ling_list->text,sizeof(LING *)*ling_list->max);
  for(i=rem;i<ling_list->max;i++) {
    ling_list->text[i] = (LING *)malloc(sizeof(LING));
  }
}

void tsFreePhoneImplementation::ling_list_free(LING_LIST *ling_list)
{
  int i;

  for(i=0;i<ling_list->max;i++) {
    free(ling_list->text[i]);
  }
  free(ling_list->text);
}

void tsFreePhoneImplementation::spl_malloc(int num, SPROSOD_LIST *spl)
{
  int i;

  spl->max = num; /* REFerence VALue  */
  spl->phoneme = (SPROSOD **)malloc(sizeof(SPROSOD *)*spl->max);
  for(i=0;i<spl->max;i++) {
    spl->phoneme[i] = (SPROSOD *)malloc(sizeof(SPROSOD));
  }
  spl->sz = 0;
}

void tsFreePhoneImplementation::spl_realloc(int num, SPROSOD_LIST *spl)
{
  int i;
  int rem = spl->max;

  spl->max = num; /* REFerence VALue  */
  spl->phoneme = (SPROSOD **)realloc(spl->phoneme,sizeof(SPROSOD *)*spl->max);
  for(i=rem;i<spl->max;i++) {
    spl->phoneme[i] = (SPROSOD *)malloc(sizeof(SPROSOD));
  }
  /* this seems totally redundant  spl->sz = 0;	WHY WHY WHY  */
}

void tsFreePhoneImplementation::spl_free(SPROSOD_LIST *spl)
{
  int i;

  for(i=0;i<spl->max;i++) {
    free(spl->phoneme[i]);
  }
  free(spl->phoneme);
}

void tsFreePhoneImplementation::ps_malloc(int nphons, int ntargs, SPN *ps)
{
  int i;

  ps->p_sz = 0;
  ps->p_max = nphons;
  ps->t_sz = 0;
  ps->t_max = ntargs;

  ps->pc_targs = (int *) malloc(sizeof(int)*(ntargs+1));
  ps->targ_phon = (int *) malloc(sizeof(int)*(ntargs+1));
  ps->targ_freq = (int *) malloc(sizeof(int)*(ntargs+1));
  ps->abs_targ = (int *) malloc(sizeof(int)*(ntargs+1));

  ps->cum_dur = (int *) malloc(sizeof(int)*(nphons+1));
  ps->duration = (int *) malloc(sizeof(int)*(nphons+1));
  ps->pb = (int *) malloc(sizeof(int)*(nphons+1));
  ps->scale = (float *) malloc(sizeof(float)*(nphons+1));
  ps->phons = (char **) malloc(sizeof(int)*(nphons+1));
  ps->diphs = (char **) malloc(sizeof(int)*(nphons+1));

  for(i=0;i<nphons;i++) {
    ps->phons[i] = (char *)malloc(sizeof(PHON_SZ));
    ps->diphs[i] = (char *)malloc(sizeof(DIPH_SZ));
  }
}

void tsFreePhoneImplementation::ps_realloc(int nphons, int ntargs, SPN *ps)
{
  int i;

  int rem_p = ps->p_max;
  ps->p_max = nphons;
  ps->t_max = ntargs;

  ps->pc_targs = (int *) realloc(ps->pc_targs,sizeof(int)*(ntargs+1));
  ps->targ_phon = (int *) realloc(ps->targ_phon,sizeof(int)*(ntargs+1));
  ps->targ_freq = (int *) realloc(ps->targ_freq,sizeof(int)*(ntargs+1));
  ps->abs_targ = (int *) realloc(ps->abs_targ,sizeof(int)*(ntargs+1));

  ps->cum_dur = (int *) realloc(ps->cum_dur,sizeof(int)*(nphons+1));
  ps->duration = (int *) realloc(ps->duration,sizeof(int)*(nphons+1));
  ps->pb = (int *) realloc(ps->pb,sizeof(int)*(nphons+1));
  ps->scale = (float *) realloc(ps->scale,sizeof(float)*(nphons+1));
  ps->phons = (char **) realloc(ps->phons,sizeof(int)*(nphons+1));
  ps->diphs = (char **) realloc(ps->diphs,sizeof(int)*(nphons+1));

  for(i=rem_p;i<nphons;i++) {
    ps->phons[i] = (char *)malloc(sizeof(PHON_SZ));
    ps->diphs[i] = (char *)malloc(sizeof(DIPH_SZ));
  }
}

void tsFreePhoneImplementation::ps_free(SPN *ps)
{
  int i;

  for(i=0;i<ps->p_max;i++) {
    free(ps->phons[i]);
    free(ps->diphs[i]);
  }

  free(ps->pc_targs);
  free(ps->targ_phon);
  free(ps->targ_freq);
  free(ps->abs_targ);

  free(ps->cum_dur);
  free(ps->duration);
  free(ps->pb);
  free(ps->scale);
  free(ps->phons);
  free(ps->diphs);
}

void tsFreePhoneImplementation::as_malloc(int nframes, int npp, ACOUSTIC *as)
{

  as->p_sz = 0;
  as->f_sz = 0;
  as->p_max = npp;
  as->f_max = nframes;

  as->mcebuf = (FRAME **) malloc(sizeof(FRAME *)*(nframes));
  /*...*/
  as->duration = (short *) malloc(sizeof(short)*(nframes));
  as->pitch = (short *) malloc(sizeof(short)*(npp));
}

void tsFreePhoneImplementation::as_realloc(int nframes, int npp, ACOUSTIC *as)
{
  as->p_max = npp;
  as->f_max = nframes;

  as->mcebuf = (FRAME **) realloc(as->mcebuf,sizeof(FRAME *)*(nframes));
  /*...*/
  as->duration = (short *) realloc(as->duration,sizeof(short)*(nframes));
  as->pitch = (short *) realloc(as->pitch,sizeof(short)*(npp));
}

void tsFreePhoneImplementation::as_free(ACOUSTIC *as)
{
  free(as->mcebuf);
  /*...*/
  free(as->duration);
  free(as->pitch);
}

/*
 * 'SENT' operations.
 *
 */

void tsFreePhoneImplementation::sent_init(SENT *sent)
{
  sent->sil_max = DEF_SENT_SIL;
  sent->sil_sz = 0; 
  sent->sil = (P_ELEM *)malloc(sizeof(P_ELEM)*sent->sil_max);
  buffer_init(&(sent->list));
}

void tsFreePhoneImplementation::sent_alloc_sil(SENT *sent, int n)
{
  if (n >= sent->sil_max) {
    sent->sil_max = ((n*3)/2) + 16;
    sent->sil = (P_ELEM*)realloc(sent->sil, sizeof(P_ELEM)*sent->sil_max);
  }
}

void tsFreePhoneImplementation::sent_free(SENT *sent)
{
  free(sent->sil);
  buffer_free(&(sent->list));
}

