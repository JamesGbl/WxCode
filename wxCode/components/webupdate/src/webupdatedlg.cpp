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
#ifndef WX_PRECOMP
	#include <wx/log.h>
	#include <wx/textctrl.h>
	#include <wx/checkbox.h>
	#include <wx/msgdlg.h>
	#include <wx/dirdlg.h>
	#include <wx/generic/dirdlgg.h>
	#include <wx/stattext.h>
	#include <wx/gauge.h>
	#include <wx/textctrl.h>
	#include <wx/listctrl.h>
	#include <wx/progdlg.h>
	#include <wx/checkbox.h>
	#include <wx/xrc/xh_all.h>
#endif

// includes
#include "wx/webupdatedlg.h"
#include "wx/md5.h"
#include <wx/wfstream.h>
#include <wx/xrc/xmlres.h>
#include <wx/image.h>


// wxWidgets RTTI
IMPLEMENT_CLASS(wxWebUpdateLocalPackage, wxObject)
IMPLEMENT_CLASS(wxWebUpdateAdvPanel, wxPanel)
IMPLEMENT_CLASS(wxWebUpdateDlg, wxDialog)

BEGIN_EVENT_TABLE(wxWebUpdateDlg, wxDialog)

	// buttons
    EVT_BUTTON(XRCID("IDWUD_OK"), wxWebUpdateDlg::OnDownload)
    EVT_BUTTON(XRCID("IDWUD_CANCEL"), wxWebUpdateDlg::OnCancel)
    EVT_BUTTON(XRCID("IDWUD_SHOWHIDEADV"), wxWebUpdateDlg::OnShowHideAdv)

	// checkbox
	EVT_CHECKBOX(XRCID("IDWUD_SHOWFILTER"), wxWebUpdateDlg::OnShowFilter)

	// listctrl
	EVT_UPDATE_UI(-1, wxWebUpdateDlg::OnUpdateUI)

	// download thread
	EVT_COMMAND(-1, wxDT_DOWNLOAD_COMPLETE, wxWebUpdateDlg::OnDownloadComplete)

	EVT_IDLE(wxWebUpdateDlg::OnIdle)

END_EVENT_TABLE()

BEGIN_EVENT_TABLE(wxWebUpdateAdvPanel, wxPanel)

	// buttons
    EVT_BUTTON(XRCID("IDWUD_BROWSE"), wxWebUpdateAdvPanel::OnBrowse)

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
// wxWEBUPDATEDLG
// ---------------------

