
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


#include "wx/RlManager.h"
#include "wx/RlPainter.h"
#include "wx/wxUtils.h"
#include "wx/sc.h"

int RlPainter::Proportion( int type ){  // Sort out those that grow in
    switch( type){                      // both x and y direction
        case RlFrameBg:
        case RlClientBg:
        case RlClientTempBg:
        case RlClientBgHint:
        case RlPaneMoveHint:

        case RlNoteBook:             // These are compound ones, let them be included
        case RlAppPane:
        	
        case RlPaneContent:

        case RlCaptionText:         // A fix, since the caption text is located inside another sizer
            return RL_DEFAULT_PROPORTION;

        default:
            return 0;
    }
}

int RlPainter::Expandable( int type ){  // Sort out those that grow in
    switch( type){                      // at least one direction
        case RlPaneEdge:              // and are simple types
        case RlMediumEdge:
        case RlThinEdge:              // (not made from several SizerItems)
        case RlLayoutEdge:
        case RlCustomEdge1:
        case RlCustomEdge2:
        	
        case RlFrameBg:
        case RlClientBg:
        case RlClientTempBg:
        case RlClientBgHint:
        case RlPaneMoveHint:

        case RlPaneContent:
        	
        case RlCaptionText:
        	
        case RlLayoutGripper:
        case RlToolGripper:

        case RlCaption:             // These are compound ones, let them be included
        case RlSizer:
        case RlNoteBook:
        case RlAppPane:
        case RlToolPane:
        case RlTabBar:
            return wxEXPAND;

        default:
            return 0;
    }
}

int RlPainter::GetMetrics( wxRlItem *item ){
    if( item->GetType()==RlSpacer )
        return item->GetMinSize().GetWidth();	// Width & height should be the same
    else
        return GetMetrics(item->GetType());
}

int RlPainter::GetMetrics2nd( wxRlItem *item ){
    if( item->GetType()==RlSpacer )
        return item->GetMinSize().GetWidth();
    else
        return GetMetrics2nd(item->GetType());
}

bool RlPainter::RenderBorder( wxDC *dc, int flag, int size, const wxRect &r ){
    return RenderBorder( dc, flag, size, r.x, r.y, r.x+r.width, r.y+r.height );
}




bool RlStubPainter::RenderBorder( wxDC *dc, int flag, int size, int x1, int y1, int x2, int y2 ){
	wxUnusedVar(dc);
	flag=size; x1=x2; y1=y2; // Be quiet compiler.
    return false;
}

void RlStubPainter::Render( wxDC *dc, wxRlItem *rli, const wxRect &r ){
	wxUnusedVar(dc);
	wxUnusedVar(rli);
	wxUnusedVar(r);
}


int RlStubPainter::GetMetrics( int type ){
    switch( type ){
        case RlPaneEdge:
        case RlCustomEdge1:
        case RlCustomEdge2:
        return 6;

        case RlMediumEdge:
        return 4;

        case RlLayoutEdge:
        return 2;
            
        case RlThinEdge:
        return 2;

        case RlFrameBg:
        break;

        case RlClientBg:
        case RlClientTempBg:
        case RlPaneMoveHint:
        break;

        case RlToolGripper:
        return 8;
        break;


        case RlCaption:
        case RlCaptionText:
        return 23;
        break;

        case RlButton:
        return 14;
        break;

        case RlTabItem:
        return 18;
        break;

        case RlNull:
        return 0;
        break;

        default:
            break;
    }
    return 0;
}

int RlStubPainter::GetMetrics2nd( int type ){
    switch( type ){
        case RlButton:
        return 14;

        case RlTabItem:
        return 18;

        case RlPaneEdge:
        case RlMediumEdge:
        case RlThinEdge:
        case RlLayoutEdge:
        case RlCustomEdge1:
        case RlCustomEdge2:
        case RlFrameBg:
        case RlClientBg:
        case RlClientTempBg:
        case RlToolGripper:
        case RlCaption:
        case RlCaptionText:
        return GetMetrics( type );
        break;

        default:
            break;
    }

    return 0;
}


static unsigned char down_bits[]={
    0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff, 0xff,0xff, 0x1f,0xfc,
    0x3f,0xfe, 0x7f,0xff, 0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff };

static unsigned char left_bits[]={
    0xff,0xff, 0xff,0xff,
    0xff,0xff, 0x7f,0xff, 0x7f,0xfe, 0x7f,0xfc,
    0x7f,0xfe, 0x7f,0xff, 0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff };

