/*
 *	wxGetFTPDialog.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Fri Jul 23 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxGetFTPDialog.cpp: implementation of the wxGetFTPDialog class.
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

#include "wxGetFTPDialog.h"

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
IMPLEMENT_CLASS(wxGetFTPDialog, wxDialog)
BEGIN_EVENT_TABLE(wxGetFTPDialog, wxDialog)
	EVT_BUTTON(XRCID("file_menu_get"), wxGetFTPDialog::OnGetUrl)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxGetFTPDialog::wxGetFTPDialog(wxWindow* pParent)
{
	wxASSERT(wxXmlResource::Get()->LoadDialog(this, pParent, wxS("get_ftp_dialog")));

	SetSize(400,400);

	m_pTextCtrl = XRCCTRL(*this, "text_ctrl", wxTextCtrl);
	m_pHostCtrl = XRCCTRL(*this, "host_text_ctrl", wxTextCtrl);

	m_pUserCtrl = XRCCTRL(*this, "user_text_ctrl", wxTextCtrl);
	m_pPassCtrl = XRCCTRL(*this, "pass_text_ctrl", wxTextCtrl);

	if(m_pUserCtrl && m_pPassCtrl)
	{
		m_szDefaultUser = m_pUserCtrl->GetValue();
		m_szDefaultPass = m_pPassCtrl->GetValue();
	}
}

wxGetFTPDialog::~wxGetFTPDialog()
{

}

//////////////////////////////////////////////////////////////////////
// Event Handlers
//////////////////////////////////////////////////////////////////////

void wxGetFTPDialog::OnGetUrl(wxCommandEvent& WXUNUSED(event))
{
	wxString szHost, szUser, szPass;

	if(m_pHostCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		szHost = m_pHostCtrl->GetValue();
		szUser = m_pUserCtrl->GetValue();
		szPass = m_pPassCtrl->GetValue();

		wxCurlFTP ftp(szHost);

		if(szUser != m_szDefaultUser)
			ftp.SetUsername(szUser);

		if(szPass != m_szDefaultPass)
			ftp.SetPassword(szPass);

		char*	szBuffer = NULL;

		if(ftp.Get(szBuffer) > 0)
		{
			if(m_pTextCtrl && szBuffer)
			{
				m_pTextCtrl->SetValue(wxString(szBuffer, wxConvUTF8));
				free(szBuffer);
			}
		}
	}
}
