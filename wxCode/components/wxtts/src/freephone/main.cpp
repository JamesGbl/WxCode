/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     Main program (obsolete)
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: main.cpp,v 1.1.1.1 2003-12-28 21:08:13 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

char version[] =  "FreeSpeech-UK-C-0.4";


#include <stdio.h>
#include <string.h>
#include "t2s.h"


int main(int argc, char **argv)
{

    CONFIG config_struct;
    BUFFER buffer_struct;
    LING_LIST ling_list_struct;
    SENT sent_struct;
    SPROSOD_LIST spl_struct;
    SPN ps_struct;
    ACOUSTIC as_struct;

    CONFIG *config = &config_struct;
    BUFFER *buffer = &buffer_struct;
    LING_LIST *ling_list = &ling_list_struct;
    SENT *sent = &sent_struct;
    SPROSOD_LIST *spl = &spl_struct;
    SPN *ps = &ps_struct;
    ACOUSTIC *as = &as_struct;

    int c;
    int errflg = 0;

    extern char *optarg;
    extern int optind;

    strcpy(config->prog,argv[0]);
    config->input_file = "-";
    config->output_file = "-";
    config->hash_file = "-";
    config->diphone_file = "-";
    config->format = "sun8k";
    strcpy(config->prog,"high_level");
    config->type = _MBROLA_;

    while((c = getopt(argc,argv,"i:o:h:ml")) != -1)
        switch(c) {
            case 'i':
                config->input_file = optarg;
                break;
            case 'o':
                config->output_file = optarg;
                break;
            case 'd':
                config->diphone_file = optarg;
                break;
            case 'h':
                config->hash_file = optarg;
                break;
            case 'f':
                config->format = optarg;
                break;
            case 's':       /* ``signal/synthesis''  */
                strcpy(config->prog,"low_level");
                break;
            case 'l':   /* ``linguistic''  */
                strcpy(config->prog,"high_level");
                config->type = _SPN_;
                break;
            case 'm':   /* ``MBROLA''  */
                strcpy(config->prog,"high_level");
                config->type = _MBROLA_;
                break;
            case 't':   /* ``silent Test flag''  */
                strcpy(config->prog,"test_diphones");
                break;
            default:
                errflg++;
        }
    if((optind!=argc) || errflg /* || (argc==1) */) {
        (void)fprintf(stderr, "usage: %s\n",argv[0]);
        (void)fprintf(stderr,"\t-i  input text file or - for standard input (default)\n");
        (void)fprintf(stderr,"\t-o  output (probably audio) file or - for standard output (default)\n");
            /* (void)fprintf(stderr,"\t-d   diphone files (default)\n");
        (void)fprintf(stderr,"\t-f output format (sun8k - default, sun10k, soundblaster8, .spn)\n");  */
        (void)fprintf(stderr,"\t-h  dictionary in hash format (no default)\n");
        /*(void)fprintf(stderr,"\t-l    linguistic (processing only)\n");*/
        /*(void)fprintf(stderr,"\t-m    linguistic (MBROLA only)\n");*/
        /* (void)fprintf(stderr,"\t-s   force use of synth prog only\n");  */

        exit (2);
        }

    init(config, buffer, ling_list, sent, spl, ps, as);

    if(!strcmp(config->prog,"low_level")) {
        go2(config, ps, as);
    } else if(!strcmp(config->prog,"high_level")) {
        go3(config, buffer, ling_list, sent, spl, ps);
    } else if(!strcmp(config->prog,"test_diphones")) {
#ifdef DEBUG
        test_diphones(config, sent, spl, ps, as);
#endif
        ;
    } else {
        go(config, buffer, ling_list, sent, spl, ps, as);
    }

    terminate(config, buffer, ling_list, sent, spl, ps, as);

    return(0);
}