static unsigned char right_bits[]={
    0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xfd, 0xff,0xfc, 0x7f,0xfc,
    0xff,0xfc, 0xff,0xfd, 0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff };

static unsigned char up_bits[]={
    0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff, 0x7f,0xff, 0x3f,0xfe,
    0x1f,0xfc, 0xff,0xff, 0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff };

static unsigned char close_bits[]={
    0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xCf,0xf9, 0x9f,0xfc, 0x3f,0xfe,
    0x3f,0xfe, 0x9f,0xfc, 0xCf,0xf9, 0xff,0xff,
    0xff,0xff, 0xff,0xff };



RlWinStdPainter::RlWinStdPainter(){
    m_col_lt_grey = wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT);
    m_col_grey = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
    m_col_dk_grey = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW);
    m_col_dk_shadow = wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW);
    m_col_active = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
    m_col_hilight_text = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT);
    m_col_text = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
    m_col_active_caption = wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION);
    m_col_inactive_caption = wxSystemSettings::GetColour(wxSYS_COLOUR_INACTIVECAPTION);
    m_col_window_frame = m_col_grey; //wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWFRAME);
    
    m_pen_lt_grey.SetColour( m_col_lt_grey );       // This is dialog and window border light grey
    m_pen_grey.SetColour( m_col_grey );             // This is darker grey around caption
    m_pen_dk_grey.SetColour( m_col_dk_grey );       // This is first shadow of 3d buttons
    m_pen_dk_shadow.SetColour( m_col_dk_shadow );   // This is second shadow of 3d buttons
    //m_pen_layout_edge.SetColour( wxColour(40, 80, 255) );
    m_pen_layout_edge.SetColour( m_col_active_caption );
    m_pen_frame.SetColour( m_col_window_frame );

    m_brush_lt_grey.SetColour( m_col_lt_grey );
    m_brush_dk_grey.SetColour( m_col_dk_grey );
    //m_brush_active.SetColour( m_col_active );
    m_brush_active.SetColour( m_col_active_caption );
    m_brush_layout_edge.SetColour( m_col_active_caption );
    m_brush_frame.SetColour( m_col_window_frame );    // m_col_inactive_caption
    //m_brush_move_hint.SetColour( m_col_active_caption );
    // A deep dark blue hint...
    m_brush_move_hint.SetColour( wxColour(0,0,80) );
    //m_brush_move_hint.SetStyle( wxCROSS_HATCH );
}

void RlWinStdPainter::Dtor(){
	// Not much to do
	int z=3;
}

void RlWinStdPainter::UpdateFrameColour( wxColour &fc ){
	m_col_window_frame = fc;
	m_brush_frame.SetColour( m_col_window_frame );
    m_pen_frame.SetColour( m_col_window_frame );
    // # After this buttons will still have old BG colour, since they 
    // used the value at creation time
}

void RlWinStdPainter::UpdateCaptionColour( wxColour &fc ){
	m_col_active_caption = fc;
	m_brush_active.SetColour( m_col_active_caption );
}

