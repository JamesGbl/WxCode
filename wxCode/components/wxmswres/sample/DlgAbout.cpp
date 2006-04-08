/////////////////////////////////////////////////////////////////////////////
// Name:        DlgAbout.cpp
// Purpose:     
// Author:      Ernesto Rangel Dallet
// Modified by: 
// Created:     12/04/05 20:16:05
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "DlgAbout.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "DlgAbout.h"
#include "MyPicsApp.h"

IMPLEMENT_DYNAMIC_CLASS( DlgAbout, wxDialog )

BEGIN_EVENT_TABLE( DlgAbout, wxDialog )
END_EVENT_TABLE()

DlgAbout::DlgAbout( )
{
}

DlgAbout::DlgAbout( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

bool DlgAbout::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
    return true;
}

void DlgAbout::CreateControls()
{    
    DlgAbout* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 1, wxGROW|wxALL, 5);

    // again, load the image from our extracted resources, this time
    // using the convenience short form of GetResourcePath(), GetRcP()
    wxBitmap itemStaticBitmap4Bitmap(
        wxGetApp().m_Resources.GetRcP(wxT("logo.jpg")), wxBITMAP_TYPE_JPEG);
    wxStaticBitmap* itemStaticBitmap4 = new wxStaticBitmap( itemDialog1, wxID_STATIC, itemStaticBitmap4Bitmap, wxDefaultPosition, wxSize(123, 150), 0 );
    itemBoxSizer3->Add(itemStaticBitmap4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("MyPics 1.0\n\nSample application for the wxMSWResources class.\n\nErnesto Rangel Dallet\nmidnight_software@yahoo.com.mx\n\nThis app was Created in about 20 minutes with the \ncoolest piece of software known to man:\nAnthemion DialogBlocks"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText5, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer6, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
}

bool DlgAbout::ShowToolTips()
{
    return true;
}

