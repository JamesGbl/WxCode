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

#ifndef RLMANAGERINT_H
#define RLMANAGERINT_H

#include "wx/RlManager.h"


/* This class remembers XML attributes/sub nodes for an wxRlItem. 
   We use it when constructing from XML data and later on saving
   it when the data cannot be directly retrieved from the object itself. */
class wxRlXmlTags : public wxObject {
public:
	wxRlXmlTags( const char *type=NULL ) : m_quits(false) { 
		if( type ) AddAttrib("type",type);
	}
	void AddAttrib( const char *attr, const char *val ){ 
		m_attribs.Add( MB2WX(attr) );
		m_attribs.Add( MB2WX(val) );
	}
	wxArrayString 	m_attribs;	// Attributes to be remembered until we save to file
	wxString   		m_enables;	// These are the IDs/names that this item enables/disables when clicked
	//wxArrayString   m_shows;	// Same but show/hide rather than enable/disable
	wxString 		m_sub_nodes;
	bool 			m_quits;
	
protected:
	DECLARE_CLASS(wxRlXmlTags);
};


// For deleting wxRlItems safely
void TrashRlItem( wxRlItem *rli, wxRlManager *rlmgr=NULL );
void TrashRlItem( wxSizer *ps, int ix, wxRlManager *rlmgr=NULL );


// Some utility functions
int 	GetOrientation( wxSizer *ps );
int 	GetSizerType( wxSizer *sizer );
bool 	IsRlItem( wxSizerItem *si );
RlUiArea GetEdgeTypeForPart( RlUiArea part );
wxRlItem *GetRootItemOf( wxRlItem *rli );
wxRlItem *ToRlItem( wxSizerItem *psi );
wxSizer  *GetSizerOf( wxRlItem *psi );

void ReparentSizerWindows( wxSizer *ps, wxWindow *wnd_newp );

// Convenience macros
//#define GetRlItemOf(ps) wxSizer2RlItem::Lookup(ps)
#define RlGetNameOf(rli) wxSizer2RlItem::GetName(rli)
#define RlSetNameOf(rli,name) wxSizer2RlItem::SetName(rli,name)
#define RlDumpNames() wxSizer2RlItem::DumpNames( );

void wxRlLog( const wxChar *str );



// This is the type of the object returned as soon as we
// try something invalid when configuring a layout
class wxRlNullClass : public wxRlItem{
    public:
        wxRlNullClass() : wxRlItem(0,0, 0,0, RlNull) { }
        virtual const wxChar *GetError( ){ return m_msg; }
        wxRlItem &Log( const wxChar *msg ){ m_msg = msg; wxRlLog(msg); return *this; }

    protected:
        wxString m_msg;
};


//
// Class used to lookup wxSizer to wxRlItem (and also from name)
// 
class wxSizer2RlItem {
public:
	static void Push( wxSizer *psz, wxRlItem *rli, const wxChar *name );
	static void Remove( wxRlItem *rli );
	static wxRlItem *Lookup( const wxChar *name );
	static void SetName( wxRlItem *rli, const wxChar *name );
	static const wxChar *GetName( wxRlItem *rli );
	static void DumpNames( );
	//static void Remove( wxSizer *psz );
	//static wxRlItem *Lookup( wxSizer *psz );
	//static bool SetName( wxSizer *psz, const wxChar *name );
	//static const wxChar *GetName( wxSizer *psz );
	
	wxSizer2RlItem( wxSizer *psz, wxRlItem *rli, const wxChar *name ) : 
		m_psz(psz), m_rli(rli), m_name(name) { }
	//bool operator == (wxSizer *psz){ return m_psz==psz; }
	bool operator == (wxRlItem *rli){ return m_rli==rli; }
	bool operator == (const wxChar *name){ return m_name==name; }
    
protected:    
	wxSizer  *m_psz;
	wxRlItem *m_rli;
	wxString m_name;
};


struct wxRlDragPaneInfo {
	wxRlDragPaneInfo( ){ m_wnd_drag=NULL; } 
public:
	wxWindow *m_wnd_drag;
	wxPoint m_offset;
	wxPoint m_mouse_last_insert;
};

struct wxRlDockInfo {
public:
	wxRlDockInfo( const wxChar *address, float rel_pos_x, float rel_pos_y ) 
		: m_address(address), m_rel_pos_x(rel_pos_x), m_rel_pos_y(rel_pos_y) { }
	//wxRlDockInfo( const wxRlDockInfo& di )
	//	: m_address(di.m_address), m_rel_pos_x(di.m_rel_pos_x), m_rel_pos_y(di.m_rel_pos_y) { }
		
	wxString m_address;
	float m_rel_pos_x;
	float m_rel_pos_y;
};

#endif // RLMANAGERINT_H


