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
#include "wx/installer.h"
#include <wx/wfstream.h>
#include <wx/xrc/xmlres.h>
#include <wx/image.h>
#include <wx/dialup.h>


// wxWidgets RTTI
IMPLEMENT_CLASS(wxWebUpdateLocalPackage, wxObject)
IMPLEMENT_CLASS(wxWebUpdateAdvPanel, wxPanel)
IMPLEMENT_CLASS(wxWebUpdateDlg, wxDialog)
IMPLEMENT_CLASS(wxWebUpdateListCtrl, wxWUDLC_BASECLASS)

BEGIN_EVENT_TABLE(wxWebUpdateListCtrl, wxWUDLC_BASECLASS)


END_EVENT_TABLE()

BEGIN_EVENT_TABLE(wxWebUpdateDlg, wxDialog)

	// buttons
    EVT_BUTTON(XRCID("IDWUD_OK"), wxWebUpdateDlg::OnDownload)
    EVT_BUTTON(XRCID("IDWUD_CANCEL"), wxWebUpdateDlg::OnCancel)	
    EVT_BUTTON(XRCID("IDWUD_SHOWHIDEADV"), wxWebUpdateDlg::OnShowHideAdv)

	// we need to intercept also the clicks on the close box in the system menu
	EVT_BUTTON(wxID_CANCEL, wxWebUpdateDlg::OnCancel)

	// checkbox
	EVT_CHECKBOX(XRCID("IDWUD_SHOWFILTER"), wxWebUpdateDlg::OnShowFilter)

	// global UI updates
	EVT_UPDATE_UI(-1, wxWebUpdateDlg::OnUpdateUI)

	// miscellaneous
	EVT_COMMAND(-1, wxDT_DOWNLOAD_COMPLETE, wxWebUpdateDlg::OnDownloadComplete)
	EVT_TEXT_URL(XRCID("IDWUD_DESCRIPTION"), wxWebUpdateDlg::OnTextURL)
	EVT_IDLE(wxWebUpdateDlg::OnIdle)

END_EVENT_TABLE()

BEGIN_EVENT_TABLE(wxWebUpdateAdvPanel, wxPanel)

	// buttons
    EVT_BUTTON(XRCID("IDWUD_BROWSE"), wxWebUpdateAdvPanel::OnBrowse)

END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxWUD_INIT);
DEFINE_EVENT_TYPE(wxWUD_DESTROY);




// ---------------------
// GLOBALS
// ---------------------

wxString wxGetSizeStr(unsigned long bytesize)
{
	wxString sz;
	if (bytesize == 0)
		sz = wxT("NA");		// not available
	else if (bytesize < 1024) 
		sz = wxString::Format(wxT("%d B"), bytesize);
	else if (bytesize < 1024*1024) 
		sz = wxString::Format(wxT("%.2f kB"), ((float)bytesize/1024.));
	else if (bytesize < 1024*1024*1024) 
		sz = wxString::Format(wxT("%.2f MB"), ((float)bytesize/(1024.*1024.)));
	else 
		// petabytes are not handled because they require a division
		// for a number of the order 2^40 which exceed the 32 bits
		// of most of the today machines...
		sz = wxString::Format(wxT("%.3f TB"), ((float)bytesize/(1024.*1024.*1024.)));

	return sz;
}




// ---------------------
// wxWEBUPDATELISTCTRL
// ---------------------

