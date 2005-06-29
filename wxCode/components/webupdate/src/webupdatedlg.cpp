/////////////////////////////////////////////////////////////////////////////
// Name:        webupdatedlg.cpp
// Purpose:     wxWebUpdateDlg
// Author:      Francesco Montorsi
// Created:     2005/06/23
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
	#pragma implementation "webupdatedlg.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#include "wx/webupdatedlg.h"
#include <wx/wfstream.h>


IMPLEMENT_CLASS(wxWebUpdateLocalPackage, wxObject)
IMPLEMENT_CLASS(wxWebUpdateDlg, wxDialog)
BEGIN_EVENT_TABLE(wxWebUpdateDlg, wxDialog)

	// buttons
    EVT_BUTTON(XRCID("IDWUD_DOWNLOAD"), wxWebUpdateDlg::OnDownload)
    EVT_BUTTON(XRCID("IDWUD_BROWSE"), wxWebUpdateDlg::OnBrowse)
    EVT_BUTTON(XRCID("IDWUD_CANCEL"), wxWebUpdateDlg::OnCancel)

	// listctrl

	// download thread
	EVT_UPDATE_UI(wxWUT_NOTIFICATION, wxWebUpdateDlg::OnDownloadComplete)

END_EVENT_TABLE()



// ---------------------
// wxWEBUPDATETHREAD
// ---------------------

void *wxWebUpdateThread::Entry()
{
	wxUpdateUIEvent updatevent(wxWUT_NOTIFICATION);
	wxLogDebug(wxT("wxWebUpdateThread::Entry - downloading ") + m_strURI);

    // with wxFileSystem a file-download is very easy !
	wxFileSystem fs;
	wxFSFile *download = fs.OpenFile(m_strURI);
	if (!download) {
		m_bSuccess = FALSE;
		m_pHandler->AddPendingEvent(updatevent);
		return (void*)FALSE;
	}
	
	// now work on streams
	wxFileOutputStream out(m_strOutput);
	wxInputStream *in = download->GetStream();
	if (in == NULL) { 
		delete download; 
		m_bSuccess = FALSE;
		m_pHandler->AddPendingEvent(updatevent);
		return (void*)FALSE; 
	}

	// write the downloaded stuff in the output file
	out.Write(*in);
	delete download;

	if (out.IsOk()) {
		m_bSuccess = TRUE;
		m_pHandler->AddPendingEvent(updatevent);
		return (void*)TRUE;
	}

	m_bSuccess = FALSE;
	m_pHandler->AddPendingEvent(updatevent);
	return (void*)FALSE;
}
/*
void wxWebUpdateThread::OnExit()
{
}
*/



// ---------------------
// wxWEBUPDATEDLG
// ---------------------

void wxWebUpdateDlg::InitWidgetsFromXRC()
{
	// build the dialog
	// ----------------

	// we need some handlers before loading resources
	wxImage::AddHandler(new wxPNGHandler);
	wxXmlResource::Get()->InitAllHandlers();
	
    // load our XRC file
    wxXmlResource::Get()->Load(wxT("../src/webupdatedlg.xrc"));

	// and build our dialog window
	wxXmlResource::Get()->LoadDialog(this, GetParent(), wxT("wxWebUpdateDlg"));	



	// init control pointers
	// ---------------------

	m_pAppNameText = XRCCTRL(*this,"IDWUD_APPNAME_TEXT",wxStaticText);
	m_pDownloadStatusText = XRCCTRL(*this,"IDWUD_PROGRESS_TEXT",wxStaticText);
	m_pTimeText = XRCCTRL(*this,"IDWUD_TIME_TEXT",wxStaticText);
	m_pUpdatesList = XRCCTRL(*this,"IDWUD_LISTCTRL",wxListCtrl);
	m_pGauge = XRCCTRL(*this,"IDWUD_GAUGE",wxGauge);
	m_pDownloadPathTextCtrl = XRCCTRL(*this,"IDWUD_DOWNLOAD_PATH",wxTextCtrl);



	// init control data
	// -----------------

	m_pAppNameText->SetLabel(m_strAppName);
	
	// get the temporary folder where we put by default the updates
	wxFileName str(wxFileName::CreateTempFileName(wxEmptyString, NULL));
	str.SetFullName(wxEmptyString);		// remove the filename and keep only the path
	m_pDownloadPathTextCtrl->SetValue(str.GetLongPath());

	// init the list control with the column names
	// (items will be inserted as soon as we load the webupdate script)the user-supplied wxWebUpdateLocalPackages
	m_pUpdatesList->InsertColumn(0, wxT("Package name"));
	m_pUpdatesList->InsertColumn(1, wxT("Latest version"));
	m_pUpdatesList->InsertColumn(2, wxT("Local version"));
	m_pUpdatesList->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);



	// init other stuff
	// ----------------

	m_thread.m_pHandler = this;



	// relayout
	// --------

	// we have changed the appname statictext control so maybe we need
	// to expand our dialog... force a layout recalculation
	GetSizer()->RecalcSizes();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
}

