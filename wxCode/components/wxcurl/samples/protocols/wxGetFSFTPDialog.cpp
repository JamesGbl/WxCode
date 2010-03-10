/*
 *	wxGetFSFTPDialog.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Wed Aug 18 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxGetFSFTPDialog.cpp: implementation of the wxGetFSFTPDialog class.
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

#include <wx/curl/ftptool.h>

#include "wxGetFSFTPDialog.h"

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
IMPLEMENT_CLASS(wxGetFSFTPDialog, wxDialog)
BEGIN_EVENT_TABLE(wxGetFSFTPDialog, wxDialog)
	EVT_BUTTON(XRCID("file_menu_getfs"), wxGetFSFTPDialog::OnGetFS)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxGetFSFTPDialog::wxGetFSFTPDialog(wxWindow* pParent)
{
	wxXmlResource::Get()->LoadDialog(this, pParent, wxS("getfs_ftp_dialog"));

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

wxGetFSFTPDialog::~wxGetFSFTPDialog()
{

}

//////////////////////////////////////////////////////////////////////
// Event Handlers
//////////////////////////////////////////////////////////////////////

void wxGetFSFTPDialog::OnGetFS(wxCommandEvent& WXUNUSED(event))
{
	wxString szHost, szUser, szPass;

	if(m_pHostCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		szHost = m_pHostCtrl->GetValue();
		szUser = m_pUserCtrl->GetValue();
		szPass = m_pPassCtrl->GetValue();

		wxCurlFTPTool ftp(szHost);

		ftp.UsePortOption(true);

		if(szUser != m_szDefaultUser)
			ftp.SetUsername(szUser);

		if(szPass != m_szDefaultPass)
			ftp.SetPassword(szPass);

		wxString szResponse;

		wxArrayFTPFs fsResponse;

		if(ftp.GetFTPFs(fsResponse))
		{
			szResponse = wxS("SUCCESS!\n\n");
			szResponse += wxString::Format(wxS("\nResponse Code: %d\n\n"), ftp.GetResponseCode());

			for(unsigned int i = 0; i < fsResponse.Count(); i++)
			{
				const wxCurlFTPFs& fsItem = fsResponse[i];

				szResponse += fsItem.GetName();
				szResponse += wxS("\n");
				szResponse += fsItem.GetFileSuffix();
				szResponse += wxS("\n\n");
			}

			if(m_pTextCtrl)
				m_pTextCtrl->SetValue(szResponse);
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

			if(m_pTextCtrl)
				m_pTextCtrl->SetValue(szResponse);
		}
	}
}