void wxWebUpdateDlg::InitWidgetsFromXRC()
{
	// build the dialog
	// ----------------

	// and build our dialog window
	wxASSERT_MSG(wxXmlResource::Get()->LoadDialog(this, GetParent(), wxT("wxWebUpdateDlg")),
			wxT("Error while building wxWebUpdateDlg; check you've loaded webupdatedlg.xrc !"));

    // Make an instance of our new custom class.
#if wxWU_USE_CHECKEDLISTCTRL
    m_pUpdatesList = new wxCheckedListCtrl(this, -1, wxDefaultPosition,
						wxDefaultSize, wxLC_REPORT|wxSUNKEN_BORDER|wxCLC_CHECK_WHEN_SELECTING);
#else
    m_pUpdatesList = new wxListCtrl(this, -1, wxDefaultPosition,
						wxDefaultSize, wxLC_REPORT|wxSUNKEN_BORDER);
#endif

	// create our "Advanced options" panel
	m_pAdvPanel = new wxWebUpdateAdvPanel(this);


    // attach our classes in the XRC placeholders
	wxXmlResource::Get()->AttachUnknownControl(wxT("IDWUD_LISTCTRL"),
                                                m_pUpdatesList);
    
	wxXmlResource::Get()->AttachUnknownControl(wxT("IDWUD_ADVPANEL"),
                                                m_pAdvPanel);


	// init control pointers
	// ---------------------

	m_pAppNameText = XRCCTRL(*this, "IDWUD_APPNAME_TEXT", wxStaticText);
	m_pSpeedText = XRCCTRL(*this, "IDWUD_PROGRESS_TEXT", wxStaticText);
	m_pTimeText = XRCCTRL(*this, "IDWUD_TIME_TEXT", wxStaticText);
	m_pGauge = XRCCTRL(*this, "IDWUD_GAUGE", wxGauge);
	m_pDescription = XRCCTRL(*this, "IDWUD_DESCRIPTION", wxTextCtrl);

	m_pOkBtn = XRCCTRL(*this,"IDWUD_OK", wxButton);
	m_pCancelBtn = XRCCTRL(*this,"IDWUD_CANCEL", wxButton);
	m_pShowHideAdvBtn = XRCCTRL(*this, "IDWUD_SHOWHIDEADV", wxButton);



	// init control data
	// -----------------

	m_pAppNameText->SetLabel(m_strAppName);

	// init the list control with the column names
	// (items will be inserted as soon as we load the webupdate script)the user-supplied wxWebUpdateLocalPackages
	m_pUpdatesList->InsertColumn(0, wxT("Package name"));
	m_pUpdatesList->InsertColumn(1, wxT("Latest version"));
	m_pUpdatesList->InsertColumn(2, wxT("Local version"));
	m_pUpdatesList->InsertColumn(3, wxT("Size"));
	m_pUpdatesList->InsertColumn(4, wxT("Importance"));
	m_pUpdatesList->InsertColumn(5, wxT("Requires"));
	m_pUpdatesList->SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
	m_pUpdatesList->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
	m_pUpdatesList->SetColumnWidth(2, wxLIST_AUTOSIZE_USEHEADER);	
	m_pUpdatesList->SetColumnWidth(4, wxLIST_AUTOSIZE_USEHEADER);
	m_pUpdatesList->SetColumnWidth(5, wxLIST_AUTOSIZE_USEHEADER);

	// size is the smallest column but it usually needs some space...
	int colwidth = m_pUpdatesList->GetColumnWidth(0) +
					m_pUpdatesList->GetColumnWidth(1) +
					m_pUpdatesList->GetColumnWidth(2) +
					m_pUpdatesList->GetColumnWidth(4) +
					m_pUpdatesList->GetColumnWidth(5);
	int size = m_pUpdatesList->GetClientSize().GetWidth()-colwidth;	
	m_pUpdatesList->SetColumnWidth(3, size > 50 ? size : 50);



	// init other stuff
	// ----------------

	m_thread = new wxDownloadThread();
	m_thread->m_pHandler = this;

	wxCommandEvent fake;
	OnShowHideAdv(fake);



	// relayout
	// --------

	// we have changed the appname statictext control so maybe we need
	// to expand our dialog... force a layout recalculation	
	GetSizer()->Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
}

int wxWebUpdateDlg::ShowModal()
{
	// as soon as the wxDownloadThread has completed its work we'll receive
	// a notification through the wxDT_NOTIFICATION events
	m_pSpeedText->SetLabel(wxT("Downloading update list..."));

	// proceed with standard processing
	return wxDialog::ShowModal();
}

