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


#include <wx/timer.h>
#include <wx/minifram.h>

#include "wx/RlManager.h"
#include "wx/RlManagerInt.h"
#include "wx/RlPainter.h"
#include "wx/SizerUtils.h"


IMPLEMENT_CLASS(wxRlManager,wxEvtHandler)
IMPLEMENT_CLASS(wxRlItem,wxSizerItem)
IMPLEMENT_CLASS(wxRlXmlTags,wxObject)
IMPLEMENT_CLASS(wxRlCaptionSi,wxRlItem)
IMPLEMENT_CLASS(wxRlAppPane,wxRlItem)
IMPLEMENT_CLASS(wxRlToolPane,wxRlItem)
IMPLEMENT_CLASS(wxRlCaptionText,wxRlItem)
IMPLEMENT_CLASS(wxRlText,wxRlItem)
IMPLEMENT_CLASS(wxRlClientBgHint,wxRlItem)
IMPLEMENT_CLASS(wxRlButton,wxRlItem)

DEFINE_EVENT_TYPE(wxEVT_RL_PANE_ACTIVATE)
DEFINE_EVENT_TYPE(wxEVT_RL_MOUSE)


// All existing managers here
ExpArrP<wxRlManager*> g_rl_mgrs;
// Cache the last active one
wxRlManager *g_rl_mgr_last_active;	

// The painter (shared by all)
RlPainter *wxRlManager::ms_painter;
RlStubPainter g_stub_painter;
//RlWinStdPainter g_stub_painter;	// # Nope, GTK will not have us construct GUI objects in global ctor phase

// We have to wait with deleteing wxRlItems until idle time
ExpArr<wxRlItem*> g_rl_garbage;


bool GetSuppressLog( ){
	// It is really annoying to have GUI dialogs showing up while dragging an
	// edge or moving a window. This detects if the GUI log is installed and
	// returns false then. 
#ifndef RL_NO_DYNAMIC_CAST	// If compiler has no dynamic cast, define this.
	return dynamic_cast<wxLogGui*>(wxLog::GetActiveTarget()) != NULL;
#else
	return false;
#endif
}


struct RlEnum2Str{
public:
    int   num;
    const wxChar *str;
    short capab;
};

RlEnum2Str g_rl_e2t [] = {
    {RlNoPart, _T("RlNoPart"), 0},

    {RlPaneEdge , _T("RlPaneEdge"), 0},
    {RlMediumEdge, _T("RlMediumEdge"), 0},
    {RlLayoutEdge, _T("RlLayoutEdge"), RL_CAPAB_REPAINT_ON_ENTER_LEAVE},
    {RlThinEdge, _T("RlThinEdge"), 0},
    {RlCustomEdge1, _T("RlCustomEdge1"), 0},
    {RlCustomEdge2, _T("RlCustomEdge2"), 0},

    {RlFrameBg, _T("RlFrameBg"), 0},
    {RlClientBg, _T("RlClientBg"), RL_CAPAB_MOUSE_WXEVENT},
    {RlClientBgHint, _T("RlClientBgHint"), RL_CAPAB_MOUSE_WXEVENT},
    {RlClientTempBg, _T("RlClientTempBg"), RL_CAPAB_TEMP_BG},
    {RlPaneMoveHint, _T("RlPaneMoveHint"), 0},

    {RlButton, _T("RlButton"), RL_CAPAB_MOUSE_WXEVENT},

    {RlSpacer, _T("RlSpacer"), 0},
    {RlSpacerTemp, _T("RlSpacerTemp"), RL_CAPAB_TEMP_BG},
    {RlSpacerEnum, _T("RlSpacerEnum"), 0},
    {RlSpacerUnknown, _T("RlSpacerUnknown"), 0},

    {RlCaptionText, _T("RlCaptionText"), RL_CAPAB_GRIPPER | RL_CAPAB_MOUSE_REPORT},
    {RlToolGripper, _T("RlToolGripper"), RL_CAPAB_GRIPPER},
    {RlTabItem, _T("RlTabItem"), 0},

    {RlWindow, _T("RlWindow"), 0},
    {RlToolWindow, _T("RlToolWindow"), 0},

    {RlSizerUnk, 	_T("RlSizerUnk"), 	RL_CAPAB_MOUSE_PROPAGATE},
    {RlSizer, 		_T("RlSizer"), 		RL_CAPAB_MOUSE_PROPAGATE},
    {RlToolbarDock, _T("RlToolbarDock"), 0},
    {RlUserArea, 	_T("RlUserArea"), 0},

//    {RlCaptionButtons , _T("RlCaptionButtons "), 0},
    {RlCaption, _T("RlCaption"), RL_CAPAB_MOUSE_WXEVENT | RL_CAPAB_MOUSE_PROPAGATE},
    {RlAppPane, _T("RlAppPane"), RL_CAPAB_ENTITY/*|RL_CAPAB_MOUSE_PROPAGATE*/},
    {RlPaneContent, _T("RlPaneContent"), RL_CAPAB_ENTITY|RL_CAPAB_FIX|RL_CAPAB_MOUSE_WXEVENT},
    {RlToolPane, _T("RlToolPane"), RL_CAPAB_ENTITY},
    {RlTabBar, 	_T("RlTabBar"), 0},
    {RlNoteBook, _T("RlNoteBook"), 0},

    {RlNull, _T("RlNull"), 0}
};



bool wxRlItem::IsEnumerable( int type ){
	// This could just be a capab flag
    switch( type ){
        case RlSizer:
        case RlToolbarDock:
        case RlSizerUnk:
        case RlStaticBoxSizer:

        case RlAppPane:
        case RlToolPane:

        case RlSpacerEnum:
        	
        case RlFrameBg:
        case RlClientBg:
        case RlClientBgHint:
        case RlClientTempBg:
        case RlPaneMoveHint:
        case RlText:

        case RlWindow:
        // case RlUserArea:
            return true;

        default:
            return false;
    }
}

bool wxRlItem::IsUserSizer( ){
    switch( GetType() ){
        case RlSizer:
        case RlSizerUnk:
        case RlToolPane:
        case RlStaticBoxSizer:
        // case RlUserArea:
            return true;

        default:
            return false;
    }
}


RlUiArea GetEdgeTypeForPart( RlUiArea part ){
	// # Also a capab?
    if( part==RlSizer || part==RlAppPane )
        return RlPaneEdge;
    else if( part==RlWindow || part==RlUserArea ||
             part==RlClientBg || part==RlClientBgHint || RlPaneMoveHint )
        return RlMediumEdge;
    else if( part==RlToolbarDock )
        return RlThinEdge;
    else if( part==RlToolPane )
        return RlNoPart;
    return RlNoPart;
}



// This is array that maps wxSizer to wxSizerItem
ExpArrP<wxSizer2RlItem*> g_sz2rli;

void wxSizer2RlItem::Push( wxSizer *psz, wxRlItem *rli, const wxChar *name ){
	//Remove(psz);
	Remove(rli);
	g_sz2rli.Push(new wxSizer2RlItem(psz,rli,name));
}

/*
void wxSizer2RlItem::Remove( wxSizer *psz ){
	int ix = g_sz2rli.FindIndexOf(psz);
	if( ix>=0 ) 
		delete g_sz2rli.RemoveIndexUnordered(ix);
}
*/

void wxSizer2RlItem::Remove( wxRlItem *rli ){
	int ix = g_sz2rli.FindIndexOf(rli);
	if( ix>=0 ) 
		delete g_sz2rli.RemoveIndexUnordered(ix);
}

/*
wxRlItem *wxSizer2RlItem::Lookup( wxSizer *psz ){
	if( !psz ) return NULL;
	
	// First try looking at child item and using parent pointer
	wxRlItem *rli = ToRlItem(psz->GetItem((size_t)0));
	if( rli && rli->GetParent() ) 
		return rli->GetParent();
	
	// Iterate through all sizers
	int ix = g_sz2rli.FindIndexOf(psz);
	if( ix<0 ) return NULL;
	return g_sz2rli.ElemSafe(ix)->m_rli;
}
*/

wxRlItem *wxSizer2RlItem::Lookup( const wxChar *name ){
	int ix = g_sz2rli.FindIndexOf(name);
	if( ix<0 ) return NULL;
	return g_sz2rli.ElemSafe(ix)->m_rli;
}

/*
bool wxSizer2RlItem::SetName( wxSizer *psz, const wxChar *name ){
	int ix = g_sz2rli.FindIndexOf(psz);
	if( ix<0 ) return false;
	g_sz2rli[ix]->m_name = name;
	return true;
}
*/

void wxSizer2RlItem::SetName( wxRlItem *rli, const wxChar *name ){
	int ix = g_sz2rli.FindIndexOf(rli);
	if( ix<0 )
		g_sz2rli.Push( new wxSizer2RlItem(rli->GetSizer(),rli,name) );
	else
		g_sz2rli[ix]->m_name = name;
}

/*
const wxChar* wxSizer2RlItem::GetName( wxSizer *psz ){
	int ix = g_sz2rli.FindIndexOf(psz);
	if( ix<0 ) return NULL;
	return g_sz2rli[ix]->m_name;
}
*/

const wxChar* wxSizer2RlItem::GetName( wxRlItem *rli ){
	int ix = g_sz2rli.FindIndexOf(rli);
	if( ix<0 ) return NULL;
	return g_sz2rli[ix]->m_name;
}


void wxSizer2RlItem::DumpNames( ){
	char buf[256];
	for( int ix=0; ix<g_sz2rli.Size(); ix++ ){
		sprintf( buf, "0x%08x - %s - %s\n", 
				g_sz2rli[ix]->m_rli,
				WX2AC(g_sz2rli[ix]->m_rli->Type2Str()),
				WX2AC(g_sz2rli[ix]->m_name) );
		puts( buf );
		wxLogMessage( MB2WX(buf) );
	}
}


// Functions to finally remove/delete wxRlItems safely
void TrashRlItem( wxRlItem *rli, wxRlManager *rlmgr ){
	if( !rli ) return;
	if( !rlmgr )
		rlmgr = wxRlManager::GetRlManagerOf( rli );

	// It can be that we're deleting items before they have been integrated with the layout
	if( rlmgr )
		rlmgr->DetachNotify( rli );
	
	// Better to remove items from name database now than in destructor
	if( rli->GetSizer() ){
		ExpArrP<wxRlItem*> rlis;
		for( rlis.Push(rli); rlis.Size(); ){
			wxSizer2RlItem::Remove( rlis.Top() );
			for( SizerIter sit(rlis.Pop()->GetSizer()); sit.Get(); sit.Step() )
				if( sit.Get()->GetSizer() ){
					wxRlItem *rli = ToRlItem(sit.Get()); 
					if( rli->GetType()==RlAppPane || rli->GetType()==RlToolPane )
						//wxASSERT( wxSizer2RlItem::Lookup(rli->GetSizer()) ); 
						wxASSERT( wxSizer2RlItem::GetName(rli) );
					if( ToRlItem(sit.Get()) )
						rlis.Push(ToRlItem(sit.Get()));
				}
		}
	}
	
	g_rl_garbage.Push( rli );
}

void TrashRlItem( wxSizer *ps, int ix, wxRlManager *rlmgr ){
	wxASSERT( ps );
	wxRlItem *rli = ToRlItem( ps->GetItem( ix ) );
	wxASSERT( rli );
	
	// We have to access the linked list to remove the item without deleting it
    int r = ps->GetChildren().DeleteObject(rli);
    wxASSERT( r );
    
	TrashRlItem( rli, rlmgr );
}
	

void ReparentSizerWindows( wxSizer *ps, wxWindow *wnd_newp ){
	if( !ps ) return;
    for( SizerIter sit(ps); sit.Get(); sit.Step() ){
        if( sit.Get()->GetWindow() )
        	sit.Get()->GetWindow()->Reparent(wnd_newp);
        else if( sit.Get()->GetSizer() )
        	ReparentSizerWindows( sit.Get()->GetSizer(), wnd_newp ); 
    }
}

wxRlItem *GetRootItemOf( wxRlItem *rli ){
	for( ;rli; rli=rli->GetParent() ){
		if( !rli->GetParent() )
			return rli;
	}
	return NULL;
}


RlEnum2Str *RlTypeProperties( int type ){
    for( int i=0; g_rl_e2t[i].num!=RlNull; i++ )
        if( g_rl_e2t[i].num==type )
            return g_rl_e2t+i;
    return NULL;
}

const wxChar *RlType2Str( int type ){
	RlEnum2Str *pre = RlTypeProperties(type);
    return pre ? pre->str : _T("? - Unknown");
}

short RlType2Capab( int type ){
	RlEnum2Str *pre = RlTypeProperties(type);
    return pre ? pre->capab : 0;
}


const wxChar *wxRlItem::Type2Str(  ){
    return ::RlType2Str( GetType() );
}

int LookItem( wxRlItem *item ){
	// For the primitive debugger...
	// Do nothing but see the variables at least.
    int flags = item->GetFlag();
    int border = item->GetBorder();
    int prop = item->GetProportion();
    int type = item->GetType();

    // For checking data in debugger.
    return flags+border+prop+type;
}


// We only come here for app generated sizers
int GetSizerType( wxSizer *sizer ){
	if( !sizer ) return RlNoPart;
	
	// # This does not make too much sense after moving specialization to 
	// wxRlItem (instead of deriving from wxBoxSizer)

    wxBoxSizer *cbp = wxDynamicCast(sizer,wxBoxSizer);
    if( cbp ) return RlSizer;

    wxSizer *bp = wxDynamicCast(sizer,wxSizer);
    if( bp )
    	return RlSizerUnk;

    wxASSERT( false );
    return RlSizerUnk;
}


int FlagToBorder( int flag ){
    if( flag&wxBORDER_NONE ) return 0;
    if( !(flag&(wxSUNKEN_BORDER|wxRAISED_BORDER|wxSIMPLE_BORDER|wxSTATIC_BORDER)) ) return 0;
    return flag&wxBORDER_DOUBLE ? 2 : 1;
}

void wxRlItem::Init( int type ){
    m_parent = NULL;
    m_state = 0;
    m_capab = 0;
    SetType( type );	// This also sets important m_capab
    if( !m_proportion )
    	m_flag |= wxFIXED_MINSIZE;
    
    LookItem( this );
}

wxRlItem::wxRlItem( RlUiArea type, int flag ) :
    wxSizerItem(    wxRlManager::GetPainter()->GetMetrics(type),
                    wxRlManager::GetPainter()->GetMetrics2nd(type),
                    RlPainter::Proportion(type),
                    RlPainter::Expandable(type)|flag,
                    FlagToBorder(flag), NULL )
{
    Init( type );
}

wxRlItem::wxRlItem( wxWindow *window,
             int proportion,
             int flag,
             int type,
             wxObject* userData ) :
    wxSizerItem( window, proportion, flag, FlagToBorder(flag), userData )
{
    Init( type );
    // If the window has min size set it for the sizer item as well.
	wxSize sz = window->GetMinSize();
	if( sz.x>0 && sz.y>0 )
		SetMinSize( sz );
}


RlUiArea wxRlItem::GetType(){
    return (RlUiArea)m_type;
}

void wxRlItem::SetType( int type ){
	// For fix items, we don't want to keep this flag
	short old_capab_mask = m_capab&RL_CAPAB_FIX_MASK;
	
    m_type = (short)type;
    m_type_check = type^0x619b;
    m_capab = RlType2Capab(type) | old_capab_mask;
}

void wxRlItem::ModifyTypeFor( wxRlItem *rli, int type ){
	wxASSERT( rli && IsChild(rli) );
	int off = (short*)&m_type - (short*)this;
	short *ps = (short*)rli;
	ps[off] = type;
}

// We want to set the m_capab field for another instance
// In C++ protected access only allows for looking at 
// ones owns member vars, not those of others of same
// type.
bool wxRlItem::SetCapabFor( wxRlItem *rli, int capab ){
	if( !rli || (capab&~0xFFFF) ) return false;
	wxASSERT( IsChild(rli) || !rli->GetParent() );
	int off = (short*)&m_capab - (short*)this;
	short *ps = (short*)rli;
	ps[off] = capab;
	return true;
}

// To check that it really is of this type
bool wxRlItem::IsRlItem(){
    return !((m_type_check^0x619b) ^ m_type);
}

bool IsRlItem( wxSizerItem *si ){
    return si && ((wxRlItem*)si)->IsRlItem();
}

RlUiArea GetTypeOf( wxRlItem *rli ){
	return !rli ? RlNoPart : rli->GetType();
}

wxRlItem *ToRlItem( wxSizerItem *psi ){
	if( !IsRlItem(psi) ) return NULL;
	return reinterpret_cast<wxRlItem*>(psi);
}

// Set the orientation flag for this item
//
// For 2d layout, it is important that the orientation flag
// for a sizer filters up to its wxSizerItem.
void wxRlItem::AdjustItemOrientation( ){
    wxSizer *sizer = GetSizer();
    int type = GetType();
    int orient = 0;
    wxBoxSizer *pbsz = wxDynamicCast(sizer,wxBoxSizer);
    
    if( pbsz ){
        orient = ((wxBoxSizer*)sizer)->GetOrientation()&wxBOTH;
    }
    else if( type==RlCaption ){
    	orient = wxHORIZONTAL;	// Hard code for now
    	// Below is wrong.
        //orient = ((wxBoxSizer*)((wxOverlapSizer*)sizer)->GetBgItem())->GetOrientation()&wxBOTH;
    }
    else{
        wxASSERT( false );
    }
    wxASSERT( orient );
    wxSizerItem::SetFlag( (GetFlag()&~wxBOTH)|orient );
}

const wxChar *GetSizerName( int type, wxSizer *psz ){
	if( !psz ) return NULL;
	//if( type==RlAppPane )
	//	return ((wxRlAppPane*)psz)->GetName();
	//if( type==RlToolbarDock )
	//	return ((wxRlToolBarDock*)psz)->GetName();
	//if( type==RlToolPane )
	//	return ((wxRlToolPane*)psz)->GetName();
	return NULL;
}


// subsizer, type given
wxRlItem::wxRlItem( wxSizer *sizer,
             int proportion,
             int flag,
             int type,
             wxObject* userData ) :
    wxSizerItem( sizer, proportion, flag, FlagToBorder(flag), userData )
{
	if(type==-1) type = GetSizerType(sizer);
    Init( type );
    //wxSizer2RlItem::Push( sizer,this, GetSizerName(type,sizer) );

    // Set the orientation flag for this item
    AdjustItemOrientation( );
}

/*
// subsizer, determine type
wxRlItem::wxRlItem( wxSizer *sizer,
             int proportion,
             int flag,
             wxObject* userData ) :
    wxSizerItem( sizer, proportion, flag, FlagToBorder(flag), userData )
{
	int type = GetSizerType(sizer);
    Init( type );
    wxSizer2RlItem::Push( sizer, this, GetSizerName(type,sizer) );

    // Set the orientation flag for this item
    AdjustItemOrientation( );
}
*/

wxRlItem::wxRlItem( int w, int h,
                     int proportion,
                     int flag,
                     int type,
                     wxObject* userData ) :
    wxSizerItem(    w, h,
                    proportion,
                    flag,
                    FlagToBorder(flag),
                    userData )
{
    Init( type );
}

// Copy everything from an existing item
wxRlItem::wxRlItem( wxSizerItem *si ) :
    wxSizerItem( si->GetSize().GetWidth(),
                 si->GetSize().GetHeight(),
                 si->GetProportion(),
                 si->GetFlag(),
                 si->GetBorder(),
                 si->GetUserData() ){
    Init( -1 );
    
    wxSize szmin = si->GetMinSize();
    wxPoint pos = si->GetPosition();
    wxRect rct = si->GetRect();

    if( si->IsWindow() ){
        AssignWindow( si->GetWindow() );
        szmin = si->CalcMin();
        SetType( RlWindow );
        wxLogMessage( _T("wxRlItem constructor based on wxSizerItem - unsafe assignment of window") );
    }
    else if( si->IsSizer() ){
        wxSizerItem::AssignSizer( si->GetSizer() );
        si->DetachSizer( );
        SetType( RlSizerUnk );
		AdjustItemOrientation( );
    }
    else if( si->IsSpacer() )
        SetType( RlSpacerUnknown );
    else
        wxASSERT( false );

	if( ::IsRlItem(si) ){
		int type = ((wxRlItem*)si)->GetType(); 
		SetType( type );
		// Also make sure any children points to this item
		if( IsSizer() ){
		    //wxSizer2RlItem::Push( GetSizer(), this, GetSizerName(type,GetSizer()) );
		    wxRlItem *part;
	        for( SizerIter sit(GetSizer()); (part=(wxRlItem*)sit.Get()); sit.Step() )
	            if( part->IsRlItem() )
	            	part->m_parent = this;
		}
	}

    si->SetUserData( NULL );
    m_minSize = szmin;
    m_pos = pos;
    m_rect = rct;
}


int wxRlItemDtor(){
	// Just to put a break point when not allowed to do that in Ctor
	int sum = 0;
	for( int ix=0; ix<10; ix++ )
		sum += ix;
	return sum;
}

