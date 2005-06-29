/////////////////////////////////////////////////////////////////////////////
// Name:        webupdatedlg.cpp
// Purpose:     wxWebUpdateDlg
// Author:      Francesco Montorsi
// Created:     2005/06/23
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
	#pragma implementation "webupdatedlg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#include "wx/webupdatedlg.h"


IMPLEMENT_CLASS(wxWebUpdateLocalPackage, wxObject)
IMPLEMENT_CLASS(wxWebUpdateDlg, wxDialog)
BEGIN_EVENT_TABLE(wxWebUpdateDlg, wxDialog)

	// buttons
    EVT_BUTTON(XRCID("IDWUD_DOWNLOAD"), wxWebUpdateDlg::OnDownload)
    EVT_BUTTON(XRCID("IDWUD_BROWSE"), wxWebUpdateDlg::OnBrowse)
    EVT_BUTTON(XRCID("IDWUD_CANCEL"), wxWebUpdateDlg::OnCancel)

	// listctrl


END_EVENT_TABLE()



// ---------------------
// wxWEBUPDATEDLG
// ---------------------

void wxWebUpdateDlg::InitWidgetsFromXRC()
{
	// we need some handlers before loading resources
	wxImage::AddHandler(new wxPNGHandler);
	wxXmlResource::Get()->InitAllHandlers();
	
    // load our XRC file
    wxXmlResource::Get()->Load(wxT("../src/webupdatedlg.xrc"));

	// and build our dialog window
	wxXmlResource::Get()->LoadDialog(this, GetParent(), wxT("wxWebUpdateDlg"));	

	// init control pointers	
	m_pAppNameText = XRCCTRL(*this,"IDWUD_APPNAME_TEXT",wxStaticText);
	m_pUpdatesList = XRCCTRL(*this,"IDWUD_LISTCTRL",wxListCtrl);
	m_pGauge = XRCCTRL(*this,"IDWUD_GAUGE",wxGauge);
	m_pDownloadPathTextCtrl = XRCCTRL(*this,"IDWUD_DOWNLOAD_PATH",wxTextCtrl);

	// init control data
	m_pAppNameText->SetLabel(m_strAppName);

	// we have changed the appname statictext control so maybe we need
	// to expand our dialog... force a layout recalculation
	GetSizer()->RecalcSizes();
	GetSizer()->Fit(this);
}


// event handlers

void wxWebUpdateDlg::OnDownload(wxCommandEvent &)
{
}

void wxWebUpdateDlg::OnBrowse(wxCommandEvent &)
{
}

void wxWebUpdateDlg::OnCancel(wxCommandEvent &)
{
	EndModal(wxCANCEL);
}