int wxWebUpdateDlg::ShowModal()
{
	// set our thread helper as a downloader for the XML update scriptf
	m_thread.m_strURI = m_strURI;
	m_thread.m_strResName = wxT("XML WebUpdate script");
	m_thread.m_strOutput = wxFileName::CreateTempFileName(wxT("webupdate"));
	m_pTimeText->SetLabel(wxWUD_TIMETEXT_PREFIX wxT("60 s"));

	// this special flag makes easier the #OnDownloadComplete handler
	m_bDownloadingScript = TRUE;

	// launch a separate thread for the webupdate script download
	if (m_thread.Create() != wxTHREAD_NO_ERROR ||
		m_thread.GetThread()->Run() != wxTHREAD_NO_ERROR) {
		wxMessageBox(wxString(wxT("Cannot download the script file from\n")) + 
					m_strURI + wxT("\nbecause of the low resources..."), 
					wxT("Error"), wxOK | wxICON_ERROR);
		return NULL;
	}

	// as soon as the wxWebUpdateThread has completed its work we'll receive
	// a notification through the wxWUT_NOTIFICATION events
	m_pDownloadStatusText->SetLabel(wxT("Downloading update list..."));

	// proceed with standard processing
	return wxDialog::ShowModal();
}

void wxWebUpdateDlg::ShowErrorMsg(const wxString &str) const
{
	wxMessageBox(str + wxT("\nContact the support team of ") + m_strAppName +
					wxT(" for help."), wxT("Error"), wxOK | wxICON_ERROR);
}

void wxWebUpdateDlg::OnScriptDownload(const wxString &xmluri)
{
	// ok, we can now parse the XML doc
	if (!m_xmlScript.Load(xmluri)) {
		ShowErrorMsg(wxT("Cannot parse the XML update script downloaded as: ") + 
						m_thread.m_strOutput);
		EndModal(wxCANCEL);		// this is a unrecoverable error !
	}
	
	// now load all the packages we need in local cache

	for (int i=0; i < m_nLocalPackages; i++) {

		wxListItem li;
		li.SetId(i);

		// set the properties for the first column (NAME)
		li.SetText(m_pLocalPackages[i].m_strName);
		li.SetColumn(0);
		m_pUpdatesList->InsertItem(li);

		// set the properties for the second column (LATEST VERSION)
		//li.SetText();
		li.SetColumn(1);
		m_pUpdatesList->SetItem(li);
	}
}



// event handlers

void wxWebUpdateDlg::OnDownload(wxCommandEvent &)
{
	EndModal(wxOK);
}

void wxWebUpdateDlg::OnBrowse(wxCommandEvent &)
{
	// get the current value of the the "download path" from the textctrl
	wxString path = m_pDownloadPathTextCtrl->GetValue();
	wxDirDialog dlg(this, wxT("Choose a directory"), 
					path, wxDD_DEFAULT_STYLE | wxDD_NEW_DIR_BUTTON);
	if (dlg.ShowModal() == wxID_OK) {

		m_pDownloadPathTextCtrl->SetValue(dlg.GetPath());
		
	} else {

		// just don't change nothing
	}
}

void wxWebUpdateDlg::OnCancel(wxCommandEvent &)
{
	EndModal(wxCANCEL);
}

void wxWebUpdateDlg::OnDownloadComplete(wxUpdateUIEvent &)
{
	// first of all, we need to know if download was successful
	if (!m_thread.DownloadWasSuccessful()) {

		ShowErrorMsg(wxT("Could not download the ") + m_thread.m_strResName + 
					wxT(" from\n\n") + m_thread.m_strURI + wxT("\n\nURL... "));
		
		m_pDownloadStatusText->SetLabel(wxT("Download failed !"));
		m_pTimeText->SetLabel(wxT("No downloads running..."));

		if (m_bDownloadingScript)
			EndModal(wxCANCEL);		// this is a unrecoverable error !

		return;

	} else {

		m_pDownloadStatusText->SetLabel(wxT("Download successfully completed"));
		m_pTimeText->SetLabel(wxT("No downloads running..."));

		if (m_bDownloadingScript) {

			// handle the XML parsing & control update 
			OnScriptDownload(m_thread.m_strOutput);
			m_bDownloadingScript = FALSE;

		} else {

			// handle the installation of this package

		}
	}
}
