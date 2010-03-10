/*
 *	wxFTPRmdirDialog.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Fri Jul 23 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxFTPRmdirDialog.cpp: implementation of the wxFTPRmdirDialog class.
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

#include "wxFTPRmdirDialog.h"

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
IMPLEMENT_CLASS(wxFTPRmdirDialog, wxDialog)
BEGIN_EVENT_TABLE(wxFTPRmdirDialog, wxDialog)
	EVT_BUTTON(XRCID("rmdir_button"), wxFTPRmdirDialog::OnRmdir)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxFTPRmdirDialog::wxFTPRmdirDialog(wxWindow* pParent)
{
	wxXmlResource::Get()->LoadDialog(this, pParent, wxS("rmdir_ftp_dialog"));

	SetSize(400, -1);

	m_pRmdirCtrl	= XRCCTRL(*this, "rmdir_text_ctrl", wxTextCtrl);
	m_pUserCtrl		= XRCCTRL(*this, "user_text_ctrl", wxTextCtrl);
	m_pPassCtrl		= XRCCTRL(*this, "pass_text_ctrl", wxTextCtrl);

	m_pResponseCtrl	= XRCCTRL(*this, "response_text_ctrl", wxTextCtrl);

	if(m_pRmdirCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		m_szDefaultRmdir	= m_pRmdirCtrl->GetValue();
		m_szDefaultUser		= m_pUserCtrl->GetValue();
		m_szDefaultPass		= m_pPassCtrl->GetValue();
	}
}

wxFTPRmdirDialog::~wxFTPRmdirDialog()
{

}

//////////////////////////////////////////////////////////////////////
// Event Handlers
//////////////////////////////////////////////////////////////////////

void wxFTPRmdirDialog::OnRmdir(wxCommandEvent& WXUNUSED(event))
{
	if(m_pRmdirCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		wxString szRmdir	= m_pRmdirCtrl->GetValue();
		wxString szUser		= m_pUserCtrl->GetValue();
		wxString szPass		= m_pPassCtrl->GetValue();

		wxString szResponse;

		if((szRmdir == m_szDefaultRmdir))
		{
			wxMessageBox(wxS("Please change the RMDIR location."), wxS("Error..."), wxICON_INFORMATION|wxOK, this);
		}
		else if((szUser == m_szDefaultUser) && (szPass == m_szDefaultPass))
		{
			wxMessageBox(wxS("Please change the username or password."), wxS("Error..."), wxICON_INFORMATION|wxOK, this);
		}
		else
		{
			// Do it!
			wxCurlFTP ftp(szRmdir, szUser, szPass);

			if(ftp.RmDir())
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

