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


#ifndef RL_DIALOG_H
#define RL_DIALOG_H

// This is all built for loading state, so skip if this flag is set
#ifndef RL_NO_STATE

#include "wx/rlayoutdef.h"

#include <wx/dialog.h>
#include <wx/timer.h>
#include <wx/xrc/xmlres.h>

#include "wx/RlDialogMgr.h"
#include "wx/RlManagerInt.h"
#include "wx/SizerUtils.h"

#include "utils/ExpArr.hpp"
#include "utils/CharBuf.h"

// These are default values for dialog/panel non-client size 
// After creating an instance, we can measure it and know exactly.
#define RLDLG_DEF_DIALOG_NC_SZ_X  10
#define RLDLG_DEF_DIALOG_NC_SZ_Y  27
#define RLDLG_DEF_PANEL_NC_SZ_X   6
#define RLDLG_DEF_PANEL_NC_SZ_Y   6

// Define how much extra space to have in dialogs (ratio larger than absolute min size)
#ifdef __WXMSW__
	// Dialog elements are usually a lot smaller on Windows
	#define DEF_SPACE_RATIO 1.2
#else
	// This is better for GTK
	#define DEF_SPACE_RATIO 1.1
#endif


template <class T>
class WXDLLIMPEXP_RLAYOUT wxRlBaseWindow : public T {
public:
	// Constructor for wxDialog base class
	wxRlBaseWindow( wxWindow* parent, wxWindowID id, const wxChar *title, 
					const wxChar *file_or_data, bool is_file, bool show_edges, double space_ratio, 
					const wxPoint& pos, const wxSize& size, 
					long style, const wxChar *name );
	
	// Constructor for wxPanel base class 
	wxRlBaseWindow( wxWindow* parent, wxWindowID id,  
			const wxChar *file_or_data, bool is_file, bool show_edges, double space_ratio,
			const wxPoint& pos, const wxSize& size, 
			long style, const wxChar *name );

	
	// Shared functions
#if wxUSE_UNICODE==1
	// A wchar_t version
	bool SetLayout( const wxChar *file_or_data, bool is_file=false, double space_ratio=DEF_SPACE_RATIO, bool subst_quotes=true );
#endif
	bool SetLayout( const char *file_or_data, bool is_file=false, double space_ratio=DEF_SPACE_RATIO, bool subst_quotes=true );
	bool SetLayout( wxRlItem *prli, double space_ratio=DEF_SPACE_RATIO );
	bool SetLayout( wxSizer *psz, int flag=0, double space_ratio=DEF_SPACE_RATIO );
	
	bool IsOk(){ return m_loaded; }
	// If any state of the dialog has been changed between reading the layout
	// and showing it, call here manually for wxPanel. wxDialog::ShowModal 
	// does it automatically.
	void GoThroughEnablers( );
	
	wxWindow *FindWindow( int id );
	wxSizer *FindSizer( const wxChar *name );
	
	wxString GetLabelOf( int id );
	wxString GetValueOf( int id );
	bool 	SetValueOf( int id, const wxChar *val );
	int 	GetIntValueOf( int id );
	bool 	SetIntValueOf( int id, int val );
	
	void ShowEdges( bool do_show );
	void ShowGrippers( bool do_show );
	
protected:
	// Common part of constructors
	void Init( const wxChar *file_or_data, bool is_file, bool show_edges, double space_ratio );
	void Resize( double space_ratio );
	bool SetLayoutCommon( double space_ratio );
	wxRlXmlTags *FindTagsOf( wxCommandEvent &evt );
    	
	bool 	m_loaded;
    wxRect 	m_rct;
	wxRlDialogManager 	m_rlmgr;
	ExpArrP<wxRlItem*> 	m_enablers;
	static wxSize ms_nc_sz;
    wxTimer		m_timer;	
    
	// Event handlers
	void OnRadioSelected( wxCommandEvent& evt);
	void OnCheckBoxSelected( wxCommandEvent& evt);
    void OnTimer( wxTimerEvent& event );
};


class WXDLLIMPEXP_RLAYOUT wxRlDialog : public wxRlBaseWindow<wxDialog> {
public:
	wxRlDialog( wxWindow* parent, wxWindowID id, const wxChar *title, 
			const wxChar *file_or_data=NULL, bool is_file=false, bool show_edges=false, double space_ratio=DEF_SPACE_RATIO, 
			const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, 
			long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, const wxChar *name = _T("dialogBox") );

