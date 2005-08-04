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
#include <wx/tokenzr.h>

#if wxUSE_HTTPENGINE
	#include <wx/httpengine/proxysettingsdlg.h>
#endif

// wxWidgets RTTI
IMPLEMENT_CLASS(wxWebUpdateAdvPanel, wxPanel)
IMPLEMENT_CLASS(wxWebUpdateDlg, wxDialog)
IMPLEMENT_CLASS(wxWebUpdateListCtrl, wxWUDLC_BASECLASS)

BEGIN_EVENT_TABLE(wxWebUpdateListCtrl, wxWUDLC_BASECLASS)
	EVT_LIST_ITEM_CHECKED(-1, wxWebUpdateListCtrl::OnItemCheck)
	EVT_LIST_ITEM_UNCHECKED(-1, wxWebUpdateListCtrl::OnItemUncheck)

	EVT_CACHESIZE_COMPLETE(-1, wxWebUpdateListCtrl::OnCacheSizeComplete)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(wxWebUpdateDlg, wxDialog)

	// buttons
    EVT_BUTTON(XRCID("IDWUD_OK"), wxWebUpdateDlg::OnDownload)
    EVT_BUTTON(XRCID("IDWUD_CANCEL"), wxWebUpdateDlg::OnCancel)	
    EVT_BUTTON(XRCID("IDWUD_SHOWHIDEADV"), wxWebUpdateDlg::OnShowHideAdv)
    EVT_BUTTON(XRCID("IDWUD_ABOUT"), wxWebUpdateDlg::OnAbout)

	// we need to intercept also the clicks on the close box in the system menu
	EVT_BUTTON(wxID_CANCEL, wxWebUpdateDlg::OnCancel)

	// checkbox
	EVT_CHECKBOX(XRCID("IDWUD_SHOWFILTER"), wxWebUpdateDlg::OnShowFilter)

	// global UI updates
	EVT_UPDATE_UI(-1, wxWebUpdateDlg::OnUpdateUI)

	// miscellaneous
	EVT_TEXT_URL(XRCID("IDWUD_DESCRIPTION"), wxWebUpdateDlg::OnTextURL)
	
	// from threads
	EVT_DOWNLOAD_COMPLETE(-1, wxWebUpdateDlg::OnDownloadComplete)
	EVT_INSTALLATION_COMPLETE(-1, wxWebUpdateDlg::OnInstallationComplete)

END_EVENT_TABLE()

BEGIN_EVENT_TABLE(wxWebUpdateAdvPanel, wxPanel)

	// buttons
    EVT_BUTTON(XRCID("IDWUAP_BROWSE"), wxWebUpdateAdvPanel::OnBrowse)
    EVT_BUTTON(XRCID("IDWUAP_CONNSETTINGS"), wxWebUpdateAdvPanel::OnConnSettings)

END_EVENT_TABLE()

DEFINE_EVENT_TYPE(wxWUD_INIT);
DEFINE_EVENT_TYPE(wxWUD_DESTROY);

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_USER_EXPORTED_OBJARRAY(wxArrayBool);




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

wxWebUpdateCheckFlag wxWebUpdateListCtrl::SetLocalVersionFor(int idx, wxWebUpdatePackage &curr)
{
	// here we need some further work to find if this package
	// is already installed...
	wxWebUpdateLocalPackage &local = GetLocalPackage(curr.GetName());

	if (!local.IsOk()) {

		// a matching local package does not exist...
		SetItem(idx, 2, wxT("not installed"));
		return wxWUCF_NOTINSTALLED;
	}
	
	SetItem(idx, 2, local.GetVersion());
	
	// compare versions
	wxWebUpdateCheckFlag f = curr.Check(local.GetVersion());
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
	}
	
	// let the caller handle this flag
	return f;
}

