/*
 *	wxGetFileDialog.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Tue Jun 08 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxGetFileDialog.cpp: implementation of the wxGetFileDialog class.
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

#include "wxGetFileDialog.h"

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
IMPLEMENT_CLASS(wxGetFileDialog, wxDialog)
BEGIN_EVENT_TABLE(wxGetFileDialog, wxDialog)
	EVT_BUTTON(XRCID("get_button"), wxGetFileDialog::OnGet)
	EVT_BUTTON(XRCID("dest_button"), wxGetFileDialog::OnBrowseDest)
	EVT_CURL_DOWNLOAD(wxID_ANY, wxGetFileDialog::OnProgress)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxGetFileDialog::wxGetFileDialog(wxWindow* pParent)
{
	wxXmlResource::Get()->LoadDialog(this, pParent, wxS("get_file_dialog"));

	SetSize(400, -1);

	m_pDestCtrl	= XRCCTRL(*this, "dest_text_ctrl", wxTextCtrl);
	m_pSrcCtrl	= XRCCTRL(*this, "source_text_ctrl", wxTextCtrl);
	m_pUserCtrl	= XRCCTRL(*this, "user_text_ctrl", wxTextCtrl);
	m_pPassCtrl	= XRCCTRL(*this, "pass_text_ctrl", wxTextCtrl);

	m_pRespCtrl	= XRCCTRL(*this, "response_text_ctrl", wxTextCtrl);

	if(m_pDestCtrl && m_pSrcCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		m_szDefaultDest	= m_pDestCtrl->GetValue();
		m_szDefaultSrc	= m_pSrcCtrl->GetValue();
		m_szDefaultUser	= m_pUserCtrl->GetValue();
		m_szDefaultPass	= m_pPassCtrl->GetValue();
	}

	m_pDlGauge = XRCCTRL(*this, "dnload_gauge", wxGauge);

	if(m_pDlGauge)
	{
		m_pDlGauge->SetRange(100);
	}
}

wxGetFileDialog::~wxGetFileDialog()
{

}

//////////////////////////////////////////////////////////////////////
// Event Handlers
//////////////////////////////////////////////////////////////////////

void wxGetFileDialog::OnGet(wxCommandEvent& WXUNUSED(event))
{
	if(m_pSrcCtrl && m_pDestCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		wxString szSrc	= m_pSrcCtrl->GetValue();
		wxString szDest	= m_pDestCtrl->GetValue();
		wxString szUser	= m_pUserCtrl->GetValue();
		wxString szPass	= m_pPassCtrl->GetValue();

		wxString szResponse;

		if((szSrc == m_szDefaultSrc) || (szDest == m_szDefaultDest))
		{
			wxMessageBox(wxS("Please change the source and destination locations."), wxS("Error..."), wxICON_INFORMATION|wxOK, this);
		}
		else if((szUser == m_szDefaultUser) && (szPass == m_szDefaultPass))
		{
			wxMessageBox(wxS("Please change the username or password."), wxS("Error..."), wxICON_INFORMATION|wxOK, this);
		}
		else
		{
			// Do It!
			wxCurlHTTP http(szSrc, szUser, szPass, this, true);

			if(http.Get(szDest))
			{
					szResponse = wxS("SUCCESS!\n\n");
					szResponse += wxString::Format(wxS("\nResponse Code: %d\n\n"), http.GetResponseCode());
					szResponse += http.GetResponseHeader();
					szResponse += wxS("\n\n");
					szResponse += http.GetResponseBody();

					if(m_pRespCtrl)
						m_pRespCtrl->SetValue(szResponse);

                    wxString ret;
                    http.GetVerboseString(ret);
                    wxLogMessage(ret);
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

					if(m_pRespCtrl)
						m_pRespCtrl->SetValue(szResponse);
			}
		}
	}
}

void wxGetFileDialog::OnBrowseDest(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog dlg(this, wxS("Choose a file"), wxEmptyString, wxEmptyString, 
                     wxS("All files (*.*)|*.*"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

	if(dlg.ShowModal()==wxID_OK)
	{
		wxString szVal = dlg.GetPath();

		if(m_pDestCtrl)
		{
			m_pDestCtrl->SetValue(szVal);
		}
	}
}

void wxGetFileDialog::OnProgress(wxCurlDownloadEvent& event)
{
	if(m_pDlGauge)
	{
		m_pDlGauge->SetValue((int)event.GetPercent());
	}
}
