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
    EVT_BUTTON(XRCID("IDWUD_OK"), wxWebUpdateDlg::OnDownload)
    EVT_BUTTON(XRCID("IDWUD_BROWSE"), wxWebUpdateDlg::OnBrowse)
    EVT_BUTTON(XRCID("IDWUD_CANCEL"), wxWebUpdateDlg::OnCancel)

	// checkbox
	EVT_CHECKBOX(XRCID("IDWUD_SHOWFILTER"), wxWebUpdateDlg::OnShowFilter)

	// listctrl
	EVT_UPDATE_UI(-1, wxWebUpdateDlg::OnUpdateUI)

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

	while (!TestDestroy()) {

		if (m_strURI.IsEmpty()) {
			//wxLogDebug(wxT("wxWebUpdateThread::Entry - sleeping "));
			wxThread::Sleep(1000);
			continue;
		}

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
		//m_pHandler->AddPendingEvent(updatevent);
		wxPostEvent(m_pHandler, updatevent);
		//return (void*)TRUE;
		m_strURI = wxEmptyString;
		continue;
	}

	m_bSuccess = FALSE;
	//m_pHandler->AddPendingEvent(updatevent);
	wxPostEvent(m_pHandler, updatevent);
	m_strURI = wxEmptyString;
	}

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
						wxDefaultSize, wxLC_REPORT|wxSUNKEN_BORDER|wxCLC_CHECK_WHEN_SELECTING);
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

	m_thread = new wxWebUpdateThread();
	m_thread->m_pHandler = this;



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
	m_thread->m_strURI = m_strURI;
	m_thread->m_strResName = wxT("XML WebUpdate script");
	m_thread->m_strOutput = wxFileName::CreateTempFileName(wxT("webupdate"));
	m_pTimeText->SetLabel(wxWUD_TIMETEXT_PREFIX wxT("60 s"));

	// this special flag makes easier the #OnDownloadComplete handler
	m_bDownloadingScript = TRUE;

	// launch a separate thread for the webupdate script download
	if (m_thread->Create() != wxTHREAD_NO_ERROR ||
		m_thread->Run() != wxTHREAD_NO_ERROR) {
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
	wxLogDebug(str);
	wxMessageBox(str + wxT("\nContact the support team of ") + m_strAppName +
					wxT(" for help."), wxT("Error"), wxOK | wxICON_ERROR);
}

void wxWebUpdateDlg::OnScriptDownload(const wxString &xmluri)
{
	// ok, we can now parse the XML doc
	if (!m_xmlScript.Load(xmluri)) {
		ShowErrorMsg(wxT("Cannot parse the XML update script downloaded as: ") + 
						m_thread->m_strOutput);
		EndModal(wxCANCEL);		// this is a unrecoverable error !
	}
	
	// now load all the packages we need in local cache
	m_arrUpdatedPackages = m_xmlScript.GetAllPackages();
	for (int i=0; i < (int)m_arrUpdatedPackages.GetCount(); i++)
		m_arrUpdatedPackages[i].CacheDownloadSizes();
	RebuildPackageList();

	// handle the case that there are no packages in the listctrl
	if (m_pUpdatesList->GetItemCount() == 0) {

		wxMessageBox(wxT("Could not found any valid package for ") + m_strAppName
					+ wxT("... exiting the update dialog."), wxT("Warning"),
					wxOK | wxICON_EXCLAMATION);
		EndModal(wxCANCEL);
	}
}

