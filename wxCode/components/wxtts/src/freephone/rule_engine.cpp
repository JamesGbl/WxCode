/////////////////////////////////////////////////////////////////////////////
// Name:        rule_engine.cpp
// Purpose:     ??
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: rule_engine.cpp,v 1.1.1.1 2003-12-28 21:08:21 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

/*
    Note --- several changes to buffer management by Josh Yelon, Dec 10 1997.
    Trying to fix buffer overrun problems.  Think it works.  Don't understand
    the purpose of this subroutine, though, which makes me nervous. :)  Here
    are some old comments that may or may not apply:

           Now we are ready to apply the rules
           for now there is not compactify/decompactify
           for now there is no null target
           and what about a stepping through version -- 
             should be quite easy
*/
 
#include "t2s.h"

void tsFreePhoneImplementation::rule_exec(int type,int nrules, RULE *rule, SENT *sent)
{
  int i;
  int lmatch = 0, rmatch = 0;
  char *input = buffer_text(&(sent->list));
  BUFFER ib;
  BUFFER prev;
  
  buffer_init(&ib);
  buffer_init(&prev);
  while (*input) {
    for(i=0;i<nrules;i++) {
      lmatch = 0;
      rmatch = 0;
      if(!strncmp(input,rule[i].target,strlen(rule[i].target))) {
	/* candidate for rule match  */
	lmatch = regexec(rule[i].lc,buffer_text(&prev));
	rmatch = regexec(rule[i].rc,input+strlen(rule[i].target));
	if(lmatch && rmatch) {
	  buffer_add_str(&ib, rule[i].output);
	  input += strlen(rule[i].target);
	  buffer_add_str(&prev, rule[i].target);
	  break;
	}
      }
    }
    if(lmatch && rmatch) {
      ;
    } else if (type|SAME) {
      buffer_add_char(&ib, *input);
      buffer_add_char(&prev, *input);
      input++;
    } else {
      buffer_free(&prev);
      buffer_free(&ib);
      return;
    }
  }
  buffer_clear(&(sent->list));
  buffer_add_str(&(sent->list), buffer_text(&ib));
  buffer_free(&prev);
  buffer_free(&ib);
}
