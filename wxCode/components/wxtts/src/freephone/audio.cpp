/////////////////////////////////////////////////////////////////////////////
// Name:        audio.cpp
// Purpose:     Audio I/O implementation
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: audio.cpp,v 1.1.1.1 2003-12-28 21:08:15 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#ifdef _WINDOWS
#include <windows.h>
#endif

#include <stdio.h>
#include <string.h>
#include "t2s.h"

unsigned char hdr16k[] =  {
0x2e, 0x73, 0x6e, 0x64, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x03,
0x00, 0x00, 0x3e, 0x80, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
 
unsigned char hdr10k[] =  {
0x2e, 0x73, 0x6e, 0x64, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x03,
0x00, 0x00, 0x25, 0x80, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
 
unsigned char hdr8k[] =  {
0x2e, 0x73, 0x6e, 0x64, 0x00, 0x00, 0x00, 0x20, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01,
0x00, 0x00, 0x1f, 0x40, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


void tsFreePhoneImplementation::output_open(CONFIG *config)
{

	if(!strcmp(config->output_file,"-")) {
		config->ofd = stdout;
	} else if((config->ofd=fopen(config->output_file,"w")) == NULL) {
#ifdef _WINDOWS
    MessageBox(NULL, "Cannot open output file", "FreePhone", MB_OK);
    return;
#else
		(void)fprintf(stderr,"Cannot open output file: %s\n",config->output_file);
		exit(2);
#endif
	}
	if(!strcmp(config->prog,"high_level")) {
		return;
	}	/* the formats son't matter  */


	if(!strcmp(config->format,"sun10k")) {
		fwrite(hdr10k,1,32,config->ofd);
	} else if(!strcmp(config->format,"sun16k")) {
		fwrite(hdr16k,1,32,config->ofd);
	} else if(!strcmp(config->format,"sun8k")) {
		fwrite(hdr8k,1,32,config->ofd);
	} else if(!strcmp(config->format,"soundblaster8")) {
		; /* nothing, as far as I know  */
	}

}

#if 0
void audio_play(short *start,int sz,int number,CONFIG *config)
{
	unsigned char intermediate[OUT_BUF];
	int i;

	if(!strcmp(config->format,"sun10k")) {
		fwrite(start,sz,number,config->ofd);
	} else if(!strcmp(config->format,"sun16k")) {
		if(FT_LITTLE_ENDIAN) {
			for(i=0;i<number;i++) {
				start[i] = SWAPSHORT(start[i]);
			}
		}
		fwrite(start,sz,number,config->ofd);
	} else if(!strcmp(config->format,"sun8k")) {
		for(i=0;i<number;i++)
			intermediate[i] = linear2ulaw(start[i]);
		fwrite(intermediate,sizeof(char),number,config->ofd);
	} else if(!strcmp(config->format,"soundblaster8")) {
		/* not implemented, but very simple  */
		/* eg just add 128 to a 7-bit signal */;
	}
}
#endif


void tsFreePhoneImplementation::output_close(CONFIG *config)
{
	if(config->ofd && (config->ofd != stdout))
		fclose(config->ofd);
}