wxRlItem::~wxRlItem(){
	wxRlItemDtor();
	if( GetSizer() )
		wxSizer2RlItem::Remove(this);
	//wxLogMessage( _T("wxRlItem::~wxRlItem - %X"), this );
	//printf( "wxRlItem::~wxRlItem %x\n", this );
}



// This is the item that is returned when an operation fails
wxRlNullClass g_rl_null;

bool wxRlItem::IsOk(){
    if( this==&g_rl_null ) return false;
    return IsRlItem();
}

#include <stdio.h>
void wxRlLog( const wxChar *s ){
	wxLogMessage( s );
}

wxSizerItem *GetItemSkipNonEnum( wxSizer *sizer, int app_ix ){
    wxRlItem *part;
    if( app_ix>=0 ){
        for( SizerIter sit(sizer); (part=(wxRlItem*)sit.Get()) && app_ix>=0; sit.Step() ){
            if( part->IsRlItem() && part->IsEnumerable() ){
                if( !app_ix-- )
                    break;
            }
        }
    }else{
        for( SizerIter sit(sizer,false); (part=(wxRlItem*)sit.Get()) && app_ix<=0; sit.StepBack() ){
            if( part->IsRlItem() && part->IsEnumerable() ){
                if( !++app_ix )
                    break;
            }
        }
    }

    return part;
}

bool GetIndexSkipNonEnum( wxSizer *sizer, int app_ix, int *ixp ){
    wxRlItem *part;
    SizerIter sit(sizer);
    if( app_ix>=0 ){
        for( sit.InitSetDirection(true); (part=(wxRlItem*)sit.Get()); sit.Step() ){
            if( part->IsRlItem() && part->IsEnumerable() ){
                if( --app_ix<0 )
                    break;
            }
        }
        if( app_ix<=0 ){
            *ixp=sit.GetIndex();
            return true;
        }
    }else{
        // app_ix -1 is a special case, it means append. -2 means insert before last existing element
        if( app_ix==-1 ){
            *ixp = sizer->GetChildren().GetCount();
            return true;
        }
        else app_ix++;
        for( sit.InitSetDirection(false); (part=(wxRlItem*)sit.Get()); sit.StepBack() ){
            if( part->IsRlItem() && part->IsEnumerable() ){
                if( !++app_ix )
                    break;
            }
        }
        if( app_ix>=0 ){
            *ixp=sit.GetIndex();
            return true;
        }
    }

    return false;
}


// Goes to sub item nr index:th
wxRlItem &wxRlItem::Part( int index ){
    int type = GetType();
    wxSizer *sizer = GetSizer();
    if( !sizer ){
        g_rl_null.Log( wxString::Format( _T("Part: Sub part is not sizer %08x"), (int)this ) );
        return g_rl_null;
    }

    if( type==RlAppPane ){
        if( index<-1 || index>0 ){
            g_rl_null.Log( wxString::Format( _T("Part, AppPane: Index out of range %d, item %08x"), index, (int)this ) );
            return g_rl_null;
        }

        wxASSERT( 0 );	// Don't think we ever get here
        /*
        wxASSERT( GetSizer() );
        wxRlAppPane *ap = wxDynamicCast( this, wxRlAppPane );
        part = ap->m_contents;
        wxASSERT( part );
        return *part;
        */
    }

    // Enumerate child items until we find the right one
    wxSizerItem *psi = GetItemSkipNonEnum( sizer, index );
    if( ::IsRlItem(psi) ) return *(wxRlItem*)psi; 

    g_rl_null.Log( wxString::Format( _T("Part, Sizer: Index out of range %d, item %08x"), index, (int)this ) );
    return g_rl_null;
}

// return index of this item in parent sizer
int wxRlItem::IndexOf(  ){
	if( !m_parent ) return -1;
	int ix=0;
	wxRlItem *part;
    for( SizerIter sit(m_parent->GetSizer()); (part=(wxRlItem*)sit.Get()); sit.Step() ){
        if( part->IsRlItem() && part->IsEnumerable() ){
        	if( part==this ) return ix;
        	ix++;
        }
    }
    return -1;
}

// Only app level items are enumerable
bool wxRlItem::IsEnumerable( ){
    return IsEnumerable( GetType() );
}

// Goes to the parent item
wxRlItem &wxRlItem::Up(){
    // We are forgiving and don't give the RlNull item
    if( !m_parent )
        return *this;

    return *m_parent;
}

// Returns address of this item
wxString wxRlItem::AddressOf( wxRlItem *rli ){
	int ix = 0;
    for( SizerIter sit(GetSizer()); sit.Get(); sit.Step() ){
    	if( !::IsRlItem(sit.Get()) || 
    		!((wxRlItem*)sit.Get())->IsEnumerable() ) continue;
    	if( sit.Get()==rli ){
    		wxString s = wxString::Format(_T("%d"),ix); 
    		if( m_parent )
    			return m_parent->AddressOf(this) + _T(",") + s;
    		else
    			return s; 
    	}
    	ix++;
    }
    wxASSERT(_T("AddressOf: Item not found")); 
    return _T("Error, not found");
}


// Inserts an item into this sizer
wxRlItem &wxRlItem::Insert( int pos, wxRlItem *item, RlUiArea edge ){
    if( !item || !item->IsEnumerable() ){
        g_rl_null.Log( wxString::Format( _T("Insert: The item is not valid, item %08x"), (int)item ) );
        return g_rl_null;
    }

    wxSizer *sizer = GetSizer();
    if( !sizer || !IsUserSizer() ){
        if( !sizer )
            g_rl_null.Log( wxString::Format( _T("Insert: The current item is not a sizer, item %08x"), (int)this ) );
        else
            g_rl_null.Log( wxString::Format( _T("Insert, Sizer: Not possible to modify, item %08x"), (int)this ) );
        return g_rl_null;
    }

    if( m_capab&RL_CAPAB_FIX ){
    	return g_rl_null.Log( wxString::Format( _T("Insert, sizer has fixed contents (RL_CAPAB_FIXED)"), (int)this ) );
    }

    // See where to put the new item.
    int ix;
    if( !GetIndexSkipNonEnum( sizer, pos, &ix ) ){
        g_rl_null.Log( wxString::Format( _T("Insert, Sizer: Index out of range: %d, item %08x"), pos, (int)this ) );
        return g_rl_null;
    }

    // User specified edge?
    RlUiArea type = item->GetType();
    if( edge>=0 ){
        if( !RL_IS_KNOWN_EDGE(edge) ){
            g_rl_null.Log( wxString::Format( _T("Insert, Edge: Unknown edge: %d, item %08x"), edge, (int)this ) );
            return g_rl_null;
        }
    }
    else edge = GetEdgeTypeForPart( type );

    // Check to see whether there is a single 'lonely'
    // temp bg item there in the sizer. If so, remove it
    if( ix<(int)sizer->GetChildren().GetCount() ){
        wxRlItem *rli = ToRlItem( sizer->GetItem(ix) );
        if( rli && rli->m_capab&RL_CAPAB_TEMP_BG ){
            //sizer->Remove( (size_t)ix );
        	TrashRlItem( sizer, ix );
        }
    }

    // Which sides do we have edges on?
    bool left_edge = ix>0 ? true : false;
    bool right_edge = ix<(int)sizer->GetChildren().GetCount() ? true : false;

    // Insert the new item
    sizer->Insert( ix, item );
    item->m_parent = this;

    // Create or replace edges
    if( edge!=RlNoPart ){
        if( left_edge ){
            // Are we inserting at the last position in the line?
            // Then there is no old edge to look at
            if( !right_edge )
                sizer->Insert( ix, new wxRlItem(edge) );
            else{
                wxRlItem *rli = ToRlItem(sizer->GetItem(ix-1));
                if( rli->GetType()!=edge ){
                    if( RL_IS_KNOWN_EDGE(rli->GetType()) )
                        //sizer->Remove( ix-1 );
                    	TrashRlItem( sizer, ix-1 );
                    sizer->Insert( ix-1, new wxRlItem(edge) );
                }
            }
        }
        if( right_edge ){
            // Right edge is easier. It is always new.
            sizer->Insert( ix+1, new wxRlItem(edge) );
        }
    }

    return *item;
}

wxRlItem &wxRlItem::Insert( int pos, wxWindow *wnd, int prop, int flag, RlUiArea edge ){
    // Check that window is OK
    if( !wnd ){
        g_rl_null.Log( wxString::Format( _T("Insert: The window is not valid, item %08x"), wnd ) );
        return g_rl_null;
    }

    // Create a new item and insert it
    wxRlItem *rli = new wxRlItem( wnd, prop, flag, RlWindow );
    if( rli==&Insert(pos,rli,edge) ){
    	// We don't use this feature, but to make sure we're immune to 
    	// its problem side, set it
    	if( m_parent )
    		wnd->SetContainingSizer( m_parent->GetSizer() );
        return *rli;
    }
    else{
        // Have to delete item in case of insert failure
        delete rli; // TrashRlItem(rli);
        return g_rl_null;
    }
}

wxRlItem &wxRlItem::Insert( int pos, wxSizer *sizer, int prop, int flag, RlUiArea edge ){
    // Check that sizer is OK
    int type = GetSizerType( sizer );
    if( !sizer || !IsEnumerable(type) ){
        g_rl_null.Log( wxString::Format( _T("Insert: The sizer item is not valid, item %08x"), sizer ) );
        return g_rl_null;
    }

    // Create a new item and insert it
    wxRlItem *rli = new wxRlItem( sizer, prop, flag, type );
    if( rli==&Insert(pos,rli,edge) ){
	    //wxSizer2RlItem::Push( sizer,rli, GetSizerName(type,sizer) );
	    // Correct newly inserted item
	    // NO - We cannot do this always at construction
        return *rli;
    }
    else{
        // Have to delete item in case of insert failure
        rli->DetachSizer();
        delete rli; // TrashRlItem(rli);
        return g_rl_null;
    }
}

#if wxUSE_UNICODE==1
	#define SCUNICODE
#endif 
#include "utils/sc.h"

wxRlItem &wxRlItem::FindAddress( const wxChar *addr ){
	if( !addr )
        return g_rl_null.Log( wxString::Format( _T("FindAddress: NULL address given") ) );
	
	// parse the address
	while( *addr==_T(' ') ) addr++;
	wxChar *aend = NULL;
	int pix = (int)scstrtol(addr,&aend,10);
	if( aend==addr )
		return *this;
	if( !aend )
        return g_rl_null.Log( wxString::Format( _T("FindAddress: Failed parsing number in address") ) );

	wxRlItem &sub_item = Part(pix);
	if( !sub_item.IsOk() )
		return sub_item;
	else if( *aend!=_T(',') )
		return sub_item;
	else
		return sub_item.FindAddress(aend+1);
}


wxRlItem &wxRlItem::Enclose( int flag, bool take_over_border ){
    // Don't want to enclose invalid objects
    if( !IsEnumerable() )
        return g_rl_null.Log( wxString::Format( _T("Enclose, Item cannot be put in sizer %d, item %08x"), GetType(), (int)this ) );

    // If parent is a tool pane, we cannot do this
    if( m_parent && m_parent->GetType()==RlToolPane )
        return g_rl_null.Log( wxString::Format( _T("Enclose, New child sizers cannot be put in tool panes, item %08x"), (int)this ) );

    // If no direction given, take opposite direction of parent (or child)
    if( !(flag&wxBOTH) ){
        int dir = 0;
        wxSizer *ps = m_parent ? m_parent->GetSizer() : NULL;
        if( !ps ) ps = GetSizer();	// This takes the opposite direction to what we're eenclosing
        if( ps ) dir = GetOrientation( ps ); 
        if( dir==wxVERTICAL ) flag |= wxHORIZONTAL;
        else flag |= wxVERTICAL;
    }

    // Create the enclosing sizer
    wxBoxSizer *sizer = new wxBoxSizer( flag&(wxVERTICAL|wxHORIZONTAL) );
    wxPoint pos = GetPosition();
    wxSize sz = GetSize();
    // Proportion states the relationship with the enclosing sizer, 
    // so that should move to the new sizer
    int prop = GetProportion( );

    // Create a new object to hold the new enclosing sizer
    // We (this) become the inner enclosed object
    wxRlItem *rli = new wxRlItem(sizer,prop,wxEXPAND,RlSizer);
    wxSizerItem::SetProportion( RL_DEF_PROP );

	if( take_over_border ){
    	// In case the item has a border, we take over it. The border belongs with
    	// the outside, not the sizers.
		int border_flags = GetFlag()&(wxBORDER_DOUBLE|wxSUNKEN_BORDER|wxRAISED_BORDER|wxSIMPLE_BORDER|wxSTATIC_BORDER|wxTOP|wxLEFT|wxBOTTOM|wxRIGHT); 
		flag |= border_flags;
		SetFlag( GetFlag()^border_flags );
		SetBorder(0);
	}
	rli->SetFlag( flag );
	rli->SetBorder( FlagToBorder(flag) );

	// If we have difference in orientation between the two, the 
	// meaning of prop_sum==0 and !wxEXPAND changes
	if( m_parent && (m_parent->GetFlag()^rli->GetFlag())&wxBOTH ){
		bool ps_zero = !GetPropSum(this);
		bool not_expand = !(GetFlag()&wxEXPAND);
		if( ps_zero ){
			wxSizerItem::SetProportion( RL_DEF_PROP );
			SetFlag( GetFlag()&~wxEXPAND );
		}
		if( not_expand ){
			wxSizerItem::SetProportion( 0 );
			SetFlag( GetFlag()|wxEXPAND );
		}
	}
	
	// Detach from parent sizer and insert new item
	wxRlItem *parent = m_parent;
	if( parent ){
		int ix = IndexOf();
		DetachInternal( false );
		wxASSERT( !m_parent );
		if( !parent->Insert( ix, rli ).IsOk() ){
			// # We might loose some flags here, we're in an error condition
			delete rli;
			return g_rl_null;
		}
	}
	else{
	    // Check if this was the root item of the RlManager - if so, change to new item
	    wxRlManager *rlmgr = wxRlManager::GetRlManagerOf(this); 
	    wxASSERT( rlmgr );
	    if( rlmgr && &rlmgr->GetRoot()==this )
	    	rlmgr->m_root = rli;
	    	// SetRoot causes difficulty, it deletes us!
	    	// rlmgr->SetRoot( rli );
	}
	
    // Put ourself into the new sizer
    rli->Insert(0,this);

    // Do layout
    rli->SetDimension( pos, sz );

    // Now we have an enclosing top item
    return *rli;
}

// Come here when a child sets wxEXPAND or detaches from the sizer
// possibly removing constraint
void wxRlItem::ChildReleaseNoExpand( ){
	wxASSERT( GetSizer() );
	
	// If any child still not have wxEXPAND, nothing changed
	for( SizerIter sit(GetSizer()); sit.Get(); sit.Step() )
		if( !(sit.Get()->GetFlag()&wxEXPAND) )
			return;
	
	if( m_parent ){
		if( !((m_parent->GetFlag()^GetFlag())&wxBOTH) ){
			// Same direction
			if( !(GetFlag()&wxEXPAND) ){
				SetFlag( GetFlag()|wxEXPAND );
				m_parent->ChildReleaseNoExpand();
			}
		}
		else{
			// Different direction
			if( !GetProportion() ) {
				wxSizerItem::SetProportion( GetPropSum(m_parent)?1:RL_DEF_PROP );
				m_parent->ChildReleaseNoProportion( );
			}
		}
	}
	
	else{
		// Root item case
		if( (GetFlag()&wxBOTH)==wxVERTICAL ){
			wxSizerItem::SetProportion( RL_DEF_PROP );
		}
		else{
			SetFlag( GetFlag()|wxEXPAND );
		}
		wxRlManager *rl_mgr = wxRlManager::GetRlManagerOf(this);
		if( rl_mgr )
			rl_mgr->AdjustWindowMinSize( );
	}
}

// Come here when a child regains proportion
void wxRlItem::ChildReleaseNoProportion( ){
	// If several children have proportion, nothing changed
	int n = 0;
	for( SizerIter sit(GetSizer()); sit.Get(); sit.Step() )
		if( sit.Get()->GetProportion() )
			n++;
	if( n>1 ) return;
	
	if( m_parent ){
		if( !((m_parent->GetFlag()^GetFlag())&wxBOTH) ){
			// Same direction
			if( !GetProportion() ) {
				wxSizerItem::SetProportion( GetPropSum(m_parent)?1:RL_DEF_PROP );
				m_parent->ChildReleaseNoProportion( );
			}
		}
		else{
			// Different direction
			if( !(GetFlag()&wxEXPAND) ){
				SetFlag( GetFlag()|wxEXPAND );
				m_parent->ChildReleaseNoExpand();
			}
		}
	}
	
	else{
		// Root item case
		if( (GetFlag()&wxBOTH)==wxVERTICAL )
			SetFlag( GetFlag()|wxEXPAND );
		else{
			wxSizerItem::SetProportion( RL_DEF_PROP );
		}
		wxRlManager *rl_mgr = wxRlManager::GetRlManagerOf(this);
		if( rl_mgr )
			rl_mgr->AdjustWindowMinSize( );
	}
}

bool wxRlItem::IsEmptyContent(){
	if( m_type==RlWindow && !GetWindow() ) 
		return true;
	if( m_type==RlSizer && (!GetSizer() || !GetSizer()->GetChildren().GetCount()) )
		return true;
	return false;
}



// Detaches the current item from the layout
wxRlItem &wxRlItem::DetachInternal( bool detach_final ){
	wxRlManager *rlmgr = wxRlManager::GetRlManagerOf(this);
	// OK if we don't have wxRlManger - then assume not yet integrated in layout
	//if( !rlmgr )
	//	return g_rl_null.Log( wxString::Format( _T("Detach, Object has no wxRlManager, %08x"), (void*)this ) );
	
	// There are two possible meanings to RL_CAPAB_FIX:
	// 1 - Do not modify its contents
	// 2 - It is in a fix place in a layout
	// We treat it like 1 here.
	//if( m_capab&RL_CAPAB_FIX )
	//	return g_rl_null.Log( wxString::Format( _T("Detach, Object has RL_CAPAB_FIX, should not detach, %08x"), (void*)this ) );
	
	
	// We will be deleting items below and destructor needs to know about it.
	g_rl_mgr_last_active = rlmgr;
	
	// Let wxRlManager clear away any dangling pointers
    if( detach_final && rlmgr ) 
    	rlmgr->DetachNotify( this );
    
    if( !m_parent ){
    	// So we are we the root. Detach ourselves from wxRlManager 
    	// Replace with a temp bg if final release
    	wxASSERT( rlmgr );
    	rlmgr->m_root = detach_final ? new wxRlClientTempBg : NULL; // SetRoot( NULL );
    	return *this;
    }

    wxSizer *sizer = m_parent->GetSizer();
    wxASSERT( sizer );

    wxSizerItemList &sl = sizer->GetChildren();
    int ix = sl.IndexOf(this);
    wxASSERT( ix!=wxNOT_FOUND );

    // Remove edge before (left/above)?
    wxRlItem *rli;
    if( ix>0 && (rli=(wxRlItem*)sizer->GetItem(ix-1)) &&
        ::IsRlItem(rli) && RL_IS_KNOWN_EDGE(rli->GetType()) ){
    	TrashRlItem( sizer, ix-1, rlmgr );
        ix--;
    }

    // Remove edge after (right/below)?
    else if( ix<(int)sl.GetCount()-1 && (rli=(wxRlItem*)sizer->GetItem(ix+1)) &&
        ::IsRlItem(rli) && RL_IS_KNOWN_EDGE(rli->GetType()) ){
    	// Have to delete proprly since wxRlManager can hold pointers
    	TrashRlItem( sizer, ix+1, rlmgr );
    }

    // Now we want to remove this object itself from the linked list
    // (using sizer interface, we would have to recreate the wxRlItem object)
    int r = sl.DeleteObject(this);
    wxASSERT( r );
    
    // We might have to change edge type, if item being removed had other
    // edges than the new neighbours wants
    if( ix>0 && ix<(int)sl.GetCount()-1 ){
	    wxRlItem *rli0 = (wxRlItem*)sizer->GetItem(ix-1);
	    wxRlItem *rli = (wxRlItem*)sizer->GetItem(ix);
	    wxRlItem *rli2 = (wxRlItem*)sizer->GetItem(ix+1);
	    int e0 = ::IsRlItem(rli0) ? GetEdgeTypeForPart(rli0->GetType()) : RlNoPart;
	    int e2 = ::IsRlItem(rli2) ? GetEdgeTypeForPart(rli2->GetType()) : RlNoPart;
	    int e = ::IsRlItem(rli) ? rli->GetType() : RlNoPart;
	    if( RL_IS_KNOWN_EDGE(e) ){
	    	int ebest = (e0!=RlNoPart && (e0<e2) ? e0 : e2);
	    	if( RL_IS_KNOWN_EDGE(ebest) ){
	    		rli->SetSpacer( wxSize(wxRlManager::GetPainter()->GetMetrics(ebest),
	    							   wxRlManager::GetPainter()->GetMetrics2nd(ebest)), 
	    						ebest );
	    	}
	    }
    }
    
    // If the containing sizer is the root element, we should insert a temp BG
    if( detach_final && !m_parent->m_parent && !sl.GetCount() ){
    	wxRlItem *tbg_rli = new wxRlClientTempBg;
    	m_parent->Insert( 0, tbg_rli );
    }

    wxRlItem *parent = m_parent;
    // We're an orphan now...
    m_parent = NULL;
    
    // If we have 0 proportion or !wxEXPAND, we should undo that effect in the sizer we're 
    // moving from. The next time UpdateItem is called it will set these flags if needed, but 
    // it never clears them.
    if( !(GetFlag()&wxEXPAND) ){
    	if( parent )
    		parent->ChildReleaseNoExpand( );
    }
    if( GetSizer() ){
    	SetFlag( GetFlag()|wxEXPAND );
    	if( !GetProportion() && !(GetFlag()&wxFIXED_MINSIZE) )
    		wxSizerItem::SetProportion( RL_DEF_PROP );
    }
    
    // done
    return *this;
}

