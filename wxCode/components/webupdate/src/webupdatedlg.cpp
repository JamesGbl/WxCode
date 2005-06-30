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

	// checkbox
	EVT_CHECKBOX(XRCID("IDWUD_SHOWFILTER"), wxWebUpdateDlg::OnShowFilter)

	// listctrl

	// download thread
	EVT_UPDATE_UI(wxWUT_NOTIFICATION, wxWebUpdateDlg::OnDownloadComplete)

END_EVENT_TABLE()





// ---------------------
// GLOBALS
// ---------------------

wxString wxGetSizeStr(unsigned long bytesize)
{
	wxString sz;
	if (bytesize < 1024) 
		sz = wxString::Format(wxT("%d B"), bytesize);
	else if (bytesize < 1024*1024) 
		sz = wxString::Format(wxT("%d kB"), bytesize/1024);
	else if (bytesize < 1024*1024*1024) 
		sz = wxString::Format(wxT("%d MB"), bytesize/(1024*1024));
	else 
		// petabytes are not handled because they require a division
		// for a number of the order 2^40 which exceed the 32 bits
		// of most of the today machines...
		sz = wxString::Format(wxT("%d TB"), bytesize/(1024*1024*1024));

	return sz;
}





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

    // Make an instance of our new custom class.
#if wxWU_USE_CHECKEDLISTCTRL
    m_pUpdatesList = new wxCheckedListCtrl(this, -1, wxDefaultPosition,
						wxDefaultSize, wxLC_REPORT|wxSUNKEN_BORDER);
#else
    m_pUpdatesList = new wxListCtrl(this, -1, wxDefaultPosition,
						wxDefaultSize, wxLC_REPORT|wxSUNKEN_BORDER);
