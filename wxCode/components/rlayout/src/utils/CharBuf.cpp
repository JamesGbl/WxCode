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
#include <stdio.h>

#include "utils/CharBuf.h"


// Debug macros
#ifdef CHARBUF_DEBUG
	#define CB_DEBUG_SET_LENGTH(len) m_len=len
	#define CB_DEBUG_CHECK_LENGTH ChkLen(m_str,m_len)
#else
	#define CB_DEBUG_SET_LENGTH(len) 
	#define CB_DEBUG_CHECK_LENGTH
#endif

#if !defined(__WINDOWS__) && !defined(_WIN32) && !defined(__WIN32__)
	#define stricmp   strcasecmp
	#define strnicmp  strncasecmp
#endif


// To trigger break on error under GDB - mingw
void CBDebugHook( int len, int test ){
	printf("CBDebugHook: %d, %d\n", len, test);
#ifdef CHARBUF_DEBUG
	// Generate seg fault
	*(int*)0 = 0;
#endif
	int z=3;
	//scanf("%d",&z);
}

// Check that the trailing -1 is in the right place
void ChkLen( char *pc, int l ){
	if( !pc ) return;
	char *pc1 = (char*)memchr(pc,-1,CB_MAX_RESERVE);
	if( pc1 && pc1-pc!=l )
		CBDebugHook(l,pc1-pc);
}



CharBuf::CharBuf( bool dummy, const char *s1, const char *s2, int reserve )  : m_str(NULL) {
	dummy &= false;
	int l1 = s1 ? strlen(s1) : 0;
	int l2 = s2 ? strlen(s2) : 0;
	if( !Init( s1?s1:"", l1, l2+reserve ) )
		// # Alloc failure
		return;
	// Concat, we don't disturb trailing -1
	memcpy( m_str+l1, s2, l2+1 );
}


bool CharBuf::Init( const char *str, int len, int reserve ) {
   CB_DEBUG_SET_LENGTH(0);
   if( m_str ){ delete [] m_str; m_str = NULL; }
   if ( reserve>CB_MAX_RESERVE ) return false;
   if( !str ) return true;
   if( len==-1 ) len = strlen(str);
   
   // New alloc size: strlen + 1 + reserve + 1
   m_str = new char[len+1+reserve+1];
   CB_DEBUG_SET_LENGTH( len+1+reserve );
   if ( !m_str ) return false;
   memcpy( m_str, str, len );			// Copy string
   memset( m_str+len, 0, reserve+1 );	// Fill up with zeros
   m_str[len+1+reserve] = (char)-1;	    // Set trailing -1
   CB_DEBUG_CHECK_LENGTH;

   return true;
}

int CharBuf::Length(){
	return m_str ? strlen(m_str) : 0;
}

bool CharBuf::ResizeInternal( int len, int sl ){
	if( sl==-1 ) sl= m_str?strlen(m_str):0;
	if( len<sl ) return true;
	
	int bl = 0;
	if( m_str ){
		// Make sure we can take away previous trailing -1, after resizing, should not be there
		const char *pc = (const char*)memchr(m_str+sl,-1,CB_MAX_RESERVE);
		if( !pc ) CBDebugHook(-1,-1);
		bl = pc-m_str;
	}
	if( len<bl ) return true;
	
    // So we have to realloc
    char *n_str = new char[len+1+1];
    if ( !n_str ) return false;
   
    CB_DEBUG_SET_LENGTH( len+1 );
    memcpy( n_str, m_str, sl );	   // Copy string
    memset( n_str+sl, 0, len-sl+1 );
    n_str[len+1] = (char)-1;	   // Set trailing -1
    char *o_str = m_str;
    m_str = n_str;
    delete [] o_str;
    CB_DEBUG_CHECK_LENGTH;
	   
	return true;
}

bool CharBuf::operator == ( const char *str ) {
	if( !str || !m_str )
		return m_str==str;
	
	return !strcmp(str,m_str);
}

bool CharBuf::operator < ( const char *str ) {
	if( !str || !m_str )
		return !m_str ? (str?true:false) : false;
	
	return strcmp(m_str,str)<0;
}

bool CharBuf::eqi( const char *str ) {
	if( !str || !m_str )
		return (!str || !*str) && (!m_str || !*m_str);
	
	return !stricmp(m_str,str);
}



bool CharBuf::operator = ( const char *str ) {
   if ( !str ) {
      // We accept the NULL case
      delete [] m_str;
      m_str = NULL;
      return true;
   }
   return Assign( str, -1 );
}

CharBuf& CharBuf::Reset(){
    if( m_str ){
        int sl = strlen(m_str);
        // Overwrite with zeros in case there is -1 inside
        memset( m_str, 0, sl );
    }
    return *this;
}