// Base case for wxSizers
bool wxRlItem::SetContents( wxRlItem *rli, int ix ){
	wxSizer *ps = GetSizer(); 
	if( !ps ) return false;
	// The indeces only count the window parts, so we have to step manually
	for( SizerIter si(ps); si.Get(); si.Step() ){
		wxRlItem *rli_old = ToRlItem(si.Get());
		if( rli_old && (rli_old->GetType()==RlWindow || 
			(rli_old->GetType()==RlSizer && rli_old->GetCapab()&RL_CAPAB_ENTITY)) ){
			if( (ix>=0 && !ix--) || (ix==-1 && rli_old->IsEmptyContent()) )
				return rli_old->Replace( rli ).IsOk();
		}
	}
	return false;
}

wxRlItem *wxRlItem::GetContents( int ix ){
	wxSizer *ps = GetSizer(); 
	if( !ps ) return false;
	// The indeces only count the window parts, so we have to step manually
	for( SizerIter si(ps); si.Get(); si.Step() ){
		wxRlItem *rli_old = ToRlItem(si.Get());
		// We treat windows like contents and also plain sizers that
		// have the entity bit set
		if( rli_old && (rli_old->GetType()==RlWindow || 
			(rli_old->GetType()==RlSizer && rli_old->GetCapab()&RL_CAPAB_ENTITY)) ){
			if( !ix-- )
				return rli_old;
		}
	}
	return false;
}

wxRlItem *wxRlItem::DetachContents( int ix ){
	wxSizer *ps = GetSizer(); 
	if( !ps ) return NULL;
	// The indeces only count the window parts, so we have to step manually
	for( SizerIter si(ps); si.Get(); si.Step() ){
		wxRlItem *rli_old = ToRlItem(si.Get());
		if( rli_old && (rli_old->GetType()==RlWindow || 
			(rli_old->GetType()==RlSizer && rli_old->GetCapab()&RL_CAPAB_ENTITY)) ){
			if( !ix-- ){
				// The temp item we replace with must behave decently
				// It will be counted past in this loop later on.
				wxRlItem *rli = new wxRlItem( rli_old->GetType(), rli_old->GetFlag() );
				SetCapabFor( rli, rli_old->GetCapab() );
				if( rli_old->ReplaceDetach(rli).IsOk() )
					return rli_old;
				delete rli;
				return NULL;
			}
		}
	}
	return NULL;
}


void wxRlItem::SetWindow( wxWindow *wnd ){
	wxWindow *old_wnd = GetWindow();
	// This happens automatically in wxSizerItem for wx >= 2.8.4, but to be sure...
	if( old_wnd )
		old_wnd->SetContainingSizer(NULL);
	wxSizerItem::AssignWindow( wnd );
	// Make sure the window has the correct containing sizer pointer
	if( m_parent && wnd )
		wnd->SetContainingSizer( m_parent->GetSizer() );
	SetType( RlWindow );
}

void wxRlItem::SetSizer( wxSizer *sizer, int type ){
	SetType( type==-1 ? GetSizerType(sizer) : type );
	wxSizerItem::AssignSizer( sizer );
}

void wxRlItem::SetSpacer( const wxSize &size, int type ){
	SetType( type );
	wxSizerItem::AssignSpacer( size );
}


// The difference is that we free the sizer in these functions
// so that we can be sure that whatever the item contained before 
// still exists.
void wxRlItem::ReplaceWith( wxWindow *wnd ){
	if( GetSizer() ) DetachSizer( );
	SetWindow( wnd );
}

void wxRlItem::ReplaceWith( wxSizer *sizer, int type ){
	if( GetSizer() ) DetachSizer( );
	SetSizer( sizer, type );
}

void wxRlItem::ReplaceWith( const wxSize &size, int type ){
	if( GetSizer() ) DetachSizer( );
	SetSpacer( size, type );
}


static int g_dump_level;
void wxRlItem::Dump( int rec ){
	if( !g_dump_level ){
		printf("\nwxRlItem::Dump - \n");
		wxLogMessage( _T("wxRlItem::Dump - ") );
	}
	
	wxString ind;
	char sbuf[256];
	for( int ix=0; ix<g_dump_level; ix++ ) 
		ind += _T("   ");
	const wxChar *name = RlGetNameOf(this);
	sprintf( sbuf, "%s0x%08X - %s - %s - (%s)", 
			WX2AC(ind), 
			this, WX2AC(Type2Str()), 
			GetWindow()?"Window":(GetSizer()?"Sizer":"SI"), 
			WX2AC(name?name:_T("")) );
	printf( "%s\n", sbuf );
	wxLogMessage( AC2WX(sbuf) );
	
	if( rec<0 || !GetSizer() ) return;
	
	for( SizerIter si(GetSizer()); si.Get(); si.Step() ){
		wxRlItem *rli = ToRlItem(si.Get());
		int sub_rec = rec>0 ? rec : -1;
		if( sub_rec==1 && (rli->GetCapab()&RL_CAPAB_ENTITY) ) 
			sub_rec = -1;
		g_dump_level++;
		rli->Dump( sub_rec );
		g_dump_level--;
	}
}



// Deletes the current item
bool wxRlItem::Destroy( bool destroy_wnd ){
	wxRlManager *rlmgr = wxRlManager::GetRlManagerOf(this);
	if( !rlmgr ) return false;
    if( !DetachInternal(true).IsOk() ) return false;
    if( destroy_wnd )
    	DeleteWindows( );
    TrashRlItem(this,rlmgr);
    return true;
}

wxRlItem &wxRlItem::Replace( wxRlItem *new_item, bool destroy_wnd ){
    if( !new_item || !new_item->IsEnumerable( ) )
        return g_rl_null.Log( wxString::Format( _T("Replace, New item not enumerable, type %d, %08x"), new_item->GetType(), (int)this ) );
    
    wxRlItem *parent = m_parent;
    if( !m_parent ){
    	wxRlManager *prlm = wxRlManager::GetRlManagerOf(this);
    	if( !prlm )
    		return g_rl_null.Log( wxString::Format( _T("Replace, Already detached (object has no parent), %08x"), (int)this ) );
        if( destroy_wnd )
        	DeleteWindows( ); 
    	prlm->SetRoot( new_item );
    	return *new_item;
    }

    // Find our position in parent sizer
    wxSizer *sizer = m_parent->GetSizer();
    wxASSERT( sizer );
    int ix = sizer->GetChildren().IndexOf( this );

    // Destroy ordered
    if( destroy_wnd )
    	DeleteWindows( ); 
    TrashRlItem( sizer, ix );

    // Insert new item
    new_item->m_parent = parent;
    sizer->Insert( ix, new_item );

    // Done
    return *new_item;
}

wxRlItem &wxRlItem::ReplaceDetach( wxRlItem *new_item ){
    if( !new_item || !new_item->IsEnumerable( ) )
        return g_rl_null.Log( wxString::Format( _T("Replace, New item not enumerable, type %d, %08x"), new_item->GetType(), (int)this ) );
    
    wxRlItem *parent = m_parent;
    if( !m_parent ){
    	wxRlManager *prlm = wxRlManager::GetRlManagerOf(this);
    	if( !prlm )
    		return g_rl_null.Log( wxString::Format( _T("Replace, Already detached (object has no parent), %08x"), (int)this ) );
    	prlm->SetRoot( new_item );
    	return *new_item;
    }

    // Find our position in parent sizer
    wxSizer *sizer = m_parent->GetSizer();
    wxASSERT( sizer );
    int ix = sizer->GetChildren().IndexOf( this );

    // Now we want to remove this object itself from the linked list
    int r = sizer->GetChildren().DeleteObject(this);
    m_parent = NULL;
    wxASSERT( r );
    
    // Insert new item
    new_item->m_parent = parent;
    sizer->Insert( ix, new_item );

    // Done
    return *new_item;
}


bool wxRlItem::InsertAt( wxRlItem *item, const wxChar *addr ){
	// We will exist in two places for a short while, but it's easier 
	// this way, since the address holds, which might
	// not be the case if we would remove ourselves now.
    bool no_log = GetSuppressLog( );
    
	// Now time to insert. The address is either like this:
    //   "1,0,2T" (sizer 1, sizer 0, sizer 2, enclose and put on top)
    //   "1,0,3"  (sizer 1, sizer 0, put as item 3)
    //   "1,0,#3" (sizer 1, sizer 0, put as item 3)
    // For case 2, we need to truncate before last digit
	wxString s(addr);
	wxChar lc = s.Last();
	int pix = -1;	// Position to insert
	if( isdigit(char(lc)) ){
		int p = s.Find(_T('#'),true);
		if( p==wxNOT_FOUND ) p = s.Find(_T(','),true);
		if( p==wxNOT_FOUND ) p = 0;
		else p++;
		pix = scstrtol( s.c_str()+p, NULL, 10 );
		s = s.Left(p);	// Truncate
	}

	wxRlItem *pdst = &FindAddress( s );
	if( !pdst->IsOk() ){
		if(!no_log) wxLogMessage( wxString::Format( _T("InsertAt: Could not find destination: %s"), (const wxChar*)s) );
		return false;
	}
	
	// First see if we need to enclose the item
	if( scstrchr(_T("TBLR"),lc) ){
		pix = (lc==_T('R') || lc==_T('B')) ? 1 : 0;
		int orient = (lc==_T('T') || lc==_T('B')) ? wxVERTICAL : wxHORIZONTAL;
		if( pdst->GetType()==RlSizer && pdst->GetFlag()&orient ){
			pix=-1;
			if(!no_log) wxLogMessage(_T("InsertAt: Skipping insertion, same orientation"));
		}
		else {
			/*
			// A 2nd check for toolbar orientation
			if( item->GetType()==RlToolbarDock && !((orient^item->GetFlag())&wxBOTH) ){
				wxLogMessage( _T("InsertAt: Toolbar, wrong enclose orientation") );
				return false;
			}
			*/
			pdst = &pdst->Enclose();
		}
	}
	if( pix==-1 ) return false;

	// Toolbars should not be inserted perpendicular to their own direction
	if( item->GetType()==RlToolbarDock ){
		if( !((item->GetFlag()^pdst->GetFlag())&wxBOTH) ){
			if(!no_log) wxLogMessage( _T("InsertAt: Skipping, wrong sizer orientation") );
			return false;
		}
	}

	// Save where we came from, if insert fails, we have to go back
	wxRlItem *psrc = item->GetParent( ); 
	wxRlManager *rlmgr_dst = wxRlManager::GetRlManagerOf( this );
	wxRlManager *rlmgr_src = wxRlManager::GetRlManagerOf( item );
	int ix = item->IndexOf( );
	// Internal move only if source and target are the same
	if( rlmgr_src && !item->DetachInternal(rlmgr_src==rlmgr_dst?false:true).IsOk() ){
		if(!no_log) wxLogMessage(_T("InsertAt: Failed DetachInternal"));
		return false;
	}
	
	// If moving in the same sizer, and our old index is smaller than 
	// new index, decrease, since we just removed ourselves
	if( psrc==pdst && pix>ix ) pix--;

	// See if we're changing to non-zero proprtion in dest
	bool no_prop_release = item->GetProportion() && !GetPropSum( pdst );
	
	// Insert at new location
	if( !pdst->Insert( pix, item ).IsOk() ){
		if(!no_log) wxLogMessage(_T("InsertAt: Failed Insert"));
		if( psrc ){
			if( !psrc->Insert(ix,item).IsOk() ){
				if(!no_log) wxLogMessage(_T("InsertAt: Failed Re-insert after 1st failure..."));
				// # Maybe we should delete homeless item (exceptional situation)
				item->Destroy( true );
			}
		}
		else{
			if( rlmgr_src && (!rlmgr_src->m_root || rlmgr_src->m_root->IsSpacer()) )
				// Restore it as root
				rlmgr_src->SetRoot(item);
			else{
				if(!no_log) wxLogMessage(_T("InsertAt: Failed Re-insert after 1st failure (probably root item)..."));
				item->Destroy( true );
			}
		}
		return false;
	}
	
	if( no_prop_release )
		pdst->ChildReleaseNoProportion( );
	
	// In general a sizer has its 0 proportion and !wxEXPAND as a consequence
	// of child items and orientation of parent sizer. Clear that here in a new 
	// context. They will be set again in UpdateItem if they should be set.
	if( item->IsSizer() ){
		item->SetFlag( item->GetFlag()|wxEXPAND );
		if( !item->GetProportion() && !(item->GetFlag()&wxFIXED_MINSIZE) ) 
			item->wxSizerItem::SetProportion( 1 );
	}
	
	return true;
}


wxWindow *wxRlItem::GetParentWindow( ){
	if( m_parent /*&& m_parent!=this*/ ) 
		return m_parent->GetParentWindow( );
	wxRlManager *mgr = wxRlManager::GetRlManagerOf( this );
	if( !mgr ) return NULL;
	return mgr->GetWindow( );
}

wxRlItem *wxRlItem::GetParentWithType( RlUiArea type ){ 
	if( !m_parent ) return NULL;
	if( m_parent->m_type==type ) return m_parent;
	return m_parent->GetParentWithType( type );
}

wxRlItem *wxRlItem::GetParentWithCapab( int capab ){ 
	if( !m_parent ) 
		return NULL;
	if( (m_parent->m_capab&capab)==capab ) 
		return m_parent;
	return m_parent->GetParentWithCapab( capab );
}

wxRlItem *wxRlItem::GetChildWithType( RlUiArea type, bool also_self, bool l2r ){ 
	if( m_type==type && also_self ) return this;
	
	wxSizer *sz = GetSizer();
	if( !sz ) return NULL;
	wxRlItem *rli;
	for( SizerIter sit(sz,l2r); sit.Get(); ){
		if( rli = ToRlItem(sit.Get()) ){
			if( rli->m_type==type ) return rli;
			if( rli->IsSizer() ){
				rli = rli->GetChildWithType(type,false,l2r);
				if( rli ) return rli;
			}
		}
		if( l2r ) sit.Step();
		else sit.StepBack();
	}
	return NULL;
}

bool wxRlItem::IsChild( wxRlItem *rli ){
	while( rli ){
		if( rli==this ) return true;
		rli = rli->GetParent();
	}
	return false;
}

wxRlItem *wxRlItem::FindItemAt( const wxPoint &pt ){
	if( !this || !GetRectWithBorders(this).Contains(pt) ) return NULL;
	if( !GetSizer() ) return this;
	// Look for best match
	for( SizerIter sit(GetSizer()); sit.Get(); sit.Step() ){
		wxRlItem *rli = ToRlItem(sit.Get())->FindItemAt(pt);
		if( rli ) return rli;
	}
	return this;
}

// If we find an arbitrary wxSizerItem somewhere, we convert it to
// a wxRlItem here. One reason is that we need to be able to retrieve
// parent information for all items.
wxRlItem *wxRlItem::ConvertToRlItem( SizerIter &sit, wxSizerItem *si, wxSizerItem *node ){

    // Create new item to mimic old item. The constructor does
    // the work of copying everything from the old object
    wxRlItem *rli = new wxRlItem( si );

    // Replace the old item. It does not affect the linked list node
    // So our iterator is safe. (checked source)
    node->GetSizer()->Replace( sit.GetIndex(), rli );

    // Set parent
    wxASSERT( ::IsRlItem(node) );
    rli->m_parent = (wxRlItem*)node;

    return rli;
}


void wxRlItem::UpdateItem( int has_bg ){
    wxSizer *node = GetSizer();
    wxASSERT( node );

	wxRlManager *rlmgr = NULL;
    if( has_bg==-1 ){
    	rlmgr = wxRlManager::GetRlManagerOf(this);
    	if( rlmgr ) has_bg = rlmgr->IsDialog();
    	else has_bg = 0;
    }
    
    // When resizing with a background, the situation is different.
    // (In dialogs/panels)
    
    // If one or more child items do not have the flag wxEXPAND,
    // we cannot resize this dock. Set the flag wxFIXED_MINSIZE
    // Also, its proportion should be 0.
    //
    // The rules with child sizers are:
    // - A: If 1+ child does not have wxEXPAND => B: we get prop=0 and wxFIXED_MINSIZE
    // - C: If all children have prop=0 => D: we get !wxEXPAND
    // If a vertical dock is contained in another vertical dock,
    // or horizontal in a horizontal), then it is swapped around:
    // - A => D
    // - C => B
    //
    //
    // NOTE: This function should only be called on a ready-built layout /sub-layout,
    // since otherwise, prop_sum might be 0, which is perfectly OK
    // while building a layout (when finished, we must trigger on this though)
    //
    wxRlItem *rli;
    int flags = wxEXPAND;
    int prop_sum = 0;
    wxRect sz, szmin;
    int type = GetType(); type+=0;
    wxWindow *wnd_prnt = GetParentWindow();

    // Iterate over linked list of children
    for( SizerIter sit(node); rli=(wxRlItem*)sit.Get(); sit.Step() ){
        // If we encounter non RlItems (wxSizerItems), convert to wxRlItem
        if( !::IsRlItem(rli) )
            rli = ConvertToRlItem( sit, rli, this );
        rli->m_parent = this;
        if( rli->IsSizer() )
            rli->UpdateItem( has_bg );
        else if( rli->IsWindow() ){
            // If a window 'escapes' we reparent it back here (happened in wxGTK)
        	if( wnd_prnt && rli->GetWindow()->GetParent()!=wnd_prnt ){
        		wxLogMessage(_T("wxRlItem::UpdateItem - NOTE: Reparenting window to main window"));
                rli->GetWindow()->Reparent( wnd_prnt );
        	}
        	// This is if a window has incorrect containing sizer pointer (should not happen)
            if( rli->GetWindow()->GetContainingSizer()!=node ){
                if( rli->GetWindow()->GetContainingSizer() && node )
                    wxLogMessage(_T("wxRlItem::UpdateItem - NOTE: Correcting wxWindow - containing sizer"));
                if( node )    
                    rli->GetWindow()->SetContainingSizer(node);
            }
        }
        
        // If sizer/window with no area, make it a little visible, otherwise it 
        // 'hampers' the layout
        if( rli->IsEmptyContent() ){
			// An empty sizer, remove it with a log message
            wxLogMessage(_T("wxRlItem::UpdateItem - NOTE: Destroying empty RlWindow/RlSizer at at %s"), (const wxChar*)rli->GetAddress() );
			rli->Destroy( );
			sit.InitSetDirection( );
			continue;
        	/*
        	wxSize sz = rli->GetMinSize();
        	if( !sz.IsFullySpecified() || !sz.x || !sz.y )
        		rli->SetMinSize( wxSize(RL_WINDOW_MIN_SIZE,RL_WINDOW_MIN_SIZE));
        	if( !rli->GetProportion() && !(rli->GetFlag()&wxFIXED_MINSIZE) )
        		rli->wxSizerItem::SetProportion( 1 );
        	rli->SetFlag( rli->GetFlag()|wxEXPAND );
        	*/
        }
        
        if( !(rli->GetFlag()&wxEXPAND) )
            flags &= ~wxEXPAND;
        prop_sum += rli->GetProportion();
    }

    if( !has_bg ){
	    if( m_parent && (m_parent->GetFlag()&wxBOTH)!=(GetFlag()&wxBOTH) ){
	        // Normal case. (vert in horz or horz in vert)
	        if( !(flags&wxEXPAND) ){
	            wxSizerItem::SetProportion(0);
	        }
	        if( !prop_sum )
	            SetFlag( GetFlag()&~wxEXPAND );
	    }
	    else{
	        // vert in vert or horz in horz
	        if( !prop_sum ){
	            wxSizerItem::SetProportion(0);
	        }
	        if( !(flags&wxEXPAND) )
	            SetFlag( GetFlag()&~wxEXPAND );
	    }
    }
 
	// Do simplifications of layout
	if( type==RlSizer ){
	     // Do simplification of the layout
	    wxRlItem *gcrli;	// Grandchildren
	    for( SizerIter sit(node); rli=(wxRlItem*)sit.Get(); sit.Step() ){
	    	
    		if( rli->GetType()==RlSizer && !(rli->m_capab&RL_CAPAB_ENTITY) ){
	    		// Vert in vert or horz in horz?
    			// Or a sizer with just one member? (we can treat it as a list though)
       			// Or if we only contain one RlSizer (then we have to change orientation)
    			int g_child_cnt = rli->GetSizer()->GetChildren().GetCount();
    			if( !g_child_cnt ){
    				// An empty sizer, remove it with a log message
                    wxLogMessage(_T("wxRlItem::UpdateItem - NOTE: Destroying empty RlSizer at %s"), (const wxChar*)rli->GetAddress() );
    				rli->Destroy( );
	    			// Re-init iterator after removing item
	    			sit.InitSetDirection( );
	    			continue;
    			}
    			int child_cnt = GetSizer()->GetChildren().GetCount();
    			int prop = rli->GetProportion();
    			bool same_dir = (GetFlag()&wxBOTH)==(rli->GetFlag()&wxBOTH);
    			bool same_dir_final = child_cnt>1 ? same_dir : !same_dir;	// Compensate for changing orientation after loop
    			if( same_dir || g_child_cnt==1 || child_cnt==1 ){
					int ix_insert = sit.GetIndex()+1;
					int prop_sum = GetPropSum( rli );
					
	    			// Insert items in this sizer
					wxSizerItemList &sl = rli->GetSizer()->GetChildren();
					wxSizerItemList::compatibility_iterator llnode, llnext;
	    			for( llnode = sl.GetFirst(); llnode; llnode=llnext ){
	    				llnext = llnode->GetNext();
	    				gcrli = (wxRlItem*)llnode->GetData();
	    				gcrli->m_parent = this;
	    				GetSizer()->Insert( ix_insert++, gcrli );
	    				sl.DeleteObject( gcrli );	// Only deletes from list
	    				
    					bool ne = !(gcrli->GetFlag()&wxEXPAND);
    					bool np = !GetPropSum(gcrli);
	    				if( prop_sum )
	    					gcrli->wxSizerItem::SetProportion( (gcrli->GetProportion()*prop)/prop_sum );
	    				else
	    					gcrli->wxSizerItem::SetProportion( prop/g_child_cnt );
	    				if( !same_dir_final ){
	    					if( ne != np ){
	    						gcrli->SetFlag( (gcrli->GetFlag()&~wxEXPAND) | (ne?wxEXPAND:0) );
	    						if( ne ) gcrli->wxSizerItem::SetProportion( 0 );
	    					}
	    				}
	    			}
	    			
	    			// See if we have to change orientation (usually for root item)
	    			if( child_cnt==1 && (rli->GetFlag()&wxBOTH)!=(GetFlag()&wxBOTH) ){
	    				 SetFlag( (GetFlag()&~wxBOTH) | (rli->GetFlag()&wxBOTH) );
	    				 ((wxBoxSizer*)GetSizer())->SetOrientation(rli->GetFlag()&wxBOTH);  
	    			}

	    			if( !rlmgr ){
	    				rlmgr = wxRlManager::GetRlManagerOf(this);
	    				wxASSERT( rlmgr );
	    			}
	    			// node->Remove( sit.GetIndex() );
	    			TrashRlItem( node, sit.GetIndex(), rlmgr );
	    			
	    			// Re-init iterator after removing item
	    			sit.InitSetDirection( );
    			}
       		}
    	}
	}
}

