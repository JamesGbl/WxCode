/////////////////////////////////////////////////////////////////////////////
// Name:        MyFrame.cpp
// Purpose:     
// Author:      Ernesto Rangel Dallet
// Modified by: 
// Created:     12/04/05 19:31:56
// RCS-ID:      
// Copyright:   
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation "MyFrame.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "MyFrame.h"
#include "MyPicsApp.h"
#include "DlgAbout.h"


IMPLEMENT_CLASS( MyFrame, wxFrame )

BEGIN_EVENT_TABLE( MyFrame, wxFrame )
    EVT_MENU( ID_QUIT, MyFrame::OnQuitClick )
    EVT_MENU( ID_ABOUT, MyFrame::OnAboutClick )
    EVT_COMBOBOX( ID_CBO_PICS, MyFrame::OnCboPicsSelected )
END_EVENT_TABLE()

MyFrame::MyFrame( )
{
}

MyFrame::MyFrame( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create( parent, id, caption, pos, size, style );
}

bool MyFrame::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    m_pCboPicList = NULL;
    m_pLblDescription = NULL;
    m_pBmpThePic = NULL;
    m_pTxtHFile = NULL;
    m_pTxtCppFile = NULL;

    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();

    // load the .h file extracted from resources and show it
    wxString hFile = wxGetApp().m_Resources.GetResourcePath(wxT("wxmswres.h"));
    if(!hFile.IsEmpty())
        m_pTxtHFile->LoadFile(hFile);

    // use "Raw" data access to resources to load the .cpp file, and show it
    // weather or not we need to delete the HGLOBAL handle, is still a matter
    // of controversy to me :) but it wont hurt doing it
    HGLOBAL hGlobal = NULL;
    DWORD dtaSize = 0;
    void* pData = wxGetApp().m_Resources.GetResData(wxT("wxmswres.cpp"),
        wxT("txtfile"), hGlobal, dtaSize);
    if(pData && dtaSize>0)
    {
        wxString theText((wxChar*)pData, dtaSize);
        m_pTxtCppFile->SetValue(theText);
        DeleteObject((HGDIOBJ)hGlobal);
    }

    // set the titlebar icon, this time just let wxWidgets handle it
    // (we dont extract it)
    SetIcon(wxICON(appicon));

    Centre();
    return true;
}

void MyFrame::CreateControls()
{    
    MyFrame* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(ID_QUIT, _("&Exit"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("&File"));
    wxMenu* itemMenu5 = new wxMenu;
    itemMenu5->Append(ID_ABOUT, _("About this POS App..."), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu5, _("&Help"));
    itemFrame1->SetMenuBar(menuBar);

    wxNotebook* itemNotebook7 = new wxNotebook( itemFrame1, ID_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxNB_TOP );

    wxPanel* itemPanel8 = new wxPanel( itemNotebook7, ID_PNL_PICS, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxVERTICAL);
    itemPanel8->SetSizer(itemBoxSizer9);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer9->Add(itemBoxSizer10, 0, wxGROW, 5);
    wxString m_pCboPicListStrings[] = {
        _("tree"),
        _("wolf"),
        _("mountain"),
        _("me")
    };
    m_pCboPicList = new wxComboBox( itemPanel8, ID_CBO_PICS, _T(""), wxDefaultPosition, wxSize(100, -1), 4, m_pCboPicListStrings, wxCB_READONLY );
    itemBoxSizer10->Add(m_pCboPicList, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText12 = new wxStaticText( itemPanel8, wxID_STATIC, _("Description:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemStaticText12, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    m_pLblDescription = new wxStaticText( itemPanel8, wxID_STATIC, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(m_pLblDescription, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxBitmap m_pBmpThePicBitmap(wxNullBitmap);
    m_pBmpThePic = new wxStaticBitmap( itemPanel8, wxID_STATIC, m_pBmpThePicBitmap, wxDefaultPosition, wxSize(640, 480), 0 );
    itemBoxSizer9->Add(m_pBmpThePic, 1, wxALIGN_CENTER_HORIZONTAL, 5);

    itemNotebook7->AddPage(itemPanel8, _("Pictures"));

    wxPanel* itemPanel15 = new wxPanel( itemNotebook7, ID_PNL_SRCH, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxVERTICAL);
    itemPanel15->SetSizer(itemBoxSizer16);

    m_pTxtHFile = new wxTextCtrl( itemPanel15, ID_TEXTCTRL, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxHSCROLL );
    itemBoxSizer16->Add(m_pTxtHFile, 1, wxGROW|wxALL, 5);

    itemNotebook7->AddPage(itemPanel15, _("wxmswres.h"));

    wxPanel* itemPanel18 = new wxPanel( itemNotebook7, ID_PNL_SRCCPP, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxVERTICAL);
    itemPanel18->SetSizer(itemBoxSizer19);

    m_pTxtCppFile = new wxTextCtrl( itemPanel18, ID_TEXTCTRL1, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxHSCROLL );
    itemBoxSizer19->Add(m_pTxtCppFile, 1, wxGROW|wxALL, 5);

    itemNotebook7->AddPage(itemPanel18, _("wxmswres.cpp"));
}

bool MyFrame::ShowToolTips()
{
    return true;
}

void MyFrame::OnCboPicsSelected( wxCommandEvent& event )
{
    wxString picName = m_pCboPicList->GetStringSelection();
    if(!picName.IsEmpty())
        ShowPic(picName);
}


void MyFrame::OnQuitClick( wxCommandEvent& event )
{
    Close();
}

void MyFrame::OnAboutClick( wxCommandEvent& event )
{
    DlgAbout dlg(this);
    dlg.ShowModal();
}

void MyFrame::ShowPic(const wxString& PicName)
{
    // make up a description for the pic;
    wxString desc;
    wxString thePicName;
    if(PicName==_("tree"))
    {
        thePicName = wxT("tree.jpg");
        desc = _("Some nice Tree in the open");
    }

    else if(PicName==_("wolf"))
    {
        desc = _("a wolf thinking about some yummy sheep");
        thePicName = wxT("wolf.jpg");
    }
    else if(PicName==_("mountain"))
    {
        desc = _("some mountain i climed (LOL yea right!)");
        thePicName = wxT("mountain.jpg");
    }
    else if(PicName==_("me"))
    {
        desc = _("the guy who did this P.O.S. app at the gym");
        thePicName = wxT("me.jpg");
    }
    else
        wxFAIL_MSG(wxT("this shouldnt happen!!!"));

    // use this function to get the full path to the desired resource file
    // in this case to the image to load it will return empty if not found
     wxString fullName = 
        wxGetApp().m_Resources.GetResourcePath(thePicName);

     if(fullName.IsEmpty())
     {
         wxLogDebug(wxT("Image %s not found!"), fullName.c_str());
         return;
     }

    // load the image file, from our previously extracted resources
    // and yes, we could just load the resource directly using
    // wxBITMAP_TYPE_JPEG_RESOURCE, this is just for demonstration.
    // remember that the main goal of wxMSWResources is NOT to manage
    // images, but rather other kind of resources like .mo catalogs, etc.
    wxBitmap thePic(fullName, wxBITMAP_TYPE_JPEG);
    if(thePic.Ok())
    {
        m_pLblDescription->SetLabel(desc);
        m_pBmpThePic->SetBitmap(thePic);
        Refresh();
    }
    else
    {
        wxMessageBox(_("Failed to load the Picutre!"), _("Error"),
        wxOK | wxCENTRE | wxICON_ERROR, this);
    }
}