#define rb_line(pen,x1,y1,x2,y2) {dc->SetPen(pen);dc->DrawLine(x1,y1,x2,y2);}
bool RlWinStdPainter::RenderBorder( wxDC *dc, int flag, int size, int x1, int y1, int x2, int y2 ){

    // Don't support thicker borders than 2 pixels
    if( size>2 ) return false;
    if( flag&wxBORDER_NONE ) return false;
    int style = (flag&wxBORDER_MASK) & ~wxBORDER_DOUBLE;

    // In case we're drawing adjacent borders we should not draw on top of each other
    // If we're drawing a border on it's own, we want it to cover the full side.
    int dlft, drgt, dtop, dbot;
    dlft = flag&wxLEFT?1:0;
    drgt = flag&wxRIGHT?1:0;
    dtop = flag&wxTOP?1:0;
    dbot = flag&wxBOTTOM?1:0;

    // Set up for generic border drawing
    wxPen pen1, pen2, pen3, pen4;

    if( style&wxSUNKEN_BORDER ){
        if( size==2 ){
            pen1 = m_pen_dk_grey;
            pen2 = m_pen_dk_shadow;
            pen3 = m_col_window_frame;
            pen4 = *wxWHITE;
        }else{
            pen1 = m_pen_dk_shadow;
            pen3 = *wxWHITE;
        }
    }

    else if( style&wxRAISED_BORDER ){
        if( size==2 ){
            pen1 = m_col_window_frame;
            pen2 = *wxWHITE;
            pen3 = m_pen_dk_grey;
            pen4 = m_pen_dk_shadow;
        }else{
            pen1 = *wxWHITE;
            pen3 = m_pen_dk_grey;
        }
    }

    else if( style&wxSIMPLE_BORDER ){
        if( size==2 ){
            pen1 = m_pen_dk_grey;
            pen2 = *wxWHITE;
            pen3 = *wxWHITE;
            pen4 = m_pen_dk_grey;
        }else{
            pen1 = m_pen_dk_grey;
            pen3 = m_pen_dk_grey;
        }
    }
    
    else if( style&wxSTATIC_BORDER ){
        if( size==2 ){
            pen1 = *wxBLACK;
            pen2 = m_pen_lt_grey;
            pen3 = m_pen_lt_grey;
            pen4 = *wxBLACK;
        }else{
            pen1 = *wxBLACK;
            pen3 = *wxBLACK;
        }
    }
    else
        return false;

    // Draw the lines as indicated by flag bits
    if( flag&wxTOP ){
        rb_line(pen1, x1,y1,x2,y1);
        if( size==2 )
            rb_line(pen2, x1+dlft,y1+1,x2-drgt,y1+1);
    }
    if( flag&wxLEFT ){
        rb_line(pen1, x1,y1,x1,y2);
        if( size==2 )
            rb_line(pen2, x1+1,y1+dtop,x1+1,y2-dbot);
    }
    if( flag&wxBOTTOM ){
        rb_line(pen3, x1+dlft,y2-1,x2,y2-1);
        if( size==2 )
            rb_line(pen4, x1+dlft*2,y2-2,x2-drgt,y2-2);
    }
    if( flag&wxRIGHT ){
        rb_line(pen3, x2-1, y1+dtop,x2-1,y2);
        if( size==2 )
            rb_line(pen4, x2-2, y1+dtop*2,x2-2,y2-dbot);
    }
    return true;
}


bool RlWinStdPainter::SetButtonBitmap( int button, const char *bits, int w, int h ){
    if( button<0 || button>=RLP_MAX_BUTTONS ) return false;
    wxBitmap bm = wxAuiBitmapFromBits( (const unsigned char*)bits, w, h,
            m_col_window_frame, *wxBLACK, false );
    if( !bm.IsOk() ) return false;
    int dx = (w-(RLP_SZ_BUTTON-4-1))/2;
    int dy = (h-(RLP_SZ_BUTTON-4))/2;
    wxRect r( dx,dy, RLP_SZ_BUTTON-4, RLP_SZ_BUTTON-4 );
    m_buttons[button] = bm.GetSubBitmap( r );
    return m_buttons[button].IsOk();
}

bool RlWinStdPainter::SetButtonBitmap( int button, wxBitmap bm ){
    if( button<0 || button>=RLP_MAX_BUTTONS ) return false;
    if( !bm.IsOk() ) return false;
    int w = bm.GetWidth();
    int h = bm.GetHeight();
    int dx = (w-(RLP_SZ_BUTTON-4-1))/2;
    int dy = (h-(RLP_SZ_BUTTON-4))/2;
    wxRect r( dx,dy, RLP_SZ_BUTTON-4-1, RLP_SZ_BUTTON-4 );
    m_buttons[button] = bm.GetSubBitmap( r );
    return m_buttons[button].IsOk();
}


