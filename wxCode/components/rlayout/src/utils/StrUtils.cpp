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


#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "utils/StrUtils.h"

bool keywordieq(const char *str, const char *kw, int kwl){
	if( !str || !kw ) return false;
	if( kwl==-1 ) kwl=strlen(kw);
	str = SkipSpace(str);
	int r = strnicmp(str,kw,kwl);
	if( r ) return false;
	return !isalnum(str[kwl]); //!str[kwl] || isspace(str[kwl]);
}

bool keywordeq(const char *str, const char *kw, int kwl){
	if( !str || !kw ) return false;
	if( kwl==-1 ) kwl=strlen(kw);
	str = SkipSpace(str);
	int r = strncmp(str,kw,kwl);
	if( r ) return false;
	return !isalnum(str[kwl]); //!str[kwl] || isspace(str[kwl]);
}

/*
const char *keywordifind(const char *str, const char *kw, const char *pend){
	if( !str || !kw ) return NULL;
	int kwl = strlen(kw);
	int sl = strlen(str);
	if( pend && str+sl>pend ) sl=pend-str;
	const char *str_in = str;
	while(kwl<=sl && strnicmp(str,kw,kwl))
		str++, sl--;
	if( sl<kwl ) return NULL;
	if( str>str_in && isalnum(str[-1]) ) return NULL;
	// Change: return pointer to character directly after keyword
	return isalnum(str[kwl]) ? NULL : str+kwl; //!str[kwl] || isspace(str[kwl]);
	//return isalnum(str[kwl]) ? NULL : str; //!str[kwl] || isspace(str[kwl]);
}
*/

const char *keywordifind(const char *str, const char *kw, const char *pend, bool advance_past_keyword) {
	if( !str || !kw ) return NULL;
	const char *str_in = str;
	str = stristr_nostr(str,kw,pend);
	if( !str ) return NULL;
	if( str>str_in && isalnum(str[-1]) ) return NULL;
	// Change: return pointer to character directly after keyword
	int kwl = strlen(kw);
	return isalnum(str[kwl]) ? NULL : str+(advance_past_keyword?kwl:0); 
}

const char *keywordfind(const char *str, const char *kw, const char *pend, bool advance_past_keyword) {
	if( !str || !kw ) return NULL;
	const char *str_in = str;
	str = strstr_nostr(str,kw,pend);
	if( !str ) return NULL;
	if( str>str_in && isalnum(str[-1]) ) return NULL;
	// Change: return pointer to character directly after keyword
	int kwl = strlen(kw);
	return isalnum(str[kwl]) ? NULL : str+(advance_past_keyword?kwl:0); 
}


const char *SkipSpace( const char *pc ){
	if( !pc ) return NULL;
	while( isspace(*pc) ) pc++;
	return pc;
}

const char *SkipToSpace( const char *pc ){
	if( !pc ) return NULL;
	while( *pc && !isspace(*pc) ) pc++;
	return pc;
}

const char *SkipSpaceRev( const char *pc ){
	if( !pc ) return NULL;
	while( isspace(pc[-1]) ) pc--;
	return pc;
}

// Get word before pointer, 0-terminate
char *GetWordRev( char *pc ){
	// Skip trailing space
	for( ;isspace(*pc); pc-- ); pc++;
	char *pc1=pc-1;
	for( ; !isspace(*pc1); pc1-- );
	*pc = 0;
	return pc1+1;
}

// Get word after pointer, 0-terminate
char *GetWordFwd( char *pc ){
	// Skip trailing space
	pc = (char*)SkipSpace(pc);
	char *pc1 = (char*)SkipToSpace(pc);
	*pc1 = 0;
	return pc;
}

const char *StrAccept( const char* str, const char *token ){
	if( !str || !token ) return NULL;
	str = SkipSpace(str);
	int tl = strlen(token);
	if( strncmp(str,token,tl) ) return NULL;
	str += tl;
	if( isalnum(*str) ) return NULL;
	return SkipSpace(str);
}


const char *strrpbrk( const char *str, const char *seps ){
	if( !str || !seps ) return NULL;
	const char *pc1 = NULL;
	while( *seps ){
		const char *pc = strrchr(str,*seps);
		if( pc>pc1 ) pc1 = pc;
		seps++;
	}
	return pc1;
}

