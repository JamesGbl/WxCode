/*
 *	wxPostDialog.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Thu Jul 15 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 */

// wxPostDialog.cpp: implementation of the wxPostDialog class.
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

#include "wxPostDialog.h"

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
IMPLEMENT_CLASS(wxPostDialog, wxDialog)
BEGIN_EVENT_TABLE(wxPostDialog, wxDialog)
	EVT_BUTTON(XRCID("file_menu_post"), wxPostDialog::OnPost)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxPostDialog::wxPostDialog(wxWindow* pParent)
{
	wxXmlResource::Get()->LoadDialog(this, pParent, "post_dialog");

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

wxPostDialog::~wxPostDialog()
{
}

//////////////////////////////////////////////////////////////////////
// Event Handlers
//////////////////////////////////////////////////////////////////////

void wxPostDialog::OnPost(wxCommandEvent& WXUNUSED(event))
{
	if(m_pDestCtrl && m_pUserCtrl && m_pPassCtrl)
	{
		wxString szDest	= m_pDestCtrl->GetValue();
		wxString szUser	= m_pUserCtrl->GetValue();
		wxString szPass	= m_pPassCtrl->GetValue();
		wxString szData = m_pDataCtrl->GetValue();

		wxString szResponse;

		if((szUser == m_szDefaultUser) && (szPass == m_szDefaultPass))
		{
			wxMessageBox("Please change the username or password.", "Error...", wxICON_INFORMATION|wxOK, this);
		}
		else if(szData.IsEmpty())
		{
			wxMessageBox("Please enter some data into the data control.", "Error...", wxICON_INFORMATION|wxOK, this);
		}
		else
		{
			// Do It!
			wxCurlHTTP http(szDest, szUser, szPass);

			http.Verbose(true);

/*			struct curl_forms testform[2];

			wxString szValue = "My Name is Bob the Builder";
			testform[0].option = CURLFORM_COPYCONTENTS;
			testform[0].value = szValue.c_str();
			testform[1].option = CURLFORM_END;

			if(!http.AddForm(false, _T("firstname"), testform))
				return;*/

			if(http.Post(szData, szData.Len() * sizeof(char)))
//			if(http.Post())
			{
				wxMemoryOutputStream outStream;

				wxString szVerbose;
				http.GetVerboseString(szVerbose);

				szResponse = "SUCCESS!\n\n";
				szResponse += http.GetResponseBody();
				szResponse += "\n\nVERBOSE DATA:\n";
				szResponse += szVerbose;

				if(m_pRespCtrl)
					m_pRespCtrl->SetValue(szResponse);
			}
			else
			{
				szResponse = "FAILURE!\n\n";
				szResponse += wxString::Format("\nResponse Code: %d\n\n", http.GetResponseCode());
				szResponse += http.GetResponseHeader();
				szResponse += "\n\n";
				szResponse += http.GetResponseBody();
				szResponse += "\n\n";
				szResponse += http.GetErrorString();

				if(m_pRespCtrl)
					m_pRespCtrl->SetValue(szResponse);
			}
		}
	}
}

