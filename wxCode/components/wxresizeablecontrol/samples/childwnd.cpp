/////////////////////////////////////////////////////////////////////////////
// Name:        childwnd.cpp - shows child windows in a wxResizeableControl
// Purpose:     wxResizeableControl
// Author:      Markus Greither
// Modified by:
// Created:     11/11/02
// RCS-ID:      $Id: childwnd.cpp,v 1.3 2005-09-11 18:05:25 magr Exp $
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

IMPLEMENT_DYNAMIC_CLASS(ParentControl,wxResizeableParentControl)

BEGIN_EVENT_TABLE(ParentControl,wxResizeableParentControl)
    EVT_PAINT(ParentControl::OnPaint)
END_EVENT_TABLE()

void ParentControl::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);
    wxSize size = GetClientSize();
    Paint(dc,false,size);
    if (m_hasfocus)
        DrawSizeRect(dc);
}

void ParentControl::Paint(wxDC &dc,bool printing,wxSize &WXUNUSED(size))
{
    // FIXME: Text output for printer
}