	virtual int ShowModal(){
		GoThroughEnablers( );
		//wxDialog::Show( true );
		//wxWindow::SetSize( m_rct );
		return wxDialog::ShowModal( );
	}
	virtual bool Show( bool show = true ){
		GoThroughEnablers( );
		return wxDialog::Show( show );
	}
	
protected:
    //wxButton    *m_escape_dummy;	// A button enabling escape key in dialogs	
	void OnButtonClicked( wxCommandEvent& evt );
	void OnRadioSelected( wxCommandEvent& evt);
    DECLARE_EVENT_TABLE()
};

//template<class T>
//wxSize wxRlBaseWindow<T>::ms_nc_sz;


class WXDLLIMPEXP_RLAYOUT wxRlPanel : public wxRlBaseWindow<wxPanel> {
public:
	wxRlPanel( wxWindow* parent, wxWindowID id,  
			const wxChar *file_or_data=NULL, bool is_file=false, bool show_edges=false, double space_ratio=DEF_SPACE_RATIO,  
			const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, 
			long style = wxTAB_TRAVERSAL, const wxChar *name = _T("panel") );

protected:
    DECLARE_EVENT_TABLE()
};



template<class T>
wxRlBaseWindow<T>::wxRlBaseWindow( wxWindow* parent, wxWindowID id, const wxChar *title, 
					const wxChar *file_or_data, bool is_file, bool show_edges, double space_ratio, 
					const wxPoint& pos, const wxSize& size, 
					long style, const wxChar *name )
	: T( parent, id, title, pos, size, style, name), 
	  m_rlmgr(this, 0/*, RlSpacerTemp*/ ) {

	Init( file_or_data, is_file, show_edges, space_ratio );
}

template<class T>
wxRlBaseWindow<T>::wxRlBaseWindow( wxWindow* parent, wxWindowID id,  
		const wxChar *file_or_data, bool is_file, bool show_edges, double space_ratio, 
		const wxPoint& pos, const wxSize& size, 
		long style, const wxChar *name ) : 
	T( parent, id, pos, size, style, name) , 
	m_rlmgr(this, 0/*, RlSpacerTemp*/ ) {

	Init( file_or_data, is_file, show_edges, space_ratio );
}


template<class T>
void wxRlBaseWindow<T>::Init( const wxChar *file_or_data, bool is_file, bool show_edges, double space_ratio ){
	
	m_loaded = false;
	// Show or hide edges/grippers
	ShowEdges(show_edges);
	ShowGrippers(show_edges);
	//m_rlmgr.ItemHideAll( !show_edges );
	
	// So that dialog initially has focus
	// # Leave this to dialog layout parsing/creation
	//wxWindow::SetFocus();
	
	if( file_or_data )
		SetLayout( file_or_data, is_file, space_ratio );
	
	m_timer.SetOwner(this);
	m_timer.Start( 250, false);
    
	Connect( wxEVT_TIMER, wxEvtFnCast(Timer) &wxRlBaseWindow<T>::OnTimer, NULL, this );
	Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxEvtFnCast(Command) &wxRlBaseWindow<T>::OnRadioSelected, NULL, this );
	Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxEvtFnCast(Command) &wxRlBaseWindow<T>::OnCheckBoxSelected, NULL, this );
}


// Set dialog size
template<class T>
void wxRlBaseWindow<T>::Resize( double space_ratio ){
	// At this point the dialog has not been laid out and does not have 
	// borders, client size and so on set up. None the less we have
	// to center it. 
	wxSize sz_cl = m_rlmgr.GetRoot().CalcMin();
	sz_cl.Scale(space_ratio,space_ratio);
	wxSize sz(sz_cl);
	// WXGTK sets up top-level windows mixing not accounting for non-client
	// size. When asking for window size 100x100 we actually get client 
	// size 100x100. Keep add nc size for calculation, remove after.
	sz += ms_nc_sz;	// Add client and non-client size
	wxRect r = T::GetScreenRect();
	wxPoint pt = T::ClientToScreen(wxPoint(0,0));
	
	wxWindow *wnd = wxWindow::GetParent();
	if( wnd ){
		wxRect rp = wnd->GetScreenRect();
		r.width = sz.GetWidth();
		r.height = sz.GetHeight();
		r.x = rp.x + (rp.width-r.width)/2;
		r.y = rp.y + (rp.height-r.height)/2;
	}

	/*
#ifdef __WXGTK__
	// Remove these since wxGTK adds them once again.
	r.width -= ms_nc_sz.GetWidth();
	r.height -= ms_nc_sz.GetHeight();
#endif
	*/
	
	T::SetSize( r );
	m_rct = r;
	m_rlmgr.Update();
}