void RlWinStdPainter::Render( wxDC *dc, wxRlItem *rli, const wxRect &r ){
    // Convenient to unpack coords
    int x1 = r.GetX();
    int y1 = r.GetY();
    int x2 = x1+r.GetWidth();
    int y2 = y1+r.GetHeight();
    bool vert = r.GetHeight()>r.GetWidth();

    switch( rli->GetType() ){
        case RlPaneEdge:
            //dc->SetPen( m_pen_lt_grey );
            dc->SetPen( m_pen_dk_grey );
            if(vert) dc->DrawLine( x1, y1, x1, y2 );
            else dc->DrawLine( x1, y1, x2, y1 );
            dc->SetPen( *wxWHITE_PEN );
            if(vert) dc->DrawLine( x1+1, y1, x1+1, y2 );
            else dc->DrawLine( x1, y1+1, x2, y1+1 );
            //dc->SetPen( m_pen_lt_grey );
            dc->SetPen( m_pen_frame );
            dc->SetBrush( m_brush_frame );
            if(vert) dc->DrawRectangle(x1+2,y1,RLP_SZ_PANE_EDGE-4,y2-y1);
            else dc->DrawRectangle(x1,y1+2,x2-x1,RLP_SZ_PANE_EDGE-4);
            dc->SetPen( m_pen_dk_grey );
            if(vert) dc->DrawLine( x1+RLP_SZ_PANE_EDGE-2, y1, x1+RLP_SZ_PANE_EDGE-2, y2 );
            else dc->DrawLine( x1, y1+RLP_SZ_PANE_EDGE-2, x2, y1+RLP_SZ_PANE_EDGE-2 );
            dc->SetPen( *wxBLACK_PEN );
            if(vert) dc->DrawLine( x1+RLP_SZ_PANE_EDGE-1, y1, x1+RLP_SZ_PANE_EDGE-1, y2 );
            else dc->DrawLine( x1, y1+RLP_SZ_PANE_EDGE-1, x2, y1+RLP_SZ_PANE_EDGE-1 );
        break;

        case RlMediumEdge:
            dc->SetPen( *wxWHITE_PEN );
            if(vert) dc->DrawLine( x1, y1, x1, y2 );
            else dc->DrawLine( x1, y1, x2, y1 );
            //dc->SetPen( m_pen_lt_grey );
            dc->SetPen( m_pen_frame );
            if(vert) dc->DrawLine( x1+1, y1, x1+1, y2 );
            else dc->DrawLine( x1, y1+1, x2, y1+1 );
            dc->SetPen( m_pen_dk_grey );
            if(vert) dc->DrawLine( x1+2, y1, x1+2, y2 );
            else dc->DrawLine( x1, y1+2, x2, y1+2 );
            dc->SetPen( *wxBLACK_PEN );
            if(vert) dc->DrawLine( x1+3, y1, x1+3, y2 );
            else dc->DrawLine( x1, y1+3, x2, y1+3 );
        break;

        case RlThinEdge:
            dc->SetPen( m_pen_dk_grey );
            if(vert) dc->DrawLine( x1, y1, x1, y2 );
            else dc->DrawLine( x1, y1, x2, y1 );
            dc->SetPen( *wxBLACK_PEN );
            if(vert) dc->DrawLine( x1+1, y1, x1+1, y2 );
            else dc->DrawLine( x1, y1+1, x2, y1+1 );
        break;

        case RlClientBg:
        case RlClientTempBg:
        case RlClientBgHint:
            dc->SetBrush( m_brush_dk_grey );
            dc->SetPen( m_pen_dk_grey );
            dc->DrawRectangle( r.GetX(), r.GetY(), r.GetWidth(), r.GetHeight() );
            
            if( rli->GetType()!=RlClientBgHint )
            	break;
            
            {
	            // Draw text
	        	wxRlClientBgHint *pch = wxDynamicCast( rli, wxRlClientBgHint );
	        	wxASSERT( pch );
	        	
	        	// Setup either default or custom sized font
	        	static wxFont st_fhint( 13, wxDECORATIVE, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD, false, _T("") );
	        	if( pch->m_point_size ){
	        		if( pch->m_point_size==-1 )
	        			pch->m_font = st_fhint;
	        		else{
		        		wxFont fhint( pch->m_point_size, wxDECORATIVE, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_BOLD, false, _T("") );
		        		pch->m_font = fhint;
	        		}
	        		pch->m_point_size = 0;
	        	}
	        	if( !pch->m_font.IsOk() ){
	        		static bool did_log;
	        		if( !did_log )
	        			wxLogMessage(_T("RlWinStdPainter::Render - Failed creating hint font"));
	        		did_log = true;
	        		return;
	        	}
	        	
	            // Prepare to draw text
	            dc->SetTextForeground( m_col_text );
	            dc->SetFont(pch->m_font);
	            
	            // Split on multiple lines
	            wxSize sz_last;
	            ExpArr<const wxChar*> segs;
	            ExpArr<int>     widths;
	            const wxChar *str = pch->m_hint;
	            int w = x2-x1;
	            int h = -1;
	            while( str && *str ){
	            	const wxChar *pw = str;
	            	// Extend word by word
	            	while( pw && *pw ){
	            		const wxChar *pw_new = scstrchr(pw+1,_T(' ')); 
	            		if( !pw_new ) pw_new = pw+scstrlen(pw);
	            		wxSize sz = dc->GetTextExtent(wxString(str,pw_new-str));
	            		h = sz.GetHeight();
	    	            if( sz.GetWidth()>w-10 )
	    	            	break;
	    	            pw = pw_new;
	    	            sz_last = sz;
	            	}
	            	if( pw==str ) break;
	            	segs.Push( pw );
	            	widths.Push( sz_last.GetWidth() );
	            	str = *pw ? pw+1 : pw;
	            }
	            
	            // Now draw each segment
	            int hs = (h*5)/4;	// Some line spacing
	            // If too many lines vertically, remove some
	            while( segs.Size() && ((int)segs.Size()*hs)>(y2-y1-10) )
	            	segs.Pop();
	            int tot_h = segs.Size() * hs;
	            str = pch->m_hint;
	            //dc->SetBrush(m_brush_active);
	            // Now paint
	            for( int ix=0; ix<segs.Size(); str=segs[ix++] ){
	            	//dc->DrawRectangle( x1+5 + (w-10-widths[ix])/2, y1+(y2-y1-tot_h)/2 + ix*hs, widths[ix], h );
	            	if( *str==_T(' ') ) str++;
	            	dc->DrawText( wxString(str,segs[ix]-str), x1+5 + (w-10-widths[ix])/2, y1+(y2-y1-tot_h)/2 + ix*hs );
	            }
	            
	            dc->SetFont( wxNullFont );
            }
            break;

        case RlFrameBg:
            //dc->SetBrush( m_brush_lt_grey );
            //dc->SetPen( m_pen_lt_grey );
            dc->SetBrush( m_brush_frame );
            dc->SetPen( m_pen_frame );
            dc->DrawRectangle( r.GetX(), r.GetY(), r.GetWidth(), r.GetHeight() );
        break;

        case RlPaneMoveHint:
            dc->SetPen( m_pen_frame );
        	dc->SetBrush( m_brush_move_hint );
            dc->DrawRectangle( r.GetX(), r.GetY(), r.GetWidth(), r.GetHeight() );
        break;
        
            
        case RlLayoutEdge:{
        	// Check the RL_FLAG_HOVER to decide on edge color
        	static wxPen st_pen_hover(wxColour(0,160,0));
        	static wxBrush st_brush_hover(wxColour(0,160,0));
            dc->SetPen( rli->GetState()&RL_STATE_MOUSE_OVER? st_pen_hover : m_pen_layout_edge );
            dc->SetBrush( rli->GetState()&RL_STATE_MOUSE_OVER? st_brush_hover : m_brush_layout_edge );
            dc->DrawRectangle( x1,y1, x2-x1, y2-y1 );
        }
        break;
        
        case RlToolGripper:
            //dc->SetBrush( m_brush_lt_grey );
            //dc->SetPen( m_pen_lt_grey );
            dc->SetBrush( m_brush_frame );
            dc->SetPen( m_pen_frame );
            dc->DrawRectangle( x1,y1, x2-x1,y2-y1 );
            if( y2-y1>x2-x1 ){
                dc->SetPen( *wxWHITE_PEN );
                dc->DrawLine( x1+2,y1+3, x1+2,y2-3 );
                dc->DrawLine( x1+5,y1+3, x1+5,y2-3 );
                dc->SetPen( m_pen_dk_grey );
                dc->DrawLine( x1+3,y1+3, x1+3,y2-3 );
                dc->DrawLine( x1+6,y1+3, x1+6,y2-3 );
            }else{
                dc->SetPen( *wxWHITE_PEN );
                dc->DrawLine( x1+3,y1+2, x2-3,y1+2 );
                dc->DrawLine( x1+3,y1+5, x2-3,y1+5 );
                dc->SetPen( m_pen_dk_grey );
                dc->DrawLine( x1+3,y1+3, x2-3,y1+3 );
                dc->DrawLine( x1+3,y1+6, x2-3,y1+6 );
            }
        break;

        case RlLayoutGripper:
        	// For design purposes, should be clear and small 
        break;

        case RlCaption:
        {
        	// The caption background area
            dc->SetPen( m_pen_grey );
            //dc->SetBrush( rlc->m_is_active ? m_brush_active : m_brush_lt_grey );
            dc->SetBrush( rli->GetState()&RL_STATE_ACTIVE ? m_brush_active : m_brush_frame );
            dc->DrawRectangle( x1, y1, x2-x1, y2-y1-1 );
            dc->SetPen( m_pen_dk_grey );	// *wxBLACK_PEN ); // A dark line separating caption a bit
            dc->DrawLine( x1,y2-1, x2,y2-1 );

            // Now paint 3d border background
            x1++, y1++, x2--,y2-=2;  // Move in one pixel
            RenderBorder( dc, wxBORDER_ALL_SIDES|wxRAISED_BORDER, 1, x1,y1,x2,y2 );
        }
        break;
        	
        case RlCaptionText:
        {
            wxRlCaptionText* rlc = wxDynamicCast(rli,wxRlCaptionText);
            wxASSERT( rlc );

            x1++, y1++, x2--,y2-=2;  // Move in one pixel - needed ?

            // Prepare to draw text
            dc->SetTextForeground( rli->GetState()&RL_STATE_ACTIVE ? m_col_hilight_text : m_col_text );
            dc->SetFont(*wxNORMAL_FONT);
            
            wxString str_chop;
            const wxChar *str = rlc->m_caption;
            // Note: Store width for external use
            wxSize sz = dc->GetTextExtent(str);	 
            rlc->m_dc_width = sz.GetWidth(); 
            int h=sz.GetHeight();
            int w = (x2-x1-3); 
            if( rlc->m_dc_width>w ){
            	str_chop = DcChopText(dc,rlc->m_caption,w,&h);
            	str = str_chop;
            }
            dc->DrawText(str, x1+4, y1+(y2-y1-h)/2 );
            dc->SetFont( wxNullFont );
        }
        break;

        case RlButton:
        {
            x1++; // we leave one pixel space in front of button
            int xsz = x2-x1;
            int ysz = y2-y1;

            // Load button bitmap if needed
            int button = ((wxRlButton*)rli)->m_button;
            wxASSERT( button<32 );
            if( !m_buttons[button].IsOk() ){
                unsigned char *button_datas[] = {close_bits, down_bits, left_bits, right_bits, up_bits};
                wxASSERT( button<5 );
                wxBitmap bm = wxAuiBitmapFromBits( button_datas[button], 16, 12,
                        m_col_window_frame, *wxBLACK, false );
                // 4 is for 2+2 border pixels
                int dx = (16-(xsz-4))/2;
                int dy = (12-(ysz-4))/2;
                wxRect r( dx,dy, xsz-4, ysz-4 );
                m_buttons[button] = bm.GetSubBitmap( r );
            }
            dc->DrawBitmap( m_buttons[button], x1+2, y1+2, false );

            // Draw button borders
            RenderBorder( dc, wxBORDER_ALL_SIDES|wxRAISED_BORDER, 2, x1,y1,x2,y2 );
        }
        break;
        
        // case RlText: // Has dedicated renderer
        
        case RlTabItem:
        break;

        default:
        	// Come here for items that do no painting also - return quickly
            return;
            
        dc->SetBrush( wxNullBrush );
        dc->SetPen( wxNullPen );
    }
}