void wxRlItem::SetState( int state ){
	wxASSERT( state<0x10000 );
	m_state = (short)state;
}

void wxRlItem::SetIsEntity( ){
    m_capab |= RL_CAPAB_ENTITY;
}

wxRlItem &wxRlItem::SetProportion( int prop ){
	// When explicitely setting proportion, we adjust the wxFIXED_MINSIZE
	// flag if the proportion goes to 0. 
	// If we have proportion 0 and no wxFIXED_MINSIZE, it is OK to set 
	// proportion when moving items between sizers
	int flag = wxSizerItem::GetFlag();
	if( !prop )
		flag |= wxFIXED_MINSIZE;
	else 
		flag &= ~wxFIXED_MINSIZE;
	wxSizerItem::SetFlag( flag );
	wxSizerItem::SetProportion( prop );
	
	return *this;
}

void wxRlItem::CopyGeometryFrom( wxRlItem *rli ){
	wxASSERT( rli );
	SetMinSize( rli->GetMinSize() );
	wxSizerItem::SetProportion( rli->GetProportion() );
	SetFlag( (GetFlag()&~wxGEOMETRY_MASK) | (rli->GetFlag()&wxGEOMETRY_MASK) );
}


wxString wxRlItem::CheckLayout( ){
    wxSizer *node = GetSizer();
    if( !node ) return wxEmptyString;
    
    wxWindow *wnd_prnt = GetParentWindow();
    wxRlItem *rli;
    wxString s;
    for( SizerIter sit(node); rli=(wxRlItem*)sit.Get(); sit.Step() ){
    	if( rli->IsWindow() ){
    		if( rli->GetWindow()->GetParent()!=wnd_prnt && wnd_prnt ){
	    		wxRect rct = rli->GetRect();
	    		s += wxString::Format(_T("RlItem %x (x:%d,y:%d,w:%d,h:%d), Wnd %x has incorrect parent %x [expected %x]"), 
	    			rli, rct.x, rct.y, rct.GetWidth(), rct.GetHeight(),
	    			rli->GetWindow(), rli->GetWindow()->GetParent(), wnd_prnt ); 
    		}
    		wxSizer *pcsz = rli->GetWindow()->GetContainingSizer(); 
    		if( pcsz && pcsz!=node ){
	    		wxRect rct = rli->GetRect();
	    		s += wxString::Format(_T("RlItem %x (x:%d,y:%d,w:%d,h:%d), Wnd %x - wrong containing sizer %x [expected %x]"), 
	    			rli, rct.x, rct.y, rct.GetWidth(), rct.GetHeight(),
	    			rli->GetWindow(), pcsz, node ); 
    		}
    	}
    	if( ::IsRlItem(rli) && rli->IsSizer() )
    		s += rli->CheckLayout( );
    }
    return s;
}

// Standard render function for wxRlItem
// Hand it over to the painter and do the border
void wxRlItem::Render( wxDC *dc ){
    // Retrieve our rectangle
    wxRect r = GetRect();

    // Render contents - also for sizers (provide background, ...)
    wxRlManager::GetPainter()->Render( dc, this, r );
}

void wxRlItem::RenderBorder( wxDC *dc ){
    int bs = GetBorder();
    if( bs ){
    	wxRect r = GetRect();
        int flag = GetFlag();
        if( flag&wxTOP ) r.y-=bs, r.height+=bs;
        if( flag&wxLEFT ) r.x-=bs, r.width+=bs;
        if( flag&wxBOTTOM ) r.height+=bs;
        if( flag&wxRIGHT ) r.width+=bs;
        wxRlManager::GetPainter()->RenderBorder( dc, flag, bs, r );
    }
}


void wxRlItem::MouseEvent( int mouse, const wxPoint &pt ){
	// Default behaviour, report if we should or generate a wxEvent
	if( RL_REASON(mouse)!=RL_MOUSE_MOVE && 
	    (m_capab&(RL_CAPAB_MOUSE_REPORT|RL_CAPAB_MOUSE_WXEVENT)) )
		RlNotify( this, mouse, pt );
}

void wxRlItem::RlNotify( wxRlItem *rli_hit, int mouse, const wxPoint &pt ){
	// Do we report to the outside world when mouse happens to us?
	if( m_capab&RL_CAPAB_MOUSE_WXEVENT && RL_REASON(mouse)!=RL_MOUSE_MOVE ){
    	// Fire an event that something happened to us
		wxWindow *wnd = GetParentWindow();
		wxASSERT(wnd);
		wxCommandEvent event( wxEVT_RL_MOUSE, wnd->GetId() );
	    event.SetEventObject( this );
	    // The containing pane
	    event.SetClientData( m_parent ? m_parent->GetSizer() : NULL );
	    event.SetInt( mouse );
	    wnd->AddPendingEvent( event );
	}
	
	// We always report requests. Also, propagate mouse event if we have 
	// the MOUSE_PROPAGATE flag set.
	if( m_parent ){
		if( mouse&RL_REQUEST_MASK || m_capab&RL_CAPAB_MOUSE_PROPAGATE || rli_hit==this )
			m_parent->RlNotify( rli_hit, mouse, pt );
	}
	else{
		// Top level item
		if( RL_REQUEST(mouse)==RL_REQUEST_REPAINT ){
			wxRlManager *rl_mgr = wxRlManager::GetRlManagerOf(this);
			wxASSERT(rl_mgr);
			wxDC *pdc = rl_mgr->GetClientDc(this);
			if( pdc ){
				rl_mgr->RenderSizerItem(pdc,this);
				delete pdc;
			}
		}
	}
}


bool wxRlItem::GetXmlAttribs( wxString &xml, int options ){
	// Set up default values. If it matches, no need to write to file
	// These matches the default load/creation parameters 
	int def_prop = IsWindow() ? 0 : RL_DEF_PROP;
	int def_border = IsWindow() ? RL_DEF_WINDOW_BORDER : 0;
	DWORD def_flags = IsWindow() ? (DWORD)RL_DEF_WINDOW_FLAGS : (DWORD)wxEXPAND;
	
    // These are attribs saved since loading
    wxRlXmlTags *prxt = wxDynamicCast( GetUserData(), wxRlXmlTags );
    if( prxt ){
    	for( int ix=0; ix<(int)prxt->m_attribs.GetCount(); ix+=2 )
    		xml += prxt->m_attribs[ix]+_T("=\"")+prxt->m_attribs[ix+1]+_T("\" ");
    }

    // The static box sizer is a special case
    if( GetType()==RlStaticBoxSizer ){
    	xml += _T("type=\"staticbox\" ");
    	wxStaticBoxSizer *psbsz = wxDynamicCast(GetSizer(),wxStaticBoxSizer);
    	if( psbsz )
    		xml += _T("label=\"") + psbsz->GetStaticBox()->GetLabel() + _T("\" ");  
    }
    
    if( options&(RL_XML_VERBOSE|RL_XML_ALL_DETAILS) )
    	xml += wxString::Format( _T("rltype=\"%d,%s\" "), GetType(), Type2Str() );
    else{
    	// Don't write type when it is default
    	if( (IsSizer() && (GetType()!=RlSizer && GetType()!=RlStaticBoxSizer)) || 
    		(IsSpacer() && GetType()!=RlSpacerEnum) || 
    		(IsWindow() && GetType()!=RlWindow) )
    		xml += wxString::Format( _T("rltype=\"%s\" "), Type2Str() );
    }

    if( m_capab!=RlType2Capab(m_type) )
    	xml += wxString::Format( _T("capab=\"0x%x\" "), m_capab );
    
    // If writing layout to file, we want to use a minimalistic syntax
    // For the flags, we usually only need to keep horiz/vert info
    if( GetType()!=RlAppPane || options&RL_XML_ALL_DETAILS ){
	    if( GetSizer() && GetType()!=RlSizerUnk && !(options&(RL_XML_VERBOSE|RL_XML_ALL_DETAILS)) ){
	    	if( GetType()!=RlSizer || !m_parent || GetOrientation(GetSizer())==GetOrientation(m_parent->GetSizer()) )
	    		xml += wxString::Format( _T("orient=\"%s\" "), GetFlag()&wxVERTICAL?_T("vert"):_T("horz") );
	    }
	    // Expand is set by default for sizers & spacers, but not for windows
	    if( (GetWindow() && GetFlag()&wxEXPAND) || (!GetWindow() && !(GetFlag()&wxEXPAND)) ){
	    	xml += _T("expand=\"");
	    	xml += GetFlag()&wxEXPAND?_T("1"):_T("0");
	    	xml += _T("\" ");
	    }
	    	
	    if( ((GetFlag()^def_flags)&~(wxBOTH|wxEXPAND|wxBORDER_MASK)) || (options&RL_XML_ALL_DETAILS) ){
		    if( !(options&RL_XML_VERBOSE) )
		        xml += wxString::Format( _T("flag=\"0x%X\" "), GetFlag() );
		    else
		        xml += wxString::Format( _T("flag=\"0x%X,%s\" "), GetFlag(), wxFlag2Str(GetFlag()) );
	    }
    }

    if( GetProportion()!=def_prop || options&RL_XML_ALL_DETAILS ){
    	// We divide proportion by 100 to get something more managable when editing dialog xml
    	// Also, don't let it drop below 1 and express 1 as a special value
    	wxString s;
    	int p = GetProportion();
    	if( p==0 ) s=_T("0");
    	else if( p==1 ) s=_T("min");
    	else{
    		p /= 100;
    		if( !p ) p=1;
    		s = wxString::Format(_T("%d"),p);
    	}
    	xml += _T("prop=\"") + s + _T("\" "); 
    }

    if( GetBorder()!=def_border || GetFlag()&wxBORDER_MASK || options&(RL_XML_VERBOSE|RL_XML_ALL_DETAILS) ){
    	wxString s;
    	if( GetBorder()!=def_border ) s+= wxString::Format(_T("%d,"),GetBorder() );
    	if( (GetFlag()&wxALL_DIR)!=wxALL_DIR ){
    		if( GetFlag()&wxLEFT ) s += _T("L");
    		if( GetFlag()&wxRIGHT ) s += _T("R");
    		if( GetFlag()&wxTOP ) s += _T("T");
    		if( GetFlag()&wxBOTTOM ) s += _T("B");
    		s += _T(",");
    	}
    	if( GetFlag()&wxRAISED_BORDER ) s+=_T("raised,");
    	if( GetFlag()&wxSIMPLE_BORDER ) s+=_T("simple,");
    	if( GetFlag()&wxSTATIC_BORDER ) s+=_T("static,");
    	if( GetFlag()&wxSUNKEN_BORDER ) s+=_T("sunken,");
    	if( GetFlag()&wxDOUBLE_BORDER ) s+=_T("double,");
    	if( s.EndsWith(_T(",")) ) s.RemoveLast();
    	if( s.Length() )
    		xml += _T("border=\"") + s + _T("\" ");
    }

    if( options&RL_XML_ALL_DETAILS ){
        wxRect r = GetRect();
        wxSize sz = GetMinSize();
        xml += wxString::Format( _T("rect=\"x:%d,y:%d,w:%d,h:%d\" "), r.x, r.y, r.width, r.height );
        xml += wxString::Format( _T("min=\"w:%d,h:%d\" "), sz.x, sz.y );
        xml += wxString::Format( _T("self=\"0x%x\" "), (int)this );
        xml += wxString::Format( _T("parent=\"0x%x\" "), m_parent );
    }

    // The name
    wxString name;
    name = wxSizer2RlItem::GetName(this);
    if( name.Length() )
    	xml += wxString::Format( _T("name=\"%s\" "), (const wxChar*)name );
    
    return true;
}

// Output layout as an XML string
bool wxRlItem::ToXml( wxString &xml, int options, int rec_lvl ){
	wxString indent;
    for( int i=0; i<rec_lvl; i++ )
        indent += _T("  ");
    xml += indent;
    
    const wxChar *pclose_tag = NULL;
    if( IsSpacer() ) 	 	xml += _T("<space "), pclose_tag = _T("</space>");
    else if( IsSizer() ) 	xml += _T("<sizer "), pclose_tag = _T("</sizer>");
    else if( IsWindow() )	xml += _T("<window "), pclose_tag = _T("</window>");
    else wxASSERT(0);
    GetXmlAttribs( xml, options );

    // See if we have XML to insert from creation
    wxRlXmlTags *prxt = wxDynamicCast( GetUserData(), wxRlXmlTags );
    wxSizer *sizer = GetSizer();
    // This prints type specific info as 'sub XML' 
    wxString config = ConfigToXml();
    bool did_close = false;
    
    if( sizer && (!(GetCapab()&RL_CAPAB_ENTITY) || options&RL_XML_FULL_RECURSION) ){        
    	xml += _T(">\n");
        if( config.Length() ){
        	xml += indent+_T("  ");
        	xml += config +_T("\n") + indent; 
        }
        if( prxt ) xml += prxt->m_sub_nodes;	// Saved nodes since loading
        
        // Iterate over children
        wxRlItem *rli;
        static int ix_debug;
        for( SizerIter sit(sizer); rli=(wxRlItem*)sit.Get(); sit.Step() ){
            if( ::IsRlItem(rli) && (!(options&RL_XML_ENUMERABLE) || rli->IsEnumerable()) )
                rli->ToXml( xml, options, rec_lvl+1 );
            ix_debug++;
        }
        for( int i=0; i<rec_lvl; i++ ) xml += _T("  ");
    }
    else{
    	if( !config.Length() && (!prxt || !prxt->m_sub_nodes.Length()) ){
    		did_close = true;
    		xml += _T("/>\n");
    	}
    	else{
            xml += _T(">\n");
            if( config.Length() ){
            	xml += indent+_T("  ");
            	xml += config +_T("\n") + indent; 
            }
            if( prxt ) xml += prxt->m_sub_nodes;	// Saved nodes since loading
    	}
    }

    if( !did_close ){
    	xml += pclose_tag; 
    	xml += _T("\n");
    }
    
    return true;
}


#include "utils/StrUtils.h"
#include "utils/CharBuf.h"

#define FIA_LR 1	// Check left & right sides
#define FIA_TB 2	// Check top & bottom sides
#define FIA_INIT_DIST 0x7FFFFFFF	// Initial best distance (max int)

bool TestInsertRect( wxPoint &pt, wxRect &r, wxString &sbest, int &dbest, int dir, int side  ){
	bool bret = false;
	int d, dx, dy; 
	if( dir&FIA_LR ){
		dy = pt.y-(r.y+r.height/2);
		dx = pt.x-(side?r.x+r.width:r.x);
		d = dy*dy+dx*dx;
		if( d<=dbest ){
			dbest = d;
			sbest = side ? _T("R") : _T("L");
			bret = true;
		}
	}		
	
	if( dir&FIA_TB ){
		dx = pt.x-(r.x+r.width/2);
		dy = pt.y-(side?r.y+r.height:r.y);
		d = dy*dy+dx*dx;
		if( d<=dbest ){
			dbest = d;
			sbest = side ? _T("B") : _T("T");
			bret = true;
		}
	}		
	
	return bret;
}

// Recursive part of FindInsertionAddress
wxString wxRlItem::FindInsertionAddress( wxPoint &pt, wxRlItem *rli_insert, int &dbest ){
	// Initial entry into this wxRlItem (false when recursive call)
	bool is_init_entry = (dbest==FIA_INIT_DIST);
	
	// Find direction to do self check
	int dir_self_check = FIA_LR|FIA_TB;
	if( m_parent ){
		if( (GetType()==RlSizer) && !(m_capab&RL_CAPAB_ENTITY) )
			dir_self_check = 0;			
		else
			dir_self_check = GetOrientation(m_parent->GetSizer())==wxHORIZONTAL ? FIA_TB : FIA_LR;
	} 
	else if( GetType()==RlSizer )
		dir_self_check = GetOrientation(GetSizer())==wxHORIZONTAL ? FIA_TB : FIA_LR;

	// Prevent inserting items that don't scale next to items that are 
	// larger in non-scaling direction. NOTE: The same applies when this
	// item has no porportion in a certain dir.
	for( wxRlItem *rtmp=rli_insert; rtmp; ){
		if( !rtmp->GetProportion() ){
			if( rtmp->GetFlag()&wxHORIZONTAL )
				dir_self_check &= ~FIA_LR;
			if( rtmp->GetFlag()&wxVERTICAL )
				dir_self_check &= ~FIA_TB;
		}
		rtmp = rtmp==this?NULL:this;
	}
	
	wxRect r = GetRect( );
	wxString sret;
	// A trick to make the focus point of these rectangles
	// fall outside of the contained items
	r.Inflate( 20, 20 );	
							
	// This tests inserting at the same level as the item itself
	// (above/below or left/right)
	TestInsertRect(pt,r,sret,dbest,dir_self_check,0);
	TestInsertRect(pt,r,sret,dbest,dir_self_check,1);

	
	// If no sizer, or if recursively trying to enter 'separate entity', then return
	if( !GetSizer() || /*!GetRect().Contains(pt) ||*/  
		(m_capab&RL_CAPAB_ENTITY && !is_init_entry) )
		//if( GetType()!=RlSizer || m_capab&RL_CAPAB_ENTITY )
		return sret;

	
	// Test inserting inside the sizer (in each user position)
	int dir = GetOrientation(GetSizer())==wxHORIZONTAL ? FIA_LR : FIA_TB;
	int ix = 0;
	if( !rli_insert->GetProportion() ){
		// Disable inserting item into sizer where it cannot adjust
		if( rli_insert->GetFlag()&wxHORIZONTAL )
			dir &= ~FIA_LR;
		if( rli_insert->GetFlag()&wxVERTICAL )
			dir &= ~FIA_TB;
	}
    for( SizerIter sit(GetSizer()); sit.Get(); sit.Step() ){
        wxRlItem *rli = ToRlItem(sit.Get());
    	if( !rli || !rli->IsEnumerable() /*|| (rli->m_capab&RL_CAPAB_ENTITY)*/ ) 
            continue;
		r = rli->GetRect();
		/*if( r.Contains(pt) )*/{
			if( TestInsertRect(pt,r,sret,dbest,dir,0) )
				sret = wxString::Format(_T("#%d"),ix);
			if( TestInsertRect(pt,r,sret,dbest,dir,1) )
				sret = wxString::Format(_T("#%d"),ix+1);
			wxString s = rli->FindInsertionAddress(pt,rli_insert,dbest);
			if( s.Length() )
				sret = wxString::Format(_T("%d,"),ix)+s;
		}
		ix++;
    }
    
    return sret;
}


