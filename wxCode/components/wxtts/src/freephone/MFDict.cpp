/////////////////////////////////////////////////////////////////////////////
// Name:        MFDict.cpp
// Purpose:     Simple dictionary look-up class
// Author:      Mirko Buffoni
// Modified by:
// Created:
// RCS-ID:      $Id: MFDict.cpp,v 1.1.1.1 2003-12-28 21:08:19 wyo Exp $
// Copyright:   (c) 1998 Mirko Buffoni
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include "MFDict.h"

#include <string.h>

// Comment it to remove unuseful debugging stuff
// #define TEST_VERSION

#ifdef TEST_VERSION
#define Debug   printf
#else
#define Debug   //
#endif


/*  Here just because generally C doesn't come with
 *  a GetFileSize function.  If anyone knows a better
 *  way to handle this, please let me know!
 */
unsigned long filesize(void *f)
{
    fpos_t pos,rv;
    fgetpos((FILE *)f, &pos);
    fseek((FILE *)f, 0, SEEK_END);
    fgetpos((FILE *)f, &rv);
    fsetpos((FILE *)f, &pos);

    return (unsigned long)rv;
}


Dictionary::Dictionary(char * indexFile, char * keywordFile, char * traslateFile)
{
    int   i;
    FILE *f;

    isValid_ = 0;

    for (i=0; i < 256; i++)
    {
        indexStart_[i] = 0;
        indexEnd_[i] = 0;
    }
    nKeywords_ = 0;
    keyword_   = NULL;
    traslation_= NULL;
    index_     = NULL;

    Debug("Loading ... %s\n", indexFile);
    if ((f = fopen(indexFile, "rb")) != NULL)
    {
        int size = filesize(f);

        nKeywords_ = size/sizeof(TIndex);
        if ((index_ = new TIndex [nKeywords_]) == NULL)
            return;     // Not enough memory
        fread(index_,1,size,f);
        fclose(f);
    }
    else return;        // File not found

    Debug("Loading ... %s\n", keywordFile);
    if ((f = fopen(keywordFile,"rb")) != NULL)
    {
        int size = filesize(f);

        keyword_ = new char [size];
        if (keyword_ == NULL)
            return;     // Not enough memory
        fread(keyword_,1,size,f);
        fclose(f);
    }
    else return;        // File not found

    Debug("Loading ... %s\n", traslateFile);
    if ((f = fopen(traslateFile,"rb")) != NULL)
    {
        int size = filesize(f);

        if ((traslation_ = new char [size]) == NULL)
            return;     // Not enough memory
        fread(traslation_,1,size,f);
        fclose(f);
    }
    else return;        // File not found

    int j;

    for (j=0; j<nKeywords_; j++)
      indexEnd_[ *(keyword_ + index_[j].Key) ] = j+1;
    for (j=nKeywords_-1; j>=0; j--)
      indexStart_[ *(keyword_ + index_[j].Key) ] = j;

    isValid_ = 1;
}

Dictionary::~Dictionary(void)
{
    if (traslation_)
        delete [] traslation_;
    if (keyword_)
        delete [] keyword_;
    if (index_)
        delete [] index_;
}

char * Dictionary::FindDataIC(char * keyword)
{
    long  rvi;     //IndiceRicercaVeloce;
    int   len = strlen(keyword);

    if (len > 0)
    {
        char s = UpCase(keyword[0]);

        for (rvi=indexStart_[s]; rvi < indexEnd_[s] ; rvi++)
        {
            char * kw = keyword_ + index_[rvi].Key;

#ifdef _WINDOWS
            if (strnicmp( kw , keyword, len) == 0)
#else
            if (strncasecmp( kw , keyword, len) == 0)
#endif
                return traslation_ + index_[rvi].Trad;
        }
    }

    return NULL;
}

char * Dictionary::FindData(char * keyword)
{
    long  rvi;
    int   len = strlen(keyword);

    if (len > 0)
    {
        unsigned char s = keyword[0];

        for (rvi=indexStart_[s]; rvi < indexEnd_[s] ; rvi++)
        {
            char * kw = keyword_ + index_[rvi].Key;

//            if (strncmp( kw , keyword, len) == 0)
            if (strcmp( kw , keyword) == 0)
                return traslation_ + index_[rvi].Trad;
        }
    }

    return NULL;
}

/*
 * For use by C programs
 */

// Returns a handle to a Dictionary object
void* MFOpenDictionary(char * indexFile, char * keywordFile, char * translateFile)
{
    return (void*) MFOpenDictionaryEx(indexFile, keywordFile, translateFile);
}

Dictionary* MFOpenDictionaryEx(char * indexFile, char * keywordFile, char * traslateFile)
{
    Dictionary* dict = new Dictionary(indexFile, keywordFile, traslateFile);
    if (!dict->IsValid())
    {
        delete dict;
        return NULL;
    }
    else
    {
        return dict;
    }
}

// Pass a handle to a Dictionary object
void MFCloseDictionary(void* dict)
{
    Dictionary* d = (Dictionary*) dict;
    delete d;
}

void MFCloseDictionaryEx(Dictionary* dict)
{
    delete dict;
}

// Pass a handle to a Dictionary object
char* MFFindData(void* dict, char* keyword)
{
    Dictionary* d = (Dictionary*) dict;
    return d->FindData(keyword);
}

// Pass a handle to a Dictionary object
char* MFFindDataIC(void* dict, char* keyword)
{
    Dictionary* d = (Dictionary*) dict;
    return d->FindDataIC(keyword);
}

#ifdef TEST_VERSION

#include <stdlib.h>

void main(void)
{
    Dictionary dict("Index.MF", "Keywords.MF", "Translate.MF");

    if (!dict.IsValid())
    {
        printf("Error loading vocabularies.\n");
        exit(-1);
    }

    char * src = "Kate";    // Do not forget to add MIRKO to the dictionary ;)

    printf("Finding '%s' -> %s\n", src, dict.FindDataIC(src));
}

#endif