void wxWebUpdateDlg::OnIdle(wxIdleEvent &)
{
	if (m_xmlScript.IsOk() || m_thread->IsRunning())
		return;

	// set our thread helper as a downloader for the XML update scriptf
	m_thread->m_strURI = m_strURI;
	m_thread->m_strResName = wxT("XML WebUpdate script");
	m_thread->m_strOutput = wxFileName::CreateTempFileName(wxT("webupdate"));
	//m_pTimeText->SetLabel(wxWUD_TIMETEXT_PREFIX wxT("60 s"));

	// this special flag makes easier the #OnDownloadComplete handler
	m_bDownloadingScript = TRUE;

	// launch a separate thread for the webupdate script download
	if (m_thread->Create() != wxTHREAD_NO_ERROR ||
		m_thread->Run() != wxTHREAD_NO_ERROR) {
		wxMessageBox(wxString(wxT("Cannot download the script file from\n")) + 
					m_strURI + wxT("\nbecause of the low resources..."), 
					wxT("Error"), wxOK | wxICON_ERROR);
		//return wxCANCEL;
		AbortDialog();
	}

	//UpdateWindowUI();
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
		AbortDialog();		// this is a unrecoverable error !
		return;
	}
	
	// now load all the packages we need in local cache
	m_arrUpdatedPackages = m_xmlScript.GetAllPackages();
	for (int i=0; i < (int)m_arrUpdatedPackages.GetCount(); i++)
		m_arrUpdatedPackages[i].CacheDownloadSizes();
	
	// is everything up to date ?
	bool allupdated = TRUE;
	for (int j=0; j < (int)m_arrUpdatedPackages.GetCount(); j++) {

		// get remote & local package info
		wxWebUpdatePackage &curr = m_arrUpdatedPackages.Item(j);		
		const wxWebUpdateLocalPackage *local = GetLocalPackage(curr.GetName());

		// do the version check
		if (!local || curr.Check(local->m_version) == wxWUCF_OUTOFDATE) {
			allupdated = FALSE;
			break;		// not all packages are uptodate
		}
	}

	if (allupdated) {

		// show to the user the "update not available" message
		wxString defaultmsg = wxT("You have the latest version of all packages of ") +
							m_strAppName + wxT("... exiting the update dialog.");
		wxString usermsg = m_xmlScript.GetUpdateNotAvailableMsg();
		wxMessageBox(usermsg.IsEmpty() ? defaultmsg : usermsg, 
					m_strAppName, wxOK | wxICON_INFORMATION);

		AbortDialog();
		return;	
	} else {

		// some updates are available... show the message only if the webupdate
		// script contains an explicit user-customized message for this evenience
		wxString usermsg = m_xmlScript.GetUpdateAvailableMsg();
		if (!usermsg.IsEmpty())
			wxMessageBox(usermsg, m_strAppName, wxOK | wxICON_INFORMATION);
	}

	// what if we could not found any valid package in the webupdate script ?
	RebuildPackageList();
	if (m_pUpdatesList->GetItemCount() == 0) {

		wxMessageBox(wxT("Could not found any valid package for ") + m_strAppName
					+ wxT(" in the WebUpdate script. Exiting the update dialog."), 
					wxT("Warning"), wxOK | wxICON_INFORMATION);
		AbortDialog();
		return;
	}
}

const wxWebUpdateLocalPackage *wxWebUpdateDlg::GetLocalPackage(const wxString &name) const
{
	const wxWebUpdateLocalPackage *local = NULL;
	for (int j=0; j < m_nLocalPackages; j++)
		if (m_pLocalPackages[j].m_strName == name)
			local = &m_pLocalPackages[j];

	return local;
}

wxWebUpdatePackage &wxWebUpdateDlg::GetRemotePackage(const wxString &name)
{
	for (int j=0; j<(int)m_arrUpdatedPackages.GetCount(); j++)
		if (m_arrUpdatedPackages[j].GetName() == name)
			return m_arrUpdatedPackages[j];
	return wxEmptyWebUpdatePackage;
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
		const wxWebUpdateLocalPackage *local = GetLocalPackage(curr.GetName());

		// did we find a local matching package ?
		bool tocheck = FALSE;
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
				li.SetTextColour(*wxRED);
				m_pUpdatesList->SetItem(li);
				tocheck = TRUE;

			} else if (f == wxWUCF_UPDATED) {

				// we already have the latest version... check if the
				// filter is enabled or disabled
				// (OOD stays for: out of date)
				wxCheckBox *box = XRCCTRL(*this, "IDWUD_SHOWFILTER", wxCheckBox);
				bool onlyOOD = FALSE;
				if (box) onlyOOD = box->GetValue();

				if (onlyOOD) {
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
			tocheck = TRUE;
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
#if wxWU_USE_CHECKEDLISTCTRL
			m_pUpdatesList->Check(idx, tocheck);
#endif
			break;
		case wxWUPI_NORMAL:
			m_pUpdatesList->SetItem(idx, 4, wxT("normal"));
#if wxWU_USE_CHECKEDLISTCTRL
			m_pUpdatesList->Check(idx, tocheck);
#endif
			break;
		case wxWUPI_LOW:
			m_pUpdatesList->SetItem(idx, 4, wxT("low"));
#if wxWU_USE_CHECKEDLISTCTRL
			m_pUpdatesList->Check(idx, FALSE);
#endif
			break;
		default:
			wxASSERT_MSG(0, wxT("Invalid package !"));
		}


		// set the properties for the sixth column (REQUIRES)
		// ----------------------------------------------------

		wxString str(curr.GetPrerequisites());
		m_pUpdatesList->SetItem(idx, 5, str.IsEmpty() ? wxT("none") : str);
	}
}

