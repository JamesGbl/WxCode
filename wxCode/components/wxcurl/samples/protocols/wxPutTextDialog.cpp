/*
 *	wxPutTextDialog.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Tue Jun 08 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxPutTextDialog.cpp: implementation of the wxPutTextDialog class.
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

#include <wx/curl/http.h>

#include "wxPutTextDialog.h"

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
IMPLEMENT_CLASS(wxPutTextDialog, wxDialog)
BEGIN_EVENT_TABLE(wxPutTextDialog, wxDialog)
	EVT_BUTTON(XRCID("put_button"), wxPutTextDialog::OnPut)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxPutTextDialog::wxPutTextDialog(wxWindow* pParent)
{
	wxXmlResource::Get()->LoadDialog(this, pParent, wxT("put_text_dialog"));

	SetSize(400, -1);

	m_pDestCtrl		= XRCCTRL(*this, "dest_text_ctrl", wxTextCtrl);;
	m_pUserCtrl		= XRCCTRL(*this, "user_text_ctrl", wxTextCtrl);;
	m_pPassCtrl		= XRCCTRL(*this, "pass_text_ctrl", wxTextCtrl);;

	m_pDataCtrl		= XRCCTRL(*this, "data_text_ctrl", wxTextCtrl);;
	m_pRespCtrl		= XRCCTRL(*this, "response_text_ctrl", wxTextCtrl);;

	if(m_pDestCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		m_szDefaultDest		= m_pDestCtrl->GetValue();
		m_szDefaultUser		= m_pUserCtrl->GetValue();
		m_szDefaultPass		= m_pPassCtrl->GetValue();
	}
}

wxPutTextDialog::~wxPutTextDialog()
{
}

//////////////////////////////////////////////////////////////////////
// Event Handlers
//////////////////////////////////////////////////////////////////////

void wxPutTextDialog::OnPut(wxCommandEvent& WXUNUSED(event))
{
	if(m_pDestCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		wxString szDest	= m_pDestCtrl->GetValue();
		wxString szUser	= m_pUserCtrl->GetValue();
		wxString szPass	= m_pPassCtrl->GetValue();
		wxString szData = m_pDataCtrl->GetValue();

		wxString szResponse;

		if((szDest == m_szDefaultDest))
		{
			wxMessageBox(wxT("Please change the destination location."), wxT("Error..."), wxICON_INFORMATION|wxOK, this);
		}
		else if((szUser == m_szDefaultUser) && (szPass == m_szDefaultPass))
		{
			wxMessageBox(wxT("Please change the username or password."), wxT("Error..."), wxICON_INFORMATION|wxOK, this);
		}
		else if(szData.IsEmpty())
		{
			wxMessageBox(wxT("Please enter some data into the data control."), wxT("Error..."), wxICON_INFORMATION|wxOK, this);
		}
		else
		{
			// Do It!
			wxCurlHTTP http(szDest, szUser, szPass);

			if(http.Put(szData.ToAscii(), szData.Len() * sizeof(char), wxEmptyString))
			{
					szResponse = wxT("SUCCESS!\n\n");
					szResponse += wxString::Format(wxT("\nResponse Code: %d\n\n"), http.GetResponseCode());
					szResponse += http.GetResponseHeader();
					szResponse += wxT("\n\n");
					szResponse += http.GetResponseBody();

					if(m_pRespCtrl)
						m_pRespCtrl->SetValue(szResponse);
			}
			else
			{
					szResponse = wxT("FAILURE!\n\n");
					szResponse += wxString::Format(wxT("\nResponse Code: %d\n\n"), http.GetResponseCode());
					szResponse += http.GetResponseHeader();
					szResponse += wxT("\n\n");
					szResponse += http.GetResponseBody();
					szResponse += wxT("\n\n");
					szResponse += http.GetErrorString();

					if(m_pRespCtrl)
						m_pRespCtrl->SetValue(szResponse);
			}
		}
	}
}

