/*
 *	wxMkcolDialog.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Wed Jun 02 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxMkcolDialog.cpp: implementation of the wxMkcolDialog class.
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

#include <wx/curl/dav.h>

#include "wxMkcolDialog.h"

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
IMPLEMENT_CLASS(wxMkcolDialog, wxDialog)
BEGIN_EVENT_TABLE(wxMkcolDialog, wxDialog)
	EVT_BUTTON(XRCID("mkcol_button"), wxMkcolDialog::OnMkcol)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxMkcolDialog::wxMkcolDialog(wxWindow* pParent)
{
	wxXmlResource::Get()->LoadDialog(this, pParent, wxS("mkcol_dialog"));

	SetSize(400, -1);

	m_pMkcolCtrl	= XRCCTRL(*this, "mkcol_text_ctrl", wxTextCtrl);
	m_pUserCtrl		= XRCCTRL(*this, "user_text_ctrl", wxTextCtrl);
	m_pPassCtrl		= XRCCTRL(*this, "pass_text_ctrl", wxTextCtrl);

	m_pResponseCtrl	= XRCCTRL(*this, "response_text_ctrl", wxTextCtrl);

	if(m_pMkcolCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		m_szDefaultMkcol	= m_pMkcolCtrl->GetValue();
		m_szDefaultUser		= m_pUserCtrl->GetValue();
		m_szDefaultPass		= m_pPassCtrl->GetValue();
	}
}

wxMkcolDialog::~wxMkcolDialog()
{

}

//////////////////////////////////////////////////////////////////////
// Event Handlers
//////////////////////////////////////////////////////////////////////

void wxMkcolDialog::OnMkcol(wxCommandEvent& WXUNUSED(event))
{
	if(m_pMkcolCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		wxString szMkcol	= m_pMkcolCtrl->GetValue();
		wxString szUser		= m_pUserCtrl->GetValue();
		wxString szPass		= m_pPassCtrl->GetValue();

		wxString szResponse;

		if((szMkcol == m_szDefaultMkcol))
		{
			wxMessageBox(wxS("Please change the MKCOL location."), wxS("Error..."), wxICON_INFORMATION|wxOK, this);
		}
		else if((szUser == m_szDefaultUser) && (szPass == m_szDefaultPass))
		{
			wxMessageBox(wxS("Please change the username or password."), wxS("Error..."), wxICON_INFORMATION|wxOK, this);
		}
		else
		{
			// Do it!
			wxCurlDAV dav(szMkcol, szUser, szPass);

			if(dav.Mkcol())
			{
					szResponse = wxS("SUCCESS!\n\n");
					szResponse += wxString::Format(wxS("\nResponse Code: %d\n\n"), dav.GetResponseCode());
					szResponse += dav.GetResponseHeader();
					szResponse += wxS("\n\n");
					szResponse += dav.GetResponseBody();

					if(m_pResponseCtrl)
						m_pResponseCtrl->SetValue(szResponse);
			}
			else
			{
					szResponse = wxS("FAILURE!\n\n");
					szResponse += wxString::Format(wxS("\nResponse Code: %d\n\n"), dav.GetResponseCode());
					szResponse += dav.GetResponseHeader();
					szResponse += wxS("\n\n");
					szResponse += dav.GetResponseBody();
					szResponse += wxS("\n\n");
					szResponse += dav.GetErrorString();

					if(m_pResponseCtrl)
						m_pResponseCtrl->SetValue(szResponse);
			}
		}
	}
}