void wxWebUpdateDlg::AbortDialog()
{
	if (m_thread->IsPaused())
		m_thread->Run();		// we need the thread running if we want to delete it !
	if (m_thread->IsRunning())
		m_thread->Delete();
	EndModal(wxCANCEL);
}

wxWindow *wxWebUpdateDlg::ShowHideChild(const wxString &name)
{
	wxWindow *p = FindWindowByName(name, this);
	if (!p) return NULL;

	// now, modify the sizer which contain that window
	wxSizer *sizer = p->GetContainingSizer();
	wxSizerItem *item = sizer->GetItem(p);
	if (!item) return NULL;

	// invert the show state
    if (p->IsShown())
		item->Show(FALSE);
    else
		item->Show(TRUE);
	return p;
}



// event handlers

void wxWebUpdateDlg::OnDownload(wxCommandEvent &)
{
	int nDownloads = 0;
	wxASSERT_MSG(!m_thread->IsDownloading(), wxT("The wxWUD_OK button should be disabled !"));
	
	// launch the download of the selected packages
	for (int i=0; i<m_pUpdatesList->GetItemCount(); i++) {
		if (m_pUpdatesList->IsChecked(i)) {

			// find this name in our array of packages; this is required
			// since we can't trust the current index "i" since some view
			// filter could have been applied on the listctrl and thus 
			// there maybe some hidden packages which make "i" out of synch
			// with the i-th package contained into m_arrUpdatedPackages
			wxString name = m_pUpdatesList->GetItemText(i);
			wxWebUpdatePackage &pkg = GetRemotePackage(name);
			wxWebUpdateDownload &dl = pkg.GetDownloadPackage();
			
			// init thread variables
			m_thread->m_strOutput = m_pAdvPanel->GetDownloadPath() + dl.GetFileName();
			m_thread->m_strURI = dl.GetDownloadString();
			m_thread->m_strMD5 = dl.GetMD5Checksum();
			m_thread->m_strResName = name + wxT(" package");

			// advanced options
			m_thread->m_strHTTPAuthUsername = m_pAdvPanel->GetUsername();
			m_thread->m_strHTTPAuthPassword = m_pAdvPanel->GetPassword();
			m_thread->m_strProxyPort = m_pAdvPanel->GetProxyPortNumber();
			m_thread->m_strProxyHostname = m_pAdvPanel->GetProxyHostName();

			// reset the gauge GUI
			m_pGauge->SetRange(dl.GetDownloadSize());
			
			// launch the download
			wxLogDebug(wxT("wxWebUpdateDlg::OnDownload - launching download of ") + m_thread->m_strURI);

			m_thread->BeginNewDownload();
			nDownloads++;
			break;
		}
	}

	wxASSERT_MSG(nDownloads > 0, 
		wxT("The wxWUD_OK button should be enabled only when one or more packages are checked"));
}

void wxWebUpdateDlg::OnCancel(wxCommandEvent &)
{
	if (m_thread->IsDownloading()) {

		// we are now labeled as wxWUD_CANCEL_DOWNLOAD...
		// thus we only stop the download
		m_bUserAborted = TRUE;
		m_thread->AbortDownload();		
		return;
	}

	AbortDialog();
}

void wxWebUpdateDlg::OnShowFilter(wxCommandEvent &)
{
	// hide/show items in the listctrl
	RebuildPackageList();
}

