/*
 *	wxGetFTPFileDialog.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Fri Jul 23 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxGetFTPFileDialog.cpp: implementation of the wxGetFTPFileDialog class.
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

#include <wx/filedlg.h>

#include <wx/curl/ftp.h>

#include "wxGetFTPFileDialog.h"

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
IMPLEMENT_CLASS(wxGetFTPFileDialog, wxDialog)
BEGIN_EVENT_TABLE(wxGetFTPFileDialog, wxDialog)
	EVT_BUTTON(XRCID("get_button"), wxGetFTPFileDialog::OnGet)
	EVT_BUTTON(XRCID("dest_button"), wxGetFTPFileDialog::OnBrowseDest)
	EVT_CURL_PROGRESS(wxID_ANY, wxGetFTPFileDialog::OnProgress)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxGetFTPFileDialog::wxGetFTPFileDialog(wxWindow* pParent)
{
	wxXmlResource::Get()->LoadDialog(this, pParent, "get_file_ftp_dialog");

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

wxGetFTPFileDialog::~wxGetFTPFileDialog()
{

}

//////////////////////////////////////////////////////////////////////
// Event Handlers
//////////////////////////////////////////////////////////////////////

void wxGetFTPFileDialog::OnGet(wxCommandEvent& WXUNUSED(event))
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
			wxMessageBox("Please change the source and destination locations.", "Error...", wxICON_INFORMATION|wxOK, this);
		}
		else if((szUser == m_szDefaultUser) && (szPass == m_szDefaultPass))
		{
			wxMessageBox("Please change the username or password.", "Error...", wxICON_INFORMATION|wxOK, this);
		}
		else
		{
			// Do It!
			wxCurlFTP ftp(szSrc, szUser, szPass, this, true);

			if(ftp.Get(szDest))
			{
					szResponse = "SUCCESS!\n\n";
					szResponse += wxString::Format("\nResponse Code: %d\n\n", ftp.GetResponseCode());
					szResponse += ftp.GetResponseHeader();
					szResponse += "\n\n";
					szResponse += ftp.GetResponseBody();

					if(m_pRespCtrl)
						m_pRespCtrl->SetValue(szResponse);
			}
			else
			{
					szResponse = "FAILURE!\n\n";
					szResponse += wxString::Format("\nResponse Code: %d\n\n", ftp.GetResponseCode());
					szResponse += ftp.GetResponseHeader();
					szResponse += "\n\n";
					szResponse += ftp.GetResponseBody();
					szResponse += "\n\n";
					szResponse += ftp.GetErrorString();

					if(m_pRespCtrl)
						m_pRespCtrl->SetValue(szResponse);
			}
		}
	}
}

void wxGetFTPFileDialog::OnBrowseDest(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog dlg(this, "Choose a file", "", "", "All files (*.*)|*.*", wxSAVE|wxOVERWRITE_PROMPT);

	if(dlg.ShowModal()==wxID_OK)
	{
		wxString szVal = dlg.GetPath();

		if(m_pDestCtrl)
		{
			m_pDestCtrl->SetValue(szVal);
		}
	}
}

void wxGetFTPFileDialog::OnProgress(wxCurlProgressEvent& event)
{
	if(m_pDlGauge)
	{
		m_pDlGauge->SetValue((int)event.DownloadPercent());
	}
}