#endif

    // "custom_control_placeholder" is the name of the "unknown" tag in the
    // custctrl.xrc XRC file.
    wxXmlResource::Get()->AttachUnknownControl(wxT("IDWUD_LISTCTRL"),
                                                m_pUpdatesList);



	// init control pointers
	// ---------------------

	m_pAppNameText = XRCCTRL(*this,"IDWUD_APPNAME_TEXT",wxStaticText);
	m_pDownloadStatusText = XRCCTRL(*this,"IDWUD_PROGRESS_TEXT",wxStaticText);
	m_pTimeText = XRCCTRL(*this,"IDWUD_TIME_TEXT",wxStaticText);
	//m_pUpdatesList = XRCCTRL(*this,"IDWUD_LISTCTRL",wxListCtrl);
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
	m_pUpdatesList->InsertColumn(3, wxT("Size"));
	m_pUpdatesList->InsertColumn(4, wxT("Importance"));
	m_pUpdatesList->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
	m_pUpdatesList->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
	m_pUpdatesList->SetColumnWidth(2, wxLIST_AUTOSIZE_USEHEADER);	
	m_pUpdatesList->SetColumnWidth(4, wxLIST_AUTOSIZE_USEHEADER);

	// size is the smallest column but it usually needs some space...
	int colwidth = m_pUpdatesList->GetColumnWidth(0) +
					m_pUpdatesList->GetColumnWidth(1) +
					m_pUpdatesList->GetColumnWidth(2) +
					m_pUpdatesList->GetColumnWidth(4);
	int size = m_pUpdatesList->GetClientSize().GetWidth()-colwidth;	
	m_pUpdatesList->SetColumnWidth(3, size > 50 ? size : 50);



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
	m_arrUpdatedPackages = m_xmlScript.GetAllPackages();	

	int idx = 0;		// could go out of synch with 'i' because
						// some packages could not be added to the list....
	for (int i=0; i < (int)m_arrUpdatedPackages.GetCount(); i++, idx++) {

		wxWebUpdatePackage &curr = m_arrUpdatedPackages.Item(i);		

		// set the properties for the first column (NAME)
		// ----------------------------------------------
		m_pUpdatesList->InsertItem(idx, curr.GetName());
		

		// set the properties for the second column (LATEST VERSION)
		// ---------------------------------------------------------
		m_pUpdatesList->SetItem(idx, 1, curr.GetLatestVersion());


		// set the properties for the third column (LOCAL VERSION)
		// here we need some further work to find if this package
		// is already installed...
		// -------------------------------------------------------
		const wxWebUpdateLocalPackage *local = NULL;
		for (int j=0; j < m_nLocalPackages; j++)
			if (m_pLocalPackages[j].m_strName == curr.GetName())
				local = &m_pLocalPackages[j];

		// did we find a local matching package ?
		if (local) {
			m_pUpdatesList->SetItem(idx, 2, local->m_version);

			// compare versions
			wxWebUpdateCheckFlag f = curr.Check(local->m_version);
			if (f == wxWUCF_OUTOFDATE) {

				// build a bold font
				wxFont font(m_pUpdatesList->GetFont());
				font.SetWeight(wxFONTWEIGHT_BOLD);

				// and set it for this item
				wxListItem li;
				li.SetId(idx);
				li.SetFont(font);
				li.SetBackgroundColour(*wxWHITE);
				li.SetTextColour(*wxBLACK);
				m_pUpdatesList->SetItem(li);

			} else if (f == wxWUCF_UPDATED) {

				// we already have the latest version...
#if wxWU_USE_CHECKEDLISTCTRL
				m_pUpdatesList->Enable(idx, FALSE);
#endif
			} else if (f == wxWUCF_FAILED) {

				// error !
				m_pUpdatesList->DeleteItem(idx);
				idx--;

				continue;		// continue with next package
			}

		} else {

			// a matching local package does not exist...
			m_pUpdatesList->SetItem(idx, 2, wxT("not installed"));
		}


		// set the properties for the fourth column (SIZE)
		// -----------------------------------------------

		unsigned long bytesize = curr.GetDownloadPackage().GetDownloadSize();
		m_pUpdatesList->SetItem(idx, 3, wxGetSizeStr(bytesize));



		// set the properties for the fifth column (IMPORTANCE)
		// ----------------------------------------------------
		switch (curr.GetImportance()) {
		case wxWUPI_HIGH:
			m_pUpdatesList->SetItem(idx, 4, wxT("high!"));
			break;
		case wxWUPI_NORMAL:
			m_pUpdatesList->SetItem(idx, 4, wxT("normal"));
			break;
		case wxWUPI_LOW:
			m_pUpdatesList->SetItem(idx, 4, wxT("low"));
			break;
		default:
			wxASSERT_MSG(0, wxT("Invalid package !"));
		}
	}

	// handle the case that there are no packages in the listctrl
	if (m_pUpdatesList->GetItemCount() == 0) {

		wxMessageBox(wxT("Could not found any valid package for ") + m_strAppName
					+ wxT("... exiting the update dialog."), wxT("Warning"),
					wxOK | wxICON_EXCLAMATION);
		EndModal(wxCANCEL);
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

void wxWebUpdateDlg::OnShowFilter(wxCommandEvent &)
{
	// OFD: out of date
	wxCheckBox *box = XRCCTRL(*this, "IDWUD_SHOWFILTER", wxCheckBox);
	bool onlyOFD = FALSE;
	if (box) onlyOFD = box->GetValue();

	// hide/show items in the listctrl

}

void wxWebUpdateDlg::OnDownloadComplete(wxUpdateUIEvent &)
{
	// first of all, we need to know if download was successful
	if (!m_thread.DownloadWasSuccessful()) {

		ShowErrorMsg(wxT("Could not download the ") + m_thread.m_strResName + 
					wxT(" from\n\n") + m_thread.m_strURI + wxT("\n\nURL... "));
		
		m_pDownloadStatusText->SetLabel(wxT("Download status: failed !"));
		m_pTimeText->SetLabel(wxT("No downloads running..."));

		if (m_bDownloadingScript)
			EndModal(wxCANCEL);		// this is a unrecoverable error !

		return;

	} else {

		m_pDownloadStatusText->SetLabel(wxT("Download status: successfully completed"));
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