// Starting point for best address with given point
wxString wxRlItem::FindInsertionAddress( wxPoint &pt, wxRlItem *rli_insert ){
	int dbest = FIA_INIT_DIST;
	return FindInsertionAddress( pt, rli_insert, dbest );
}


// Iterator cache
int g_rl_iter_ix = -1;
wxRlItem *g_rl_iter_base;
ExpArr<wxSizerItemList::compatibility_iterator> g_rl_iter_nodes;


int wxRlItem::First( int ){
	if( !GetSizer() ) return -1;
	g_rl_iter_ix = 0;
	g_rl_iter_base = this;
	// For the int case we don't need the stack really, but it doesn't harm
	g_rl_iter_nodes.Empty();
	g_rl_iter_nodes.Push( GetSizer()->GetChildren().GetFirst() );
	// Step past non wxRlItem:s
	while( g_rl_iter_nodes.Top() && !ToRlItem(g_rl_iter_nodes.Top()->GetData()) )
		g_rl_iter_nodes.Top() = g_rl_iter_nodes.Top()->GetNext();
	
	return 0;
}

wxRlItem *wxRlItem::First( wxRlItem * ){
	if( !GetSizer() ) return NULL;
	First( 0 );
	return g_rl_iter_base ? ToRlItem(g_rl_iter_nodes.Top()->GetData()) : NULL;
}

int wxRlItem::Next( int ix ){
	if( g_rl_iter_ix!=ix || g_rl_iter_base!=this ){
		First( 0 );
		while( ix-->=0 )
			Next( g_rl_iter_ix );
	}
	else{
		g_rl_iter_ix++;
		// Go to next
		if( g_rl_iter_nodes.Top()  )
			g_rl_iter_nodes.Top() = g_rl_iter_nodes.Top()->GetNext();
		// Step past non wxRlItem:s
		while( g_rl_iter_nodes.Top() && !ToRlItem(g_rl_iter_nodes.Top()->GetData()) )
			g_rl_iter_nodes.Top() = g_rl_iter_nodes.Top()->GetNext();
	}
	return g_rl_iter_ix;
}

wxRlItem *wxRlItem::Next( wxRlItem *rli ){
	wxASSERT( !rli || (g_rl_iter_nodes.Size() && g_rl_iter_nodes.Top()->GetData()==rli) );
	
	// First try to go downwards
	while( rli->GetSizer() && rli->GetSizer()->GetChildren().GetFirst() ){
		g_rl_iter_nodes.Push( rli->GetSizer()->GetChildren().GetFirst() );
		rli = ToRlItem( g_rl_iter_nodes.Top()->GetData() );
		if( rli->IsRlItem() ) return rli;
	}
	
	while( g_rl_iter_nodes.Size() ){
		// Try going to the side
		wxSizerItemList::compatibility_iterator next = g_rl_iter_nodes.Top()->GetNext();  
		if( next ){
			while( next && !ToRlItem(next->GetData()) )
				next = next->GetNext();
			if( next ){
				g_rl_iter_nodes.Top() = next;
				return ToRlItem(next->GetData());
			}
		}
		// Pop an element of the node stack (go to parent)
		g_rl_iter_nodes.Pop();
	}
	
	return NULL;
}

bool wxRlItem::IndexValid( int ix ){
	if( ix<0 || !GetSizer() || ix>=(int)GetSizer()->GetChildren().GetCount() ) 
		return false;
	return Elem(ix) ? true : false;
}

bool wxRlItem::IndexValid( wxRlItem *rli ){
	return rli && g_rl_iter_base->IsChild(rli);
}

wxRlItem *wxRlItem::Elem( int ix ){
	if( ix<0 || !GetSizer() ) return NULL;
	if( g_rl_iter_ix!=ix || g_rl_iter_base!=this ){
		First( 0 );
		while( ix-->0 )
			Next( g_rl_iter_ix );
	}
	return g_rl_iter_nodes.Top() ? ToRlItem(g_rl_iter_nodes.Top()->GetData()) : NULL;
}



// A bit of a hack, we want Unicode version of sc.h now:
#undef SC_FORCE_ANSI
#undef SC_H
#include "utils/sc.h"


RlPainter *wxRlManager::GetPainter(){
    return ms_painter ? ms_painter : &g_stub_painter;
}

void wxRlManager::SetPainter( RlPainter *painter ){
    ms_painter = painter;
}

wxRlManager *wxRlManager::FloatPane( const wxChar *name, wxWindow *wnd, wxPoint offset ){
	return Float( FindAppPane(name), wnd, offset );
}
	
wxRlManager *wxRlManager::Float( wxRlItem *rli, wxWindow *wnd, wxPoint offset ){
	if( !m_wnd || !rli ) return NULL;
	
	// Save original position
	wxRect rct = rli->GetRect( );
	wxRect rct_root = m_root->GetRect();
	
	// Find where to put floating window
	wxPoint pt_flt( rct.x+offset.x, rct.y+offset.y );
	pt_flt = m_wnd->ClientToScreen( pt_flt ); 
	wxSize sz_flt( rct.width, rct.height );
	
	int flags = wxRESIZE_BORDER/*|wxSTAY_ON_TOP*/;
	
	// Creating the floating window 
	// For wxGTK, we have patched src/gtk/MiniFram.cpp so that it can give us 
	// miniframes without the builtin caption
	if( !wnd ) 
		wnd = new wxMiniFrame( m_wnd, wxID_ANY, _T(""), pt_flt, sz_flt, wxRESIZE_BORDER/*|wxSTAY_ON_TOP*/ );
	else
		// Application has created a window for us to use - position it
		wnd->SetSize( pt_flt.x, pt_flt.y, sz_flt.x,sz_flt.y );
	
	// Create a manager for the new window
	wxRlManager *prlm = new wxRlManager( wnd, wxVERTICAL );
	if( !prlm || !wnd ){
		if(wnd) wnd->Destroy();
		return NULL;
	}

	// Save original position
	prlm->m_pdock_info = new wxRlDockInfo( rli->GetAddress(),
			  						(float)(rct.x+rct.width/2)/rct_root.width,
			  						(float)(rct.y+rct.height/2)/rct_root.height ); 
	
	// Detach from previous layout and attach to new manager as root item
	rli->DetachInternal(true);
	prlm->SetRoot( rli );	// This will also update the layout for new window
	wnd->Show( true );

	// Update main layout
	Update( );
	
	return prlm;
}

bool wxRlManager::Dock( wxRlManager *prlm, wxString addr, wxPoint pt_dock ){
	if( !prlm ) return false;
	
	// See if we should use original address
	if( addr==wxEmptyString && prlm->m_pdock_info )
		addr = prlm->m_pdock_info->m_address;
	if( pt_dock==wxDefaultPosition && prlm->m_pdock_info ){
		wxRect rct = m_root->GetRect();
		pt_dock = wxPoint( int(rct.width*prlm->m_pdock_info->m_rel_pos_x), int(rct.height*prlm->m_pdock_info->m_rel_pos_y) );
	}
	
	// Find an insertion address
	wxRlItem *rli = prlm->m_root;
	if( !m_root->InsertAt(rli,addr) ){
		// Try to get address from the position 
		addr = m_root->FindInsertionAddress( pt_dock, rli );
		if( !m_root->InsertAt(rli,addr) )
			return false;
	}

	// have to reparent back to this managers window	
	ReparentSizerWindows( rli->GetSizer(), m_wnd );

	// Make sure layout is coherent in new place
	rli->UpdateItem( false );
	
	// Usually one want to activate the newly docked window
	// This is also to keep state synced (it was active prior to docking)
	if( rli->GetState()&RL_STATE_ACTIVE )
		SetActive( rli, true, false );
	
	
	// This is to stop the manager from sending messages before destroyed
	prlm->m_did_init_layout = false;

	// Should be empty now
	wxASSERT( !prlm->m_root || !prlm->m_root->GetSizer() );

	// delete and destroy in the right order
	// Note: Window manager has taken over ownership of these now, so we leave them.  
	//wxWindow *wnd = prlm->GetWindow();
	//delete prlm;
	//wnd->Destroy();
	
	Update( );
	
	return true;
}


// # Are these events safe here? 
BEGIN_EVENT_TABLE(wxRlManager, wxEvtHandler)
    EVT_TIMER(wxID_ANY, wxRlManager::OnTimer)
    EVT_LEAVE_WINDOW(wxRlManager::OnLeaveWindow)
END_EVENT_TABLE()


wxMouseEvent g_rl_dummy_mouse_evt;

wxRlManager::wxRlManager(wxWindow * wnd, int init_sizer_flag, RlUiArea default_bg)
        : wxEvtHandler(), m_cursor_drag_edge_v(wxCURSOR_SIZENS), 
        m_cursor_drag_edge_h(wxCURSOR_SIZEWE), m_cursor_drag_pane(wxCURSOR_SIZING), 
        m_mouse_evt(g_rl_dummy_mouse_evt) {
	g_rl_mgrs.Push( this );
    m_wnd = NULL;
    //m_flags = 0;
    //m_mouse_evt = 0;
    m_mouse_over = NULL;
    m_active = NULL;
    m_app_is_active = false;
    m_did_init_layout = false;
    m_no_paint = false;
    SetWindow( wnd );
    m_mouse_action = RlNoAction;
    m_wnd_cursor = NULL;
    m_pane_drag_info = NULL;
    m_action_node = NULL;
    m_timer_cursor.SetOwner(this);
    m_timer_drag.SetOwner(this);
    m_timer_debug.SetOwner(this);
    m_timer_leave.SetOwner(this);
    m_timer_debug.Start( 1000, false );
    m_active_from_event = true; 	// Top level windows will change this automatically.
									// used to disable invalid activation messages (OnChildFocus)
    m_root = NULL;
    m_pdock_info = NULL;
    
    InitRootItem( init_sizer_flag, default_bg );
}

// This will NOT delete the managed window
wxRlManager::~wxRlManager(){
	Dtor();
}

void wxRlManager::Dtor(){
	g_rl_mgrs.RemoveUnordered( this );
	if( g_rl_mgr_last_active==this )
		g_rl_mgr_last_active = NULL;
	// This will NOT delete the managed window
	SetWindow(NULL);
	delete m_root;
	delete m_pdock_info;
	delete m_pane_drag_info;
}

wxRlManager *wxRlManager::GetRlManagerOf( wxRlItem *rli ){
	if( !rli ) return NULL;
	rli = GetRootItemOf(rli);
	
	// This will work almost always
	if( g_rl_mgr_last_active && g_rl_mgr_last_active->m_root==rli )
		return g_rl_mgr_last_active;
	
	// So search all
	return g_rl_mgrs.Find( rli );
}

wxRlManager *wxRlManager::GetRlManagerOf( wxWindow *wnd ){
	if( !wnd ) return NULL;
	
	// This will work almost always
	if( g_rl_mgr_last_active && g_rl_mgr_last_active->m_wnd==wnd )
		return g_rl_mgr_last_active;
	
	// So search all - and recursively for parent windows
	while( wnd ){
		wxRlManager *prlm = g_rl_mgrs.Find( wnd );
		if( prlm ) return prlm;
		wnd = wnd->GetParent();
	}
	
	return NULL; 
}

// ExpArrP<wxRlContents*> m_contents;
bool wxRlManager::DetachContents( wxRlContentStore &cs, wxRlItem *rli ){
	if( !rli ) rli=m_root;
	wxASSERT(rli);

	wxRlContents *pc = NULL;
	for( int ix=0; true; ix++ ){
		wxRlItem *rli_content = rli->GetContents( ix );
		if( !rli_content ) break;
		rli_content = rli->DetachContents( ix );
		if( !rli_content ){
			wxLogMessage(_T("wxRlManager::DetachContents - Failed detaching content for item 0x%0X"),rli);
			break;
		}
		rli_content->Show( false );
		
		// Found some contents... 
		if( !pc ){
			wxString sname = RlGetNameOf(rli);
			if( !sname.Length() ){
				// The sizer is not yet named, so we do it now
				static int st_cnt;
				sname.Printf( _T("RlMgr-Auto-%d"), ++st_cnt );
			}
			cs.m_contents.Push( pc = new wxRlContents(sname) );
		}
		pc->m_rli_contents.Push(rli_content);
	}

	// Recursive bit - if entity tag, we don't enter (unless plain sizer)
	if( rli->GetSizer() && (rli->GetType()==RlSizer || !(rli->GetCapab()&RL_CAPAB_ENTITY)) ){
		for( SizerIter si(rli->GetSizer()); si.Get(); si.Step() ){
			wxRlItem *rli_sub = ToRlItem(si.Get());
			if( rli_sub && rli_sub->GetSizer() )
				DetachContents( cs, rli_sub );
		}
	}
	
	return true;
}


bool wxRlManager::AttachContents( wxRlContentStore &cs, wxRlItem *rli ){
	if( !rli ) rli=m_root;
	wxASSERT(rli);
	
	// We should iterate he other way around, go over the tree based in rli
	// and see what names we encounter.
	const wxChar *name = RlGetNameOf(rli);
	if( name ){
		int ix=cs.m_contents.Size(), cnt=0;
		// Starting from the end will use fresh things
		while( (ix=cs.m_contents.FindIndexOf(name,ix-1,-1))>=0 ){
			wxRlContents *pc = cs.m_contents[ix];
			// If some elements were taken out, we settle with inserting 
			// wherever there is an 'empty space' (instead of fix index)
			bool ordered = !cnt && !pc->m_used;
			// For all saved content items, put back in this wxRlItem
			for( int jx=pc->m_rli_contents.Size()-1; jx>=0; jx-- ){
				if( rli->SetContents(pc->m_rli_contents[jx], ordered?jx:-1) ){
					pc->m_rli_contents[jx]->Show( true );
					// It went well, so we don't own the item anymore
					pc->m_rli_contents.RemoveIndexOrdered(jx);
					pc->m_used = true;
					cnt++;
				}
			}
			// If we used all bits, remove this content
			if( !pc->m_rli_contents.HasItems() ){
				delete pc;
				cs.m_contents.RemoveIndexUnordered(ix);
				//ix--; // Going down, not needed
			}
		}
	}
	
	// Recursive bit - if entity tag, we don't enter (unless plain sizer)
	if( rli->GetSizer() && (rli->GetType()==RlSizer || !(rli->GetCapab()&RL_CAPAB_ENTITY)) ){
		for( SizerIter si(rli->GetSizer()); si.Get(); si.Step() ){
			wxRlItem *rli_sub = ToRlItem(si.Get());
			if( rli_sub && rli_sub->GetSizer() )
				AttachContents( cs, rli_sub );
		}
	}
	
	return true;
}

bool wxRlManager::VerifyNodeNames( wxRlItem *rli ){
	if( !rli ) rli=m_root;
	wxASSERT(rli);
	if( !rli->GetSizer() ) return false;
	
	// The logic is that a node that has content which it can detach, should 
	// also have a name so that it can later be re-attached
	if( rli->GetContents(0) ){
		wxString sname = RlGetNameOf(rli);
		if( !sname.Length() ){
			// The sizer is not yet named, so we do it now
			static int st_cnt;
			sname.Printf( _T("RlMgr-Auto-%d"), ++st_cnt );
			RlSetNameOf( rli, sname );
		}
	}
	
	// Recursive bit - if entity tag, we don't enter (unless plain sizer)
	if( rli->GetSizer() && (rli->GetType()==RlSizer || !(rli->GetCapab()&RL_CAPAB_ENTITY)) ){
		for( SizerIter si(rli->GetSizer()); si.Get(); si.Step() ){
			wxRlItem *rli_sub = ToRlItem(si.Get());
			if( rli_sub && rli_sub->GetSizer() )
				VerifyNodeNames( rli_sub );
		}
	}
	
	return true;
}



bool wxRlManager::GetState( wxString& state ){
	if( !m_root ) return false;
	return m_root->ToXml( state, /*RL_XML_STOP_AT_PANE|*/RL_XML_ENUMERABLE, 0 );
}

// Can turn off support for loading layouts in this way
#ifndef RL_NO_STATE
#include "wx/RlFromXml.h"

bool wxRlManager::SetState( const char *state, int opts, wxRlContentStore *pcs ){
	if( m_is_dialog ) opts |= RL_XMLLOAD_IS_DIALOG;
	wxRlItem *pi = wxRlXmlLoad( state, m_wnd, NULL, opts );
	if( pi ){
		if( pcs ) AttachContents( *pcs, pi );
		SetRoot( pi );
		return true;
	}
	else 
		return false;
}

#if wxUSE_UNICODE==1
bool wxRlManager::SetState( const wxChar *state, int opts, wxRlContentStore *pcs ){
    CharBuf cb = (const char*)wxString(state).mb_str(wxConvUTF8);
    return SetState( cb, opts, pcs );
}
#endif // wxUSE_UNICODE
#endif // RL_NO_STATE


