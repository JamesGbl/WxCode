#include "MFDict.h"
#include <stdlib.h>

#define MAXINDEX 0x40000    // Max number of keywords in vocabulary.  Change to whatever you like.

void StripExtension(char *buffer);

char * minstring(char * s)
{
    char * p = s;

    while (*p != 0 && *p == 32) p++;
    while (p[strlen(p)-1] == 32 && strlen(p) > 0) p--;

    return p;
}

long FileGetPos(void * f)
{
    fpos_t pos;
    if (fgetpos((FILE*)f, &pos) == 0)
        return pos;
    return -1;
}

void main(int argc, char * argv[])
{
    FILE *f;
    FILE *fKey;
    FILE *fTrad;
    FILE *fIndex;

    long    nKey, nTrad, total;
    long    maxlen = 0;     // Just to know how long is the longest traslation

    char * buf = new char [16384];
    TIndex * index = new TIndex [MAXINDEX];

    for (total=0; total < MAXINDEX; total++)
    {
        index[total].Key  = -1;
        index[total].Trad = -1;
    }

    if (argc < 2)
    {
        printf("Dictionary Parser - (C)1998 by Mirko Buffoni\n\n");
        printf("This program create a file of keywords (.key), a file of translations (.dct)\nand an index file (.idx), starting from an input text file.\n\n");
        printf("USAGE:  MkDict.exe  <filename>\n");
        exit(-1);
    }

    printf("\nParsing file:  %s\n\n", argv[1]);
    f = fopen(argv[1], "r");

    char keywordsFilename[512];
    char dataFilename[512];
    char indexFilename[512];

    strcpy(keywordsFilename, argv[1]);
    strcpy(dataFilename, argv[1]);
    strcpy(indexFilename, argv[1]);

    StripExtension(keywordsFilename);
    StripExtension(dataFilename);
    StripExtension(indexFilename);

    strcat(keywordsFilename, ".key");
    strcat(dataFilename, ".dct");
    strcat(indexFilename, ".idx");

    fKey   = fopen(keywordsFilename, "wb");
    fTrad  = fopen(dataFilename, "wb");
    fIndex = fopen(indexFilename, "wb");

    nKey = 0;
    nTrad = 0;
    total = 0;

    while (f && !feof(f))
    {
        memset(buf,0,8192);
        fgets(buf, 8192, f);
        if (strlen(buf) > 0) buf[strlen(buf)-1] = 0;
        if (strlen(buf) > maxlen)
            maxlen = strlen(buf);

        buf = minstring(buf);

        if (strlen(buf) > 0)    // Skip empty lines
        {
            char *p = buf;
            p = strtok(buf," ");
            if (p)              // If there is no space, it's probably an error :)
            {
                if (index[total].Key == -1)
                {
                    index[total].Key = FileGetPos(fKey);
                    index[total].Trad = FileGetPos(fTrad);
                }

//              printf("%s -> ", p);
                fwrite(p, 1, strlen(p)+1, fKey);
                p += strlen(p)+1;
//              printf("%s\n", p);
                fwrite(p, 1, strlen(p)+1, fTrad);
                total++;
                printf("\x0dProc. terms: %d", total);


            }
        }

    }

    printf("\nTotal number of keywords in dictionary:  %d\n", total);
    fwrite(index,sizeof(TIndex),total,fIndex);

    fclose(fIndex);
    fclose(fTrad);
    fclose(fKey);

    delete [] index;
    delete [] buf;
}

/*
 * Strip off any extension (dot something) from end of file,
 * IF one exists. Inserts zero into buffer.
 *
 */
 
void StripExtension(char *buffer)
{
  int len = strlen(buffer);
  int i = len-1;
  while (i > 0)
  {
    if (buffer[i] == '.')
    {
      buffer[i] = 0;
      break;
    }
    i --;
  }
}