#if wxUSE_UNICODE==1
template<class T>
bool wxRlBaseWindow<T>::SetLayout( const wxChar *file_or_data, bool is_file, double space_ratio, bool subst_quotes ){
    CharBuf s = (const char*)wxString(file_or_data).mb_str(wxConvUTF8);
	return SetLayout( s, is_file, space_ratio, subst_quotes );
}
#endif

// Also an Ascii/UTF8 version
template<class T>
bool wxRlBaseWindow<T>::SetLayout( const char *file_or_data, bool is_file, double space_ratio, bool subst_quotes ){
	CharBuf s;
	m_loaded = false;
	if( is_file ){
		if( !s.LoadFromFile(file_or_data) )
			return false;
		file_or_data = s.Str();
	}
	else if( subst_quotes ){
		s = file_or_data;
		s.Subst( '\'', '\"' );
		file_or_data = s.Str();
	}
	if( !m_rlmgr.SetState(file_or_data) ) return false;
	
	return SetLayoutCommon( space_ratio );
}

template<class T>
bool wxRlBaseWindow<T>::SetLayoutCommon( double space_ratio ){
	m_loaded = true;
	Resize( space_ratio );

	// Find all 'enablers' and store in one array
	m_enablers.Empty();
	ExpArrP<wxSizerItem*> asi;
	asi.Push( &m_rlmgr.GetRoot() );
	wxSizerItem *psi;
	wxSizer *psz;
	while( asi.Size() ){
		if( !(psi = asi.Pop()) ) continue;
		wxRlXmlTags *prxt = wxDynamicCast(psi->GetUserData(),wxRlXmlTags);
		if( prxt && prxt->m_enables.Length() ){
			wxASSERT( ::IsRlItem(psi) );
			m_enablers.Push(ToRlItem(psi));
		}
		if( !(psz=psi->GetSizer()) ) continue;
        for( SizerIter it(psz); psi=it.Get(); it.Step() )
	        asi.Push(psi);
	}
	
	// Make sure enabled state is correct from start
	GoThroughEnablers( );
	
	return true;
}


template<class T>
bool wxRlBaseWindow<T>::SetLayout( wxRlItem *prli, double space_ratio ){
	m_rlmgr.SetRoot( prli );
	if( !(m_loaded = (&m_rlmgr.GetRoot()==prli)) ) return false;
	return SetLayoutCommon( space_ratio );
}

template<class T>
bool wxRlBaseWindow<T>::SetLayout( wxSizer *psz, int flag, double space_ratio ){
	m_rlmgr.SetRoot( psz, flag );
	if( !(m_loaded = (m_rlmgr.GetRoot().GetSizer()==psz)) ) return false;
	return SetLayoutCommon( space_ratio );
}


template<class T>
wxWindow * wxRlBaseWindow<T>::FindWindow( int id ){
	return FindWindowById( id, this );
}

template<class T>
wxSizer *wxRlBaseWindow<T>::FindSizer( const wxChar *name ){
	return m_rlmgr.FindSizer(name);
}

template<class T>
wxString wxRlBaseWindow<T>::GetLabelOf( int id ){
	wxWindow *wnd = FindWindow(id);
	if( !wnd ) return wxEmptyString;
	return wnd->GetLabel(); 
}