void wxWebUpdateDlg::OnShowHideAdv(wxCommandEvent &)
{
	// since our wxWebUpdateAdvPanel is built using a <unknown> tag
	// we need to show/hide both the real panel and its container.
	wxWindow *p = ShowHideChild(wxT("IDWUD_ADVPANEL"));
	wxASSERT_MSG(p, wxT("Invalid XRC file !"));
	p = ShowHideChild(wxT("IDWUD_ADVPANEL_container"));
	wxASSERT_MSG(p, wxT("Invalid XRC file !"));

	// resize this dialog to reflect the change
	GetSizer()->Layout();
	GetSizer()->SetSizeHints(this);
	this->SetSize(-1, -1, 0, 0);
	this->CenterOnScreen();

	if (!p->IsShown())
		m_pShowHideAdvBtn->SetLabel(wxWUD_SHOWHIDEADV_SHOW);
	else
		m_pShowHideAdvBtn->SetLabel(wxWUD_SHOWHIDEADV_HIDE);
}

void wxWebUpdateDlg::OnDownloadComplete(wxCommandEvent &)
{
	bool forceremove = FALSE;

	// first of all, we need to know if download was successful
	if (!m_thread->DownloadWasSuccessful()) {

		if (m_bDownloadingScript)
			forceremove = TRUE;		// always remove the script

		if (m_bUserAborted)
			wxMessageBox(wxT("Download aborted..."), 
						wxT("Warning"), wxOK | wxICON_EXCLAMATION);
		else
			ShowErrorMsg(wxT("Could not download the ") + m_thread->m_strResName + 
					wxT(" from\n\n") + m_thread->m_strURI + wxT("\n\nURL... "));
		
		wxLogDebug(wxT("wxWebUpdateDlg::OnDownloadComplete - Download status: failed !"));
		if (m_bDownloadingScript)
			AbortDialog();		// this is a unrecoverable error !

		m_bUserAborted = FALSE;		// reset flag
		return;

	} else {

		wxLogDebug(wxT("wxWebUpdateDlg::OnDownloadComplete - Download status: successfully completed"));
		if (m_bDownloadingScript) {

			// handle the XML parsing & control update 
			OnScriptDownload(m_thread->m_strOutput);
			forceremove = TRUE;			// always remove the script

		} else {

			if (m_thread->m_strMD5.IsEmpty()) {

				// handle the installation of this package
				return;
			}

			// verify that the download was successful
			m_pSpeedText->SetLabel(wxWUD_SPEEDTEXT_PREFIX wxT("computing file hash"));
			wxString localmd5 = wxMD5::GetFileMD5(m_thread->m_strOutput);
			if (localmd5 != m_thread->m_strMD5) {

				wxMessageBox(wxT("The downloaded file \"") + m_thread->m_strOutput + 
						wxT("\"\nis corrupted. MD5 checksum is:\n\n\t") + localmd5 +
						wxT("\n\ninstead of:\n\n\t") + m_thread->m_strMD5 +
						wxT("\n\nPlease retry the download."), 
						wxT("Error"), wxOK | wxICON_ERROR);
				//forceremove = TRUE;		// remove this corrupted package

			} else {

				// handle the installation of this package
				return;
			}
		}
	}

	// remove the temporary files...
	bool donotremove = m_pAdvPanel->RemoveFiles();
	if (forceremove || !donotremove) {		// XML webupdate script is always removed
		wxLogDebug(wxT("wxWebUpdateDlg::OnDownloadComplete - ")
			wxT("Removing the downloaded file: ") + m_thread->m_strOutput);
		wxRemoveFile(m_thread->m_strOutput);
	}

	// reset flag
	if (m_bDownloadingScript) m_bDownloadingScript = FALSE;
}


