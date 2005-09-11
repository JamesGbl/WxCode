/////////////////////////////////////////////////////////////////////////////
// Name:        childwnd.cpp - shows child windows in a wxResizeableControl
// Purpose:     wxResizeableControl
// Author:      Markus Greither
// Modified by:
// Created:     11/11/02
// RCS-ID:      $Id:     1.05 2003/12/19 magr
// Copyright:   (c) Markus Greither
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// GCC implementation
//-----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "childwnd.h"
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
#include "wx/textctrl.h"
#include "childwnd.h"

//-----------------------------------------------------------------------------
// Remaining headers: Needed wx headers, then wx/contrib headers, then application headers
//-----------------------------------------------------------------------------

// Since setting an icon
#include "wx/image.h"

#if wxUSE_CLIPBOARD
#include "wx/clipbrd.h"
#endif

BEGIN_EVENT_TABLE(CustomTextControl,wxTextCtrl)
    EVT_SET_FOCUS(CustomTextControl::OnSetFocus)
    EVT_KILL_FOCUS(CustomTextControl::OnKillFocus)
END_EVENT_TABLE()

void CustomTextControl::OnKillFocus(wxFocusEvent &event)
{
	wxCommandEvent notification(wxEVT_COMMAND_HIDE_SIZERS);
	if (GetParent())
	    GetParent()->ProcessEvent(notification);
	event.Skip();
}

void CustomTextControl::OnSetFocus(wxFocusEvent &event)
{
	wxCommandEvent notification(wxEVT_COMMAND_SHOW_SIZERS);
	if (GetParent())
	    GetParent()->ProcessEvent(notification);
	event.Skip();
}

IMPLEMENT_DYNAMIC_CLASS(ChildControl,wxResizeableControl)

BEGIN_EVENT_TABLE(ChildControl,wxResizeableControl)
    EVT_PAINT(ChildControl::OnPaint)
    EVT_SIZE(ChildControl::OnSize)
END_EVENT_TABLE()

void ChildControl::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);
    wxSize size = GetClientSize();
    Paint(dc,false,size);
    if (m_hasfocus)
        DrawSizeRect(dc);
}

void ChildControl::OnSize(wxSizeEvent &event)
{
    wxSize size = event.GetSize();
    Editor->SetSize(wxResizeableControl::SizeXRad*2,
                    wxResizeableControl::SizeYRad*2,
                    size.x-2*wxResizeableControl::SizeXRad*2,
                    size.y-2*wxResizeableControl::SizeYRad*2);
    Refresh();
    event.Skip();
}

void ChildControl::Paint(wxDC &dc,bool printing,wxSize &WXUNUSED(size))
{
    // FIXME: Text output for printer
}