void wxWebUpdateDlg::RebuildPackageList()
{
	m_pUpdatesList->DeleteAllItems();

	int idx = 0;		// could go out of synch with 'i' because
						// some packages could not be added to the list....
	for (int i=0; i < (int)m_arrUpdatedPackages.GetCount(); i++, idx++) {

		wxWebUpdatePackage &curr = m_arrUpdatedPackages.Item(i);		
		wxLogDebug(wxT("wxWebUpdateDlg::RebuildPackageList - Adding the '") + 
			curr.GetName() + wxT("' package to the wxListCtrl"));


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

				// we already have the latest version... check if the
				// filter is enabled or disabled
				// (OFD stays for: out of date)
				wxCheckBox *box = XRCCTRL(*this, "IDWUD_SHOWFILTER", wxCheckBox);
				bool onlyOFD = FALSE;
				if (box) onlyOFD = box->GetValue();

				if (onlyOFD) {
					m_pUpdatesList->DeleteItem(idx);
					idx--;
					continue;		// continue with next package
				}

#if wxWU_USE_CHECKEDLISTCTRL
				// at least disable this item...
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
}


// event handlers

void wxWebUpdateDlg::OnDownload(wxCommandEvent &)
{
	// launch the download of the selected packages
	for (int i=0; i<m_pUpdatesList->GetItemCount(); i++) {
		if (m_pUpdatesList->IsChecked(i)) {

			wxWebUpdateDownload dl = m_arrUpdatedPackages[i].GetDownloadPackage();
			wxString name = m_pUpdatesList->GetItemText(i);
			
			m_thread->m_strOutput = m_pDownloadPathTextCtrl->GetValue() + dl.GetFileName();
			m_thread->m_strURI = dl.GetDownloadString();
			m_thread->m_strResName = name + wxT(" package");

			wxLogDebug(wxT("wxWebUpdateDlg::OnDownload - launching download of ") + m_thread->m_strURI);
			wxThreadError err = m_thread->Resume();
			break;
		}
	}

	//EndModal(wxOK);
}

void wxWebUpdateDlg::OnBrowse(wxCommandEvent &)
{
	// get the current value of the the "download path" from the textctrl
	wxString path = m_pDownloadPathTextCtrl->GetValue();
	wxDirDialog dlg(this, wxT("Choose a directory"), 
					path, wxDD_DEFAULT_STYLE | wxDD_NEW_DIR_BUTTON);
	if (dlg.ShowModal() == wxID_OK) {

		m_pDownloadPathTextCtrl->SetValue(dlg.GetPath());
		wxLogDebug(wxT("wxWebUpdateDlg::OnBrowse - New output path is ") + dlg.GetPath());
		
	} else {

		// just don't change nothing
	}
}

void wxWebUpdateDlg::OnCancel(wxCommandEvent &)
{
	m_thread->Delete();

	EndModal(wxCANCEL);
}

void wxWebUpdateDlg::OnShowFilter(wxCommandEvent &)
{
	// hide/show items in the listctrl
	RebuildPackageList();
}

void wxWebUpdateDlg::OnUpdateUI(wxUpdateUIEvent &ev)
{
	if (!m_thread->IsRunning()) {
		m_pGauge->SetValue(0);
		return;
	}

	unsigned long now = GetTickCount();
	static unsigned long begin = GetTickCount();
	m_pGauge->SetValue((now-begin)/1000);

	if (ev.GetId() == wxWUT_NOTIFICATION)
		OnDownloadComplete(ev);
}

void wxWebUpdateDlg::OnDownloadComplete(wxUpdateUIEvent &)
{
	//m_thread->Pause();

	// first of all, we need to know if download was successful
	if (!m_thread->DownloadWasSuccessful()) {

		ShowErrorMsg(wxT("Could not download the ") + m_thread->m_strResName + 
					wxT(" from\n\n") + m_thread->m_strURI + wxT("\n\nURL... "));
		
		wxLogDebug(wxT("wxWebUpdateDlg::OnDownloadComplete - Download status: failed !"));
		m_pDownloadStatusText->SetLabel(wxT("Download status: failed !"));
		m_pTimeText->SetLabel(wxT("No downloads running..."));

		if (m_bDownloadingScript)
			EndModal(wxCANCEL);		// this is a unrecoverable error !

		return;

	} else {

		wxLogDebug(wxT("wxWebUpdateDlg::OnDownloadComplete - Download status: successfully completed"));
		m_pDownloadStatusText->SetLabel(wxT("Download status: successfully completed"));
		m_pTimeText->SetLabel(wxT("No downloads running..."));

		if (m_bDownloadingScript) {

			// handle the XML parsing & control update 
			OnScriptDownload(m_thread->m_strOutput);

		} else {

			// handle the installation of this package
return;
		}
	}

	// remove the temporary files...
	bool donotremove = FALSE;
	if (m_bDownloadingScript || !donotremove) {		// XML webupdate script is always removed
		wxLogDebug(wxT("wxWebUpdateDlg::OnDownloadComplete - ")
			wxT("Removing the downloaded file: ") + m_thread->m_strOutput);
		wxRemoveFile(m_thread->m_strOutput);
	}

	// reset flag
	if (m_bDownloadingScript) m_bDownloadingScript = FALSE;
}
