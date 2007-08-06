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

#ifndef RLDIALOGMGR_H 
#define RLDIALOGMGR_H

#include "wx/RlManager.h"

// A class to manage dialog windows (as a special case)
class WXDLLIMPEXP_RLAYOUT wxRlDialogManager : public wxRlManager {
public:
	wxRlDialogManager( wxWindow *wnd, int init_sizer_flag=wxVERTICAL|wxEXPAND );
    virtual ~wxRlDialogManager(){ Dtor(); }
    void Dtor();
    
    virtual void  SetWindow( wxWindow *wnd );
    virtual wxDC *GetClientDc( wxRlItem *rli );	
        
protected:
    wxTimer		m_timer_sbpaint;
	wxBrush 	m_brush_bg;
	wxPen		m_pen_bg;
	
    virtual void RenderSizerItem( wxDC *pdc, wxSizerItem *node );

    // Event handler
    void OnPaint(wxPaintEvent& evt);
    void OnTimer( wxTimerEvent& event );
    
    DECLARE_EVENT_TABLE()
};

#endif // RLDIALOGMGR_H

