/////////////////////////////////////////////////////////////////////////////
// Name:        msw/palettefrm.cpp
// Purpose:     wxExtMiniFrame, wxExtMiniFrame, wxMiniButton
//              wxCloseBox, wxMaximizeBox, wxMinimizeBox, wxCollapseBox
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
#endif


// includes
#include <wx/wx.h>
#include <wx/settings.h>
#include <wx/tokenzr.h>
#include <wx/defs.h>
#include <wx/object.h>
#include <wx/frame.h>
#include <wx/dcscreen.h>

#include "wx/minibtn.h"


#ifdef wxEXTMINIFRM_USE_MINIBTN


// RTTI class declarations
IMPLEMENT_DYNAMIC_CLASS(wxCloseBox, wxMiniButton)
IMPLEMENT_DYNAMIC_CLASS(wxMaximizeBox, wxMiniButton)
IMPLEMENT_DYNAMIC_CLASS(wxMinimizeBox, wxMiniButton)
IMPLEMENT_DYNAMIC_CLASS(wxCollapseBox, wxMiniButton)





// ------------------------------------
// wxCLOSEBOX
// ------------------------------------

void wxCloseBox::Draw(wxDC &dc, int x, int y)
{
	wxEXTMF_LOG(wxT("wxCloseBox::Draw() at %d;%d"), x, y);

	DrawMSWStdButton(dc, x, y, DFCS_CAPTIONCLOSE);
}


// ------------------------------------
// wxMAXIMIZEBOX
// ------------------------------------

void wxMaximizeBox::Draw(wxDC &dc, int x, int y)
{
	wxEXTMF_LOG(wxT("wxMaximizeBox::Draw()"));
	
	// draw the maximize or restore box...
	DrawMSWStdButton(dc, x, y, (GetParent()->IsRolled() ?
					DFCS_CAPTIONRESTORE : DFCS_CAPTIONMAX));
}


// ------------------------------------
// wxMINIMIZEBOX
// ------------------------------------

void wxMinimizeBox::Draw(wxDC &dc, int x, int y)
{
	wxEXTMF_LOG(wxT("wxMinimizeBox::Draw()"));

	DrawMSWStdButton(dc, x, y, DFCS_CAPTIONMIN);
}


#endif		// wxEXTMINIFRM_USE_MINIBTN