const char *stristr(const char *sbig, const char *ssmall){
    if( !sbig || !ssmall ) return NULL;
	int kwl = strlen(ssmall);
	int sl = strlen(sbig);
	while(kwl<=sl && strnicmp(sbig,ssmall,kwl))
		sbig++, sl--;
	return sl>=kwl ? sbig : NULL;
}


const char *Int2Str( int i, const char *fmt ){
	static char sbuf[32];
	sprintf(sbuf,fmt?fmt:"%d",i);
	return sbuf;
}

const char *TruncStr( const char *str, int max_len ){
	if( !str ) return NULL;
	static char sbuf[256];
	int sl = strlen(str);
	if( max_len>255 ) max_len=255; 
	strncpy(sbuf,str,max_len<sl ? max_len : sl);
	return sbuf;
}



const char *strchr_b( const char *str, char ch, const char *pend ){
	if( !str ) return NULL;
	if( !pend ) return strchr(str,ch);
	while( str<pend ){
		if( !*str ) return NULL;
		if( *str==ch ) return str;
		str++;
	}
	return NULL;
}

const char *strstr_b( const char *str, const char *ps, const char *pend ){
	if( !str || !ps ) return NULL;
	if( !pend ) return strstr(str,ps);
	int sl = strlen(ps);
	while( str+sl<=pend ){
		// # This is not obvious
		if( !*str ) return NULL;
		if( !strncmp(str,ps,sl) ) return str;
		str++;
	}
	return NULL;
}

const char *stristr_b( const char *str, const char *ps, const char *pend ){
	if( !str || !ps ) return NULL;
	if( !pend ) return stristr(str,ps);
	int sl = strlen(ps);
	while( str+sl<=pend ){
		// # This is not obvious
		if( !*str ) return NULL;
		if( !strnicmp(str,ps,sl) ) return str;
		str++;
	}
	return NULL;
}



// Search forwards for unescaped version of character
const char *strchr_noesc( const char *str, const char ch, const char *pend ){
	if( !str ) return NULL;
	const char *pc = strchr_b(str,ch,pend);
	while( true ){
		if( !pc ) return NULL;
		if( pc==str || pc[-1]!='\\' ) return pc;
		pc = strchr_noesc( pc+1, ch, pend );
	}
}


const char *strchr_nostr( const char *str, char ch, const char *pend ){
	if( !str ) return NULL;
	const char *pc = strchr_b(str,ch,pend);
	if( !pc ) return NULL;
	// See if we're inside a string, search for opening quote
	const char *pq = strchr_noesc( str, '\"', pend );
	while( true ){
		if( !pq || pc<pq ) return pc;
		// Search for closing quote
		pq = strchr_noesc( pq+1, '\"', pend );
		if( !pq ) return NULL;
		if( pc<pq ){
			// Search for new occurence
			pc = strchr_b(pq,ch,pend);
			if( !pc ) return NULL;
		}
		// Search for new opening quote
		pq = strchr_noesc( pq+1, '\"', pend );
	}
}

const char *strstr_nostr( const char *str, const char *substr, const char *pend ){
	if( !str || !substr ) return NULL;
	const char *pc = strstr_b(str,substr,pend);
	if( !pc ) return NULL;
	// See if we're inside a string, search for opening quote
	const char *pq = strchr_noesc( str, '\"', pend );
	while( true ){
		if( !pq || pc<pq ) return pc;
		// Search for closing quote
		pq = strchr_noesc( pq+1, '\"', pend );
		if( !pq ) return NULL;
		if( pc<pq ){
			pc = strstr_b(pq,substr,pend);
			if( !pc ) return NULL;
		}
		// Search for new opening quote
		pq = strchr_noesc( pq+1, '\"', pend );
	}
}

const char *stristr_nostr( const char *str, const char *substr, const char *pend ){
	if( !str || !substr ) return NULL;
	const char *pc = stristr_b(str,substr,pend);
	if( !pc ) return NULL;
	// See if we're inside a string, search for opening quote
	const char *pq = strchr_noesc( str, '\"', pend );
	while( true ){
		if( !pq || pc<pq ) return pc;
		// Search for closing quote
		pq = strchr_noesc( pq+1, '\"', pend );
		if( !pq ) return NULL;
		if( pc<pq ){
			pc = stristr_b(pq,substr,pend);
			if( !pc ) return NULL;
		}
		// Search for new opening quote
		pq = strchr_noesc( pq+1, '\"', pend );
	}
}

