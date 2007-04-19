/*
 *	wxCURLApp.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Mon May 24 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

//////////////////////////////////////////////////////////////////////
// Headers
//////////////////////////////////////////////////////////////////////
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif
#include <wx/xrc/xmlres.h>

#include <wx/curl/dav.h>

#include "wxCURLApp.h"
#include "wxCURLDlg.h"
#include "resource.h"

// Create a new application object
IMPLEMENT_APP(wxCURLApp)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxCURLApp::wxCURLApp()
{
}

wxCURLApp::~wxCURLApp()
{
	// Shutdown CurlLib
	wxCurlDAV::Shutdown();
}

//////////////////////////////////////////////////////////////////////
// Overridden Virtual Methods
//////////////////////////////////////////////////////////////////////

bool wxCURLApp::OnInit()
{
	wxXmlResource::Get()->InitAllHandlers();
	wxXmlResource::Get()->Load(wxT("resource/main_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/get_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/get_file_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/put_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/put_text_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/mkcol_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/delete_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/propfind_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/getfs_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/options_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/head_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/trace_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/post_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/get_ftp_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/get_file_ftp_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/put_ftp_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/put_text_ftp_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/mkdir_ftp_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/rmdir_ftp_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/delete_ftp_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/rename_ftp_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/info_ftp_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/list_ftp_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/nlst_ftp_dialog.xrc"));
	wxXmlResource::Get()->Load(wxT("resource/getfs_ftp_dialog.xrc"));

	// create the main application window
	wxCURLDialog *dialog = new wxCURLDialog(_("wxCURL"),
								wxPoint(50, 50), wxSize(400, 300));

	// and show it (the frames, unlike simple controls, are not shown when
	// created initially)
	dialog->Show(TRUE);
	SetTopWindow(dialog);

	// Initialize LibCURL
	wxCurlDAV::Init();

	// success: wxApp::OnRun() will be called which will enter the main message
	// loop and the application will run. If we returned FALSE here, the
	// application would exit immediately.
	return TRUE;
}