int RlWinStdPainter::GetMetrics( int type ){
    switch( type ){
        case RlPaneEdge:
        return RLP_SZ_PANE_EDGE;

        case RlMediumEdge:
        return RLP_SZ_MEDIUM_EDGE;

        case RlLayoutEdge:
            return RLP_SZ_LAYOUT_EDGE;
            
        case RlThinEdge:
        return RLP_SZ_THIN_EDGE;

        case RlFrameBg:
        break;

        case RlClientBg:
        case RlClientTempBg:
        break;

        case RlLayoutGripper:
            return RLP_XSZ_LAYOUT_GRIPPER;
            
        case RlToolGripper:
        return 8;
        break;


        case RlCaption:
        case RlCaptionText:
        return RLP_YSZ_CAPTION;
        break;

        case RlButton:
        return RLP_SZ_BUTTON;
        break;

        case RlTabItem:
        return 18;
        break;

        case RlNull:
        return 0;
        break;

        default:
            break;
    }

    return 0;
}


// The second direction, for those that don't expand
int RlWinStdPainter::GetMetrics2nd( int type ){
    switch( type ){
        case RlButton:
        return RLP_SZ_BUTTON;

        case RlTabItem:
        return 18;

        case RlPaneEdge:
        case RlMediumEdge:
        case RlThinEdge:
        case RlLayoutEdge:
        	
        case RlFrameBg:
        case RlClientBg:
        case RlClientTempBg:
        case RlToolGripper:
        case RlLayoutGripper:
        case RlCaption:
        case RlCaptionText:
        return GetMetrics( type );
        break;

        default:
            break;
    }

    return 0;
}

