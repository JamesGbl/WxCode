/*
 *	wxPutDialog.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Tue May 25 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxPutDialog.cpp: implementation of the wxPutDialog class.
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

#include <wx/filedlg.h>

#include <wx/curl/http.h>

#include "wxPutDialog.h"

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
IMPLEMENT_CLASS(wxPutDialog, wxDialog)
BEGIN_EVENT_TABLE(wxPutDialog, wxDialog)
	EVT_BUTTON(XRCID("source_button"), wxPutDialog::OnBrowse)
	EVT_BUTTON(XRCID("put_button"), wxPutDialog::OnPutFile)
	EVT_CURL_UPLOAD(wxID_ANY, wxPutDialog::OnProgress)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxPutDialog::wxPutDialog(wxWindow* pParent)
{
	wxXmlResource::Get()->LoadDialog(this, pParent, wxS("put_dialog"));

	SetSize(400, -1);

	m_pSourceCtrl	= XRCCTRL(*this, "source_text_ctrl", wxTextCtrl);
	m_pDestCtrl		= XRCCTRL(*this, "dest_text_ctrl", wxTextCtrl);
	m_pUserCtrl		= XRCCTRL(*this, "user_text_ctrl", wxTextCtrl);
	m_pPassCtrl		= XRCCTRL(*this, "pass_text_ctrl", wxTextCtrl);

	m_pResponseCtrl	= XRCCTRL(*this, "response_text_ctrl", wxTextCtrl);

	if(m_pSourceCtrl && m_pDestCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		m_szDefaultSource	= m_pSourceCtrl->GetValue();
		m_szDefaultDest		= m_pDestCtrl->GetValue();
		m_szDefaultUser		= m_pUserCtrl->GetValue();
		m_szDefaultPass		= m_pPassCtrl->GetValue();
	}

	m_pGauge			= XRCCTRL(*this, "upload_gauge", wxGauge);

	if(m_pGauge)
	{
		m_pGauge->SetRange(100);
	}
}

wxPutDialog::~wxPutDialog()
{
}

//////////////////////////////////////////////////////////////////////
// Event Handlers
//////////////////////////////////////////////////////////////////////

void wxPutDialog::OnBrowse(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog dlg(this, wxS("Choose a file"), wxEmptyString, wxEmptyString,
                     wxS("All files (*.*)|*.*"), wxFD_OPEN);

	if(dlg.ShowModal()==wxID_OK)
	{
		wxString szVal = dlg.GetPath();

		if(m_pSourceCtrl)
		{
			m_pSourceCtrl->SetValue(szVal);
		}
	}
}

void wxPutDialog::OnPutFile(wxCommandEvent& WXUNUSED(event))
{
	if(m_pSourceCtrl && m_pDestCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		wxString szSource	= m_pSourceCtrl->GetValue();
		wxString szDest		= m_pDestCtrl->GetValue();
		wxString szUser		= m_pUserCtrl->GetValue();
		wxString szPass		= m_pPassCtrl->GetValue();

		wxString szResponse;

		if((szSource == m_szDefaultSource) || (szDest == m_szDefaultDest))
		{
			wxMessageBox(wxS("Please change the source and destination locations."), wxS("Error..."), wxICON_INFORMATION|wxOK, this);
		}
		else if((szUser == m_szDefaultUser) && (szPass == m_szDefaultPass))
		{
			wxMessageBox(wxS("Please change the username or password."), wxS("Error..."), wxICON_INFORMATION|wxOK, this);
		}
		else
		{
			if(wxFileExists(szSource))
			{
				// Do it!
				wxCurlHTTP http(szDest, szUser, szPass, this, true);

				if(http.Put(szSource))
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
			else
			{
				wxMessageBox(wxS("File Could Not Be Found."), wxS("Error..."), wxICON_ERROR|wxOK, this);
			}
		}
	}
}

void wxPutDialog::OnProgress(wxCurlUploadEvent& event)
{
	if(m_pGauge)
	{
		m_pGauge->SetValue((int)event.GetPercent());
		m_pGauge->Refresh();
	}
}
