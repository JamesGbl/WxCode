/////////////////////////////////////////////////////////////////////////////
// Name:        broad_cats.cpp
// Purpose:     Categories
// Author:      Alistair Conkie
// Modified by:
// Created:
// RCS-ID:      $Id: broad_cats.cpp,v 1.1.1.1 2003-12-28 21:08:21 wyo Exp $
// Copyright:   (c) 1996 Alistair Conkie
// Licence:     You may distribute under the terms of the GNU General Public
//              Licence as specified in the README file.
/////////////////////////////////////////////////////////////////////////////

#include "t2s.h"

KEY broad_cats[] = {
  {        "*", STRESS, },     /* because it is always beside a vowel	*/
  {        "+", BOUND, },      /* morph boundary */
  {        "-", BOUND, },
  {        ".", BOUND, },      /* to force boundary			*/
  {        "a", VOWEL, },
  {        "aa", VOWEL, },
  {        "ai", VOWEL, },
  {        "air", VOWEL, },
  {        "ar", VOWEL, },
  {        "aw", VOWEL, },
  {        "b", FREEPHONE_CONST, },
  {        "ch", FREEPHONE_CONST, },
  {        "d", FREEPHONE_CONST, },
  {        "dh", FREEPHONE_CONST, },
  {        "e", VOWEL, },
  {        "ee", VOWEL, },
  {        "eer", VOWEL, },
  {        "er", VOWEL, },
  {        "f", FREEPHONE_CONST, },
  {        "g", FREEPHONE_CONST, },
  {        "h", FREEPHONE_CONST, },
  {        "i", VOWEL, },
  {        "ie", VOWEL, },
  {        "j", FREEPHONE_CONST, },
  {        "k", FREEPHONE_CONST, },
  {        "l", FREEPHONE_CONST, },
  {        "m", FREEPHONE_CONST, },
  {        "n", FREEPHONE_CONST, },
  {        "ng", FREEPHONE_CONST, },
  {        "o", VOWEL, },
  {        "oa", VOWEL, },
  {        "oi", VOWEL, },
  {        "oo", VOWEL, },
  {        "oor", VOWEL, },
  {        "ou", VOWEL, },
  {        "p", FREEPHONE_CONST, },
  {        "r", FREEPHONE_CONST, },
  {        "s", FREEPHONE_CONST, },
  {        "sh", FREEPHONE_CONST, },
  {        "t", FREEPHONE_CONST, },
  {        "th", FREEPHONE_CONST, },
  {        "u", VOWEL, },
  {        "uu", VOWEL, },
  {        "v", FREEPHONE_CONST, },
  {        "w", FREEPHONE_CONST, },
  {        "y", FREEPHONE_CONST, },
  {        "z", FREEPHONE_CONST, },
  {        "zh", FREEPHONE_CONST, },
  {        "|", BOUND, },         /* word boundary */
  {        "~", STRESS, },        /* because it is always beside a vowel	*/
  {	"", 0, },
};

