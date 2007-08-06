//
// Copyright (C) 2007 Arne Steinarson <arst at users dot sourceforge dot net>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must
//    not claim that you wrote the original software. If you use this
//    software in a product, an acknowledgment in the product
//    documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must
//    not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source
//    distribution.
//


#ifndef STRUTILS_H
#define STRUTILS_H

#include "pi/platform.h"
#ifndef __WINDOWS__
	#define stricmp   strcasecmp
	#define strnicmp  strncasecmp
#endif

// As strstr but case insensitive
const char *stristr(const char *sbig, const char *ssmall);
// Not reentrant
const char *Int2Str( int i, const char *fmt=NULL );
const char *TruncStr( const char *str, int max_len );

// Look for keyword at str (skips space). If keyword length know, pass along in kwl.
bool keywordieq(const char *str, const char *kw, int kwl=-1);
bool keywordeq(const char *str, const char *kw, int kwl=-1);

// Search for keyword after str
const char *keywordifind(const char *str, const char *kw, const char *pend=NULL, bool advance_past_keyw=true);
const char *keywordfind(const char *str, const char *kw, const char *pend=NULL, bool advance_past_keyw=true);

const char *SkipSpace( const char *pc );
const char *SkipSpaceRev( const char *pc );
const char *SkipToSpace( const char *pc );

// Accept token at str, consume whitespace before and after
const char *StrAccept( const char* str, const char *token );

// Get word before pointer, NOTE!: 0-terminates
char *GetWordRev( char *pc );

// Get word after pointer, NOTE!: 0-terminates
char *GetWordFwd( char *pc );


// As strpbrk but reversed
const char *strrpbrk( const char *str, const char *seps );


// Bounded version of strchr, strstr, stristr 

// Look for ch between str and pend (bounded)
const char *strchr_b( const char *str, char ch, const char *pend );

// Look for ps between str and pend (bounded)
const char *strstr_b( const char *str, const char *ps, const char *pend );

// Look for ps between str and pend (bounded)
const char *stristr_b( const char *str, const char *ps, const char *pend );


// Search for an unescaped version of the character
const char *strchr_noesc( const char *str, const char ch, const char *pend=NULL );

// Search for an unqoted version of the character
const char *strchr_nostr( const char *str, char ch, const char *pend=NULL );

// Search for an unqoted version of the string
const char *strstr_nostr( const char *str, const char *substr, const char *pend=NULL );

// Search for an unqoted version of the string (case insensitive)
const char *stristr_nostr( const char *str, const char *substr, const char *pend=NULL );

#endif // STRUTILS_H

