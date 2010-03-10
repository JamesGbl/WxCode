/*
 *	wxHeadDialog.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Wed Jul 14 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxHeadDialog.cpp: implementation of the wxHeadDialog class.
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

#include <wx/curl/http.h>

#include "wxHeadDialog.h"

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
IMPLEMENT_CLASS(wxHeadDialog, wxDialog)
BEGIN_EVENT_TABLE(wxHeadDialog, wxDialog)
	EVT_BUTTON(XRCID("file_menu_head"), wxHeadDialog::OnHead)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxHeadDialog::wxHeadDialog(wxWindow* pParent)
{
	wxXmlResource::Get()->LoadDialog(this, pParent, wxS("head_dialog"));

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

wxHeadDialog::~wxHeadDialog()
{
}

//////////////////////////////////////////////////////////////////////
// Event Handlers
//////////////////////////////////////////////////////////////////////

void wxHeadDialog::OnHead(wxCommandEvent& WXUNUSED(event))
{
	wxString szHost, szUser, szPass;

	if(m_pHostCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		szHost = m_pHostCtrl->GetValue();
		szUser = m_pUserCtrl->GetValue();
		szPass = m_pPassCtrl->GetValue();

		wxCurlHTTP http(szHost);

		if(szUser != m_szDefaultUser)
			http.SetUsername(szUser);

		if(szPass != m_szDefaultPass)
			http.SetPassword(szPass);

		if(http.Options())
		{
			if(m_pTextCtrl)
			{
				m_pTextCtrl->SetValue(http.GetResponseHeader());
			}
		}
	}
}
