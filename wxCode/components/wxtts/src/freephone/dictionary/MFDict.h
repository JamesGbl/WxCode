/*  Generic Dictionary class
 *
 *  Copyright (C) 1998 by Mirko Buffoni (mix@lim.dsi.unimi.it)
 *
 *  This class has been implemented to help in fast recovering
 *  of traslation association, given a known keyword (usually a
 *  definition).  It requires three separate files that can be
 *  generated with the application "CreaDict", and that are:
 *  - Index file
 *  - Keywords file
 *  - Traslations file
 *  The algorythm is quite simple:  substantially it's a sequential
 *  search, just a little smarter, meaning that the class 'knows'
 *  exactly the range where a word starting with a certain character
 *  resides.  If you suppose to have many many words starting with
 *  the same letter, sequential search can be substituted by a
 *  binary search, resulting in a computational time of O(LOG N).
 */


#ifndef _MF_DICT_H_
#define _MF_DICT_H_


#include <stdio.h>
#include <string.h>

#define MF_OK   1
#define MF_NOK  0

typedef struct Index
{
    long    Key;
    long    Trad;
} TIndex;

/*  Remove the following functions if you have them yet */
inline char UpCase(char A){ return (((A>0x60)&&(A<0x7b))?(A-0x20):A); }
inline char LoCase(char A){ return (((A>0x40)&&(A<0x5b))?(A+0x20):A); }


class Dictionary
{
private:
    /*  These are the indexes for each ASCII char, for fast recovering
     *  (When you search in a telephony book, you have an idea of which
     *  part of the book to start from.  This applies a similar method).
     */
    long    indexStart_[256];
    long    indexEnd_[256];

    /*  Number of known keywords */
    long    nKeywords_;

    /*  The keywords dictionary */
    char   *keyword_;

    /*  The equivalent set of traslations */
    char   *traslation_;

    /*  Index, automatically built by CreaDict application. */
    TIndex *index_;

    /*  Validity flag   */
    int     isValid_;

public:

    /*  Constructor:  requires the names of the files holding index,
     *                keywords and traslations.  Check IsValid() to see
     *                if everything went well.
     */

    Dictionary(char * indexFile, char * keywordFile, char * traslateFile);
    ~Dictionary(void);

    /*  Returns 1 if initialization goes well, 0 otherwise
     */
    int   IsValid(void) { return isValid_; }

    /*  Returns the total number of known keywords
     */
    int   GetTotalKeywords(void) { return nKeywords_; }

    /*  Returns the pointer to the traslation of the search keyword
     *  Returns NULL no matching keyword has been found
     */
    char *FindData(char * keyword);

    /*  Same as find data, but case insensitive
     */
    char *FindDataIC(char * keyword);
};

#endif