template<class T>
wxString wxRlBaseWindow<T>::GetValueOf( int id ){
	wxWindow *wnd = FindWindow(id);
	if( !wnd ) return wxEmptyString;
	
	wxCheckBox *pcb = wxDynamicCast( wnd, wxCheckBox );
	if( pcb )
		return pcb->GetValue() ? _T("1") : _T("0");
		
	wxTextCtrl *ptc = wxDynamicCast( wnd, wxTextCtrl );
	if( ptc )
		return ptc->GetValue();
	
	wxRadioButton *prb = wxDynamicCast( wnd, wxRadioButton );
	if( prb ){
		int v = GetIntValueOf(id);
		if( v>=0 ) return wxString::Format(_T("%d"),v);
		return wxEmptyString;
	}
	
	wxControlWithItems *pcwi = wxDynamicCast( wnd, wxControlWithItems );
	if( pcwi )
		return pcwi->GetString( pcwi->GetSelection() ); 
	
	return wxEmptyString;
}

template<class T>
int wxRlBaseWindow<T>::GetIntValueOf( int id ){
	wxWindow *wnd = FindWindow(id);
	if( !wnd ) return -1;
	
	wxCheckBox *pcb = wxDynamicCast( wnd, wxCheckBox );
	if( pcb )
		return pcb->GetValue() ? 1 : 0;
		
	wxTextCtrl *ptc = wxDynamicCast( wnd, wxTextCtrl );
	if( ptc ){
		long v;
		if( ptc->GetValue().ToLong(&v) )
			return (int)v;
		else 
			return 0x80000000;	// Invalid number
	}
	
	wxRadioButton *prb = wxDynamicCast( wnd, wxRadioButton );
	if( prb ){
		if( !m_rlmgr.GetRoot().IsOk() || !m_rlmgr.GetRoot().GetSizer() ) 
			return false;
		wxRlItem *rli = ToRlItem( m_rlmgr.GetRoot().GetSizer()->GetItem(prb,true) );
		if( !rli ) return -1;
		wxSizer *psz = rli->GetParent()->GetSizer();
		if( !psz ) return -1;
		int ix = psz->GetChildren().IndexOf(rli);
		
		// Step back until we find the group item
		int ix_fst_radio = -1;
		while( ix>=0 ){
			wxSizerItem *psi = psz->GetItem(ix);
			if( psi && psi->GetWindow() ){
				prb = wxDynamicCast( psi->GetWindow(), wxRadioButton );
				if( prb ){
					ix_fst_radio = ix;
					// # This would break if we have several radio groups on one sizer
					// and the wxRB_GROUP flag could not be read
					if( prb->GetWindowStyle()&wxRB_GROUP )
						break;
				}
			}
			ix--;
		}
		if( ix_fst_radio<0 ) return -1;
		ix = ix_fst_radio;
		
		// Step forwards until we find the selected one
		int item_cnt = psz->GetChildren().GetCount();
		int cnt = 0;
		while( ix<item_cnt ){
			wxSizerItem *psi = psz->GetItem(ix);
			if( psi && psi->GetWindow() ){
				prb = wxDynamicCast( psi->GetWindow(), wxRadioButton );
				if( prb ){
					if( prb->GetValue() )
						return cnt;
					cnt++;
				}
			}
			ix++;
		}
		return -1;
	}
	
	wxControlWithItems *pcwi = wxDynamicCast( wnd, wxControlWithItems );
	if( pcwi )
		return pcwi->GetSelection(); 

	return 0x80000000;	// Invalid number
}

template<class T>
bool wxRlBaseWindow<T>::SetValueOf( int id, const wxChar *val ){
	wxWindow *wnd = FindWindow(id);
	if( !wnd ) return false;
	
	wxCheckBox *pcb = wxDynamicCast( wnd, wxCheckBox );
	if( pcb ){
		bool b = val && (!wxStricmp(val,_T("on")) || !wxStricmp(val,_T("true")) || 
				!wxStricmp(val,_T("check")) || !wxStricmp(val,_T("1")) );
		pcb->SetValue(b);
		return true;
	}
		
	wxTextCtrl *ptc = wxDynamicCast( wnd, wxTextCtrl );
	if( ptc ){
		ptc->SetValue(val);
		return true;
	}
	
	wxRadioButton *prb = wxDynamicCast( wnd, wxRadioButton );
	wxControlWithItems *pcwi = wxDynamicCast( wnd, wxControlWithItems );
	if( prb || pcwi ){
		long l;
		if( !wxString(val).ToLong(&l) ) return false;
		return SetIntValueOf( id, (int)l );
	}
	
	return false;
}

