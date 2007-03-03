/////////////////////////////////////////////////////////////////////////////
// Name:        common/minibtncmn.cpp
// Purpose:     wxCloseBoxBase, wxMaximizeBoxBase, wxMinimizeBoxBase,
//              wxCollapseBoxBase
// Author:      Francesco Montorsi
// Created:     2004/03/03
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/window.h"
    #include "wx/log.h"
    #include "wx/dc.h"
#endif

// additional includes
#include "wx/minibtn.h"



#ifdef wxEXTMINIFRM_USE_MINIBTN

// define the event type for wxMiniButtons
DEFINE_EVENT_TYPE( wxEVT_COMMAND_CLOSEBOX_CLICKED )
DEFINE_EVENT_TYPE( wxEVT_COMMAND_MAXIMIZEBOX_CLICKED )
DEFINE_EVENT_TYPE( wxEVT_COMMAND_MINIMIZEBOX_CLICKED )
DEFINE_EVENT_TYPE( wxEVT_COMMAND_COLLAPSEBOX_CLICKED )



// ------------------------------------
// wxCLOSEBOX
// ------------------------------------

void wxCloseBoxBase::Draw(wxDC &dc, int x, int y)
{
    int bw = GetSize().GetWidth();
    int bh = GetSize().GetHeight();

    wxEXTMF_LOG(wxT("wxCloseBoxBase::Draw() at %d;%d - %d;%d"), x, y, bw, bh);
    
    // draw our frame
    DrawButtonFrame(dc, x, y);
    
    //wxPen pen(wxColour(64, 64, 64), 1, wxSOLID);
    dc.SetPen(*wxBLACK_PEN);//pen);

    int width = bw/2;
    int height = bh-GetBorderSize()*3;

    int xOfs = (m_bPressed) ? 2 : 1;
    int yOfs = (m_bPressed) ? 2 : 1;
    yOfs += m_pParent->GetBorderSize();
    xOfs += m_pParent->GetBorderSize();

    // draw four lines: that is, an X two pixels thick
    for (int i = 0; i != 2; ++i)
    {
        dc.DrawLine(x + xOfs + i,
                    y + yOfs,
                    x + xOfs + i + width,
                    y + yOfs + height);

        dc.DrawLine(x + xOfs + i + width ,
                    y + yOfs,
                    x + xOfs + i,
                    y + yOfs + height);
    }
}

bool wxCloseBoxBase::Exec()
{
    wxEXTMF_LOG(wxT("wxCloseBoxBase::Exec() - sending a wxEVT_COMMAND_CLOSEBOX_CLICKED to %d"), GetParent());

    wxCommandEvent ev(wxEVT_COMMAND_CLOSEBOX_CLICKED, GetParent()->GetId());
    GetParent()->AddPendingEvent(ev);
    
    return TRUE;
}





// ------------------------------------
// wxMAXIMIZEBOX
// ------------------------------------

void wxMaximizeBoxBase::Draw(wxDC &dc, int x, int y)
{
    int bw = GetSize().GetWidth();
    int bh = GetSize().GetHeight();

    wxEXTMF_LOG(wxT("wxMaximizeBoxBase::Draw() at %d;%d - %d;%d"), x, y, bw, bh);
    
    // draw our frame
    DrawButtonFrame(dc, x, y);
    
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen(*wxBLACK_PEN);

    // just draw a black rectangle centered in the button
    const int space = GetBorderSize()+1;
    dc.DrawRectangle(x+space, y+space,
                    bw-space*2, bh-space*2);
                    
    // and make the top border thicker
    dc.DrawLine(x+space, y+space+1, x+bw-space, y+space+1);
    dc.DrawLine(x+space, y+space+2, x+bw-space, y+space+2);
}

bool wxMaximizeBoxBase::Exec()
{
    wxEXTMF_LOG(wxT("wxMaximizeBoxBase::Exec() - sending a wxEVT_COMMAND_MAXIMIZEBOX_CLICKED"));

    // the behaviour of this function should be defined in the event handler
    // of wxEVT_COMMAND_MAXIMIZEBOX_CLICKED... 
    // anyway, I could not invent any special function for this button...
    wxCommandEvent ev(wxEVT_COMMAND_MAXIMIZEBOX_CLICKED, GetParent()->GetId());
    GetParent()->AddPendingEvent(ev);

    return TRUE;
}




