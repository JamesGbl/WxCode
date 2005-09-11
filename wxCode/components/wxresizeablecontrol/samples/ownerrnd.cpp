/////////////////////////////////////////////////////////////////////////////
// Name:        ownerrnd.cpp - Owner rendered resizeable control
// Purpose:     wxResizeableControl
// Author:      Markus Greither
// Modified by:
// Created:     11/11/02
// RCS-ID:      $Id: ownerrnd.cpp,v 1.2 2005-09-11 15:25:31 frm Exp $
// Copyright:   (c) Markus Greither
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// GCC implementation
//-----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "ownerrnd.h"
#endif

//-----------------------------------------------------------------------------
// Standard wxWindows headers
//-----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// For all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//-----------------------------------------------------------------------------
// Header of this .cpp file
//-----------------------------------------------------------------------------

#include "wx/image.h"
#include "wx/resizec.h"
#include "ownerrnd.h"

//-----------------------------------------------------------------------------
// Remaining headers: Needed wx headers, then wx/contrib headers, then application headers
//-----------------------------------------------------------------------------

// Since setting an icon
#include "wx/image.h"

#if wxUSE_CLIPBOARD
#include "wx/clipbrd.h"
#endif

IMPLEMENT_DYNAMIC_CLASS(OwnerRenderedControl,wxResizeableControl)

BEGIN_EVENT_TABLE(OwnerRenderedControl,wxResizeableControl)
    EVT_PAINT(OwnerRenderedControl::OnPaint)
END_EVENT_TABLE()

void OwnerRenderedControl::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);
    wxSize size = GetClientSize();
    Paint(dc,false,size);
    if (m_hasfocus)
        DrawSizeRect(dc);
}

void OwnerRenderedControl::Paint(wxDC &dc,bool printing,wxSize &WXUNUSED(size))
{
    dc.SetPen(*wxRED_PEN);
    dc.SetBrush(wxNullBrush);
    dc.DrawRectangle(10,10,200,100);
    dc.DrawText(_("Owner Drawn control!"),10,10);
    if (printing)
        dc.DrawText(_("Yes we can also be printed!"),10,30);
    else
    {
        dc.DrawText(_("Drag me around and/or"),10,35);
        dc.DrawText(_("resize me and watch"),10,50);
        dc.DrawText(_("the scrollbar updating!"),10,65);
    }    
    dc.SetPen(wxNullPen);
}