template<class T>
bool wxRlBaseWindow<T>::SetIntValueOf( int id, int v ){
	wxWindow *wnd = FindWindow(id);
	if( !wnd ) return false;
	
	wxCheckBox *pcb = wxDynamicCast( wnd, wxCheckBox );
	if( pcb ){
		pcb->SetValue(v);
		return true;
	}
		
	wxTextCtrl *ptc = wxDynamicCast( wnd, wxTextCtrl );
	if( ptc ){
		wxString s;
		s << v;
		ptc->SetValue(s);
		return true;	
	}
	
	wxRadioButton *prb = wxDynamicCast( wnd, wxRadioButton );
	if( prb ){
		if( !m_rlmgr.GetRoot().IsOk() || !m_rlmgr.GetRoot().GetSizer() )
			return false;
		wxRlItem *rli = ToRlItem( m_rlmgr.GetRoot().GetSizer()->GetItem(prb,true) );
		if( !rli ) return -1;
		wxSizer *psz = rli->GetParent()->GetSizer();
		if( !psz ) return -1;
		int ix = psz->GetChildren().IndexOf(rli);
		
		// Step back until we find the group item
		int ix_fst_radio = -1;
		while( ix>=0 ){
			wxSizerItem *psi = psz->GetItem(ix);
			if( psi && psi->GetWindow() ){
				prb = wxDynamicCast( psi->GetWindow(), wxRadioButton );
				if( prb ){
					ix_fst_radio = ix;
					// # This would break if we have several radio groups on one sizer
					// and the wxRB_GROUP flag could not be read
					if( prb->GetWindowStyle()&wxRB_GROUP )
						break;
				}
			}
			ix--;
		}
		if( ix_fst_radio<0 ) return false;
		ix = ix_fst_radio;
		
		// Step forwards, deselect until we find the selected one
		int item_cnt = psz->GetChildren().GetCount();
		int cnt = 0;
		while( ix<item_cnt ){
			wxSizerItem *psi = psz->GetItem(ix);
			if( psi && psi->GetWindow() ){
				prb = wxDynamicCast( psi->GetWindow(), wxRadioButton );
				if( prb ){
					if( cnt==v ){
						prb->SetValue( true );
						return true;
					}
					else prb->SetValue( false );
					cnt++;
				}
			}
			ix++;
		}
		return false;
	}
	
	wxControlWithItems *pcwi = wxDynamicCast( wnd, wxControlWithItems );
	if( pcwi ){
		pcwi->SetSelection( v );
		return true;
	}

	return false;	
}


template<class T>
void wxRlBaseWindow<T>::ShowEdges( bool do_show ){
	//m_rlmgr.ItemHideAll( !do_show );
	if( do_show ) m_rlmgr.ItemTypeShow(RlLayoutEdge);
	else m_rlmgr.ItemTypeHide(RlLayoutEdge);
}

template<class T>
void wxRlBaseWindow<T>::ShowGrippers( bool do_show ){
	//m_rlmgr.ItemHideAll( !do_show );
	if( do_show ) m_rlmgr.ItemTypeShow(RlLayoutGripper);
	else m_rlmgr.ItemTypeHide(RlLayoutGripper);
}


template<class T>
wxRlXmlTags* wxRlBaseWindow<T>::FindTagsOf( wxCommandEvent &evt ){
	// First find wxRlItem of this radio button
	wxSizer *psz = m_rlmgr.GetRoot().GetSizer();
	if( !psz ) return NULL;
	wxRlItem *rli = ToRlItem( psz->GetItem( (wxWindow*)evt.GetEventObject(), true ) );
	if( !rli ) return NULL;
	wxRlXmlTags *prxt = wxDynamicCast(rli->GetUserData(),wxRlXmlTags);
	return prxt;
}


