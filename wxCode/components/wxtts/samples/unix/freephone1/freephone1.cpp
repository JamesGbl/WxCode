/////////////////////////////////////////////////////////////////////////////
// Name:        freephone.cpp
// Purpose:     Demonstrates use of wxFreePhone classes: simple command line
//              input of text, output of .pho file
// Author:      Julian Smart
// Modified by:
// Created:     10/6/98
// Copyright:   Julian Smart
// Licence:
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include <freephone.h>

wxFreePhone g_freephone("..\\..\\..\\dictionary\\english");

int main(int argc, char* argv[])
{
	g_freephone.Init();

    char buf[512];

    while (1)
    {
        printf("> ");
        int ch = 0;
        int i = 0;
        while ((ch = fgetc(stdin)) != '\n')
        {
            buf[i] = ch;
            i ++;
        }
        buf[i] = 0;

        g_freephone.TextToPhonemeFile(buf, "test.pho");
    }

    return 0;
}