void wxWebUpdateListCtrl::RebuildPackageList()
{
	// remove old contents
	DeleteAllItems();

	int idx = 0;		// could go out of synch with 'i' because
						// some packages could not be added to the list....
	for (int i=0; i < (int)m_arrUpdatedPackages.GetCount(); i++, idx++) {

		wxWebUpdatePackage &curr = m_arrUpdatedPackages.Item(i);		
		wxLogDebug(wxT("wxWebUpdateDlg::RebuildPackageList - Adding the '") + 
			curr.GetName() + wxT("' package to the wxListCtrl"));


		// set the properties for the first column (NAME)
		// ----------------------------------------------
		InsertItem(idx, curr.GetName());
		

		// set the properties for the second column (LATEST VERSION)
		// ---------------------------------------------------------
		SetItem(idx, 1, curr.GetLatestVersion());


		// set the properties for the third column (LOCAL VERSION)
		// here we need some further work to find if this package
		// is already installed...
		// -------------------------------------------------------
		const wxWebUpdateLocalPackage *local = GetLocalPackage(curr.GetName());

		// did we find a local matching package ?
		bool tocheck = FALSE;
		if (local) {
			SetItem(idx, 2, local->m_version);

			// compare versions
			wxWebUpdateCheckFlag f = curr.Check(local->m_version);
			if (f == wxWUCF_OUTOFDATE) {

				// build a bold font
				wxFont font(GetFont());
				font.SetWeight(wxFONTWEIGHT_BOLD);

				// and set it for this item
				wxListItem li;
				li.SetId(idx);
				li.SetFont(font);
				li.SetBackgroundColour(*wxWHITE);
				li.SetTextColour(*wxRED);
				SetItem(li);
				tocheck = TRUE;

			} else if (f == wxWUCF_UPDATED) {

				// we already have the latest version... check if the
				// filter is enabled or disabled
				// (OOD stays for: out of date)
				wxCheckBox *box = XRCCTRL(*this, "IDWUD_SHOWFILTER", wxCheckBox);
				bool onlyOOD = FALSE;
				if (box) onlyOOD = box->GetValue();

				if (onlyOOD) {
					DeleteItem(idx);
					idx--;
					continue;		// continue with next package
				}

#if wxWU_USE_CHECKEDLISTCTRL
				// at least disable this item...
				Enable(idx, FALSE);
#endif
			} else if (f == wxWUCF_FAILED) {

				// error !
				DeleteItem(idx);
				idx--;

				continue;		// continue with next package
			}

		} else {

			// a matching local package does not exist...
			SetItem(idx, 2, wxT("not installed"));
			tocheck = TRUE;
		}


		// set the properties for the fourth column (SIZE)
		// -----------------------------------------------

		unsigned long bytesize = curr.GetDownloadPackage().GetDownloadSize();
		SetItem(idx, 3, wxGetSizeStr(bytesize));



		// set the properties for the fifth column (IMPORTANCE)
		// ----------------------------------------------------
		switch (curr.GetImportance()) {
		case wxWUPI_HIGH:
			SetItem(idx, 4, wxT("high!"));
#if wxWU_USE_CHECKEDLISTCTRL
			Check(idx, tocheck);
#endif
			break;
		case wxWUPI_NORMAL:
			SetItem(idx, 4, wxT("normal"));
#if wxWU_USE_CHECKEDLISTCTRL
			Check(idx, tocheck);
#endif
			break;
		case wxWUPI_LOW:
			SetItem(idx, 4, wxT("low"));
#if wxWU_USE_CHECKEDLISTCTRL
			Check(idx, FALSE);
#endif
			break;
		default:
			wxASSERT_MSG(0, wxT("Invalid package !"));
		}


		// set the properties for the sixth column (REQUIRES)
		// ----------------------------------------------------

		wxString str(curr.GetPrerequisites());
		SetItem(idx, 5, str.IsEmpty() ? wxT("none") : str.c_str());
	}

	// select the first item of the list
	if (GetItemCount() > 0)
		SetItemState(0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

const wxWebUpdateLocalPackage *wxWebUpdateListCtrl::GetLocalPackage(const wxString &name) const
{
	const wxWebUpdateLocalPackage *local = NULL;
	for (int j=0; j < (int)m_arrLocalPackages.GetCount(); j++)
		if (m_arrLocalPackages[j].m_strName == name)
			local = &m_arrLocalPackages[j];

	return local;
}

wxWebUpdatePackage &wxWebUpdateListCtrl::GetRemotePackage(const wxString &name)
{
	for (int j=0; j<(int)m_arrUpdatedPackages.GetCount(); j++)
		if (m_arrUpdatedPackages[j].GetName() == name)
			return m_arrUpdatedPackages[j];
	return wxEmptyWebUpdatePackage;
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
    m_pUpdatesList = new wxWebUpdateListCtrl(this, -1, wxDefaultPosition,
						wxDefaultSize, wxLC_REPORT|wxSUNKEN_BORDER|wxCLC_CHECK_WHEN_SELECTING);

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

	m_dThread = new wxDownloadThread();
	m_dThread->m_pHandler = this;
	m_iThread = new wxWebUpdateInstallThread();
	m_iThread->m_pHandler = this;

	wxCommandEvent fake;
	OnShowHideAdv(fake);	// begin with adv options hidden



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
	// are we connected ?
	wxDialUpManager *mng = wxDialUpManager::Create();
	if (mng->IsOk()) {

		if (!mng->IsOnline()) {

			wxMessageBox(wxT("You are not connected to Internet ! Please connect and then retry..."),
						wxT("Error"), wxOK | wxICON_ERROR);
			delete mng;
			return 0;
		}
	}

	delete mng;

	// launch a separate thread for the webupdate script download
	if (m_dThread->Create() != wxTHREAD_NO_ERROR ||
		m_dThread->Run() != wxTHREAD_NO_ERROR) {
		wxMessageBox(wxT("Low resources; cannot run the WebUpdate dialog...\n")
					wxT("Close some applications and then retry."), 
					wxT("Error"), wxOK | wxICON_ERROR);
		AbortDialog();
	}

	// init also our installer thread
	if (m_iThread->Create() != wxTHREAD_NO_ERROR ||
		m_iThread->Run() != wxTHREAD_NO_ERROR) {
		wxMessageBox(wxT("Low resources; cannot run the WebUpdate dialog...\n")
					wxT("Close some applications and then retry."), 
					wxT("Error"), wxOK | wxICON_ERROR);
		AbortDialog();
	}

	// send a notification to the global wxWebUpdater
	wxCommandEvent ce(wxWUD_INIT);
	wxWebUpdater::Get()->AddPendingEvent(ce);

	// proceed with standard processing
	return wxDialog::ShowModal();
}

bool wxWebUpdateDlg::Destroy()
{
	// send a notification to the global wxWebUpdater
	wxCommandEvent ce(wxWUD_DESTROY);
	wxWebUpdater::Get()->AddPendingEvent(ce);

	return wxDialog::Destroy();
}

void wxWebUpdateDlg::OnIdle(wxIdleEvent &)
{
	if (m_iThread->IsRunning() || m_dThread->IsRunning())
		return;
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
						m_dThread->m_strOutput);
		AbortDialog();		// this is a unrecoverable error !
		return;
	}
	
	// now load all the packages we need in local cache
	wxWebUpdatePackageArray arr = m_xmlScript.GetAllPackages();
	for (int i=0; i < (int)arr.GetCount(); i++)
		arr[i].CacheDownloadSizes();
	
	// is everything up to date ?
	bool allupdated = TRUE;
	for (int j=0; j < (int)arr.GetCount(); j++) {

		// get remote & local package info
		wxWebUpdatePackage &curr = arr.Item(j);		
		const wxWebUpdateLocalPackage *local = 
			m_pUpdatesList->GetLocalPackage(curr.GetName());

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
	m_pUpdatesList->SetRemotePackages(arr);
	m_pUpdatesList->RebuildPackageList();
	if (m_pUpdatesList->GetItemCount() == 0) {

		wxMessageBox(wxT("Could not found any valid package for ") + m_strAppName
					+ wxT(" in the WebUpdate script. Exiting the update dialog."), 
					wxT("Warning"), wxOK | wxICON_INFORMATION);
		AbortDialog();
		return;
	}
}

void wxWebUpdateDlg::AbortDialog()
{
	if (m_dThread->IsPaused())
		m_dThread->Run();		// we need the thread running if we want to delete it !
	if (m_dThread->IsRunning())
		m_dThread->Delete();

	if (m_iThread->IsPaused())
		m_iThread->Run();		// we need the thread running if we want to delete it !
	if (m_iThread->IsRunning())
		m_iThread->Delete();

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

void wxWebUpdateDlg::OnTextURL(wxTextUrlEvent& event)
{
    const wxMouseEvent& ev = event.GetMouseEvent();

    // filter out mouse moves, too many of them
    if ( ev.Moving() )
        return;

    long start = event.GetURLStart(),
         end = event.GetURLEnd();

    wxLaunchDefaultBrowser(m_pDescription->GetRange(start, end));
}

void wxWebUpdateDlg::OnDownload(wxCommandEvent &)
{
	int nDownloads = 0;
	wxASSERT_MSG(!m_dThread->IsDownloading(), 
		wxT("The wxWUD_OK button should be disabled !"));
	

	// check if we already downloaded the XML webupdate script
	if (!m_xmlScript.IsOk()) {
		m_dThread->m_strURI = m_strURI;
		m_dThread->m_strResName = wxT("XML WebUpdate script");
		m_dThread->m_strOutput = wxFileName::CreateTempFileName(wxT("webupdate"));
		m_dThread->m_strID = wxWUD_XMLSCRIPT_ID;
		m_dThread->BeginNewDownload();
		return;
	}

	// launch the download of the selected packages
	for (int i=0; i<m_pUpdatesList->GetItemCount(); i++) {
		if (m_pUpdatesList->IsChecked(i)) {

			// find this name in our array of packages; this is required
			// since we can't trust the current index "i" since some view
			// filter could have been applied on the listctrl and thus 
			// there maybe some hidden packages which make "i" out of synch
			// with the i-th package contained into m_arrUpdatedPackages
			wxString name = m_pUpdatesList->GetItemText(i);
			wxWebUpdatePackage &pkg = m_pUpdatesList->GetRemotePackage(name);
			wxWebUpdateDownload &dl = pkg.GetDownloadPackage();
			
			// init thread variables
			m_dThread->m_strOutput = m_pAdvPanel->GetDownloadPath() + dl.GetFileName();
			m_dThread->m_strURI = dl.GetDownloadString();
			m_dThread->m_strMD5 = dl.GetMD5Checksum();
			m_dThread->m_strResName = name + wxT(" package");
			m_dThread->m_strID = name;

			// advanced options
			m_dThread->m_strHTTPAuthUsername = m_pAdvPanel->GetUsername();
			m_dThread->m_strHTTPAuthPassword = m_pAdvPanel->GetPassword();
			m_dThread->m_strProxyPort = m_pAdvPanel->GetProxyPortNumber();
			m_dThread->m_strProxyHostname = m_pAdvPanel->GetProxyHostName();

			// reset the gauge GUI
			m_pGauge->SetValue(0);
			m_pGauge->SetRange(dl.GetDownloadSize());
			
			// launch the download
			wxLogDebug(wxT("wxWebUpdateDlg::OnDownload - launching download of ") + m_dThread->m_strURI);

			m_dThread->BeginNewDownload();
			nDownloads++;
			break;
		}
	}

	wxASSERT_MSG(nDownloads > 0, 
		wxT("The wxWUD_OK button should be enabled only when one or more packages are checked"));
}

void wxWebUpdateDlg::OnCancel(wxCommandEvent &)
{
	if (m_dThread->IsDownloading() 
#ifdef wxDT_USE_MD5
		|| m_dThread->IsComputingMD5()
#endif
		) {

		// we are now labeled as wxWUD_CANCEL_DOWNLOAD...
		// thus we only stop the download
		m_bUserAborted = TRUE;
		m_dThread->AbortDownload();		
		return;

	} else if (m_nStatus == wxWUDS_INSTALLING) {

		// we are now labeled as wxWUD_CANCEL_INSTALLATION...
		// thus we only stop the installation
		// FIXME
		return;
	}

	AbortDialog();
}

void wxWebUpdateDlg::OnShowFilter(wxCommandEvent &)
{
	// hide/show items in the listctrl
	m_pUpdatesList->RebuildPackageList();
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
	bool forceremove = FALSE, downloadingScript = (m_dThread->m_strID == wxWUD_XMLSCRIPT_ID);

	// first of all, we need to know if download was successful
	if (!m_dThread->DownloadWasSuccessful()) {

		if (downloadingScript)
			forceremove = TRUE;		// always remove the script

		if (m_bUserAborted)
			wxMessageBox(wxT("Download aborted..."), 
						wxT("Warning"), wxOK | wxICON_EXCLAMATION);
		else
			ShowErrorMsg(wxT("Could not download the ") + m_dThread->m_strResName + 
					wxT(" from\n\n") + m_dThread->m_strURI + wxT("\n\nURL... "));
		
		wxLogDebug(wxT("wxWebUpdateDlg::OnDownloadComplete - Download status: failed !"));
		if (downloadingScript)
			AbortDialog();		// this is a unrecoverable error !

		m_bUserAborted = FALSE;		// reset flag
		return;

	} else {

		wxLogDebug(wxT("wxWebUpdateDlg::OnDownloadComplete - Download status: successfully completed"));
		if (downloadingScript) {

			// handle the XML parsing & control update 
			OnScriptDownload(m_dThread->m_strOutput);
			forceremove = TRUE;			// always remove the script

		} else {

			if (m_dThread->m_strMD5.IsEmpty() || m_dThread->IsMD5Ok()) {

				// handle the installation of this package
				const wxWebUpdatePackage &pkg = m_pUpdatesList->GetRemotePackage(m_dThread->m_strID);
				const wxWebUpdateDownload &download = pkg.GetDownloadPackage();

				//m_nStatus = wxWUDS_INSTALLING;
				m_iThread->m_pDownload = &download;
				m_iThread->m_strUpdateFile = download.GetFileName();
				m_iThread->BeginNewInstall();
				//download.Install();
				//m_nStatus = wxWUDS_UNDEFINED;
				//this->Close(TRUE);
				//EndModal(wxOK);
				//GetParent()->Close(TRUE);
				//::wxExit();
				//wxTheApp->Exit();
				//wxTheApp->CleanUp();
				//exit(0);
				
			} else {

				wxMessageBox(wxT("The downloaded file \"") + m_dThread->m_strOutput + 
						wxT("\"\nis corrupted. MD5 checksum is:\n\n\t") + 
						m_dThread->GetComputedMD5() +
						wxT("\n\ninstead of:\n\n\t") + 
						m_dThread->m_strMD5 +
						wxT("\n\nPlease retry the download."), 
						wxT("Error"), wxOK | wxICON_ERROR);
				//forceremove = TRUE;		// remove this corrupted package
			}
		}
	}

	// remove the temporary files...
	bool donotremove = m_pAdvPanel->RemoveFiles();
	if (forceremove || !donotremove) {		// XML webupdate script is always removed
		wxLogDebug(wxT("wxWebUpdateDlg::OnDownloadComplete - ")
			wxT("Removing the downloaded file: ") + m_dThread->m_strOutput);
		wxRemoveFile(m_dThread->m_strOutput);
	}
}

void wxWebUpdateDlg::OnUpdateUI(wxUpdateUIEvent &)
{
	// help us to avoid useless calls to SetLabel() function which
	// cause flickering; the initial value ensures that at least a
	// label update takes place when showing the dialog
	static int nLabelMode = 0xFFFFFF;
	static wxDateTime lastupdate = wxDateTime::UNow();

	// change the description label eventually
	if (m_pUpdatesList->GetSelectedItemCount() > 0) {
	
		for (int i=0; i<m_pUpdatesList->GetItemCount(); i++) {
			if (m_pUpdatesList->GetItemState(i, wxLIST_STATE_SELECTED)) {

				// show the description of the first selected item
				wxString name = m_pUpdatesList->GetItemText(i);
				wxWebUpdatePackage &pkg = m_pUpdatesList->GetRemotePackage(name);
				m_pDescription->SetValue(pkg.GetDescription());
				break;
			}
		}
	}

	// update our state var looking at the thread status
	if (m_dThread->IsRunning()) {

		switch (m_dThread->GetStatus()) {
		case wxDTS_DOWNLOADING:
			m_nStatus = wxWUDS_DOWNLOADING;
			break;
		case wxDTS_COMPUTINGMD5:
			m_nStatus = wxWUDS_COMPUTINGMD5;
			break;
		case wxDTS_WAITING:
			if (m_nStatus == wxWUDS_DOWNLOADING || m_nStatus == wxWUDS_COMPUTINGMD5)
				m_nStatus = wxWUDS_UNDEFINED;
			else
				// leave our status var untouched
				// (i.e. in the wxWUDS_UNDEFINED or wxWUDS_INSTALLING states)
			break;
		}

	} else {

		wxASSERT_MSG(m_nStatus != wxWUDS_DOWNLOADING &&
						m_nStatus != wxWUDS_COMPUTINGMD5,
						wxT("The status var has not been updated properly"));
	}

	if (m_iThread->IsRunning()) {

		switch (m_iThread->GetStatus()) {
		case wxWUITS_INSTALLING:
			m_nStatus = wxWUDS_INSTALLING;
			break;
		case wxWUITS_WAITING:
			if (m_nStatus == wxWUDS_INSTALLING)
				m_nStatus = wxWUDS_UNDEFINED;
			else
				// leave our status var untouched
				// (i.e. in the wxWUDS_UNDEFINED or wxWUDS_INSTALLING states)
			break;
		}
	}

	// change UI labels according to the current status
	if (m_dThread->IsDownloading()) {

		// need to change labels ?
		if (nLabelMode != wxWUDS_DOWNLOADING) {
			m_pOkBtn->Disable();
			m_pCancelBtn->SetLabel(wxWUD_CANCEL_DOWNLOAD);
			//m_pUpdatesList->EnableAll(FALSE);
			nLabelMode = wxWUDS_DOWNLOADING;
		}

		// update the meters each 500 milliseconds to avoid flickering
		wxDateTime current = wxDateTime::UNow();
		wxTimeSpan diff = current - lastupdate;
		if (diff.GetMilliseconds().ToLong() > 500) {

			//wxLogDebug(wxT("wxWebUpdateDlg::OnUpdateUI - updating meters"));
			lastupdate = current;

			// update our gauge control
			long value = m_dThread->GetCurrDownloadedBytes();	
			m_pGauge->SetValue(value >= 0 ? value : 0);

			// update speed meter
			m_pSpeedText->SetLabel(wxWUD_SPEEDTEXT_PREFIX wxT("downloading \"") + 
								m_dThread->m_strResName + wxT("\" at ") + 
								m_dThread->GetDownloadSpeed());

			// update time meter
			m_pTimeText->SetLabel(wxWUD_TIMETEXT_PREFIX + 
								m_dThread->GetRemainingTime());
		}

	} else if (m_dThread->IsComputingMD5()) { 

		m_pGauge->SetValue(0);
		if (nLabelMode != wxWUDS_COMPUTINGMD5) {
			m_pSpeedText->SetLabel(wxWUD_SPEEDTEXT_PREFIX wxT("computing the file hash"));
			m_pTimeText->SetLabel(wxT("No downloads running..."));
			m_pCancelBtn->SetLabel(wxWUD_CANCEL_DOWNLOAD);
			m_pOkBtn->Disable();
			nLabelMode = wxWUDS_COMPUTINGMD5;
		}

	} else if (m_iThread->IsInstalling()) {

		m_pGauge->SetValue(0);
		if (nLabelMode != wxWUDS_INSTALLING) {
			m_pSpeedText->SetLabel(wxWUD_SPEEDTEXT_PREFIX wxT("installing \"") + m_dThread->m_strResName + wxT("\""));			
			m_pTimeText->SetLabel(wxT("No downloads running..."));
			m_pCancelBtn->SetLabel(wxWUD_CANCEL_INSTALLATION);
			m_pOkBtn->Disable();
			nLabelMode = wxWUDS_INSTALLING;
		}

	} else {

		// reset our gauge control
		m_pGauge->SetValue(0);
		
		// did we download our WebUpdate script ?
		bool scriptOk = m_xmlScript.IsOk();

		// re-enable what we disabled when we launched the thread
		wxASSERT(m_nStatus == wxWUDS_UNDEFINED);
		if (nLabelMode != m_nStatus) {

			// update our meters
			int d = m_dThread->GetDownloadCount(),
				i = m_iThread->GetInstallationCount();

			// we need to decrement the "d" var since we don't want to take in count
			// the download of the XML webupdate script
			d--;

			if (d > 0 && i > 0)
				m_pSpeedText->SetLabel(
					wxString::Format(wxWUD_SPEEDTEXT_PREFIX wxT("downloaded %d package(s) and installed %d package(s)"), d, i));
			else if (d > 0 && i == 0)
				m_pSpeedText->SetLabel(
					wxString::Format(wxWUD_SPEEDTEXT_PREFIX wxT("downloaded %d package(s)"), d));
			else if (d == 0 && scriptOk)
				m_pSpeedText->SetLabel(wxWUD_SPEEDTEXT_PREFIX wxT("WebUpdate script successfully downloaded"));
			else if (d == -1 && !scriptOk)
				m_pSpeedText->SetLabel(wxWUD_SPEEDTEXT_PREFIX wxT("waiting WebUpdate script download"));
			else
				m_pSpeedText->SetLabel(wxWUD_SPEEDTEXT_PREFIX wxT("download failed !"));

			// in any case (wxWUDS_INSTALLING/wxWUDS_UNDEFINED) we are not downloading
			// anything now...
			m_pTimeText->SetLabel(wxT("No downloads running..."));

			if (scriptOk)
				m_pOkBtn->SetLabel(wxT("Download"));
			else
				m_pOkBtn->SetLabel(wxT("Get update list"));

			m_pCancelBtn->SetLabel(wxWUD_CANCEL_DEFAULT_LABEL);

			//m_pUpdatesList->EnableAll();
			nLabelMode = m_nStatus;
		}

		// are there checked items in the package listctrl ?
		if (scriptOk) {

			int i;
			for (i=0; i<m_pUpdatesList->GetItemCount(); i++)
				if (m_pUpdatesList->IsChecked(i))
					break;		// found a checked item !
			
			if (i<m_pUpdatesList->GetItemCount())
				m_pOkBtn->Enable();		// yes, there are
			else
				m_pOkBtn->Disable();		// no, there aren't

		} else {

			m_pOkBtn->Enable();
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

		// save it also in our update installer
		wxWebUpdateInstaller::Get()->SetKeywordValue(wxT("downloaddir"), dlg.GetPath());
		
	} else {

		// just don't change nothing
	}
}