template<class T>
void wxRlBaseWindow<T>::GoThroughEnablers( ){
	// Instead of doing a single one, do all 'enablers', since we don't 
	// get deselection notifications.
	wxRlItem *rli;
	for( EapIter<wxRlItem*> it(m_enablers); rli=it(); it++ ){
		wxRlXmlTags *prxt = wxDynamicCast(rli->GetUserData(),wxRlXmlTags);
		if( prxt ){
			// Go through all items in the m_enables string, enable or disable
			int p0=0, p1;
			//bool do_enable = evt.IsSelection() || evt.IsChecked();
			bool do_enable = false;
			wxRadioButton *prb = wxDynamicCast(rli->GetWindow(),wxRadioButton);
			wxCheckBox *pcb = wxDynamicCast(rli->GetWindow(),wxCheckBox);
			if( prb ) do_enable = prb->GetValue();
			if( pcb ) do_enable = pcb->GetValue();
			while( true ){
				p1 = p0;
				while( p1<(int)prxt->m_enables.Length() && prxt->m_enables[p1]!=_T(',') ) p1++;
				if( p1==p0 ) break;
				wxString s(((const wxChar*)prxt->m_enables)+p0,p1-p0);
				p0 = p1+1;
				// Now we have the name of the item to enable/disable
				int id = wxXmlResource::GetXRCID(s);
				wxWindow *wnd = FindWindowById( id, this );
				if( wnd )
					wnd->Enable( do_enable );
				else
					wxLogMessage(wxString::Format(_T("wxRlBaseWindow::On...Selected - Window to enable not found %s"), (const wxChar*)s) );
			}
		}
	}
}

template<class T>
void wxRlBaseWindow<T>::OnRadioSelected( wxCommandEvent& evt ){
	// Go through all
	GoThroughEnablers( );
}

template<class T>
void wxRlBaseWindow<T>::OnCheckBoxSelected( wxCommandEvent& evt ){
	// Go through all
	GoThroughEnablers( );
}

template<class T>
void wxRlBaseWindow<T>::OnTimer( wxTimerEvent &evt ){
	if( ms_nc_sz.y==RLDLG_DEF_DIALOG_NC_SZ_Y ){
		wxSize rc = this->GetClientSize();
		wxRect r = this->GetScreenRect();
		if( rc.GetWidth()>0 && r.width>0 )
			ms_nc_sz.Set(r.width-rc.GetWidth(),r.height-rc.GetHeight());
	}
	
	//if( !m_escape_dummy )
	//	m_escape_dummy = new wxButton( this, wxID_CANCEL, _T("ED"), wxPoint(-100,-100) );
	
 	bool bx = wxGetKeyState(wxKeyCode('X'));
 	bool bs = wxGetKeyState(wxKeyCode('S'));
 	bool bh = wxGetKeyState(wxKeyCode('H'));
 	bool bsh = wxGetKeyState(wxKeyCode(WXK_SHIFT));
 	bool bct = wxGetKeyState(wxKeyCode(WXK_CONTROL));
 	//printf( "wxrd:ot - %d %d %d\n", bx, bs, bc );
 	
 	if( bsh && bct ){
 		if( bx ){
	 		wxString wxs;
	 		if( m_rlmgr.GetState( wxs ) ){
	 			//u8Str us = (const wxChar*)wxs;
                CharBuf cb = (const char*)wxs.mb_str(wxConvUTF8);
	 			cb.SaveToFile( "wxRlDialog.state" );
	 		}
 		}
 		if( bs ){
 			/*
 			if( m_rlmgr.GetHideAll() ){
 				m_rlmgr.ItemHideAll(false);
 				Refresh();
 			}
 			*/
 			int do_refresh = 0;
 			if( !m_rlmgr.ItemTypeIsShown(RlLayoutEdge) )
 				ShowEdges(true), do_refresh++;
 			if( !m_rlmgr.ItemTypeIsShown(RlLayoutGripper) )
 				ShowGrippers(true), do_refresh++;
 			if( do_refresh ) this->Refresh();
 		}
 		if( bh ){
 			/*
 			if( !m_rlmgr.GetHideAll() ){
 				m_rlmgr.ItemHideAll(true);
 				Refresh();
 			}
 			*/
 			int do_refresh = 0;
 			if( m_rlmgr.ItemTypeIsShown(RlLayoutEdge) )
 				ShowEdges(false), do_refresh++;
 			if( m_rlmgr.ItemTypeIsShown(RlLayoutGripper) )
 				ShowGrippers(false), do_refresh++;
 			if( do_refresh ) this->Refresh();
 		}
 	}
}

#endif // RL_NO_STATE

#endif // RL_DIALOG_H


