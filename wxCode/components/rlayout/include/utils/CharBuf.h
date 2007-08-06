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


#ifndef CHARBUF_H
#define CHARBUF_H

#include <string.h>


// This is to stop looking for -1 in case of corrupt buffer
#define CB_MAX_RESERVE	65536

// How many extra bytes to alloc on concatenation
#define CB_CAT_RESERVE	8


// A character buffer that is the same size as sizeof(char*). It releases
// its memory in the destructor. 
// 
// It is zero AND -1 terminated. The -1 marker allows for keeping a part 
// of the buffer reserved when the strings are shorter. Useful when doing
// several small concats to the string, or when it changes size frequently.

class CharBuf {
public:
	CharBuf( const char *str=NULL, int reserve=0 ) : m_str(NULL) {
		Init( str, -1, reserve );
	}
	
	CharBuf( const char *str, const char *end, int reserve=0 ) : m_str(NULL)  {
		if( str )
			Init( str, end?end-str:-1, reserve );
	}
	
	CharBuf( const CharBuf &cb, int reserve=0 ) : m_str(NULL)  {
		Init( cb.m_str, -1, reserve );
	}
	
	// Concatenation
	CharBuf( bool dummy, const char *s1, const char *s2, int reserve=0 );
	
	~CharBuf( ){ Free(); } 
	void Free( ){ 
		if(m_str){ delete [] m_str; m_str=NULL; } 
	}

	bool Assign( const char *str, int len );
	bool Assign( const char *str, const char *pe ){ return Assign( str, pe?pe-str:-1); }
	bool Resize( int len ){ return ResizeInternal(len,-1); }
	bool Append( const char *str, int len=-1 );
	
    void Subst( char ch_find, char ch_repl );
	
	int Length();	// Must do strlen, but good to have
    bool IsEmpty(){ return !m_str || !*m_str; }
    bool HasLength(){ return m_str && *m_str; }

    // Set to empty string
    CharBuf& Reset();
    
	bool operator = ( const char *str );	// False on allocation failure
	bool operator == ( const char *str );	// Ordinary strcmp
	bool operator < ( const char *str );	// Ordinary strcmp, for ordering 
	bool eqi( const char *str );			// Case insensitive comparison
	
	CharBuf &operator = ( const CharBuf &cb ){ Assign(cb.m_str,-1); return *this; }  
	bool operator += (const char *str ){ return Append(str,-1); }	
	char* Get(){ return m_str; }
	char* Str(){ return m_str; }
	operator const char* () const { return m_str; }

    // Take over contents of other CharBuf
	CharBuf& MoveFrom( CharBuf& cb );

    bool SaveToFile( const char *path );
    bool LoadFromFile( const char *path );
		
protected:
	bool Init( const char *str, int len, int reserve );
	bool ResizeInternal( int len_total, int len_now );
	
#ifdef CHARBUF_DEBUG
	int m_len;	// This is just for debugging
#endif
	char *m_str;
};

// Concatenation operators
CharBuf operator + (const CharBuf& s1, const char *ps2);
CharBuf operator + (const char *ps1, const CharBuf& s2);

// Type registration, if typeof.h included before us
#ifdef TYPEOF_H
    #undef TYPE_REG_FILE
    #define TYPE_REG_FILE 101
    TYPE_REG(CharBuf)
#endif


#endif // CHARBUF_H