void wxWebUpdateListCtrl::RebuildPackageList(bool bShowOnlyOutOfDate)
{
	// remove old contents
	DeleteAllItems();

	int idx = 0;		// could go out of synch with 'i' because
						// some packages could not be added to the list....
	for (int i=0; i < (int)m_arrUpdatedPackages.GetCount(); i++, idx++) {

		wxWebUpdatePackage &curr = m_arrUpdatedPackages[i];
		wxLogDebug(wxT("wxWebUpdateListCtrl::RebuildPackageList - Adding the '") + 
			curr.GetName() + wxT("' package to the wxListCtrl"));


		// set the properties for the first column (NAME)
		// ----------------------------------------------
		InsertItem(idx, curr.GetName());
		


		// set the properties for the second column (LATEST VERSION)
		// ---------------------------------------------------------
		SetItem(idx, 1, curr.GetLatestVersion());



		// set the properties for the third column (LOCAL VERSION)
		// -------------------------------------------------------

		// tocheck is TRUE for outdated items
		wxWebUpdateCheckFlag f = SetLocalVersionFor(idx, curr);
		bool tocheck = (f == wxWUCF_OUTOFDATE || f == wxWUCF_NOTINSTALLED);
			
		if ((f == wxWUCF_UPDATED && bShowOnlyOutOfDate) || f == wxWUCF_FAILED) {
			
			DeleteItem(idx);
			idx--;
			continue;		// continue with next package
		}



		// set the properties for the fourth column (SIZE)
		// -----------------------------------------------

		// we'll leave the task of updating this field to the 
		// wxSizeCacherThread that has been launched by wxWebUpdateDlg
		unsigned long bytesize = 0;
  		wxWebUpdateDownload &d = curr.GetDownload();
    	if (d.IsDownloadSizeCached())
     		bytesize = d.GetDownloadSize();
		SetItem(idx, 3, wxGetSizeStr(bytesize));



		// set the properties for the fifth column (IMPORTANCE)
		// ----------------------------------------------------
		switch (curr.GetImportance()) {
		case wxWUPI_HIGH:
			SetItem(idx, 4, wxT("high!"));
#if wxUSE_CHECKEDLISTCTRL
			Check(idx, tocheck);
#endif
			break;
		case wxWUPI_NORMAL:
			SetItem(idx, 4, wxT("normal"));
#if wxUSE_CHECKEDLISTCTRL
			Check(idx, tocheck);
#endif
			break;
		case wxWUPI_LOW:
			SetItem(idx, 4, wxT("low"));
#if wxUSE_CHECKEDLISTCTRL
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

wxWebUpdateLocalPackage &wxWebUpdateListCtrl::GetLocalPackage(const wxString &name)
{
	for (int j=0; j < (int)m_arrLocalPackages.GetCount(); j++)
		if (m_arrLocalPackages[j].GetName() == name)
			return m_arrLocalPackages[j];
	return wxEmptyWebUpdateLocalPackage;
}

wxWebUpdatePackage &wxWebUpdateListCtrl::GetRemotePackage(const wxString &name)
{
	for (int j=0; j<(int)m_arrUpdatedPackages.GetCount(); j++)
		if (m_arrUpdatedPackages[j].GetName() == name)
			return m_arrUpdatedPackages[j];
	return wxEmptyWebUpdatePackage;
}

void wxWebUpdateListCtrl::OnItemCheck(wxListEvent &ev)
{
	int n = ev.GetIndex();
	wxASSERT_MSG(IsChecked(n), wxT("Something broken in wxCheckedListCtrl"));

	// check also all the packages required by the package that the 
	// user has just checked...
	wxString required = GetRequiredList(n);
	for (int i=0; i < GetItemCount(); i++) {

		wxString str = m_arrUpdatedPackages[i].GetName();
		if (i != n &&							// don't process the just-checked item
			required.Contains(str))
			Check(i, TRUE);
	}
}

void wxWebUpdateListCtrl::OnItemUncheck(wxListEvent &ev)
{
	int n = ev.GetIndex();
	wxASSERT_MSG(!IsChecked(n), wxT("Something broken in wxCheckedListCtrl"));
	
	// collect some info
	wxString str = m_arrUpdatedPackages[n].GetName();

	// uncheck also all the packages which require the package that the
	// user has just unchecked...
	for (int i=0; i < GetItemCount(); i++)
		if (i != n &&							// don't process the just-unchecked item
			GetRequiredList(i).Contains(str))
			Check(i, FALSE);
}

void wxWebUpdateListCtrl::OnCacheSizeComplete(wxCommandEvent &ev)
{
	wxArrayLong &arr = *((wxArrayLong *)ev.GetClientData());
	
	for (int i=0; i<(int)m_arrUpdatedPackages.GetCount(); i++)
		m_arrUpdatedPackages[i].GetDownload().SetDownloadSize(arr[i]);

	// modify the items currently shown
	for (int j=0; j < GetItemCount(); j++) {
			
		wxWebUpdatePackage &p = GetRemotePackage(GetItemText(j)); 
		wxASSERT_MSG(p.GetDownload().IsDownloadSizeCached(),
					wxT("Why does this item has not a cached size ?"));
		unsigned long bytesize = p.GetDownload().GetDownloadSize();
		SetItem(j, 3, wxGetSizeStr(bytesize));
	}

	wxLogDebug(wxT("wxWebUpdateListCtrl::OnCacheSizeComplete - sizes cached"));
}

void wxWebUpdateListCtrl::CacheDownloadSizes()
{	
	// now load all the packages we need in local cache
	wxSizeCacherThread *p = new wxSizeCacherThread(this);
	//wxURLArray &urls = p->m_urls;
	for (int i=0; i < (int)m_arrUpdatedPackages.GetCount(); i++) {
		wxString u = m_arrUpdatedPackages[i].GetDownload().GetDownloadString();
		p->m_urls.Add(u);
	}
		
	wxLogDebug(wxT("wxWebUpdateListCtrl::CacheDownloadSizes - launching the size cacher thread"));		
	if (p->Create() != wxTHREAD_NO_ERROR ||
		p->Run() != wxTHREAD_NO_ERROR) {
		wxMessageBox(wxT("Low resources; cannot show the size of the packages...\n")
					wxT("Close some applications and then retry."), 
					wxT("Error"), wxOK | wxICON_ERROR);		
	}
}

bool wxWebUpdateListCtrl::IsPackageUp2date(const wxString &name)
{
	wxWebUpdatePackage &remote = GetRemotePackage(name);
	wxWebUpdateLocalPackage &local = GetLocalPackage(name);
	
	if (!remote.IsOk() || local.IsOk())
		return FALSE;
	
	if (remote.Check(local.GetVersion()) == wxWUCF_UPDATED)
		return TRUE;
	return FALSE;
}

void wxWebUpdateListCtrl::UpdatePackagesVersions(bool bShowOnlyOutOfDate)
{
	for (int j=0; j < GetItemCount(); j++) {
			
		wxWebUpdatePackage &curr = GetRemotePackage(GetItemText(j)); 
		wxWebUpdateCheckFlag f = SetLocalVersionFor(j, curr);
			
		if ((f == wxWUCF_UPDATED && bShowOnlyOutOfDate) || f == wxWUCF_FAILED) {
			
			DeleteItem(j);
			j--;
			continue;		// continue with next package
		}
	}
}




// ---------------------
// wxWEBUPDATEDLG
// ---------------------

wxWebUpdateDlg::wxWebUpdateDlg(wxWindow *parent, 
						const wxWebUpdateLocalXMLScript &script)
{
	m_parent=parent; 
	m_xmlLocal=script; 			
	m_bUserAborted=FALSE; 
	m_nStatus=wxWUDS_UNDEFINED;
	m_nCurrentIdx=-1; 
	m_nFileCount=0; 

	m_dThread = NULL;
	m_iThread = NULL;

	// init our GUI
	InitWidgetsFromXRC();

	// and then our worker threads
	InitThreads();
}

void wxWebUpdateDlg::InitWidgetsFromXRC()
{
	// be sure local XML script was correctly set
	// ------------------------------------------

	wxASSERT_MSG(m_xmlLocal.IsOk(), 
		wxT("You must provide a valid XML local script before building a wxWebUpdateDlg"));



	// build the dialog
	// ----------------

	wxString res = m_xmlLocal.GetXRCResName();
	wxLogDebug(wxT("wxWebUpdateDlg::InitWidgetsFromXRC - loading the '") + res + wxT("' resource..."));

	// and build our dialog window
	wxASSERT_MSG(wxXmlResource::Get()->LoadDialog(this, GetParent(), res),
			wxT("Error while building wxWebUpdateDlg; check you've loaded webupdatedlg.xrc and that's a valid XRC !"));

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
	m_pDescription = XRCCTRL(*this, "IDWUD_DESCRIPTION", wxTextCtrl);	// can be NULL
	m_pShowOnlyOOD = XRCCTRL(*this, "IDWUD_SHOWFILTER", wxCheckBox);

	m_pOkBtn = XRCCTRL(*this,"IDWUD_OK", wxButton);
	m_pCancelBtn = XRCCTRL(*this,"IDWUD_CANCEL", wxButton);
	m_pShowHideAdvBtn = XRCCTRL(*this, "IDWUD_SHOWHIDEADV", wxButton);	// can be NULL



	// init control data
	// -----------------

	m_pAppNameText->SetLabel(GetAppName());

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

	m_pUpdatesList->SetLocalPackages(m_xmlLocal.GetAllPackages());	

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

void wxWebUpdateDlg::ConnectionRequired()
{
	// are we connected ?
	wxDialUpManager *mng = wxDialUpManager::Create();
	if (mng->IsOk()) {
		
		if (!mng->IsOnline()) {
			
			wxMessageBox(wxT("You are not connected to Internet ! Please connect and then retry..."),
				wxT("Error"), wxOK | wxICON_ERROR);
			wxLogDebug(wxT("wxWebUpdateDlg::ConnectionRequired - not connected to Internet ! Aborting webupdate."));
			delete mng;
			AbortDialog();
		}
	}
	
	delete mng;
	wxLogDebug(wxT("wxWebUpdateDlg::ConnectionRequired - we are online..."));
}

void wxWebUpdateDlg::InitThreads()
{
	wxASSERT_MSG(m_dThread == NULL && m_iThread == NULL,
						wxT("initializing threads twice ?"));
	m_dThread = new wxDownloadThread(this);
	m_iThread = new wxWebUpdateInstallThread(this);

	// launch a separate thread for the webupdate script download
	if (m_dThread->Create() != wxTHREAD_NO_ERROR ||
		m_dThread->Run() != wxTHREAD_NO_ERROR) {
		wxMessageBox(wxT("Low resources; cannot run the WebUpdate dialog...\n")
			wxT("Close some applications and then retry."), 
			wxT("Error"), wxOK | wxICON_ERROR);
		wxLogDebug(wxT("wxWebUpdateDlg::InitThreads - cannot run the download thread !"));
		AbortDialog();
	}
	
	// init also our installer thread
	if (m_iThread->Create() != wxTHREAD_NO_ERROR ||
		m_iThread->Run() != wxTHREAD_NO_ERROR) {
		wxMessageBox(wxT("Low resources; cannot run the WebUpdate dialog...\n")
			wxT("Close some applications and then retry."), 
			wxT("Error"), wxOK | wxICON_ERROR);
		wxLogDebug(wxT("wxWebUpdateDlg::InitThreads - cannot run the installer thread !"));
		AbortDialog();
	}
	
	// send a notification to the global wxWebUpdater
	wxCommandEvent ce(wxWUD_INIT);
	ce.SetClientData(this);
	//	wxWebUpdater::Get()->AddPendingEvent(ce);
	
	wxLogDebug(wxT("wxWebUpdateDlg::InitThreads - successfully initialized the download & install threads..."));
}

void wxWebUpdateDlg::ShowErrorMsg(const wxString &str) const
{
	wxLogDebug(str);
	wxMessageBox(str + wxT("\nContact the support team of ") + GetAppName() +
					wxT(" for help."), wxT("Error"), wxOK | wxICON_ERROR);
}

bool wxWebUpdateDlg::CheckForAllUpdated(bool forcedefaultmsg)
{
	wxWebUpdatePackageArray arr = m_xmlRemote.GetAllPackages();
	bool allupdated = TRUE;
	for (int j=0; j < (int)arr.GetCount(); j++) {

		// get the local package info
		wxWebUpdateLocalPackage &local = 
			m_pUpdatesList->GetLocalPackage(arr[j].GetName());		

		// do the version check
		if (!local.IsOk()) {

			// this package is not even installed on the user's PC !
			allupdated = FALSE;
			break;
		}
		if (arr[j].Check(local.GetVersion()) == wxWUCF_OUTOFDATE) {
			allupdated = FALSE;
			break;		// not all packages are uptodate
		}
	}

	if (allupdated) {

		// show to the user the "update not available" message
		wxString defaultmsg = wxT("You have the latest version of all packages of ") +
							GetAppName() + wxT("... exiting the update dialog.");
		wxString usermsg = m_xmlRemote.GetUpdateNotAvailableMsg();
		wxMessageBox((usermsg.IsEmpty() || forcedefaultmsg) ? defaultmsg : usermsg, 
					GetAppName(), wxOK | wxICON_INFORMATION);

		AbortDialog();
		return TRUE;		// TRUE = exit this dialog

	} else {

		// some updates are available... show the message only if the webupdate
		// script contains an explicit user-customized message for this evenience
		wxString usermsg = m_xmlRemote.GetUpdateAvailableMsg();
		if (!usermsg.IsEmpty())
			wxMessageBox(usermsg, GetAppName(), wxOK | wxICON_INFORMATION);
	}

	return FALSE;			// FALSE = do not exit the dialog
}

void wxWebUpdateDlg::OnScriptDownload(const wxString &xmluri)
{
	// remove this download from our counter
	wxASSERT_MSG(m_nFileCount == 1, 
		wxT("Only the WebUpdate XML script should have been downloaded"));
	m_nFileCount--;

	// ok, we can now parse the XML doc
	if (!m_xmlRemote.Load(xmluri)) {
		ShowErrorMsg(wxT("Cannot parse the XML update script downloaded as: ") + 
						m_dThread->m_strOutput);
		AbortDialog();		// this is a unrecoverable error !
		return;
	}

	wxLogDebug(wxT("wxWebUpdateDlg::OnScriptDownload - XML script loaded successfully"));
	wxWebUpdatePackageArray arr = m_xmlRemote.GetAllPackages();
	
	wxASSERT_MSG(m_bDownloaded.GetCount() == 0 && m_bInstalled.GetCount() == 0, 
 			wxT("the webupdate script has already been downloaded before ?"));
	m_bDownloaded.Add(FALSE, arr.GetCount());
	m_bInstalled.Add(FALSE, arr.GetCount());
	
	// is everything up to date ?
	if (CheckForAllUpdated())
		return;

	// what if we could not found any valid package in the webupdate script ?
	m_pUpdatesList->SetRemotePackages(arr);
	m_pUpdatesList->RebuildPackageList(m_pShowOnlyOOD->GetValue());
	if (m_pUpdatesList->GetItemCount() == 0) {

		wxMessageBox(wxT("Could not found any valid package for ") + GetAppName()
					+ wxT(" in the WebUpdate script. Exiting the update dialog."), 
					wxT("Warning"), wxOK | wxICON_INFORMATION);
		AbortDialog();
		return;
	}
}

void wxWebUpdateDlg::AbortDialog()
{
	wxLogDebug(wxT("wxWebUpdateDlg::AbortDialog - stopping the download thread"));
	if (m_dThread->IsPaused())
		m_dThread->Run();		// we need the thread running if we want to delete it !
	if (m_dThread->IsRunning())
		m_dThread->Delete();
	wxLogDebug(wxT("wxWebUpdateDlg::AbortDialog - stopped & deleted the download thread"));

	wxLogDebug(wxT("wxWebUpdateDlg::AbortDialog - stopping the installer thread"));
	if (m_iThread->IsPaused())
		m_iThread->Run();		// we need the thread running if we want to delete it !
	if (m_iThread->IsRunning())
		m_iThread->Delete();
	wxLogDebug(wxT("wxWebUpdateDlg::AbortDialog - stopped & deleted the installer thread"));

	m_nStatus = wxWUDS_UNDEFINED;
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

bool wxWebUpdateDlg::DownloadFirstPackage()
{
	// launch the download of the selected packages
	int todownload = -1;
	for (int i=0; i<m_pUpdatesList->GetItemCount(); i++) {
	
		// download the first package which is checked and that has not been
		// downloaded yet...
		if (m_pUpdatesList->IsChecked(i) && !m_bDownloaded[i]) {
			todownload = i;
			break;
		}
	}

	if (todownload == -1)
		return FALSE;

	// find this name in our array of packages; this is required
	// since we can't trust the current index "i" since some view
	// filter could have been applied on the listctrl and thus 
	// there maybe some hidden packages which make "i" out of synch
	// with the i-th package contained into m_arrUpdatedPackages
	wxString name = m_pUpdatesList->GetItemText(todownload);
	wxWebUpdatePackage &pkg = m_pUpdatesList->GetRemotePackage(name);
	wxWebUpdateDownload &dl = pkg.GetDownload();
	
	// init thread variables
	m_nCurrentIdx = todownload;
	m_dThread->m_strOutput = m_pAdvPanel->GetDownloadPath() + dl.GetFileName();
	m_dThread->m_strURI = dl.GetDownloadString();
	m_dThread->m_strMD5 = dl.GetMD5Checksum();
	m_dThread->m_strResName = name + wxT(" package");
	m_dThread->m_strID = name;

	// reset the gauge GUI
	m_pGauge->SetValue(0);
	m_pGauge->SetRange(dl.GetDownloadSize());

	// (eventually) check that we are online 
	if (m_dThread->m_strURI.StartsWith(wxT("http://")))
		ConnectionRequired();
	
	// launch the download
	m_dThread->BeginNewDownload();
	wxLogDebug(wxT("wxWebUpdateDlg::OnDownload - launching download of ") + 
			m_dThread->m_strURI);
			
	return TRUE;
}

bool wxWebUpdateDlg::IsReadyForInstallation(int i) const
{
	wxString str = m_pUpdatesList->GetRequiredList(i);
	if (str.IsEmpty()) return TRUE;
	
	wxStringTokenizer tkz(str, wxT(","));
	while ( tkz.HasMoreTokens() )
	{
	    wxString token = tkz.GetNextToken();
	
	    // was this package installed ?
	    if (!m_pUpdatesList->IsPackageUp2date(token))
     		return FALSE;
	}
	
	return TRUE;
}

bool wxWebUpdateDlg::InstallFirstPackage()
{
	// find the first package whose requirements have already been installed
	int toinstall = -1;
	for (int i=0; i < m_pUpdatesList->GetItemCount(); i++) {
		if (IsReadyForInstallation(i) && !m_bInstalled[i]) {
			toinstall = i;
			break;
		}
	}

	if (toinstall == -1)
		return FALSE;

	const wxWebUpdatePackage &pkg = GetRemotePackages().Item(toinstall);
	const wxWebUpdateDownload &download = pkg.GetDownload();

	m_nCurrentIdx = toinstall;
	m_iThread->m_pDownload = (const wxWebUpdateDownload &)download;
	m_iThread->m_strUpdateFile = download.GetFileName();
	m_iThread->BeginNewInstall();
	
	return TRUE;
}



// ---------------------------------
// wxWEBUPDATEDLG - event handlers
// ---------------------------------

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
	wxASSERT_MSG(!m_dThread->IsDownloading(), 
		wxT("The wxWUD_OK button should be disabled !"));

#if wxUSE_HTTPENGINE
	// first update the advanced options
	m_dThread->m_proxy = m_pAdvPanel->GetProxySettings();
#endif

	// check if we already downloaded the XML webupdate script
	if (!m_xmlRemote.IsOk()) {

		if (m_xmlLocal.GetRemoteScriptURI().StartsWith(wxT("http://")))
			ConnectionRequired();

		m_dThread->m_strURI = m_xmlLocal.GetRemoteScriptURI();
		m_dThread->m_strResName = wxT("XML WebUpdate script");
		m_dThread->m_strOutput = wxFileName::CreateTempFileName(wxT("webupdate"));
		m_dThread->m_strID = wxWUD_XMLSCRIPT_ID;
		m_dThread->BeginNewDownload();
		UpdateWindowUI();
		return;
	}

	wxASSERT_MSG(DownloadFirstPackage(),
		wxT("The wxWUD_OK button should be enabled only when one or more packages are checked"));

	UpdateWindowUI();
}

void wxWebUpdateDlg::OnCancel(wxCommandEvent &)
{
	if (m_dThread->IsDownloading() 
#ifdef wxUSE_MD5
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
		m_iThread->AbortInstall();
		return;
	}

	AbortDialog();
}

void wxWebUpdateDlg::OnAbout(wxCommandEvent &)
{
/*
	wxBoxSizer *topsizer;
	wxHtmlWindow *html;
	wxDialog dlg(m_pFrame, wxID_ANY, wxString(wxT("About WebUpdater")));
	
	topsizer = new wxBoxSizer(wxVERTICAL);
	html = new wxHtmlWindow(&dlg, wxID_ANY, wxDefaultPosition,
						wxSize(380, 160), wxHW_SCROLLBAR_NEVER | wxSUNKEN_BORDER);
	html->SetBorders(0);

	wxFileName url(m_strHelpPath + wxT("about.htm"));
	mcASSERT(html->LoadFile(url), wxT("Invalid about page..."));
	html->SetSize(html->GetInternalRepresentation()->GetWidth(),
		html->GetInternalRepresentation()->GetHeight());
	
	topsizer->Add(html, 1, wxALL, 10);
	topsizer->Add(new wxStaticLine(&dlg, wxID_ANY), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
	wxButton *bu1 = new wxButton(&dlg, wxID_OK, wxT("OK"));
	bu1->SetDefault();
	topsizer->Add(bu1, 0, wxALL | wxALIGN_RIGHT, 15);
	
	dlg.SetSizer(topsizer);
	topsizer->Fit(&dlg);
	
	dlg.ShowModal();
*/
}

void wxWebUpdateDlg::OnShowFilter(wxCommandEvent &)
{
	// hide/show items in the listctrl
	m_pUpdatesList->RebuildPackageList(m_pShowOnlyOOD->GetValue());
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
	bool downloadingScript = (m_dThread->m_strID == wxWUD_XMLSCRIPT_ID);

	// first of all, we need to know if download was successful
	if (!m_dThread->DownloadWasSuccessful()) {

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
		m_nFileCount++;

		if (downloadingScript) {

			// handle the XML parsing & control update 
			OnScriptDownload(m_dThread->m_strOutput);

		} else {

			if (m_dThread->m_strMD5.IsEmpty() || m_dThread->IsMD5Ok()) {

				wxASSERT_MSG(!m_bDownloaded[m_nCurrentIdx] && !m_bInstalled[m_nCurrentIdx],
					wxT("We are downloading/installing the same package again ?"));
				m_bDownloaded[m_nCurrentIdx] = TRUE;
	
				// if this is the last package which must be downloaded
				// then we can begin the installation
				if (!DownloadFirstPackage()) {
				
					// the order of the package installation cannot be 
					// randomly chosen
					wxLogDebug(wxT("wxWebUpdateDlg::OnDownloadComplete - beginning installation of downloaded packages"));
					InstallFirstPackage();					
    			}
				
			} else {

				wxMessageBox(wxT("The downloaded file \"") + m_dThread->m_strOutput + 
						wxT("\"\nis corrupted. MD5 checksum is:\n\n\t") + 
						m_dThread->GetComputedMD5() +
						wxT("\n\ninstead of:\n\n\t") + 
						m_dThread->m_strMD5 +
						wxT("\n\nPlease retry the download."), 
						wxT("Error"), wxOK | wxICON_ERROR);
				m_bDownloaded[m_nCurrentIdx] = FALSE;
			}
		}
	}
}

void wxWebUpdateDlg::OnInstallationComplete(wxCommandEvent &)
{
	if (m_iThread->InstallationWasSuccessful()) {

		m_bInstalled[m_nCurrentIdx] = TRUE;

		// uncheck the item we have just installed...
		m_pUpdatesList->Check(m_nCurrentIdx, FALSE);
		
		// find the installed package
		wxWebUpdateLocalPackageArray arr(GetLocalPackages());
		wxWebUpdatePackage &r = GetRemotePackages().Item(m_nCurrentIdx);
		wxLogDebug(wxT("wxWebUpdateDlg::OnInstallationComplete - completed installation of \"")
					+ r.GetName() + wxT("\"..."));

		// update the version fields for the local package...
		wxWebUpdateLocalPackage &l = arr.Item(m_nCurrentIdx);
		if (!l.IsOk()) 
  			arr.Add(wxWebUpdateLocalPackage(r.GetName(), r.GetLatestVersion()));
		else
			for (int i=0; i < (int)arr.GetCount(); i++)
				if (arr[i].GetName() == r.GetName())
					arr[i].SetVersion(r.GetLatestVersion());
		
		// update & save the XML file
		m_xmlLocal.SetPackageArray(arr);
		if (!m_xmlLocal.Save()) {
		
			// don't save the new version in the listctrl array
			// since something went wrong...
			
  		} else {
  		
  			// save the new local array in our listctrl
			m_pUpdatesList->SetLocalPackages(arr);

			// and update the listctrl versions
  			m_pUpdatesList->UpdatePackagesVersions(m_pShowOnlyOOD->GetValue());
    	}
			
		// proceed with next
		if (!InstallFirstPackage()) {

			// notify the user
			wxLogDebug(wxT("wxWebUpdateDlg::OnInstallationComplete - completed the installation of all packages"));
			wxMessageBox(wxT("The installation of the selected packages has been completed successfully !"), 
						wxT("WebUpdater"), wxICON_INFORMATION | wxOK);

			CheckForAllUpdated(TRUE);
		}
		
	} else {
	
		// warn the user
		m_bInstalled[m_nCurrentIdx] = FALSE;
		wxMessageBox(wxT("The downloaded package \"") + m_dThread->m_strOutput + 
						wxT("\"\ncould not be installed.") +
						wxT("\n\nPlease contact the support team for more info."), 
						wxT("Error"), wxOK | wxICON_ERROR);
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
	if (m_pDescription && m_pUpdatesList->GetSelectedItemCount() > 0) {
	
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

			// use the "speed" label for installing status
			m_pSpeedText->SetLabel(wxWUD_SPEEDTEXT_PREFIX wxT("installing \"") + 
				m_dThread->m_strResName + wxT("\""));			
			m_pTimeText->SetLabel(wxT("No downloads running..."));
			m_pCancelBtn->SetLabel(wxWUD_CANCEL_INSTALLATION);
			m_pOkBtn->Disable();
			nLabelMode = wxWUDS_INSTALLING;
		}

	} else {

		// reset our gauge control
		m_pGauge->SetValue(0);
		
		// update our meters
		int d = m_nFileCount,		// see #m_nFileCount description
			i = m_iThread->GetInstallationCount();
		
		// did we download our WebUpdate script ?
		bool scriptOk = m_xmlRemote.IsOk();

		// re-enable what we disabled when we launched the thread
		wxASSERT(m_nStatus == wxWUDS_UNDEFINED);
		if (nLabelMode != m_nStatus) {

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
				m_pOkBtn->SetLabel(wxT("Download && install"));
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
	m_pSaveLog = XRCCTRL(*this, "IDWUAP_SAVELOG", wxCheckBox);
	m_pRemoveFiles = XRCCTRL(*this, "IDWUAP_REMOVE", wxCheckBox);

#if !wxUSE_HTTPENGINE
	wxButton *b = XRCCTRL(*this, "IDWUAP_CONNSETTINGS", wxButton);
	b->Hide();
#endif


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

void wxWebUpdateAdvPanel::OnConnSettings(wxCommandEvent &)
{
#if wxUSE_HTTPENGINE
	wxProxySettingsDlg dlg(this, -1, wxT("WebUpdate proxy settings"));

	wxTopLevelWindow *tw = wxDynamicCast(GetParent(), wxTopLevelWindow);
	if (tw) dlg.SetIcon( tw->GetIcon() );

	dlg.CenterOnScreen();
	dlg.SetHost(m_proxy.m_strProxyHostname);
	dlg.SetPortW(m_proxy.m_nProxyPort);
	dlg.SetUsername(m_proxy.m_strProxyUsername);
	dlg.SetPassword(m_proxy.m_strProxyPassword);
	dlg.SetAuthProxy(m_proxy.m_bProxyAuth);
	//dlg.SetExceptions(m_proxy.m_strProxyExceptions);
	//dlg.SetExceptionsDesc( _T("") );
	//dlg.SetExceptionsNote( _T("") );
	
	if( dlg.ShowModal() == wxID_OK )
	{
		m_proxy.m_strProxyHostname = dlg.GetHost();
		m_proxy.m_nProxyPort = dlg.GetPort();
		m_proxy.m_strProxyUsername = dlg.GetUsername( );
		m_proxy.m_strProxyPassword = dlg.GetPassword( );
		m_proxy.m_bProxyAuth = dlg.IsAuthProxy();
		//m_proxy.m_strProxyExceptions = dlg.GetExceptions();
	}
#endif
}

