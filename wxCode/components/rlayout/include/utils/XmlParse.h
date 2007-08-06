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


#ifndef XML_PARSE_H
#define XML_PARSE_H

class CharBuf;

// Read given attribute directly at this position (xml)
const char *GetAttrib( const char *xml, const char *name, CharBuf& val );
const char *GetAttrib( const char *xml, const char *name, int &i );

// Search for the given attribute
bool FindAttrib( const char *xml, const char *name, CharBuf& val, const char *pend=NULL );
bool FindAttrib( const char *xml, const char *name, int &i, const char *pend=NULL );

// Go past current attribute
const char *SkipAttrib( const char *xml );

// Return true for '1', 'on', 'yes' and 'set'
bool IsSet( const char *pc );


// Returns 0 on success, >0 requirement not fulfilled, <0 error
#define XP_NODE_OPEN   1
#define XP_NODE_CLOSE  2
#define XP_NODE_BOTH   3
#define XP_TEXT    5
#define XP_COMMENT 6
#define XP_CDATA   7
#define XP_END     8
#define XP_NODE_BAD   -10


int AcceptOpenTag( const char *&xml, CharBuf& tag );
int AcceptOpenTag( const char *&xml, const char *expect );
int AcceptAttrib( const char *&xml, CharBuf& name, CharBuf& val, const char *expect=NULL );
int AcceptAttrib( const char *&xml, CharBuf& name, int &val, const char *expect=NULL );
// Returns 0: closed with >, 1: closed with />, <0 failed 
int AcceptCloseToken( const char *&xml );
int AcceptText( const char *&xml, CharBuf &text, bool no_copy=false );
int AcceptComment( const char *&xml, CharBuf *pcomment=NULL );

// Returns 0: closed with >, 1: closed with />, <0 failed 
int SkipPastCloseToken( const char *&xml );

// Assumption: in text section (between nodes)
int SkipPastCloseNode( const char *&xml, const char *tag );

int xpGetType( const char *xml );

int ReadEntityText( const char *pc, const char *pc1, CharBuf &cb );
bool xpIsTagChar( char ch );
bool xpIsLeadTagChar( char ch );

#endif // XML_PARSE_H

