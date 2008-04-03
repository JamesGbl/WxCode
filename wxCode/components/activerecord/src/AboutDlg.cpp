/////////////////////////////////////////////////////////////////////////////
// Name:        AboutDlg.cpp
// Purpose:     
// Author:      Matías Szeftel
// Modified by: 
// Created:     Fri 09 Feb 2007 00:58:18 ART
// RCS-ID:      
// Copyright:   (c) 2006 Matías Szeftel <mszeftel@yahoo.com.ar>
// Licence:     GPL (GNU Public License)
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), Fri 09 Feb 2007 00:58:18 ART

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "AboutDlg.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "../include/AboutDlg.h"

////@begin XPM images
////@end XPM images

/*!
 * AboutDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( AboutDlg, wxDialog )

/*!
 * AboutDlg event table definition
 */

BEGIN_EVENT_TABLE( AboutDlg, wxDialog )

////@begin AboutDlg event table entries
////@end AboutDlg event table entries

END_EVENT_TABLE()

/*!
 * AboutDlg constructors
 */

AboutDlg::AboutDlg( )
{
    Init();
}

AboutDlg::AboutDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

/*!
 * AboutDlg creator
 */

bool AboutDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin AboutDlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end AboutDlg creation
    return true;
}

/*!
 * Member initialisation 
 */

void AboutDlg::Init()
{
////@begin AboutDlg member initialisation
    st_name = NULL;
////@end AboutDlg member initialisation
}
/*!
 * Control creation for AboutDlg
 */

void AboutDlg::CreateControls()
{    
////@begin AboutDlg content construction
    // Generated by DialogBlocks, Fri 09 Feb 2007 01:15:35 ART (unregistered)

    AboutDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBitmap itemStaticBitmap4Bitmap(itemDialog1->GetBitmapResource(wxT("img/wxARG128.png")));
    wxStaticBitmap* itemStaticBitmap4 = new wxStaticBitmap( itemDialog1, wxID_STATIC, itemStaticBitmap4Bitmap, wxDefaultPosition, wxSize(128, 128), 0 );
    itemBoxSizer3->Add(itemStaticBitmap4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer5, 0, wxGROW, 5);

    st_name = new wxStaticText( itemDialog1, wxID_STATIC, _("wxActiveRecordGenerator (aka wxARG)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(st_name, 0, wxALIGN_LEFT|wxALL|wxADJUST_MINSIZE, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("Developed by Mat\u00edas Szeftel - mszeftel@yahoo.com.ar\nCheck for updates at http://wxcode.sourceforge.net "), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemStaticText7, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    itemBoxSizer5->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBitmap itemStaticBitmap9Bitmap(itemDialog1->GetBitmapResource(wxT("img/builtwithwx.gif")));
    wxStaticBitmap* itemStaticBitmap9 = new wxStaticBitmap( itemDialog1, wxID_STATIC, itemStaticBitmap9Bitmap, wxDefaultPosition, wxSize(116, 49), 0 );
    itemBoxSizer5->Add(itemStaticBitmap9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemDialog1, wxID_STATIC, _("Special thanks to:\nJoseph Blough (author of DatabaseLayer - find it also in wxCode)\nThe wxWidgets Discussion Forum\nAnd the wxWidgets community in general"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemStaticText11, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

////@end AboutDlg content construction

	
}

/*!
 * Should we show tooltips?
 */

bool AboutDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap AboutDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin AboutDlg bitmap retrieval
    wxUnusedVar(name);
    if (name == _T("img/wxARG128.png"))
    {
        wxBitmap bitmap(_T("img/wxARG128.png"), wxBITMAP_TYPE_PNG);
        return bitmap;
    }
    else if (name == _T("img/builtwithwx.gif"))
    {
        wxBitmap bitmap(_T("img/builtwithwx.gif"), wxBITMAP_TYPE_GIF);
        return bitmap;
    }
    return wxNullBitmap;
////@end AboutDlg bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon AboutDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin AboutDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end AboutDlg icon retrieval
}
