/*
 *	wxPropfindDialog.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Tue Jun 08 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxPropfindDialog.cpp: implementation of the wxPropfindDialog class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Headers
//////////////////////////////////////////////////////////////////////
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif
#include <wx/xrc/xmlres.h>

#include <wx/curl/dav.h>

#include "wxPropfindDialog.h"

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
IMPLEMENT_CLASS(wxPropfindDialog, wxDialog)
BEGIN_EVENT_TABLE(wxPropfindDialog, wxDialog)
	EVT_BUTTON(XRCID("file_menu_propfind"), wxPropfindDialog::OnPropfind)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxPropfindDialog::wxPropfindDialog(wxWindow* pParent)
{
	wxXmlResource::Get()->LoadDialog(this, pParent, "propfind_dialog");

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

wxPropfindDialog::~wxPropfindDialog()
{

}

//////////////////////////////////////////////////////////////////////
// Event Handlers
//////////////////////////////////////////////////////////////////////

void wxPropfindDialog::OnPropfind(wxCommandEvent& WXUNUSED(event))
{
	wxString szHost, szUser, szPass;

	if(m_pHostCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		szHost = m_pHostCtrl->GetValue();
		szUser = m_pUserCtrl->GetValue();
		szPass = m_pPassCtrl->GetValue();

		wxCurlDAV dav(szHost);

		if(szUser != m_szDefaultUser)
			dav.SetUsername(szUser);

		if(szPass != m_szDefaultPass)
			dav.SetPassword(szPass);

		wxString szResponse;

		if(dav.Propfind())
		{
			szResponse = "SUCCESS!\n\n";
			szResponse += wxString::Format("\nResponse Code: %d\n\n", dav.GetResponseCode());
			szResponse += dav.GetResponseHeader();
			szResponse += "\n\n";
			szResponse += dav.GetResponseBody();

			if(m_pTextCtrl)
				m_pTextCtrl->SetValue(szResponse);
		}
		else
		{
			szResponse = "FAILURE!\n\n";
			szResponse += wxString::Format("\nResponse Code: %d\n\n", dav.GetResponseCode());
			szResponse += dav.GetResponseHeader();
			szResponse += "\n\n";
			szResponse += dav.GetResponseBody();
			szResponse += "\n\n";
			szResponse += dav.GetErrorString();

			if(m_pTextCtrl)
				m_pTextCtrl->SetValue(szResponse);
		}
	}
}