void wxRlManager::SetWindow(wxWindow *wnd){
	if( m_wnd /*&& wnd!=m_wnd*/ ){
        //m_wnd->RemoveEventHandler(this);
	    //m_wnd->Disconnect( wxEVT_MOUSE_CAPTURE_LOST, 
	    //	(wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&wxRlManager::OnMouseCaptureLost );

		// It's good to disconnect event handlers, in case we go 
		// out of scope before the managed window. No stray events to dead objects.
        m_wnd->Disconnect( wxEVT_PAINT, wxEvtFnCast(Paint) &wxRlManager::OnPaint, NULL, this );
        m_wnd->Disconnect( wxEVT_SIZE, wxEvtFnCast(Size) &wxRlManager::OnSize, NULL, this );
        m_wnd->Disconnect( wxEVT_LEFT_DOWN, wxEvtFnCast(Mouse) &wxRlManager::OnLeftDown, NULL, this );
        m_wnd->Disconnect( wxEVT_LEFT_UP, wxEvtFnCast(Mouse) &wxRlManager::OnLeftUp, NULL, this );
        m_wnd->Disconnect( wxEVT_RIGHT_DOWN, wxEvtFnCast(Mouse) &wxRlManager::OnRightDown, NULL, this );
        m_wnd->Disconnect( wxEVT_RIGHT_UP, wxEvtFnCast(Mouse) &wxRlManager::OnRightUp, NULL, this );
        m_wnd->Disconnect( wxEVT_MOTION, wxEvtFnCast(Mouse) &wxRlManager::OnMotion, NULL, this );
        m_wnd->Disconnect( wxEVT_CHILD_FOCUS, wxEvtFnCast(ChildFocus) &wxRlManager::OnChildFocus, NULL, this );
        m_wnd->Disconnect( wxEVT_ACTIVATE, wxEvtFnCast(Activate) &wxRlManager::OnActivate, NULL, this );
        m_wnd->Disconnect( wxEVT_IDLE, wxEvtFnCast(Idle) &wxRlManager::OnIdle, NULL, this );
        m_wnd->Disconnect( wxEVT_MOUSE_CAPTURE_LOST, wxEvtFnCast(MouseCaptureLost) &wxRlManager::OnMouseCaptureLost, NULL, this );
	}
	
    // The flag name is slightly wrong. The meaning of it is whether the window 
    // does its own background painting or not, and whether we have some bothersome
    // wxStatixBox relationships to take into account. 
    // We set it for both wxPanels and wxDialogs
    m_is_dialog = wxDynamicCast(wnd,wxDialog) || wxDynamicCast(wnd,wxPanel);
    
    m_wnd = wnd;
    if( m_wnd ){
		m_wnd->Connect( wxEVT_MOUSE_CAPTURE_LOST, 
	    	(wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&wxRlManager::OnMouseCaptureLost );
        //m_wnd->PushEventHandler(this);
        m_wnd->SetAutoLayout(false);
        if( !m_is_dialog ){
        	//m_wnd->SetBackgroundColour( GetPainter()->GetBackgroundColour() );
        	//m_wnd->SetBackgroundColour( wxColour(255,0,0) );
        	m_wnd->SetBackgroundStyle( wxBG_STYLE_CUSTOM );
        }

        m_wnd->Connect( wxEVT_PAINT, wxEvtFnCast(Paint) &wxRlManager::OnPaint, NULL, this );
        m_wnd->Connect( wxEVT_SIZE, wxEvtFnCast(Size) &wxRlManager::OnSize, NULL, this );
        m_wnd->Connect( wxEVT_LEFT_DOWN, wxEvtFnCast(Mouse) &wxRlManager::OnLeftDown, NULL, this );
        m_wnd->Connect( wxEVT_LEFT_UP, wxEvtFnCast(Mouse) &wxRlManager::OnLeftUp, NULL, this );
        m_wnd->Connect( wxEVT_RIGHT_DOWN, wxEvtFnCast(Mouse) &wxRlManager::OnRightDown, NULL, this );
        m_wnd->Connect( wxEVT_RIGHT_UP, wxEvtFnCast(Mouse) &wxRlManager::OnRightUp, NULL, this );
        m_wnd->Connect( wxEVT_MOTION, wxEvtFnCast(Mouse) &wxRlManager::OnMotion, NULL, this );
        m_wnd->Connect( wxEVT_CHILD_FOCUS, wxEvtFnCast(ChildFocus) &wxRlManager::OnChildFocus, NULL, this );
        m_wnd->Connect( wxEVT_ACTIVATE, wxEvtFnCast(Activate) &wxRlManager::OnActivate, NULL, this );
        m_wnd->Connect( wxEVT_IDLE, wxEvtFnCast(Idle) &wxRlManager::OnIdle, NULL, this );
        m_wnd->Connect( wxEVT_MOUSE_CAPTURE_LOST, wxEvtFnCast(MouseCaptureLost) &wxRlManager::OnMouseCaptureLost, NULL, this );
    }
}

void wxRlManager::SetRoot(wxRlItem *item){
	// We want to keep border flags when changing root.
	int old_flags = m_root ? m_root->GetFlag()&wxBORDER_MASK : -1; 
    TrashRlItem(m_root,this);
    if( item )
        m_root = item;
    else
        m_root = new wxRlClientTempBg;
    // In frame mode we own the client area and do drawing there
    // We assume that we can decide over the border. 
    // In dialog mode, we don't have that possibility.
    if( old_flags!=-1 && m_root && !m_is_dialog ){
    	m_root->SetFlag( (m_root->GetFlag()&~wxBORDER_MASK) | (old_flags&wxBORDER_MASK) );
    	m_root->SetBorder(FlagToBorder(old_flags));
    }

    // If moving from another window, set correct parent
    if( m_root && m_root->GetSizer() )
    	ReparentSizerWindows( m_root->GetSizer(), m_wnd );
	
    Update();
}


void wxRlManager::DeleteNotify( wxRlItem *rli ){
	if( !rli ) return;
	//printf( "DeleteNotify %x\n", rli );

	// This is to clear out potential dangling pointers
	if( m_root==rli ) m_root=NULL;
	if( m_mouse_over==rli ) m_mouse_over=NULL;
	if( m_action_node==rli  ) m_action_node=NULL;
	for( int ix=0; ix<3; ix++ )
		if( m_action_items[ix]==rli ) 
			m_action_items[ix]=NULL;
	if( m_active==rli ) m_active=NULL;
}
	
void wxRlManager::DetachNotify( wxRlItem *rli ){
	DeleteNotify( rli );
	// Have to do it recursively, child items may also be pointed to
	if( rli->GetSizer() ){
		for( SizerIter sit(rli->GetSizer()); sit.Get(); sit.Step() )
			DetachNotify(ToRlItem(sit.Get()));
	}
}


void wxRlManager::InitRootItem( int init_sizer_flag, RlUiArea default_bg ){
    TrashRlItem(m_root,this);
    // Start with a single background item
    
    // Background denoted by RlClientTempBg disappears when
    // an item with some extent is put next to it.
    int border_flag = init_sizer_flag&wxBORDER_MASK;
    
    if( default_bg==RlClientBg )
        m_root = new wxRlItem(RlClientBg,border_flag);
    
    else if( default_bg==RlSpacerTemp )
        m_root = new wxRlItem(RlSpacerTemp,border_flag);
    
    else //if( default_bg==RlClientTempBg )
        m_root = new wxRlItem(RlClientTempBg,border_flag);

    // If init_orient is wxHORIZONTAL or wxVERTICAL,
    // then create a sizer around above background element
    if( init_sizer_flag&wxBOTH )
        m_root->Enclose( wxEXPAND|(init_sizer_flag^border_flag) );
}

void wxRlManager::SetRoot(wxSizer *sizer, int flag){
    wxRlItem *psi = new wxRlItem(sizer, RL_DEFAULT_PROPORTION, flag|wxEXPAND);
    SetRoot( psi );
}

void wxRlManager::Update( ){
    if( m_root ){
        // Go through all sizers, make sure their size constraint flags are OK
        wxASSERT( m_root->IsRlItem() );
        if( m_root->IsSizer() )
            m_root->UpdateItem( m_is_dialog );

        // Make sure we have OK size 
        wxRect r = m_wnd->GetClientRect( );
        m_root->SetDimension( wxPoint(r.x,r.y), wxSize(r.width,r.height) );
        m_did_init_layout = true;
        AdjustWindowMinSize();
        m_wnd->Refresh();
    }
}

void wxRlManager::SetActivePane( wxRlAppPane *pane, bool app_is_active ){
	SetActive( pane, app_is_active, false );
}

wxSizer *wxRlManager::FindSizer( const wxChar *name ){
	wxRlItem *rli = wxSizer2RlItem::Lookup(name);
	if( !rli || GetRootItemOf(rli)!=m_root ) return NULL;
	return rli->GetSizer();
}

wxRlItem *wxRlManager::FindItem( const wxChar *name ){
	wxRlItem *rli = wxSizer2RlItem::Lookup(name);
	if( !rli ) return NULL;
	// Also check that it's our child, since we're looking in the global map here
	// Cover the case when the requested item is currently dragged
	if( GetRootItemOf(rli)==m_root ||
		(m_pane_drag_info && rli->GetParentWindow()==m_pane_drag_info->m_wnd_drag) )
		return rli;
	else
		return NULL;
}


wxRlAppPane *wxRlManager::FindAppPane( const wxChar *name ){
	wxRlItem *rli = FindItem(name);
	if( !rli || rli->GetType()!=RlAppPane ) return NULL;
	return wxDynamicCast(rli,wxRlAppPane);
}

wxWindow *wxRlManager::FindToolBar( const wxChar *name ){
	wxRlItem *rli = wxSizer2RlItem::Lookup(name);
	if( !rli || rli->GetType()!=RlToolPane ) return NULL;
	if( !rli->GetSizer() || GetRootItemOf(rli)!=m_root ) return NULL;
	// # Verify this
	wxSizerItem *psz = rli->GetSizer()->GetItem(1);
	if( !psz ) return NULL;
	return psz->GetWindow();
}

/*
wxRlItem *wxRlManager::FindRlItemOf( wxSizer *sizer ){
	wxRlItem *rli = wxSizer2RlItem::Lookup(sizer);
	return GetRootItemOf(rli)==m_root ? rli : NULL;
}
*/

wxRlItem *wxRlManager::FindRlItemOf( wxWindow *wnd ){
    if( !wnd ) return NULL;
    if( wnd==m_root->GetWindow() ) return m_root;
    if( !m_root->GetSizer() ) return NULL;
    wxSizerItem *psi = m_root->GetSizer()->GetItem(wnd, true); 
    if( IsRlItem(psi) ) return (wxRlItem*)psi;
    return NULL;
}

wxRlItem *wxRlManager::FindItemAt( const wxPoint &pt ){
	return m_root ? m_root->FindItemAt( pt ) : NULL;
}


void wxRlManager::AdjustWindowMinSize(){
    if( !m_wnd || !m_root ) return;

    // Calculate new area for managed window
    // Including menus, statusbar, borders, ...etc.
    wxSize sz_tot, sz_orig, sz_cli, sz_root;
    sz_orig = m_wnd->GetSize();
    sz_cli = m_wnd->GetClientSize();
    sz_root = m_root->CalcMin();
    
    if( sz_orig.x<sz_cli.x || sz_orig.y<sz_cli.y ){
    	wxLogMessage(_T("wxRlManager::AdjustWindowMinSize - window size < client size - incoherent, skipping update") );
    	return;
    }

    // Adjust for new root size
    sz_tot = sz_orig;
    sz_tot.DecBy( sz_cli );
    sz_tot.IncBy( sz_root );

    // This is to avoid assertions. When potentially changing both min & max size, 
    // when items are added/removed, we do a mementary trick.
    m_wnd->SetMinSize( wxSize(20,20) );
    
    // If the root sizer has no proportion, or no wxEXPAND, then also set max size
	bool ps_zero = !GetPropSum(m_root);
	bool no_expand = !(m_root->GetFlag()&wxEXPAND); 
    if( ps_zero || no_expand ){
    	wxSize sz_max(sz_tot);
    	int orient = m_root->GetFlag()&wxBOTH;
    	if( !orient && (!ps_zero || !no_expand) )
    		// If orientation is not set, we cannot differentiate between horz & vert
    		wxLogMessage( _T("wxRlManager::AdjustWindowMinSize - Root item has size limitation but orientation flag is 0") );
    	if( (orient&wxVERTICAL && !ps_zero) ||
    		(orient&wxHORIZONTAL && !no_expand) )
    		sz_max.SetHeight(100000);
    	if( (orient&wxVERTICAL && !no_expand) ||
    		(orient&wxHORIZONTAL && !ps_zero) )
    		sz_max.SetWidth(100000);
    	m_wnd->SetMaxSize( sz_max );
    }
    else
    	m_wnd->SetMaxSize( wxSize(100000,100000) );

    // Now set real min size
    m_wnd->SetMinSize( sz_tot );
    
    if( sz_tot.GetWidth()>sz_orig.GetWidth() ||
        sz_tot.GetHeight()>sz_orig.GetHeight() ){
        m_wnd->SetSize( wxDefaultCoord, wxDefaultCoord,
            sz_tot.GetWidth()>sz_orig.GetWidth() ? sz_tot.GetWidth() : sz_orig.GetWidth(),
            sz_tot.GetHeight()>sz_orig.GetHeight() ? sz_tot.GetHeight() : sz_orig.GetHeight() );
        m_wnd->Refresh( );
    }
}


void wxRlManager::RenderSizerItem( wxDC *dc, wxSizerItem *item ){
    if( !item->IsShown() || !m_did_init_layout || m_no_paint ) return;
    wxRlItem *rli = ToRlItem(item);
    if( rli && !m_hidden.Has(rli->GetType()) ) {
		// Could be hidden type?
		rli->Render( dc );
		rli->RenderBorder( dc );
    }
    
    if( item->IsSizer() )
        return RenderSizer( dc, item->GetSizer() );
}

void wxRlManager::RenderSizer( wxDC *dc, wxSizer *node ){
    if( !node ) return;
    bool dc_create = false;
    if( !dc ){
	    dc = new wxClientDC(m_wnd);
	    dc_create=true;
    }

    for( SizerIter sit(node); sit.Get(); sit.Step() ){
    	wxSizerItem *psi = sit.Get();
    	if( psi->IsShown() )
    		RenderSizerItem( dc, psi );
    }
    
    if( dc_create )
    	delete dc;
}


void wxRlManager::MouseEvtSizerItem( wxRlItem *item ){
	// Is it a leaf (spacer item)? Then leave it to the item itself
	if( item->IsSpacer() ){
	    int mouse = 0;
	    if( m_mouse_evt.Moving() )
	    	mouse |= RL_MOUSE_MOVE;
	    else if( m_mouse_evt.LeftUp() || m_mouse_evt.MiddleUp() || m_mouse_evt.RightUp() ) 
	    	mouse |= RL_MOUSE_UP;
	    else if( m_mouse_evt.LeftDown() || m_mouse_evt.MiddleDown() || m_mouse_evt.RightDown() ) 
	    	mouse |= RL_MOUSE_DOWN;
	    
	    if( m_mouse_evt.LeftUp() || m_mouse_evt.LeftDown() ) 
	    	mouse |= RL_MOUSE_LEFT;
	    else if( m_mouse_evt.MiddleUp() || m_mouse_evt.MiddleDown() ) 
	    	mouse |= RL_MOUSE_MID;
	    else if( m_mouse_evt.RightUp() || m_mouse_evt.RightDown() ) 
	    	mouse |= RL_MOUSE_RIGHT;
	    
		// Give local coordinate
		wxPoint pt = m_mouse_evt.GetPosition() - item->GetPosition();
		
		// Generate special enter/leave event ?
		if( item!=m_mouse_over ){
			wxDC *pdc = NULL;
			if( m_mouse_over ){
				// The very same thing happens in OnTimer
				m_mouse_over->MouseEvent( RL_MOUSE_LEAVE, pt );
				RL_SET_FLAG(m_mouse_over->m_state,RL_STATE_MOUSE_OVER,false);
				if( m_mouse_over->m_capab&RL_CAPAB_REPAINT_ON_ENTER_LEAVE ){
					pdc = GetClientDc(m_mouse_over);
					RenderSizerItem(pdc,m_mouse_over);
				}
			}
			RL_SET_FLAG(item->m_state,RL_STATE_MOUSE_OVER,true);
			m_mouse_over = item;
			m_mouse_over->MouseEvent( RL_MOUSE_ENTER, pt );
			if( m_mouse_over->m_capab&RL_CAPAB_REPAINT_ON_ENTER_LEAVE ){
				if(!pdc) pdc = GetClientDc(m_mouse_over);
				RenderSizerItem(pdc,m_mouse_over);
			}
			if(pdc) delete pdc;
		}
		else
			// Ordinary event
			item->MouseEvent( mouse, pt );
	}
	
    // In case the edge is not visible, just return
    if( m_hidden.Has(item->GetType()) ) return;
    
    
	// Default processing
    // # This could be (partly/gully?) moved into wxRlItem. We would need some mechanism 
    // to set mouse captures and handle that without knowing precise type of the 
    // item.
	if( RL_IS_KNOWN_EDGE(item->GetType()) ){
		/*
        case RlPaneEdge:
        case RlMediumEdge:
        case RlThinEdge:
        case RlLayoutEdge:
        case RlCustomEdge1:
        case RlCustomEdge2:
        */
        if( m_mouse_evt.LeftDown() ){
            // Start moving the edge ?

            // Verify that it's the right kind of sizer
        	wxBoxSizer *pcbsz = wxDynamicCast(item->GetParent()->GetSizer(),wxBoxSizer);
            if( !pcbsz ){
                wxMessageBox( _T("Not the right kind of sizer (should be wxBoxSizer)") );
                return;
            }

            // We have to find item before and after
            m_action_items[1] = item;
            wxASSERT( item->GetParent()->GetSizer() );

            SizerIter sit(item->GetParent()->GetSizer());
            while( sit.Get() && item!=sit.Get() )
                sit.Step();
            wxASSERT( sit.Get() );
            
            // Search for item before that can be resized
            wxRlItem *rli;
            while( true ){
            	// If no item before with proportion, stop
            	if( !sit.GetPrevious() ) return;
            	rli = ToRlItem(sit.GetPrevious());
            	if( sit.GetPrevious()->GetProportion() ) break;
            	sit.StepBack();
            }
            m_action_items[0] = ToRlItem(sit.GetPrevious());
            while( sit.Get() && item!=sit.Get() )
                sit.Step();
            // Search for item after that can be resized
            while( true ){
            	// If no item after with proportion, stop
            	if( !sit.GetNext() ) return;
            	rli = ToRlItem(sit.GetNext());
            	if( sit.GetNext()->GetProportion() ) break;
            	sit.Step();
            }
            m_action_items[2] = ToRlItem(sit.GetNext());
            if( !m_action_items[0] || !m_action_items[2] ) return;

            // Can we actually drag these items?
            /*
            if( (!m_action_items[0]->GetProportion() && !(m_action_items[0]->GetFlag()&wxEXPAND)) ||
                (!m_action_items[2]->GetProportion() && !(m_action_items[0]->GetFlag()&wxEXPAND)) )
                return;
            */

            // Yes, both can be moved, set action, capture mouse
            m_mouse_action = RlMouseDragEdge;
            m_action_node = (wxRlItem*)item->GetParent();
            m_wnd->CaptureMouse( );
            m_mouse_last = m_mouse_evt.GetPosition();
            m_wnd_cursor = FindChildWindowAt(m_wnd,m_mouse_last);
            m_wnd_cursor->SetCursor( (((wxBoxSizer*)item->GetParent()->GetSizer())->GetOrientation()&wxHORIZONTAL) ? m_cursor_drag_edge_h : m_cursor_drag_edge_v );
        }
        else {
        	// When moving over an edge, set the H/V drag edge cursor
        	// On GTK and in dialog mode, this does not work, we hilight the edge instead
        	if( m_mouse_evt.Moving() ){
	            m_mouse_set_cursor = m_mouse_evt.GetPosition();
        		if( ((wxBoxSizer*)item->GetParent()->GetSizer())->GetOrientation()&wxVERTICAL && &m_wnd->GetCursor()!=&m_cursor_drag_edge_v ){
                    m_wnd_cursor = FindChildWindowAt(m_wnd,m_mouse_set_cursor);
                    m_wnd_cursor->SetCursor( m_cursor_drag_edge_v );
        		}
	    		if( ((wxBoxSizer*)item->GetParent()->GetSizer())->GetOrientation()&wxHORIZONTAL && &m_wnd->GetCursor()!=&m_cursor_drag_edge_h ){
                    m_wnd_cursor = FindChildWindowAt(m_wnd,m_mouse_set_cursor);
                    m_wnd_cursor->SetCursor( m_cursor_drag_edge_h );
	    		}
		        m_timer_cursor.Start( 100, false );
        	}
        }
	}

	else if(item->GetType()==RlAppPane){
    	if( m_mouse_evt.LeftDown() )
    		SetActive( item, true, true );
	}
	
	else if( item->m_capab&RL_CAPAB_GRIPPER ){
		if( m_mouse_evt.LeftDown() ){
			// Find next higher up entity item
			m_action_node = item->GetParentWithCapab(RL_CAPAB_ENTITY);
			wxASSERT( m_action_node );
			// Set timer to start drag
			m_timer_drag.Start( 100, true );
			// Offset from top left corner of dragged item
			//m_mouse_drag_offset = m_mouse_evt.GetPosition()-item->GetPosition();
			if( !m_pane_drag_info )
				m_pane_drag_info = new wxRlDragPaneInfo;
			m_pane_drag_info->m_offset = m_mouse_evt.GetPosition()-item->GetPosition();
		}
	}
        	
	// Give to subitems
    if( item->IsSizer() )
        MouseEvtSizer( item );
}

bool wxRlManager::MouseEvtSizer( wxSizerItem *item_node ){
    wxSizer *node = item_node->GetSizer();
    if( !node ) return false;

    wxSizerItem *si;
    wxRlItem *rli;
    // Iterate over linked list of children
    bool rv = false;
    wxPoint pt = m_mouse_evt.GetPosition();
    for( SizerIter sit(node); si=sit.Get(); sit.Step() )
    {
        // We convert an item to RlItem if it is not already
        if( !::IsRlItem(si) )
            si = wxRlItem::ConvertToRlItem( sit, si, item_node );
        rli = ToRlItem(si);
        if( !rli->IsShown() ) continue;
        if( !rli->GetRect().Contains(pt) ) continue;
        // We have a hit
        MouseEvtSizerItem( rli );
        rv = true;
    }
    return rv;
}

void wxRlManager::DoSetActive( wxDC *pdc, wxRlItem *rli, bool active ){
	wxRlAppPane *apsi = wxDynamicCast( rli, wxRlAppPane );
	if( apsi ){
		apsi->SetActive( active );
    	RenderSizerItem( pdc, apsi->GetChildWithType(RlCaption) );
	}
	else{
		// We could have other types of captions
		wxASSERT( 0 );
	}
}


bool wxRlManager::SetActive( wxRlItem *item, bool app_is_active, bool gen_evt ){
    if( item && item->GetType()!=RlAppPane ) return false;
    
    // For top level windows, override the active flag
    if( m_wnd->IsTopLevel() )
    	app_is_active = m_active_from_event;
    	
	if( m_active!=item || m_app_is_active!=app_is_active ){
	    wxClientDC dc(m_wnd);
	
		// We can have recursive active panes, but for now, there is only a single
		// level. Recursive would work through finding the node where the old
		// and the new active item meets, and doing activation along one branch
		// and deactivation along the other branch.
	
	    if( m_active && item!=m_active ){
	    	if( m_active->GetType()==RlAppPane )
	    		DoSetActive( &dc, m_active, false );
	    }
	    m_active = item;
		m_app_is_active=app_is_active;
	    if( item && item->GetType()==RlAppPane )
    		DoSetActive( &dc, item, true );
	    
	    // This was previous outside of the big if block. However, we could
	    // not have a sibling window to the app window have the focus then.
	    // We will enter this function when focus changes between sibling
	    // windows, but no need to pass event upwards then.
	    if( gen_evt ){ 
		    // Notify that active pane has changed
			wxCommandEvent event( wxEVT_RL_PANE_ACTIVATE, wxID_ANY );
		    event.SetEventObject( this );
		    event.SetClientData( m_active );
		    // Sometimes it is important to activate a window directly.
		    // I.e. when clicking a tool on a toolbar that is in an unfocused
		    // frame, we should activate directly.
		    //m_wnd->AddPendingEvent( event );
		    m_wnd->GetEventHandler()->ProcessEvent( event );
		}
	}	    
    
    return true;
}



//
// Event handlers
//
void wxRlManager::OnPaint(wxPaintEvent& event){
    if( !m_wnd || !m_root ) return;

    // Painting of RlItems can be disabled from outside
    if( !m_no_paint ){
	    wxPaintDC dc(m_wnd);
	    RenderSizerItem(&dc,m_root);
    }
    
    // Let dialog do its painting
    if( m_is_dialog )
    	event.Skip();
}

void wxRlManager::OnEraseBackground(wxEraseEvent& event)
{
#ifdef __WXMAC__
    event.Skip() ;
#else
    wxUnusedVar(event);
#endif
}

void wxRlManager::OnSize(wxSizeEvent& event)
{
    if (m_wnd && m_root )
    {
        static wxSize cl_sz_last;
        wxSize cl_sz = m_wnd->GetClientSize();
        if( cl_sz==cl_sz_last ){
        	// This often happens on wxGTK
        	return;
        }
        cl_sz_last = cl_sz;
        
        wxPoint pt(0,0);
        // This does the relayout
        m_root->SetDimension( pt, cl_sz);
        wxClientDC dc(m_wnd);
        // Render all our items
        RenderSizerItem(&dc,m_root);
    }
    else{
        // Important only to skip when we're not doing layout ourselves.
        // Otherwise, wxwindows redoes our fresh layout
        event.Skip();
    }
}

void wxRlManager::StopDragPane( ){
	if( !m_pane_drag_info ) return;

    if( m_mouse_action==RlMouseDragPane || m_mouse_action==RlMouseDragToolBar ){
		// Find address to insert and delete temp item
		wxString addr = m_action_node ? m_action_node->GetAddress() : _T("0");
		m_action_node->Destroy();
	
		// Find manager and dragged window
		wxRlManager *rlmgr = GetRlManagerOf(m_pane_drag_info->m_wnd_drag);
		wxASSERT( rlmgr ); 
		wxRlItem *rli_dragged = rlmgr->m_root;
		wxASSERT( rli_dragged );
		if( !Dock( rlmgr, addr, m_mouse_last) ){
			// Not much more to do, the dragged window disappears
			bool no_log = GetSuppressLog( );
			if( !no_log ) wxLogMessage( _T("wxRlmanager::StopDragPane - Failed docking dragged window into main window again") );
		}
		else 
			// Reactivate in main window
			SetActive( rli_dragged, m_app_is_active, false );
		
		// Close popup window
		delete rlmgr;
		m_pane_drag_info->m_wnd_drag->Destroy();
    }
	
	delete m_pane_drag_info;
	m_pane_drag_info = NULL;
}


void wxRlManager::OnLeftDown(wxMouseEvent& event)
{
    if( !m_wnd || ! m_root ) return;

    // We get this when user clicks a non-window area
    // Find the part that owns it and want the event
    m_mouse_evt = event;
    m_mouse_last = event.GetPosition();
    MouseEvtSizerItem( m_root );
    event.Skip();
}

void wxRlManager::OnRightDown(wxMouseEvent& event)
{
    if( !m_wnd || ! m_root ) return;

    // We get this when user clicks a non-window area
    // Find the part that owns it and want the event
    m_mouse_evt = event;
    m_mouse_last = event.GetPosition();
    MouseEvtSizerItem( m_root );
    event.Skip();
}

void wxRlManager::OnLeftUp(wxMouseEvent& event)
{
    if( !m_wnd || ! m_root ) return;

    if( m_mouse_action!=RlNoAction ){
    	StopDragPane( );    		
    	
        m_mouse_action = RlNoAction;
        m_action_node = NULL;
        if( m_wnd->GetCapture()==m_wnd )
            m_wnd->ReleaseMouse();
        if( m_wnd_cursor ){
        	m_wnd_cursor->SetCursor(wxNullCursor);
        	m_wnd_cursor = NULL;
        }
    }
    
    m_mouse_evt = event;
    MouseEvtSizerItem( m_root );
   
    event.Skip();
}

void wxRlManager::OnRightUp(wxMouseEvent& event)
{
    if( !m_wnd || ! m_root ) return;

    // We get this when user clicks a non-window area
    // Find the part that owns it and want the event
    m_mouse_evt = event;
    m_mouse_last = event.GetPosition();
    MouseEvtSizerItem( m_root );
    event.Skip();
}


void wxRlManager::OnMotion(wxMouseEvent& event)
{
	g_rl_mgr_last_active = this;
	
	// Start the timer that monitors when the mouse leaves
	if( !m_timer_leave.IsRunning() )
		m_timer_leave.Start( 100, false );	
	
	if( m_mouse_action!=RlNoAction ){
		// This is a safety valve for releasing capture when library fails
		if( !wxGetMouseState().LeftDown() ){
			StopDragPane( );
			m_mouse_action = RlNoAction;
			if( m_wnd->GetCapture()==m_wnd )
				m_wnd->ReleaseMouse( );
	        if( m_wnd_cursor ){
	        	m_wnd_cursor->SetCursor(wxNullCursor);
	        	m_wnd_cursor = NULL;
	        }
		}
	}
	
    if( m_mouse_action==RlMouseDragEdge ){

        // Get the sizer and the index of the moving edge
        wxBoxSizer *sizer = (wxBoxSizer*)m_action_node->GetSizer();
        int ix = sizer->GetChildren().IndexOf( m_action_items[1] );
        int ix_prv = sizer->GetChildren().IndexOf( m_action_items[0] );
        int ix_nxt = sizer->GetChildren().IndexOf( m_action_items[2] );
        wxASSERT( ix>0 );

        // retrieve some basic values to work with
        int dsz = sizer->GetOrientation()==wxVERTICAL ?
                    event.GetY()-m_mouse_last.y :
                    event.GetX()-m_mouse_last.x;
        if( !dsz ) return;
        m_mouse_last = event.GetPosition();

        // See if we have reduced proportion down to min (1) for this item. 
        // If so, take the next one
        wxSizerItem *psi = NULL;
        if( m_action_items[2]->GetProportion()==1 && dsz>0 ){
        	int cnt = sizer->GetChildren().GetCount();
        	while( ix_nxt<cnt-1 && (psi=sizer->GetItem(++ix_nxt)) )
        		if( psi->GetProportion()>1 ) break;
        	if( !psi || psi->GetProportion()<1 ) return;
        	m_action_items[2] = ToRlItem(psi);
        }
        if( m_action_items[0]->GetProportion()==1 && dsz<0 ){
        	while( ix_prv>0 && (psi=sizer->GetItem(--ix_prv)) )
        		if( psi->GetProportion()>1 ) break;
        	if( !psi || psi->GetProportion()<1 ) return;
        	m_action_items[0] = ToRlItem(psi);
        }
        
        // Do the sizing
        if( TradeSpace(sizer,ix_prv,ix_nxt,dsz) ){
            // We have changed sizes of items, time to repaint affected items
        	wxDC *pdc = GetClientDc( m_action_node );
            while( ix_prv<=ix_nxt ){
            	wxSizerItem *psi = sizer->GetItem(ix_prv++);
            	RenderSizerItem( pdc, psi );
            }
            delete pdc;
        }
    }
    
    else if( m_mouse_action==RlMouseDragPane || m_mouse_action==RlMouseDragToolBar ){
        bool no_log = GetSuppressLog( );
        
        // Should be dragging a window
        wxASSERT( m_pane_drag_info && m_pane_drag_info->m_wnd_drag );
        
        // Move floating window accordingly 
        // This is better, it maintains the original offset with the dragged window
        m_pane_drag_info->m_wnd_drag->Move( 
        		m_wnd->ClientToScreen(event.GetPosition())-m_pane_drag_info->m_offset );
        
    	// If inside it's own rectangle, do nothing
    	wxRect r = m_action_node->GetRect();
    	if( r.Contains(event.GetPosition()) ) return;

    	// If close to last insertion point, do nothing
    	wxPoint pt = event.GetPosition() - m_pane_drag_info->m_mouse_last_insert;
    	if( pt.x*pt.x + pt.y*pt.y<50 )	// Pythagoras...
    		return;
    	
		// Look for best place to put it
		static wxString sl;
		pt = event.GetPosition();
		// Look for the mid point of the window we're dragging
		r = m_pane_drag_info->m_wnd_drag->GetScreenRect();
		pt = m_wnd->ScreenToClient( wxPoint(r.x+r.width/2, r.y+r.height/2) );

		wxString s = m_root->FindInsertionAddress(pt,m_action_node);
		/*
        static wxString s_last;
        if( s!=s_last )
            if( !no_log ) wxLogMessage( s );
        s_last = s;
        */
		
		// Insert the dragged pane at the new place
		if( s.Length() ){
    		wxString sa = m_action_node->GetAddress();
			wxString tmp(s);
			tmp.Replace(_T("#"),_T(""));
			
			// Look if the dest is the place next to us
			wxString sb;
			wxChar *end;
			int sn = scstrtol( (const wxChar*)sa+sa.Find(_T(','),true)+1, &end, 10 );
			sb = sa.Left(sa.Find(_T(','),true)+1);
			sb += wxString::Format(_T("%d"),sn+1);

			/*
			static wxString lm;
			wxString m = _T("Try: ");
			m += s + _T(" moving from: ")+sa+_T(" - ")+sb;
			if( m!=lm ){
	    		//if(!no_log) wxLogMessage( m );
	    		lm = m;
			}
			*/
				
    		if( !s.StartsWith(sa) && !tmp.StartsWith(sa) && tmp!=sb ){
    			if(!no_log) wxLogMessage( wxString(_T(""))+_T("DragPane: ") + s + _T(" moving from: ")+sa );
	    		
	    		if( m_root->InsertAt( m_action_node, s ) ){
	    			// Is the old parent empty? Or only contains a single item?
	    			// Instead of optimizing the layout here, we do it from
	    			// the top. It's too compliated otherwise.
	    			Update( );
	    			m_pane_drag_info->m_mouse_last_insert = event.GetPosition();
	    		}
    		}
    	}
    }
    else if( m_mouse_action==RlMouseDragWindow ){
    	//wxPoint pt = m_wnd->GetPosition()+ event.GetPosition() - m_mouse_last;
    	//m_wnd->Move( pt );
        m_wnd->Move( m_wnd->ClientToScreen(event.GetPosition())-m_pane_drag_info->m_offset );
    }
    else{
	    m_mouse_evt = event;
	    MouseEvtSizerItem( m_root );
    }
    
    event.Skip();
}

void wxRlManager::OnTimer(wxTimerEvent& evt){
	g_rl_mgr_last_active = this;
	
	if( evt.GetId()==m_timer_cursor.GetId() ){
		wxPoint pt = wxGetMousePosition();
		pt = m_wnd->ScreenToClient(pt);
		if( pt!=m_mouse_set_cursor ){
			if( m_mouse_action==RlNoAction ){
				if( m_wnd_cursor )
					m_wnd_cursor->SetCursor( wxNullCursor );
				m_wnd_cursor = NULL;
				m_timer_cursor.Stop();
			}
		}
	}

	else if( evt.GetId()==m_timer_leave.GetId() ){
		// When mouse leaves window, give message to mouse active item and stop timer
		// Since we sometimes leave onto child windows we never get a mouse leave
		// event. We need the timer then.
		wxPoint pt = m_wnd->ScreenToClient( wxGetMousePosition() );
		if( m_mouse_over ){
			if( !m_mouse_over->GetRect().Contains(pt) ){
				m_timer_leave.Stop();
				if( (m_mouse_over->m_state&RL_STATE_MOUSE_OVER) ){
					m_mouse_over->MouseEvent( RL_MOUSE_LEAVE, pt );
					RL_SET_FLAG(m_mouse_over->m_state,RL_STATE_MOUSE_OVER,false);
					if( m_mouse_over->m_capab&RL_CAPAB_REPAINT_ON_ENTER_LEAVE ){
						wxClientDC dc(m_wnd);
						RenderSizerItem(&dc,m_mouse_over);
					}
					m_mouse_over = NULL;
				}
			}
		}
		else
			m_timer_leave.Stop();
	}
	
	else if( evt.GetId()==m_timer_drag.GetId() ){
		if( !wxGetMouseState().LeftDown() ){
			// Cancel, mouse has been released
			m_action_node = NULL;
		}
		if( m_action_node ){
			// Special case when a pane occupies the whole window
			if( m_action_node==m_root )
				// Drag whole window
				m_mouse_action = RlMouseDragWindow;
			else{
				// # We don't really need different drag actions
				// We need to know if we're dragging the active item though
				if( m_action_node->GetType()==RlAppPane  )
					m_mouse_action = RlMouseDragPane;
				else if( m_action_node->GetType()==RlToolPane || m_action_node->GetType()==RlToolbarDock  )
					m_mouse_action = RlMouseDragToolBar;
				else
					wxASSERT(_T("Unknown m_action_node"));
				
				// Now we want to float the item we're dragging
				// and replace it with a RlPaneMoveHint  hint during the operation
				wxRlItem *src = m_action_node->GetParent();
				wxASSERT(src);
				int ix = m_action_node->IndexOf();
								
				// Insert the hint before floating pane, otherwise layout may change
				wxRlItem *rli_hint = new wxRlItem(RlPaneMoveHint);
				// We want similar geometry to the window we're moving
				rli_hint->CopyGeometryFrom( m_action_node );
				bool r = src->Insert( ix, rli_hint ).IsOk();
				wxASSERT( r );
				
				wxRlManager *prlm = Float( m_action_node, NULL, wxPoint(0,0) );
				if( !prlm ){
					// Failed floating window, abort
					rli_hint->Destroy();
					m_action_node = NULL;
					m_mouse_action = RlNoAction;
					return;
				}
				
				m_pane_drag_info->m_wnd_drag = prlm->m_wnd; 
				m_action_node = rli_hint;
				Update( );
			}
	        m_wnd->CaptureMouse( );
	        m_mouse_last = m_mouse_evt.GetPosition();
	        m_wnd_cursor = m_wnd;
	        m_wnd->SetCursor(m_cursor_drag_pane);
		}
	}

	else if( evt.GetId()==m_timer_debug.GetId() ){
		// Dump layout to stdout 
	 	bool bp = wxGetKeyState(wxKeyCode('P'));
	 	bool bs = wxGetKeyState(wxKeyCode('S'));
	 	bool bsh = wxGetKeyState(wxKeyCode(WXK_SHIFT));
	 	bool bct = wxGetKeyState(wxKeyCode(WXK_CONTROL));
	 	if( bsh && wxWindow::GetCapture()==m_wnd )
	 		wxMessageBox(_T("CaptureTest"));
	 	if( bp && bsh && bct )
	 		m_root->Dump( 2 );
	 	if( bs && bsh && bct ){
	 		wxString wxs;
	 		if( GetState( wxs ) )
	 			wxSaveString( wxs, _T("wxRlDialog.state") );
	 	}
		
		// Check layout integrity
		wxString s = m_root->CheckLayout();
		static wxString sl;
		static bool msg_box_open = false;
		if( s.Length() ){
			if( s!=sl && !msg_box_open ){
				msg_box_open = true;
				wxLogMessage( s );
				wxMessageBox( s, _T("wxRlManager CheckLayout") );
				sl = s;
				msg_box_open = false;
			}
		}
	}
	
}

void wxRlManager::OnMouseCaptureLost(wxMouseCaptureLostEvent& evt){
	wxUnusedVar(evt);
	if( m_wnd ){
	    if( m_mouse_action!=RlNoAction ){
			StopDragPane( );
	        m_mouse_action = RlNoAction;
	        m_action_node = NULL;
	        if( m_wnd_cursor )
	        	m_wnd_cursor->SetCursor( wxNullCursor );
	        m_wnd_cursor = NULL;
	    }
	    if( wxWindow::GetCapture()==m_wnd )
	        m_wnd->ReleaseMouse();
	}
    //evt.Skip();
}

void wxRlManager::OnLeaveWindow(wxMouseEvent& event)
{
	// When mouse leaves window, give message to mouse active item and stop timer
	if( m_mouse_over ){
		m_mouse_over->MouseEvent( RL_MOUSE_LEAVE, event.GetPosition() );
		RL_SET_FLAG(m_mouse_over->m_state,RL_STATE_MOUSE_OVER,false);
		if( m_mouse_over->m_capab&RL_CAPAB_REPAINT_ON_ENTER_LEAVE ){
			wxDC *pdc = GetClientDc(m_mouse_over);
			RenderSizerItem(pdc,m_mouse_over);
			delete pdc;
		}
		m_mouse_over = NULL;
	}
    //event.Skip();
}


void wxRlManager::OnChildFocus(wxChildFocusEvent& event)
{
	if( !m_did_init_layout || event.GetWindow()==m_wnd )
		return; 
    if( event.GetWindow()->IsTopLevel() ){
		//wxLogMessage( _T("OnChildFocus(%x): Initially refused %x  (gaa:%d)"), this, event.GetWindow(), m_active_from_event );
    	return;
    }

	// We want to activate the RlPane containing the window
	// that just gained focus. 
	wxWindow *wnd = event.GetWindow();
	wxRlItem *rli = NULL;
	while( wnd ){
		if( (rli = FindRlItemOf(wnd)) )
			break;
		wnd = wnd->GetParent();
	}
		
	if( !rli ){
		wxLogMessage( _T("OnChildFocus(%x): Child window not found %x"), this, event.GetWindow() );
	    event.Skip();
	}
    else {
		wxRlItem *rli_pane = rli->GetParentWithType(RlAppPane);
		// Sometimes we come here when this window is not activated. 
		// Then activation events will be fired, which is incorrect. Inhibit.
		if( rli_pane && m_active_from_event ){
			SetActive( rli_pane, m_app_is_active, true );
			/*
			if( rli->GetRect().Contains( m_wnd->ScreenToClient(wxGetMousePosition()) ) ){
				SetActive( rli_pane, m_app_is_active, true );
				wxLogMessage( _T("OnChildFocus(%x): Child window found %x  (gaa:%d)"), this, event.GetWindow(), m_active_from_event );
			}
			else
				wxLogMessage( _T("OnChildFocus(%x): Not in window area: %x  (gaa:%d)"), this, event.GetWindow(), m_active_from_event );
			*/
		}
	}
}

void wxRlManager::OnScroll( wxScrollWinEvent &evt ){
	wxWindow *wnd = (wxWindow*)evt.GetEventObject();
	//if( )
}

void wxRlManager::OnActivate(wxActivateEvent& event){
	// Also cache last activated
	if( m_active_from_event = event.GetActive() )
		g_rl_mgr_last_active = this;
	
	// Hilight or remove hilight
	SetActive( m_active, m_active_from_event, false ); 
	event.Skip();
}

int g_idle_cnt;
void wxRlManager::OnIdle(wxIdleEvent& event){
	// This should not be needed in an OnIdle handler!
	if( !wxProcessEventsUntilEmpty( ) ) 
		return;
	
	// Clear away anything old
	g_idle_cnt++;
	while( g_rl_garbage.Size() )
		delete g_rl_garbage.Pop();
	//event.Skip();
}


// This is to get a DC that also works inside dialogs, where we have to 
// paint on top of wxStaticBox:es (which cover big areas). 
// On wxGTK this works fine without this trick.
wxDC *wxRlManager::GetClientDc( wxRlItem *rli ){
	// Sepcialization code moved to wxRlDialogmanager, see comments there.
	return new wxClientDC(m_wnd);
}




//
// wxRlCaption class
//
void wxRlCaptionSi::Init( const wxChar *str, int buttons, int orient ){
	// Initialize sizer
	wxSizer *ps = new wxBoxSizer(orient);
	AssignSizer( ps );
	
	// The text goes to the left
	m_pc_txt = new wxRlCaptionText(str);
	ps->Add( m_pc_txt );
    AddButtons( buttons, 0 );
    
    // We let this happen from higher up
	//UpdateItem(false);
}


wxRlCaptionSi::wxRlCaptionSi( const wxChar *str, int buttons, int orient ) 
	: wxRlItem(RlCaption,wxEXPAND) {
    Init( str, buttons, orient );
}

void wxRlCaptionSi::SetCaption( const wxChar *str ){
    m_pc_txt->m_caption = str;
}

const wxChar *wxRlCaptionSi::GetCaption( ){
    return m_pc_txt->m_caption;
}

void wxRlCaptionSi::AddButtons( int button, int pos ){
	wxSizer *ps = GetSizer(); 
	wxASSERT(ps);
    // have a spacer first
    if( ps->GetChildren().GetCount()<=1 ){
    	// Space between text and buttons
        ps->Add( new wxRlSpacer(1,1) );
        // This is to move buttons a bit to the left from the RHS of pane
        ps->Add( new wxRlSpacer(4,0) );  
    }

    // Check all bits
    for( int i=0; i<32; i++ ){
        if( button&(1<<i) ){
            wxRlItem *rli = new wxRlButton(i,wxALIGN_CENTER_VERTICAL);
            ps->Insert( pos ? pos+2 : 2, rli );
        }
    }
}

void wxRlCaptionSi::AddButtonSeparator( int size, int pos ){
	wxASSERT( GetSizer() );
    GetSizer()->Insert( pos ? pos+2 : 2, new wxRlSpacer(size,0) );
}

void wxRlCaptionSi::SetActive( bool active ){
	m_pc_txt->SetActive( active );
	RL_SET_FLAG( m_state, RL_STATE_ACTIVE, active );
}



//
// wxRlAppPane class
//
void wxRlAppPane::Init( const wxChar *name, int buttons, int prop, wxWindow *wnd ){
	wxSizerItem::SetProportion( prop );
	
	// Only vertical mode now
	SetFlag( (GetFlag()&~wxBOTH) | wxVERTICAL );
	
	// The sizer to hold things
	wxSizer *ps = new wxBoxSizer(wxVERTICAL);
	AssignSizer( ps );
	
	// The caption
	m_caption = new wxRlCaptionSi(name,buttons); 
	ps->Add( m_caption );

	// The toolbar - we add a border along the bottom
	m_tool_pane = new wxRlToolPane( wxString(name)+_T(" Tool Bar"), wxHORIZONTAL|wxEXPAND | wxBOTTOM|wxSTATIC_BORDER );
	// Insert under the caption
	ps->Add( m_tool_pane );
	m_toolbar_vert = false;
	
	// The content item
	m_contents = wnd ? new wxRlItem(wnd,RL_DEF_PROP,wxEXPAND,RlWindow) : new wxRlItem(RlClientBg,wxEXPAND); 
	ps->Add( m_contents );

	// Register with name
	wxSizer2RlItem::Push( ps, this, name );
	
	wxRlAppPane *ap = wxDynamicCast( this, wxRlAppPane );
	
	UpdateItem(0);
}

wxRlAppPane::wxRlAppPane( const wxChar *pane_name, int buttons, int prop, int flag ) 
	: wxRlItem(RlAppPane,flag) {
	Init( pane_name, buttons, prop, NULL );
}

wxRlAppPane::wxRlAppPane( const wxChar *pane_name, wxWindow *wnd, int buttons, int prop, int flag ) 
: wxRlItem(RlAppPane,flag) {
	Init( pane_name, buttons, prop, wnd );
}

void wxRlAppPane::SetCaption( const wxChar *str ){
	m_caption->SetCaption(str);
}

wxString wxRlAppPane::GetCaption( ){
	return m_caption->GetCaption();
}


void wxRlAppPane::SetWindow( wxWindow *wnd ){
	if( wnd ){
		if( m_contents->GetType()==RlWindow )
			m_contents->SetWindow( wnd );
		else{
			wxRlItem *rli = new wxRlItem( wnd, m_contents->GetProportion(), m_contents->GetFlag(), RlWindow );
			rli->SetMinSize( m_contents->GetMinSize() );
			m_contents->Replace( rli );
			m_contents = rli;
		}
	}
	else{
		if( !m_contents->IsSpacer() ){
			wxRlItem *rli = new wxRlItem( RlClientBg );
			// Take over 
			rli->CopyGeometryFrom( m_contents );
			m_contents->Replace( rli );
			m_contents = rli;
		}
	}
}

wxWindow *wxRlAppPane::GetWindow(  ){
	return m_contents->GetWindow();
}

wxRlItem *wxRlAppPane::GetContents( int ix ){
	if( !ix )
		return m_contents;
	else 
		return m_tool_pane->GetContents( ix-1 ); 
}

bool wxRlAppPane::SetContents( wxRlItem *rli, int ix ){
	if( !rli || !rli->IsOk() ) return false;
	if( !ix || (ix==-1 && m_contents->GetType()==RlClientBg) ){
		// Take over proportion from current item if not zero
		if( rli->GetProportion() ) 
			rli->wxSizerItem::SetProportion( m_contents->GetProportion() );
		m_contents->Replace( rli );
		m_contents = rli;
		return true;
	}
	else
		return m_tool_pane->SetContents( rli, ix>=0?ix-1:ix );
}

wxRlItem *wxRlAppPane::DetachContents( int ix ){
	if( !ix ){
		// We set a temp BG when giving away our contents
		wxRlItem *rli = new wxRlItem( RlClientBg );
		rli->wxSizerItem::SetProportion(m_contents->GetProportion()); 
		if( !m_contents->ReplaceDetach( rli ).IsOk() ){
			delete rli;
			return NULL;
		}
		wxRlItem *rli_ret = m_contents;
		m_contents = rli;
		return rli_ret;
	}
	else
		return m_tool_pane->DetachContents( ix-1 );
}




void wxRlAppPane::SetActive( bool active ){
	RL_SET_FLAG( m_state, RL_STATE_ACTIVE, active );
	m_caption->SetActive(active);
}


// Flip the orientation of the 
void wxRlAppPane::FlipToolBarPane( ){
	wxSizer *ps = GetSizer();
	wxRlItem *rli = ToRlItem(ps->GetItem((size_t)1));
	wxASSERT(rli);
	if( rli==m_tool_pane ) {
		wxASSERT( !m_toolbar_vert );
		// Detach content
		m_contents->Detach();
		
		// Destroy current tool pane
		wxString name = RlGetNameOf(m_tool_pane);
		m_tool_pane->Destroy();

		// Create a horizontal sizer
		wxBoxSizer *phs = new wxBoxSizer(wxHORIZONTAL);
		ps->Add( new wxRlItem(phs) );

		// Add new tool pane and old contents
		m_tool_pane = new wxRlToolPane( name, wxVERTICAL|wxEXPAND | wxRIGHT|wxSTATIC_BORDER );
		phs->Add( m_tool_pane );
		phs->Add( m_contents );
	}
	else {
		wxASSERT( m_toolbar_vert );
		// Change to horizontal
		// Delete old tool pane
		wxString name = RlGetNameOf(m_tool_pane);
		m_tool_pane->Destroy();
		m_contents->Detach();
		// Delete the sizer
		rli->Destroy();
		// Create and add new tool pane
		m_tool_pane = new wxRlToolPane( name, wxHORIZONTAL|wxEXPAND | wxBOTTOM|wxSTATIC_BORDER );
		GetSizer()->Add( m_tool_pane );
		GetSizer()->Add( m_contents );
	}
	m_toolbar_vert ^= true;
	UpdateItem( 0 );
}

void wxRlAppPane::SetToolBar( wxWindow *ptb, bool horizontal ){
	// Hide previous toolbar if any
	wxWindow *prev_tb = GetToolBar();
	if( prev_tb ) prev_tb->Show( false );
	
	// Check that orientation is OK. If horizontal, the 2nd item is the toolbar
	// If vertical, it is a horz sizer
	wxSizer *ps = GetSizer();
	wxRlItem *rli = ToRlItem(ps->GetItem((size_t)1));
	wxASSERT(rli);
	if( (rli==m_tool_pane && !horizontal) ||
		(rli->GetType()==RlSizer && horizontal) )
		FlipToolBarPane( );
	
	// And set new one
	m_tool_pane->SetToolWindow(ptb);
}


wxWindow *wxRlAppPane::GetToolBar(  ){
	wxRlItem *rli = m_tool_pane ? m_tool_pane->GetChildWithType(RlToolWindow) : NULL;
	return rli ? rli->GetWindow() : NULL;
}

const wxChar *wxRlAppPane::GetName( ) {
	const wxChar *name = wxSizer2RlItem::GetName( this );
	return name;
}

#include "utils/XmlParse.h"
bool wxRlAppPane::ConfigFromXml( const char *&xml ){
#ifndef RL_NO_STATE 	
	CharBuf cb;
	if( !AcceptOpenTag(xml,"toolbar") ){
		int r = AcceptCloseToken(xml);
		if( r ) return r<0 ? false : true;
		if( AcceptText(xml,cb) ) return false;
		if( SkipPastCloseNode(xml,"toolbar") ) return false;
		bool vert = cb.eqi("vert"); 
		if( vert!=m_toolbar_vert )
			FlipToolBarPane( );
	}
#endif	
	return true;
}

wxString wxRlAppPane::ConfigToXml( ){
	// Check orientation and store
	if( m_toolbar_vert )
		return wxString(_T("<toolbar>vert</toolbar>"));
	else
		return wxEmptyString;
}


// A function to investigate min sizes for Toolbar windows
wxSize GetToolWindowMinSize( wxWindow *wnd, int orient ){
    wxSize s, sz = wnd->GetSize();
    wxSize szb = wnd->GetBestSize();
    wxSize szm = wnd->GetMinSize();

    // Try setting min size
    if( szm!=wxDefaultSize ){
        wnd->SetSize( szm );
        s = wnd->GetSize();
        if( szm==s )
            return szm;
    }

    // Try setting best size
    wnd->SetSize( szb );
    s = wnd->GetSize();
    // wxToolbar seems to return 1 pixel wrong size, account for that
    if( szb==s || (s.y==szb.y+1 && s.x==szb.x) ||
        (s.y==szb.y && s.x==szb.x+1) )
        return s;

    if( orient==wxVERTICAL && sz.x>szb.x ) szb.x = sz.x;
    if( orient==wxHORIZONTAL && sz.y>szb.y ) szb.y = sz.y;
    wnd->SetSize( szb );
    // s = wnd->GetSize();

    return szb;
}

//
// wxRlToolPane class
//
wxRlToolPane::wxRlToolPane( const wxChar *name, int flag, wxWindow *tool, bool with_gripper ) 
 : wxRlItem(RlToolPane,flag), m_has_gripper(with_gripper) {
	// A sizer to contain things
	wxSizer *ps = new wxBoxSizer(flag&wxBOTH);
	AssignSizer( ps );
	
	// A gripper ?
	if( with_gripper )
		ps->Add( new wxRlItem(RlToolGripper) );
	
	// Add toolbar if any
	if( tool )
		SetToolWindow( tool );
	
	// We need a trailing spacer with proportion 1, otherwise strange behaviour 
	// (prop_sum=0) when no toolbar is set.
	ps->Add( new wxRlItem(0,0, 1, wxEXPAND, RlFrameBg) );
	
	// Register with name
	wxSizer2RlItem::Push( ps, this, name );
	
	UpdateItem(0);
}

bool wxRlToolPane::SetToolWindow( wxWindow *wnd ){
	wxSizer *ps = GetSizer();
	wxASSERT( ps );
	
	// First remove all current tool windows. Hide windows 
	// Leave trailing space.
	wxSizerItemList &children = ps->GetChildren();
	int ix = m_has_gripper ? 1 : 0;
	while( (int)children.GetCount()>ix+1 ){
		wxWindow *wnd = ps->GetItem((size_t)ix)->GetWindow();
		if( wnd ){
			wnd->Show( false );
			wnd->SetContainingSizer( NULL );
		}
		// ps->Remove(ix);
		TrashRlItem( ps, ix );
	}
	
	// And add the new window
	if( wnd ){
		// Sort out min size for this toolbar
	    int orient = GetOrientation(ps);
	    wxSize sz = GetToolWindowMinSize( wnd, orient );
	    if( orient==wxVERTICAL ){
		    wnd->SetMinSize( wxSize(sz.x, 50) );
	    	//sz.y += trailing_space;
	    }
	    if( orient==wxHORIZONTAL ){
		    wnd->SetMinSize( wxSize(50,sz.y) );
	    	//sz.x += trailing_space;
	    }
	    wnd->SetSize( sz );

	    // Create and add wxRlItem
		wxRlItem *rli = new wxRlItem(wnd,RL_DEF_PROP,wxEXPAND,RlToolWindow);
		ps->Insert( ix, rli );
		wnd->Show( true );
	}
	
	else{
		// The trailing space element is good enough
	}
	
	// Make sure things are connected internally
	UpdateItem(0);
	return true;
}


wxRlItem *wxRlToolPane::GetContents( int ix ){
	int first_tb = m_has_gripper ? 1 : 0;
	wxSizer *ps = GetSizer();
	if( !ix && (int)ps->GetChildren().GetCount()>first_tb+1 ){
		wxRlItem *rli = ToRlItem(ps->GetItem(first_tb));
		if( rli && rli->GetType()==RlToolWindow )
			return rli;
	}
	return NULL; 
}

bool wxRlToolPane::SetContents( wxRlItem *rli, int ix ){
	if( !rli || !rli->IsOk() ) return false;
	if( ix>0 || rli->GetType()!=RlToolWindow ) return false;
		
	int first_tb = m_has_gripper ? 1 : 0;
	wxSizer *ps = GetSizer();
	bool is_empty = (int)ps->GetChildren().GetCount()==first_tb+1;
	if( !ix || (ix==-1 && is_empty) ){
		if( is_empty ){
			// Empty now, add item
			ps->Insert( first_tb, rli );
		}
		else{
			// Replace current item
			wxRlItem *rli_old = ToRlItem(ps->GetItem(first_tb));
			wxASSERT(rli_old);
			rli_old->Replace( rli );
		}
		UpdateItem( 0 );
		return true;
	}
	else 
		return false;
}

wxRlItem *wxRlToolPane::DetachContents( int ix ){
	if( ix>0 ) return NULL;
	int first_tb = m_has_gripper ? 1 : 0;
	wxSizer *ps = GetSizer();
	if( (int)ps->GetChildren().GetCount()==first_tb+1) return NULL;

	// Just detach without replacing with anything
	wxRlItem *rli = ToRlItem(ps->GetItem(first_tb));
	if( rli->GetType()!=RlToolWindow ) return NULL;
	if( !rli->Detach().IsOk() ) return NULL;
	
	return rli;
}


bool wxRlToolPane::ConfigFromXml( const char *&xml ){
#ifndef RL_NO_STATE 	
	CharBuf cb;
	if( !AcceptOpenTag(xml,"toolpane") ){
		int r = AcceptCloseToken(xml);
		if( r ) return r<0 ? false : true;
		if( AcceptText(xml,cb) ) return false;
		if( SkipPastCloseNode(xml,"toolpane") ) return false;
		bool gripper = cb.eqi("gripper"); 
		if( gripper!=m_has_gripper ){
			wxSizer *ps = GetSizer();
			wxASSERT( ps );
			if( gripper )
				ps->Insert( 0, new wxRlItem(RlToolGripper) );
			else{
				wxRlItem *rli = ToRlItem(ps->GetItem((size_t)0));
				wxASSERT( rli );
				rli->Destroy();
			}
			m_has_gripper = gripper;
		}
	}
#endif
	return true;
}

wxString wxRlToolPane::ConfigToXml( ){
	if( m_has_gripper )
		return wxString(_T("<toolpane>gripper</toolpane>"));
	else
		return wxEmptyString;
}


bool wxRlClientBgHint::ConfigFromXml( const char *&xml ){
#ifndef RL_NO_STATE 	
	CharBuf cb;
	if( !AcceptText(xml,cb) ){
		m_hint = MB2WX(cb);
		m_hint.Trim(true).Trim(false);
	}
	if( !AcceptOpenTag(xml,"size") ){
		int r = AcceptCloseToken(xml);
		if( r ) return r<0 ? false : true;
		if( AcceptText(xml,cb) ) return false;
		if( SkipPastCloseNode(xml,"size") ) return false;
		sscanf( cb, "%d", &m_point_size );
	}
#endif
	return true;
}

wxString wxRlClientBgHint::ConfigToXml( ){
	wxString s;
	s = m_hint;
	if( m_point_size!=-1 )
		s += wxString::Format(_T("<size>%d</size>"),m_point_size);
	// Check orientation and store
	return s;
}


/*
void SizeTest(){
	int s_rl_txt = sizeof(wxRlText);
	int s_font = sizeof(*wxNORMAL_FONT);
	int s_col = sizeof(wxColour(0,1,2));
	int s_rli = sizeof(wxRlItem);
	int s_si = sizeof(wxSizerItem);
	int s_str = sizeof(wxString(_T("Kalle")));
	int s_wnd = sizeof(wxWindow);
	int s_text_ctrl = sizeof(wxTextCtrl);
	int s_obj = sizeof(wxObject);
	int s_bs = sizeof(wxBoxSizer);
	int s_pt = sizeof(wxPoint);
	int s_sz = sizeof(wxSize);
	int s_rct = sizeof(wxRect);
}

class RWD {
public:
	RWD(){ SizeTest(); }
} g_rwd;
*/

wxRlText::wxRlText( const wxChar *str, 
        		  const wxFont font,
        		  wxColour fg_col,
        		  wxColour bg_col,
        		  int flag ) 
	: wxRlItem(RlText,flag), 
	  m_str(str), m_sel_start(0), m_sel_end(0), 
	  m_pfont(NULL), m_pfg_col(NULL), m_pbg_col(NULL)
{
	if( font!=*wxNORMAL_FONT && font!=wxNullFont )
		m_pfont = new wxFont(font);
	if( fg_col!=wxNullColour )
		m_pfg_col = new wxColour(fg_col);
	if( bg_col!=wxNullColour )
		m_pbg_col = new wxColour(bg_col);
	FitToText();
}

wxRlText::~wxRlText(){
	delete m_pfont;
	delete m_pfg_col;
	delete m_pbg_col;
}

void wxRlText::SetFont( wxFont font, bool do_fit ){
	delete m_pfont;
	if( font!=*wxNORMAL_FONT && font!=wxNullFont )
		m_pfont = new wxFont(font);
	if( do_fit )
		FitToText();
}

void wxRlText::SetText( const wxChar *str, bool do_fit ){
	m_str = str;
	if( do_fit )
		FitToText();
}

void wxRlText::SetFgCol( wxColour fg_col ){
	delete m_pfg_col;
	if( fg_col!=wxNullColour )
		m_pfg_col = new wxColour(fg_col);
}

void wxRlText::SetBgCol( wxColour bg_col ){
	delete m_pbg_col;
	if( bg_col!=wxNullColour )
		m_pbg_col = new wxColour(bg_col);
}

void wxRlText::FitToText( ){
	RL_SET_FLAG( m_state, RL_STATE_TEXT_FIT, false );
	SetProportion(0);
	wxMemoryDC dc;
	static wxBitmap st_bm(RL_TEXT_MAX_WIDTH,RL_TEXT_MAX_HEIGHT,1);
	dc.SelectObject(st_bm);
	if( dc.IsOk() ) {
		dc.SetFont(m_pfont?*m_pfont:*wxNORMAL_FONT);
		wxSize sz = dc.GetTextExtent(m_str);
		if( !(m_state&RL_STATE_TEXT_NO_MARGIN) )
			sz.SetWidth(sz.GetWidth()+RL_TEXT_MARGIN*2);
		SetMinSize( sz );
		RL_SET_FLAG( m_state, RL_STATE_TEXT_FIT, true );
	}
	else
		wxLogMessage(_T("Failed creating wxMemoryDC for font measurement"));
}


void wxRlText::Render( wxDC *dc ){
    // Retrieve our rectangle
    wxRect r = GetRect();
    
    // Setup DC font & colour
    const wxColour *pfg = NULL;
    //const wxColour *pbg = NULL;
    if(m_pfg_col){
    	pfg = &dc->GetTextForeground();
    	dc->SetTextForeground( *m_pfg_col );
    }
    // We paint BG ourselves since wx doen't do this always
    if( m_pfont ) dc->SetFont(*m_pfont);
    else dc->SetFont( *wxNORMAL_FONT );

    // Draw background ourselves here, sometimes we have a tight fit around the text, 
    // sometimes not
    if( m_pbg_col ){
    	wxBrush br(*m_pbg_col);
    	static wxPen pen(*m_pbg_col,0);
    	//wxPen st_pen(wxColour(0,0,0),0);	// Don't want a border
    	dc->SetBrush(br);
    	dc->SetPen(pen);
    	dc->DrawRectangle(r.x,r.y,r.width,r.height);
    }
    
    // # This does not draw selection yet 
    int h=r.height, w=r.width;
    int x = r.x;
    const wxChar *str = m_str;
    wxString s_chop;
    if( !(m_state&RL_STATE_TEXT_FIT) ){
    	s_chop = DcChopText(dc,str,w,&h);
    	str = s_chop;
    }
    if( !(m_state&RL_STATE_TEXT_NO_MARGIN) )
    	x += RL_TEXT_MARGIN;
    dc->DrawText(str, x, r.y + (r.height-h)/2);
    
    // Restore DC font & colour
    dc->SetFont( wxNullFont );
    if( pfg ) dc->SetTextForeground(*pfg);
    //if( pbg ) dc->SetTextBackground(*pbg);
}

// To run our global destructors before memory dump
#if (defined(__WXDEBUG__) && wxUSE_MEMORY_TRACING) || wxUSE_DEBUG_CONTEXT==1 
wxDebugContextDumpDelayCounter g_debug_rlwindow;
#endif

