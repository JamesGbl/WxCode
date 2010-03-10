/*
 *	wxDeleteDialog.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Wed Jun 02 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxDeleteDialog.cpp: implementation of the wxDeleteDialog class.
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

#include "wxDeleteDialog.h"

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
IMPLEMENT_CLASS(wxDeleteDialog, wxDialog)
BEGIN_EVENT_TABLE(wxDeleteDialog, wxDialog)
	EVT_BUTTON(XRCID("delete_button"), wxDeleteDialog::OnDelete)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxDeleteDialog::wxDeleteDialog(wxWindow* pParent)
{
	wxXmlResource::Get()->LoadDialog(this, pParent, wxS("delete_dialog"));

	SetSize(400, -1);

	m_pDeleCtrl		= XRCCTRL(*this, "dele_text_ctrl", wxTextCtrl);
	m_pUserCtrl		= XRCCTRL(*this, "user_text_ctrl", wxTextCtrl);
	m_pPassCtrl		= XRCCTRL(*this, "pass_text_ctrl", wxTextCtrl);

	m_pResponseCtrl	= XRCCTRL(*this, "response_text_ctrl", wxTextCtrl);

	if(m_pDeleCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		m_szDefaultDele		= m_pDeleCtrl->GetValue();
		m_szDefaultUser		= m_pUserCtrl->GetValue();
		m_szDefaultPass		= m_pPassCtrl->GetValue();
	}
}

wxDeleteDialog::~wxDeleteDialog()
{

}

//////////////////////////////////////////////////////////////////////
// Event Handlers
//////////////////////////////////////////////////////////////////////

void wxDeleteDialog::OnDelete(wxCommandEvent& WXUNUSED(event))
{
	if(m_pDeleCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		wxString szDele		= m_pDeleCtrl->GetValue();
		wxString szUser		= m_pUserCtrl->GetValue();
		wxString szPass		= m_pPassCtrl->GetValue();

		wxString szResponse;

		if((szDele == m_szDefaultDele))
		{
			wxMessageBox(wxS("Please change the DELETE location."), wxS("Error..."), wxICON_INFORMATION|wxOK, this);
		}
		else if((szUser == m_szDefaultUser) && (szPass == m_szDefaultPass))
		{
			wxMessageBox(wxS("Please change the username or password."), wxS("Error..."), wxICON_INFORMATION|wxOK, this);
		}
		else
		{
			// Do it!
			wxCurlHTTP http(szDele, szUser, szPass);

			if(http.Delete())
			{
					szResponse = wxS("SUCCESS!\n\n");
					szResponse += wxString::Format(wxS("\nResponse Code: %d\n\n"), http.GetResponseCode());
					szResponse += http.GetResponseHeader();
					szResponse += wxS("\n\n");
					szResponse += http.GetResponseBody();

					if(m_pResponseCtrl)
						m_pResponseCtrl->SetValue(szResponse);
			}
			else
			{
					szResponse = wxS("FAILURE!\n\n");
					szResponse += wxString::Format(wxS("\nResponse Code: %d\n\n"), http.GetResponseCode());
					szResponse += http.GetResponseHeader();
					szResponse += wxS("\n\n");
					szResponse += http.GetResponseBody();
					szResponse += wxS("\n\n");
					szResponse += http.GetErrorString();

					if(m_pResponseCtrl)
						m_pResponseCtrl->SetValue(szResponse);
			}
		}
	}
}