// ------------------------------------
// wxMINIMIZEBOX
// ------------------------------------

void wxMinimizeBoxBase::Draw(wxDC &dc, int x, int y)
{
    int bw = GetSize().GetWidth();
    int bh = GetSize().GetHeight();

    wxEXTMF_LOG(wxT("wxMinimizeBoxBase::Draw() at %d;%d - %d;%d"), x, y, bw, bh);
    
    // draw our frame
    DrawButtonFrame(dc, x, y);
    
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.SetPen(*wxBLACK_PEN);

    // just draw a black rectangle positioned in the left-bottom angle
    const int rectheight = bh/4, rectwidth = bw/2;
    dc.DrawRectangle(x+GetBorderSize(), 
                    y+bh-GetBorderSize()-rectheight-1,		// -1 is a little adjustment...
                    rectwidth, rectheight);
}

bool wxMinimizeBoxBase::Exec()
{
    wxEXTMF_LOG(wxT("wxMinimizeBoxBase::Exec() - sending a wxEVT_COMMAND_MINIMIZEBOX_CLICKED"));

    // the behaviour of this minibutton should be defined in the event handler
    // which will intercept this event...
    // however, I think this would be a good dock button in a wider
    // dock framework.
    wxCommandEvent ev(wxEVT_COMMAND_MINIMIZEBOX_CLICKED, GetParent()->GetId());
    GetParent()->AddPendingEvent(ev);

    return TRUE;
}




// ------------------------------------
// wxCOLLAPSEBOX
// ------------------------------------

void wxCollapseBoxBase::Draw(wxDC &dc, int x, int y)
{
    wxEXTMF_LOG(wxT("wxCollapseBoxBase::Draw() - parent's roll state: %d"), isParentRolled());
    dc.SetPen(*wxTRANSPARENT_PEN);

    wxPoint arr[3];

    int yOfs  = (m_bPressed) ? 1 : 0;
    int xOfs  = (m_bPressed) ? 3 : 2;
    int width = (int)(GetSize().GetWidth()*0.5);
    
    yOfs += m_pParent->GetBorderSize();

    // rotating/shifting triangle inside collapse box
    arr[0].x = xOfs;
    arr[0].y = yOfs;
    arr[2].x = xOfs;							// bottom-left
    arr[2].y = GetSize().GetHeight()-(m_pParent->GetBorderSize()-yOfs);
    arr[2].y -= 3;
    arr[1].x = xOfs + width;					// right
    arr[1].y = (arr[2].y + arr[0].y)/2;

    if (!isParentRolled()) {

        if (m_bPressed) xOfs = 0;
        arr[0].x = GetSize().GetWidth() - arr[0].x - xOfs;
        arr[1].x = GetSize().GetWidth() - arr[1].x - xOfs;
        arr[2].x = GetSize().GetWidth() - arr[2].x - xOfs;

    } else {

#ifdef __WXMSW__
    arr[0].x += 1;
    arr[1].x += 1;
    arr[2].x += 1;
#endif
    }

    // offset triangle coords to right pos
    arr[0].x += m_ptPos.x;
    arr[0].y += m_ptPos.y;
    arr[1].x += m_ptPos.x;
    arr[1].y += m_ptPos.y;
    arr[2].x += m_ptPos.x;
    arr[2].y += m_ptPos.y;
    
    // draw the button appearance frame
    DrawButtonFrame(dc, x, y);

    // choose the right color for the triangle
    if (!m_bEnabled)
        dc.SetBrush(*wxGREY_BRUSH);
    else
        dc.SetBrush(*wxBLACK_BRUSH);	
    dc.DrawPolygon(3, arr);	
}

bool wxCollapseBoxBase::Exec()
{
    wxEXTMF_LOG(wxT("wxCollapseBoxBase::Exec() - sending a wxEVT_COMMAND_COLLAPSEBOX_CLICKED"));

    // this should be typically used to roll/unroll the window...
    wxCommandEvent ev(wxEVT_COMMAND_COLLAPSEBOX_CLICKED, GetParent()->GetId());
    GetParent()->AddPendingEvent(ev);

    return TRUE;
}

bool wxCollapseBoxBase::isParentRolled() const
{
    // just use the parent function
    return ((wxExtMiniFrameBase *)m_pParent)->IsRolled();
}


#endif		// #ifdef wxEXTMINIFRM_USE_MINIBTN



