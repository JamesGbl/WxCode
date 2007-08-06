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


#include "wx/RlDialogMgr.h"
#include "wx/RlManagerInt.h"
#include "wx/SizerUtils.h"

BEGIN_EVENT_TABLE(wxRlDialogManager,wxRlManager)
	EVT_TIMER(wxID_ANY, wxRlDialogManager::OnTimer)
END_EVENT_TABLE()

wxRlDialogManager::wxRlDialogManager( wxWindow *wnd, int init_sizer_flag ) 
	: wxRlManager( wnd, init_sizer_flag, RlSpacerTemp) {
	
	// We have to redo this, since vtable not available in wxRlManager
	SetWindow( wnd );
	
    m_brush_bg.SetColour( wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT) );
    m_pen_bg.SetColour( wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT) );
    
    m_timer_sbpaint.SetOwner(this);
}

void wxRlDialogManager::Dtor(){
	SetWindow( NULL );
}


void wxRlDialogManager::SetWindow(wxWindow *wnd){
	if( m_wnd ){
        m_wnd->Disconnect( wxEVT_PAINT, wxEvtFnCast(Paint) &wxRlDialogManager::OnPaint, NULL, this );
	}
	
	// Base class does its work
	wxRlManager::SetWindow( wnd );
	
	// Connect our events
	if( m_wnd ){
        m_wnd->Connect( wxEVT_PAINT, wxEvtFnCast(Paint) &wxRlDialogManager::OnPaint, NULL, this );
	}
}


void wxRlDialogManager::RenderSizerItem( wxDC *dc, wxSizerItem *item ){
    wxRlItem *rli = ToRlItem(item);
	
    // If in dialog, we repaint background area if not filled
	// (This is for filling in dialog background when dragging items) 
    if( m_is_dialog && m_mouse_action!=RlNoAction ){
		dc->SetBrush( m_brush_bg );
		dc->SetPen( m_pen_bg );
		//dc->SetBrush( *wxGREEN_BRUSH );
		//dc->SetPen( *wxRED_PEN );
    	if( item->IsSpacer() && RL_IS_EMPTY_SPACER(rli->GetType()) ){
    		dc->DrawRectangle( item->GetRect() );
    	}
    	else if( rli->IsWindow() && rli->GetParent() ){
    		wxBoxSizer *psz = wxDynamicCast( rli->GetParent()->GetSizer(), wxBoxSizer );
    		if( psz ){
	    		wxRect rw = GetRectWithBorders( rli ); //->GetWindow()->GetRect();
	    		wxRect rs( psz->GetPosition(), psz->GetSize() );
	    		if( psz->GetOrientation()&wxHORIZONTAL ){
	    			dc->DrawRectangle( rw.x, rs.y, rw.width, rw.y-rs.y );
	    			dc->DrawRectangle( rw.x, rw.y+rw.height, rw.width, rs.y+rs.height-(rw.y+rw.height) );
	    			//dc->DrawRectangle( rw.x, rs.y, rw.width, rs.height );
	    		}
	    		else{
	    			dc->DrawRectangle( rs.x, rw.y, rw.x-rs.x, rw.height );
	    			dc->DrawRectangle( rw.x+rw.width, rw.y, rs.x+rs.width-(rw.x+rw.width), rw.height );
	    			//dc->DrawRectangle( rs.x, rw.y, rs.width, rw.height );
	    		}
    		}
    	}
    }
    
    // Call base class after painting with BG colour
	wxRlManager::RenderSizerItem( dc, item );
}


// This is to get a DC that also works inside dialogs, where we have to 
// paint on top of wxStaticBox:es (which cover big areas). 
// On wxGTK this works fine without this trick.
wxDC *wxRlDialogManager::GetClientDc( wxRlItem *rli ){
#ifdef __WXMSW__
	if( m_is_dialog ){
		// Check if we're inside a group box
		while( rli ){
			wxStaticBoxSizer *psbsz = wxDynamicCast(rli->GetSizer(),wxStaticBoxSizer);
			rli = rli->GetParent();
			if( psbsz ){
        		wxDC *pdc = new wxClientDC(psbsz->GetStaticBox());
    	        wxPoint pt = psbsz->GetPosition();
    	        pdc->SetDeviceOrigin(-pt.x,-pt.y);
    	        return pdc;
			}
		}

		// Screen DC:s can be expensive to create and have side effects
		//pdc = new wxScreenDC();
		//wxPoint pt = m_wnd->ClientToScreen(wxPoint(0,0));
		//pdc->SetDeviceOrigin(pt.x,pt.y);
	}
#endif	
	// Give to base class
	return wxRlManager::GetClientDc( rli );
}


void wxRlDialogManager::OnPaint(wxPaintEvent& event){
    if( !m_wnd || !m_root ) return;

    // painting of RlItems disabled from outside
    if( !m_no_paint ){
	    // Make sure our background brush & pen are correct
	    wxColour fc;
	    if( GetFrameColour(fc) ){
	    	m_brush_bg.SetColour(fc);
	    	m_pen_bg.SetColour(fc);
	    }
	    // For dialogs, when moving edges, we have to repaint some later
	    wxSizer *pbsz = GetRoot().GetSizer();
	    if( m_is_dialog && pbsz )
	    	m_timer_sbpaint.Start( 100, true );
    }

    wxRlManager::OnPaint( event );
}

void wxRlDialogManager::OnTimer(wxTimerEvent& evt){
	if( evt.GetId()==m_timer_sbpaint.GetId() ){
		wxASSERT( m_is_dialog );
	#ifndef __WXGTK__	
		ExpArrP<wxSizerItem*> asi;
		asi.Push( &GetRoot() );
		wxSizerItem *psi;
		wxSizer *psz;
		while( asi.Size() ){
			psi = asi.Pop();
			if( !psi || !(psz=psi->GetSizer()) ) continue;

	        wxStaticBoxSizer *psbsz = wxDynamicCast(psz,wxStaticBoxSizer);
	        if( psbsz ){
		        // Paint in the DC of the static box
		        wxClientDC dc(psbsz->GetStaticBox());
		        wxRect r = psi->GetRect();
		        dc.SetDeviceOrigin(-r.x,-r.y);
		        m_is_dialog = false;	// Hack to disable overpainting static box
		        RenderSizerItem(&dc,psi);
		        m_is_dialog = true;
	        }
			
	        for( SizerIter it(psz); psi=it.Get(); it.Step() )
		        if( psi->GetSizer() ) asi.Push(psi);
		}
	#endif
	}
	
	else 
		// Leave to base class
		evt.Skip( );
}
