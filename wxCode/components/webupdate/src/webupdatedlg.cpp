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
#ifndef WX_PRECOMP
#endif

#include "wx/webupdatedlg.h"


IMPLEMENT_CLASS(wxWebUpdateDlg, wxDialog)
BEGIN_EVENT_TABLE(wxWebUpdateDlg, wxDialog)
    /*EVT_MENU(XRCID("exit_tool_or_menuitem"),  MyFrame::OnExitToolOrMenuCommand)
    EVT_MENU(XRCID("non_derived_dialog_tool_or_menuitem"), MyFrame::OnNonDerivedDialogToolOrMenuCommand)
    EVT_MENU(XRCID("derived_tool_or_menuitem"), MyFrame::OnDerivedDialogToolOrMenuCommand)
    EVT_MENU(XRCID("controls_tool_or_menuitem"), MyFrame::OnControlsToolOrMenuCommand)
    EVT_MENU(XRCID("uncentered_tool_or_menuitem"), MyFrame::OnUncenteredToolOrMenuCommand)
    EVT_MENU(XRCID("custom_class_tool_or_menuitem"), MyFrame::OnCustomClassToolOrMenuCommand)
    EVT_MENU(XRCID("platform_property_tool_or_menuitem"), MyFrame::OnPlatformPropertyToolOrMenuCommand)
    EVT_MENU(XRCID("art_provider_tool_or_menuitem"), MyFrame::OnArtProviderToolOrMenuCommand)
    EVT_MENU(XRCID("variable_expansion_tool_or_menuitem"), MyFrame::OnVariableExpansionToolOrMenuCommand)
    EVT_MENU(XRCID("about_tool_or_menuitem"), MyFrame::OnAboutToolOrMenuCommand)*/
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
	IDWUD_IMAGE = XRCCTRL(*this,"IDWUD_IMAGE",wxStaticBitmap);
	IDWUD_INTRO_TEXT = XRCCTRL(*this,"IDWUD_INTRO_TEXT",wxStaticText);
	IDWUD_LOCAL_VERSION = XRCCTRL(*this,"IDWUD_LOCAL_VERSION",wxStaticText);
	IDWUD_WEB_VERSION = XRCCTRL(*this,"IDWUD_WEB_VERSION",wxStaticText);
	IDWUD_PROGRESS_TEXT = XRCCTRL(*this,"IDWUD_PROGRESS_TEXT",wxStaticText);
	IDWUD_GAUGE = XRCCTRL(*this,"IDWUD_GAUGE",wxGauge);
	IDWUD_TEXT1 = XRCCTRL(*this,"IDWUD_TEXT1",wxStaticText);
	IDWUD_DOWNLOAD_PATH = XRCCTRL(*this,"IDWUD_DOWNLOAD_PATH",wxTextCtrl);
	IDWUD_BROWSE = XRCCTRL(*this,"IDWUD_BROWSE",wxButton);
	IDWUD_CANCEL = XRCCTRL(*this,"IDWUD_CANCEL",wxButton);
	IDWUD_OK = XRCCTRL(*this,"IDWUD_OK",wxButton);
}

void wxWebUpdateDlg::OnDownload(wxCommandEvent &)
{
}


