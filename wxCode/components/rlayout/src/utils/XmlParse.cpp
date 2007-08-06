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


//#include <stdlib.h>
//#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "utils/CharBuf.h"
#include "utils/StrUtils.h"

#include "utils/XmlParse.h"

bool xpIsLeadTagChar( char ch ){
	return isalpha(ch) || ch==':' || ch=='_';
}

bool xpIsTagChar( char ch ){
	return isalnum(ch) || ch=='_' ||  ch=='-' || ch==':';
}


const char *GetAttrib( const char *xml, const char *name, CharBuf& val ){
    if( !xml || !name ) return NULL;
    xml = SkipSpace(xml);
    // Accept name
    int nl = strlen(name);
    if( strncmp(xml,name,nl) ) return NULL;
    xml += nl;
    xml = SkipSpace(xml);
    // Equals?
    if( *xml!='=' ) return NULL;
    xml++;
    xml = SkipSpace(xml);
    
    // Quoted value, go past escaped quotes
    if( *xml!='"' ) return NULL;
    const char *pb = ++xml;
    for( char ch=*xml; ch!='\"'; ch=*++xml ){
        if( !ch ) return NULL;
        if( ch=='\\' ){
            ch = *++xml;
            if( !ch ) return NULL;
        }
    }
    // Assign value
    val.Assign( pb, xml-pb );
    
    // Skip space
    xml = SkipSpace(xml+1);
    
    // Return new position
    return xml;
}

bool FindAttrib( const char *xml, const char *name, CharBuf& val, const char *pend ){
    if( !xml || !name ) return false;
    const char *pc = strstr_nostr( xml, name );
    if( !pc || pc>=pend ) return false;
    // # Again, what are valid attribute name chars?
    if( pc>xml && (isalnum(pc[-1]) || pc[-1]=='_') ) return false;
    return GetAttrib( pc, name, val ) ? true : false;
}


const char *GetAttrib( const char *xml, const char *name, int &i ){
    CharBuf cb;
    xml = GetAttrib( xml, name, cb );
    if( !xml ) return NULL;
    if( cb[0]=='0' && tolower(cb[1])=='x' )
    	sscanf( cb, "%x", &i );
    else
    	sscanf( cb.Str(), "%d", &i );
    return xml;
}

bool FindAttrib( const char *xml, const char *name, int &i, const char *pend ){
    CharBuf cb;
    if( !FindAttrib( xml, name, cb, pend ) ) return false;
    if( cb[0]=='0' && tolower(cb[1])=='x' )
    	sscanf( cb, "%x", &i );
    else
    	sscanf( cb.Str(), "%d", &i );
    return true;
}


const char *SkipAttrib( const char *xml ){
    if( !xml ) return NULL;
    xml = SkipSpace(xml);
    
    // Accept name - not sure about this, accept numbers/digits/_ now
    for( char ch=*xml; ch!=' ' && ch!='='; xml++ ){
    	if( !ch ) return NULL;
    	if( !isalnum(ch) && ch!='_' && ch!='-' ) return NULL;
    }
    
    xml = SkipSpace(xml);
    if( *xml!='=' ) return NULL;
    xml = SkipSpace(xml+1);
    
    // Quoted value, go past escaped quotes
    if( *xml!='"' ) return NULL;
    for( char ch=*xml; ch!='\"'; ch=*++xml ){
        if( !ch ) return NULL;
        if( ch=='\\' ){
            ch = *++xml;
            if( !ch ) return NULL;
        }
    }
    
    // Skip space
    xml = SkipSpace(xml+1);
    
    // Return new position
    return xml;
}


bool IsSet( const char *pc ){
	if( !pc ) return false;
	if( !strcmp(pc,"1") || !stricmp(pc,"on") || 
		!stricmp(pc,"yes") || !stricmp(pc,"set") )
		return true;
	else
		return false;
}


int xpGetType( const char *xml ){
	if( !xml ) return -1;
	xml = SkipSpace(xml);
	if( *xml=='<' ){
		xml = SkipSpace(xml+1);
		if( *xml=='!' ){
			xml = SkipSpace(xml+1);
			if( !strncmp(xml,"--",2) ) 
				return XP_COMMENT;
			if( *xml=='[' )
				return XP_CDATA;
			return -2;
		}
		bool is_close = false;
		if( *xml=='/' ){
			is_close=true;
			xml = SkipSpace(xml+1);
		}
		if( !xpIsLeadTagChar(*xml) )
			return XP_NODE_BAD;
		const char *pc = strchr(xml,'>');
		if( pc ) pc = SkipSpaceRev(pc-1);
		
		if( *pc=='/' ) 
			return XP_NODE_BOTH;
		else 
			return is_close ? XP_NODE_CLOSE : XP_NODE_OPEN;
	}
	// Default to text
	return XP_TEXT;
}



int ReadEntityText( const char *pc, const char *pc1, CharBuf &cb ){
	if( !pc ) return -1;
	if( !pc1 ) pc1=pc+strlen(pc);
	cb.Reset( );
	cb.Resize( pc1-pc );
	
	const char *ent;
	char ebuf[4];
	while( pc<pc1 ){
		ent = strchr(pc,'&');
		if( !ent ) break;
		cb.Append( pc,ent-pc );
		ent = SkipSpace(ent+1);
		if( !strncmp(ent,"lt",2) ) *ebuf='<', ent+=2; 
		else if( !strncmp(ent,"gt",2) ) *ebuf='>', ent+=2; 
		else if( !strncmp(ent,"amp",3) ) *ebuf='&', ent+=3; 
		else if( !strncmp(ent,"quot",4) ) *ebuf='\"', ent+=4; 
		else if( !strncmp(ent,"apos",4) ) *ebuf='\'', ent+=4; 
		else return -2;
		ent = SkipSpace(ent);
		if( *ent!=';' ) return -3;
		if( ent>=pc1 ) return -4;
		cb.Append( ebuf, 1 );
		pc = ent+1;
	}
	cb.Append( pc, pc1-pc );
	return 0;
}