// str not necessarily 0 terminated
bool CharBuf::Assign( const char *str, int sl_in ) {
   if( !str ) return false;
   int sl = sl_in>=0 ? sl_in : strlen(str);	// Wanted length
   int bl = m_str ? strlen(m_str) : 0;      // Current length
   if ( m_str ){
	   if( sl<=bl ) {
		  // If a lot of unused space, realloc
		  if( sl_in>=0 && bl>sl*2 && bl-sl>16 )
			  return Init( str, sl, CB_CAT_RESERVE );
		  
	      // New string is shorter (or equal), pad with zeros after it
	      memcpy( m_str, str, sl );
	      memset( m_str+sl,0,bl-sl );
	      return true;
	   }

	   // We might have place for it?
      char *pv = (char*)memchr( m_str+bl+1, -1, CB_MAX_RESERVE );
      if ( !pv ){
         CBDebugHook(-1,-1);
         return false;
      }
      bl = pv - m_str;
#ifdef CHARBUF_DEBUG
      if( bl!=m_len )
         CBDebugHook(bl,m_len);
#endif
	  // If a lot of unused space, realloc
	  if( sl_in>=0 && bl>sl*2 && bl-sl>16 )
		  return Init( str, sl, CB_CAT_RESERVE );
      if ( bl>sl ) {
         memcpy( m_str, str, sl );
         memset( m_str+sl,0,bl-sl );
         return true;
      }
   }

   // So we have to realloc
   char *n_str = new char[sl+1+1];
   if ( !n_str ) return false;
   
   CB_DEBUG_SET_LENGTH( sl+1 );
   memcpy( n_str, str, sl );	   // Copy string
   n_str[sl] = 0;				   // 0 terminate
   n_str[sl+1] = (char)-1;		   // Set trailing -1
   char *o_str = m_str;
   m_str = n_str;
   delete [] o_str;
   CB_DEBUG_CHECK_LENGTH;

   return true;
}

bool CharBuf::Append(const char *str, int len) {
   if ( !str || !*str ) return true;
   if( len==-1 ) len=str?strlen(str):0;
   CB_DEBUG_CHECK_LENGTH;

   // Find out how much reserved space we have
   int bl = m_str ? strlen(m_str) : 0;
   
   // Make sure we have space
   if( !ResizeInternal(bl+len+CB_CAT_RESERVE,bl) ) return false;

   // Write at tail of buffer
   strncpy( m_str+bl, str, len );
   m_str[bl+len] = 0;
   
   CB_DEBUG_CHECK_LENGTH;
   return true;
}


/*
bool CharBuf::operator += (const char *str ) {
   if ( !str ) return true;
   int sl = strlen(str);
   if ( !sl ) return true;
   CB_DEBUG_CHECK_LENGTH;

   // Find out how much reserved space we have
   int bl = m_str ? strlen(m_str) : 0;
   if ( m_str ) {
      char *pv = (char*)memchr( m_str+bl+1, -1, CB_MAX_RESERVE );
      if ( !pv ) return false;
      int res = pv - (m_str+bl);	// Number of free bytes
#ifdef CHARBUF_DEBUG
      if( res+bl!=m_len )
      	CBDebugHook(m_len,res+bl);
#endif
      if ( res>sl ) {
         memcpy( m_str+bl, str, sl+1 );
         return true;
      }
   }

   // So we have to realloc
   char *nstr = new char [bl+sl+2+CB_CAT_RESERVE];
   CB_DEBUG_SET_LENGTH( bl+sl+1+CB_CAT_RESERVE );
   if ( !nstr ) return false;
   memcpy( nstr, m_str, bl );
   memcpy( nstr+bl, str, sl );
   memset( nstr+bl+sl, 0, CB_CAT_RESERVE+1 );
   nstr[bl+sl+1+CB_CAT_RESERVE] = -1;
   delete [] m_str;
   m_str = nstr;
   
   CB_DEBUG_CHECK_LENGTH;
   return true;
}
*/

CharBuf &CharBuf::MoveFrom(CharBuf &cb){
	CB_DEBUG_CHECK_LENGTH;
	// Move the contents of cb to us
	delete [] m_str;
	m_str = cb.m_str;
	cb.m_str = NULL;
	
	CB_DEBUG_CHECK_LENGTH;
	return *this;
}


bool CharBuf::SaveToFile( const char *path ){
    if( !path ) return false;
    FILE *pf = fopen( path, "w" );
    if( !pf ) return false;
    int rv = fwrite( m_str, 1, m_str?strlen(m_str):0, pf );
    fclose( pf );
    return rv!=0;
}

bool CharBuf::LoadFromFile( const char *path ){
    if( !path ) return false;
    FILE *pf = fopen( path, "r" );
    if( !pf ) return false;
    fseek( pf, 0, SEEK_END );
    int sz = ftell( pf );
    fseek( pf, 0, SEEK_SET );
    Resize( sz );
    int rv = fread( m_str, 1, sz, pf );
    fclose( pf );
    return rv!=0;
}

void CharBuf::Subst( char ch_find, char ch_repl ){
    if( !m_str ) return;
    for( char *str=m_str; *str; str++ )
        if( *str==ch_find )
            *str = ch_repl;
}


CharBuf operator + (const CharBuf& s1, const char *ps2){
	return CharBuf(true, s1,ps2);
}

CharBuf operator + (const char *ps1, const CharBuf& s2){
	return CharBuf(true, ps1,s2);
}

