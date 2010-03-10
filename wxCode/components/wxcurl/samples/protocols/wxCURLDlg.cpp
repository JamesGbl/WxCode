/*
 *	wxCURLDlg.cpp
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

#ifdef __WXMSW__
    #include <wx/msw/msvcrt.h>      // useful to catch memory leaks when compiling under MSVC 
#endif

#include <wx/xrc/xmlres.h>

#include "wxCURLApp.h"
#include "wxCURLDlg.h"
#include "wxGetDialog.h"
#include "wxGetFileDialog.h"
#include "wxPutDialog.h"
#include "wxPutTextDialog.h"
#include "wxMkcolDialog.h"
#include "wxDeleteDialog.h"
#include "wxPropfindDialog.h"
#include "wxGetFSDialog.h"
#include "wxOptionsDialog.h"
#include "wxHeadDialog.h"
#include "wxTraceDialog.h"
#include "wxPostDialog.h"
#include "wxGetFTPDialog.h"
#include "wxGetFTPFileDialog.h"
#include "wxPutFTPDialog.h"
#include "wxPutFTPTextDialog.h"
#include "wxFTPDeleteDialog.h"
#include "wxFTPMkdirDialog.h"
#include "wxFTPRenameDialog.h"
#include "wxFTPRmdirDialog.h"
#include "wxInfoFTPDialog.h"
#include "wxListFTPDialog.h"
#include "wxNlstFTPDialog.h"
#include "wxGetFSFTPDialog.h"

//////////////////////////////////////////////////////////////////////
// Resources
//////////////////////////////////////////////////////////////////////
// the application icon
#if defined(__WXGTK__) || defined(__WXMOTIF__)
	#include "wxCURL.xpm"
#endif

//////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Event Tables and Other Macros for wxWindows
//////////////////////////////////////////////////////////////////////

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
IMPLEMENT_CLASS(wxCURLDialog, wxDialog)
BEGIN_EVENT_TABLE(wxCURLDialog, wxDialog)
	EVT_BUTTON(XRCID("file_menu_exit"), wxCURLDialog::OnQuit)
	EVT_BUTTON(XRCID("help_menu_about"), wxCURLDialog::OnAbout)
	EVT_BUTTON(XRCID("file_menu_get"), wxCURLDialog::OnGet)
	EVT_BUTTON(XRCID("file_menu_getf"), wxCURLDialog::OnGetFile)
	EVT_BUTTON(XRCID("file_menu_put"), wxCURLDialog::OnPut)
	EVT_BUTTON(XRCID("file_menu_putd"), wxCURLDialog::OnPutData)
	EVT_BUTTON(XRCID("file_menu_mkcol"), wxCURLDialog::OnMkcol)
	EVT_BUTTON(XRCID("file_menu_delete"), wxCURLDialog::OnDelete)
	EVT_BUTTON(XRCID("file_menu_propfind"), wxCURLDialog::OnPropfind)
	EVT_BUTTON(XRCID("file_menu_getfs"), wxCURLDialog::OnGetFS)
	EVT_BUTTON(XRCID("file_menu_options"), wxCURLDialog::OnOptions)
	EVT_BUTTON(XRCID("file_menu_head"), wxCURLDialog::OnHead)
	EVT_BUTTON(XRCID("file_menu_trace"), wxCURLDialog::OnTrace)
	EVT_BUTTON(XRCID("file_menu_post"), wxCURLDialog::OnPost)
	EVT_BUTTON(XRCID("file_menu_getftp"), wxCURLDialog::OnGetFtp)
	EVT_BUTTON(XRCID("file_menu_getftpf"), wxCURLDialog::OnGetFileFtp)
	EVT_BUTTON(XRCID("file_menu_putftp"), wxCURLDialog::OnPutFtp)
	EVT_BUTTON(XRCID("file_menu_putftpd"), wxCURLDialog::OnPutDataFtp)
	EVT_BUTTON(XRCID("file_menu_mkdirftp"), wxCURLDialog::OnMkdirFtp)
	EVT_BUTTON(XRCID("file_menu_rmdirftp"), wxCURLDialog::OnRmdirFtp)
	EVT_BUTTON(XRCID("file_menu_deleteftp"), wxCURLDialog::OnDeleteFtp)
	EVT_BUTTON(XRCID("file_menu_renameftp"), wxCURLDialog::OnRenameFtp)
	EVT_BUTTON(XRCID("file_menu_listftp"), wxCURLDialog::OnListFtp)
	EVT_BUTTON(XRCID("file_menu_nlstftp"), wxCURLDialog::OnNlstFtp)
	EVT_BUTTON(XRCID("file_menu_infoftp"), wxCURLDialog::OnInfoFtp)
	EVT_BUTTON(XRCID("file_menu_getfsftp"), wxCURLDialog::OnGetFSFtp)
	EVT_CLOSE(wxCURLDialog::OnClose)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxCURLDialog::wxCURLDialog(const wxString& title, const wxPoint& WXUNUSED(pos),
                           const wxSize& WXUNUSED(size))
{
	wxASSERT(wxXmlResource::Get()->LoadDialog(this, NULL, wxS("main_dialog")));

	SetTitle(title);

	// set the Dialog icon
#if !defined(__WXMAC__)
	SetIcon(wxICON(wxCURL));
#endif
}

wxCURLDialog::~wxCURLDialog()
{
}

//////////////////////////////////////////////////////////////////////
// Event Handlers
//////////////////////////////////////////////////////////////////////

void wxCURLDialog::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	this->Destroy();
}

void wxCURLDialog::OnClose(wxCloseEvent& WXUNUSED(event))
{
	this->Destroy();
}

void wxCURLDialog::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(wxS("This application is a proof-of-concept application\n")
				 wxS("for using LibCURL to issue a sub-set of WebDAV\n")
				 wxS("commands to an HTTP server.\n\n")
				 wxS("Casey O'Donnell, caseyodonnell@hotmail.com"),
				 wxS("About wxCURL..."), wxICON_INFORMATION|wxOK, this);
}

void wxCURLDialog::OnGet(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxGetDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnGetFile(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxGetFileDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnPut(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxPutDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnPutData(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxPutTextDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnMkcol(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxMkcolDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnDelete(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxDeleteDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnPropfind(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxPropfindDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnGetFS(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxGetFSDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnOptions(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxOptionsDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnHead(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxHeadDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnTrace(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxTraceDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnPost(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxPostDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnGetFtp(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxGetFTPDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnGetFileFtp(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxGetFTPFileDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnPutFtp(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxPutFTPDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnPutDataFtp(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxPutFTPTextDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnMkdirFtp(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxFTPMkdirDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnRmdirFtp(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxFTPRmdirDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnDeleteFtp(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxFTPDeleteDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnRenameFtp(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxFTPRenameDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnListFtp(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxListFTPDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnNlstFtp(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxNlstFTPDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnInfoFtp(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxInfoFTPDialog(this);

	pDlg->ShowModal();
}

void wxCURLDialog::OnGetFSFtp(wxCommandEvent& WXUNUSED(event))
{
	wxDialog* pDlg = new wxGetFSFTPDialog(this);

	pDlg->ShowModal();
}