int AcceptOpenTag( const char *&xml, CharBuf& tag ){
	if( !xml ) return -1;
	const char *pc = SkipSpace(xml);
	if( *pc!='<' ) return -1;
	pc = SkipSpace(pc+1);
	
	bool is_close = false;
	if( *pc=='/' ){
		is_close = true;
		pc = SkipSpace(pc+1);
	}
	if( !xpIsLeadTagChar(*pc) ) return -2;
	
	xml = pc;
	while( isalnum(*pc) ) pc++;
	tag.Assign( xml, pc );
	xml = SkipSpace( pc );
	
	return is_close ? 1 : 0;
}

int AcceptOpenTag( const char *&xml, const char *expect ){
	if( !xml || !xml ) return -1;
	const char *pc = SkipSpace(xml);
	if( *pc!='<' ) return -1;
	pc = SkipSpace(pc+1);
	
	bool is_close = false;
	if( *pc=='/' ){
		is_close = true;
		pc = SkipSpace(xml);
	}
	if( !xpIsLeadTagChar(*pc) ) return -2;
	
	int sl = strlen(expect);
	if( strncmp(expect,pc,sl) || isalnum(pc[sl]) ) return -3;
	xml = SkipSpace( pc+sl );
	
	return is_close ? 1 : 0;
}

int AcceptAttrib( const char *&xml, CharBuf& name, CharBuf& val, const char *expect ){
    if( !xml ) return -1;
	const char *pc = SkipSpace(xml), *pc1;
    
    // Accept name
	if( expect && !keywordeq(pc,expect) ) return 1;
	for( pc1=pc; isalnum(*pc1); pc1++ );
	if( pc1==pc ) return -1;
	name.Assign( pc,pc1 );
	pc = SkipSpace(pc1);

    // Equals?
    if( *pc!='=' ) return -2;
    pc = SkipSpace(pc+1);
    
    // Quoted value, go past escaped quotes
    if( *pc!='\"' && *pc!='\'' ) return -3;
    pc1 = strchr(pc+1,*pc);
    if( !pc1 ) return -4;
    
    // Assign value
    if( ReadEntityText(pc+1,pc1,val) )
    	return -5;
    
    // Skip space
    xml = SkipSpace(pc1+1);
    
    return 0;
}

int AcceptAttrib( const char *&xml, CharBuf& name, int &val, const char *expect ){
	CharBuf cb_val;
	const char *pc = xml;
	int r = AcceptAttrib( pc, name, cb_val, expect );
	if( r ) return r;
	if( cb_val.IsEmpty() || !isdigit(cb_val[0]) ) return 2;
	if( !strnicmp(cb_val,"0x",2) )
		sscanf( cb_val.Str()+2, "%x", &val );
	else
		sscanf( cb_val, "%d", &val );
	xml = pc;
	return 0;
}

int AcceptCloseToken( const char *&xml ){
	if( !xml ) return -1;
	const char *pc = SkipSpace(xml);
	bool node_close = false;
	if( *pc=='/' ) node_close=true, pc++;
	if( *pc!='>' ) return -2;
	xml = pc+1;
	return node_close ? 1 : 0;
}


int AcceptText( const char *&xml, CharBuf &text, bool no_copy ){
	if( !xml ) return -1;
	if( *xml=='<' ) return 0;
	
	const char *pc = xml;
	const char *pc1 = strchr(pc,'<');
	if( !pc1 ) return -2;
	if( !no_copy && ReadEntityText( pc, pc1, text ) )
		return -3;
	xml = pc1;
	
	return 0;
}

int AcceptComment( const char *&xml, CharBuf *pcomment ){
	if( !xml ) return -1;
	const char *pc = SkipSpace(xml);
	if( *pc!='<' ) return -1;
	pc = SkipSpace( pc+1 );
	if( *pc!='!' ) return -2;
	pc = SkipSpace( pc+1 );
	if( strncmp(pc,"--",2) ) return -3;
	pc += 2;
	// Comment starts here
	
	const char *pc1=pc, *pend;
	while( pc1=strstr(pc1,"--") ){
		pend = SkipSpace(pc1+2);
		if( *pend=='>' ){
			if( pcomment )
				pcomment->Assign( pc, pc1 );
			return 0;
		}
		pc1 = pend;
	}
	
	return -4; 
}

int SkipPastCloseToken( const char *&xml ){
	int r;
	// Skip past attribs
	CharBuf name, val;
	while( (r=AcceptCloseToken(xml))<0 ){
		if( AcceptAttrib(xml,name,val) )
			return -1;
	}
	return r;
}


int SkipPastCloseNode( const char *&xml, const char *tag ){
	CharBuf sub_tag;
	int r, r1;
	while( true ){
		r = AcceptText( xml, sub_tag, true );
		r = AcceptOpenTag( xml, sub_tag );
		r1 = SkipPastCloseToken(xml);
		if( sub_tag==tag )
			return r==1 && r1>=0 ? 0 : (r?-11:-20);
		if( r || r1<0 ) return r<0 || r1<0 ? -12 : -21;
		// We have an open tag to be closed. Call recursively
		r = SkipPastCloseNode(xml,sub_tag);
		if( r ) return r;
	}
}

