/*
 *	wxFTPMkdirDialog.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Fri Jul 23 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxFTPMkdirDialog.cpp: implementation of the wxFTPMkdirDialog class.
//
//////////////////////////////////////////////////////////////////////

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

#include <wx/curl/ftp.h>

#include "wxFTPMkdirDialog.h"

//////////////////////////////////////////////////////////////////////
// Resources
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Event Tables and Other Macros for wxWindows
//////////////////////////////////////////////////////////////////////

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
IMPLEMENT_CLASS(wxFTPMkdirDialog, wxDialog)
BEGIN_EVENT_TABLE(wxFTPMkdirDialog, wxDialog)
	EVT_BUTTON(XRCID("mkdir_button"), wxFTPMkdirDialog::OnMkdir)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxFTPMkdirDialog::wxFTPMkdirDialog(wxWindow* pParent)
{
	wxXmlResource::Get()->LoadDialog(this, pParent, wxS("mkdir_ftp_dialog"));

	SetSize(400, -1);

	m_pMkdirCtrl	= XRCCTRL(*this, "mkdir_text_ctrl", wxTextCtrl);
	m_pUserCtrl		= XRCCTRL(*this, "user_text_ctrl", wxTextCtrl);
	m_pPassCtrl		= XRCCTRL(*this, "pass_text_ctrl", wxTextCtrl);

	m_pResponseCtrl	= XRCCTRL(*this, "response_text_ctrl", wxTextCtrl);

	if(m_pMkdirCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		m_szDefaultMkdir	= m_pMkdirCtrl->GetValue();
		m_szDefaultUser		= m_pUserCtrl->GetValue();
		m_szDefaultPass		= m_pPassCtrl->GetValue();
	}
}

wxFTPMkdirDialog::~wxFTPMkdirDialog()
{

}

//////////////////////////////////////////////////////////////////////
// Event Handlers
//////////////////////////////////////////////////////////////////////

void wxFTPMkdirDialog::OnMkdir(wxCommandEvent& WXUNUSED(event))
{
	if(m_pMkdirCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		wxString szMkdir	= m_pMkdirCtrl->GetValue();
		wxString szUser		= m_pUserCtrl->GetValue();
		wxString szPass		= m_pPassCtrl->GetValue();

		wxString szResponse;

		if((szMkdir == m_szDefaultMkdir))
		{
			wxMessageBox(wxS("Please change the MKDIR location."), wxS("Error..."), wxICON_INFORMATION|wxOK, this);
		}
		else if((szUser == m_szDefaultUser) && (szPass == m_szDefaultPass))
		{
			wxMessageBox(wxS("Please change the username or password."), wxS("Error..."), wxICON_INFORMATION|wxOK, this);
		}
		else
		{
			// Do it!
			wxCurlFTP ftp(szMkdir, szUser, szPass);

			if(ftp.MkDir())
			{
					szResponse = wxS("SUCCESS!\n\n");
					szResponse += wxString::Format(wxS("\nResponse Code: %d\n\n"), ftp.GetResponseCode());
					szResponse += ftp.GetResponseHeader();
					szResponse += wxS("\n\n");
					szResponse += ftp.GetResponseBody();

					if(m_pResponseCtrl)
						m_pResponseCtrl->SetValue(szResponse);
			}
			else
			{
					szResponse = wxS("FAILURE!\n\n");
					szResponse += wxString::Format(wxS("\nResponse Code: %d\n\n"), ftp.GetResponseCode());
					szResponse += ftp.GetResponseHeader();
					szResponse += wxS("\n\n");
					szResponse += ftp.GetResponseBody();
					szResponse += wxS("\n\n");
					szResponse += ftp.GetErrorString();

					if(m_pResponseCtrl)
						m_pResponseCtrl->SetValue(szResponse);
			}
		}
	}
}