void wxWebUpdateDlg::OnUpdateUI(wxUpdateUIEvent &)
{
	// help us to avoid useless calls to SetLabel() function which
	// cause flickering
	static bool bLabelRunningMode = FALSE;
	static wxDateTime lastupdate = wxDateTime::UNow();

	// change the description label eventually
	if (m_pUpdatesList->GetSelectedItemCount() > 0) {
	
		for (int i=0; i<m_pUpdatesList->GetItemCount(); i++) {
			if (m_pUpdatesList->GetItemState(i, wxLIST_STATE_SELECTED)) {

				// show the description of the first selected item
				wxString name = m_pUpdatesList->GetItemText(i);
				wxWebUpdatePackage &pkg = GetRemotePackage(name);
				m_pDescription->SetValue(pkg.GetDescription());
				break;
			}
		}
	}

	if (!m_thread->IsDownloading()) {

		// reset our gauge control
		m_pGauge->SetValue(0);

		// re-enable what we disabled when we ran the thread
		if (!bLabelRunningMode) {			
			m_pCancelBtn->SetLabel(wxWUD_CANCEL_DEFAULT_LABEL);
			//m_pUpdatesList->EnableAll();
			bLabelRunningMode = TRUE;

			// update our meters
			if (m_thread->DownloadWasSuccessful())
				m_pSpeedText->SetLabel(wxWUD_SPEEDTEXT_PREFIX wxT("successfully completed"));
			else
				m_pSpeedText->SetLabel(wxWUD_SPEEDTEXT_PREFIX wxT("failed !"));
			m_pTimeText->SetLabel(wxT("No downloads running..."));
		}

		// are there checked items in the package listctrl ?
		for (int i=0; i<m_pUpdatesList->GetItemCount(); i++)
			if (m_pUpdatesList->IsChecked(i))
				break;		// found a checked item !
		
		if (i<m_pUpdatesList->GetItemCount())
			m_pOkBtn->Enable();		// yes, there are
		else
			m_pOkBtn->Disable();		// no, there aren't

	} else {

		// need to change labels ?
		if (bLabelRunningMode) {
			m_pOkBtn->Disable();
			m_pCancelBtn->SetLabel(wxWUD_CANCEL_DOWNLOAD);
			//m_pUpdatesList->EnableAll(FALSE);
			bLabelRunningMode = FALSE;
		}

		wxDateTime current = wxDateTime::UNow();
		wxTimeSpan diff = current - lastupdate;
		if (diff.GetMilliseconds().ToLong() > 500) {

			//wxLogDebug(wxT("wxWebUpdateDlg::OnUpdateUI - updating meters"));
			lastupdate = current;

			// update our gauge control
			long value = m_thread->GetCurrDownloadedBytes();	
			m_pGauge->SetValue(value > 0 ? value : 0);

			// update speed meter
			m_pSpeedText->SetLabel(wxWUD_SPEEDTEXT_PREFIX wxT("downloading \"") + 
								m_thread->m_strResName + wxT("\" at ") + 
								m_thread->GetDownloadSpeed());

			// update time meter
			m_pTimeText->SetLabel(wxWUD_TIMETEXT_PREFIX + 
								m_thread->GetRemainingTime());
		}
	}
}



// ---------------------
// wxWEBUPDATEADVPANEL
// ---------------------

void wxWebUpdateAdvPanel::InitWidgetsFromXRC()
{
	// build the dialog
	// ----------------

	// and build our dialog window
	wxASSERT_MSG(wxXmlResource::Get()->LoadPanel(this, GetParent(), wxT("wxWebUpdateAdvPanel")),
			wxT("Error while creating the wxWebUpdateAdvPanel - be sure you loaded webupdatedlg.xrc !"));


	// init control pointers
	// ---------------------

	m_pDownloadPathTextCtrl = XRCCTRL(*this, "IDWUAP_DOWNLOAD_PATH", wxTextCtrl);
	m_pProxyHostname = XRCCTRL(*this, "IDWUAP_HOSTNAME", wxTextCtrl);
	m_pProxyPortNumber = XRCCTRL(*this, "IDWUAP_PORTNUMBER", wxTextCtrl);
	m_pUsername = XRCCTRL(*this, "IDWUAP_USERNAME", wxTextCtrl);
	m_pPassword = XRCCTRL(*this, "IDWUAP_PASSWORD", wxTextCtrl);
	m_pRemoveFiles = XRCCTRL(*this, "IDWUAP_REMOVE", wxCheckBox);



	// init control data
	// -----------------
	
	// get the temporary folder where we put by default the updates
	wxFileName str(wxFileName::CreateTempFileName(wxEmptyString, NULL));
	str.SetFullName(wxEmptyString);		// remove the filename and keep only the path
	m_pDownloadPathTextCtrl->SetValue(str.GetLongPath());



	// relayout
	// --------

	// we have changed the appname statictext control so maybe we need
	// to expand our dialog... force a layout recalculation
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
}

void wxWebUpdateAdvPanel::OnBrowse(wxCommandEvent &)
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
