/////////////////////////////////////////////////////////////////////////////
// Name:        webupdatectrl.cpp
// Purpose:     wxWebUpdateListCtrl
// Author:      Francesco Montorsi
// Created:     2005/08/7
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
	#pragma implementation "webupdatectrl.h"
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
#include "wx/webupdatectrl.h"
#include "wx/installer.h"
#include <wx/wfstream.h>
#include <wx/xrc/xmlres.h>
#include <wx/image.h>
#include <wx/dialup.h>
#include <wx/tokenzr.h>

// wxWidgets RTTI
IMPLEMENT_CLASS(wxWebUpdateListCtrl, wxWUDLC_BASECLASS)
BEGIN_EVENT_TABLE(wxWebUpdateListCtrl, wxWUDLC_BASECLASS)
	EVT_LIST_ITEM_CHECKED(-1, wxWebUpdateListCtrl::OnItemCheck)
	EVT_LIST_ITEM_UNCHECKED(-1, wxWebUpdateListCtrl::OnItemUncheck)
	EVT_SIZE(wxWebUpdateListCtrl::OnSize)

	EVT_CACHESIZE_COMPLETE(-1, wxWebUpdateListCtrl::OnCacheSizeComplete)
END_EVENT_TABLE()

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
		sz = wxString::Format(wxT("%.0f kB"), ((float)bytesize/1024.));
	else if (bytesize < 1024*1024*1024) 
		sz = wxString::Format(wxT("%.1f MB"), ((float)bytesize/(1024.*1024.)));
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

wxWebUpdateListCtrl::wxWebUpdateListCtrl(wxWindow* parent, wxWindowID id, 
					const wxPoint& pos, const wxSize& size, long style, 
			  		const wxValidator& validator, const wxString& name)
		: wxWUDLC_BASECLASS(parent, id, pos, size, style, validator, name)
{
	m_bLocked = FALSE;

	// init the list control with the column names
	// (items will be inserted as soon as we load the webupdate script)the user-supplied wxWebUpdateLocalPackages
	InsertColumn(0, wxT("Package name"));
	InsertColumn(1, wxT("Latest version"));
	InsertColumn(2, wxT("Local version"));
	InsertColumn(3, wxT("Size"));
	InsertColumn(4, wxT("Importance"));
	InsertColumn(5, wxT("Requires"));
}

void wxWebUpdateListCtrl::OnSize(wxSizeEvent &)
{
	SetColumnWidth(0, wxLIST_AUTOSIZE_USEHEADER);
	SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
	SetColumnWidth(2, wxLIST_AUTOSIZE_USEHEADER);	
	SetColumnWidth(4, wxLIST_AUTOSIZE_USEHEADER);
	SetColumnWidth(5, wxLIST_AUTOSIZE_USEHEADER);

	// size is the smallest column but it usually needs some space...
	int colwidth = GetColumnWidth(0) +
					GetColumnWidth(1) +
					GetColumnWidth(2) +
					GetColumnWidth(4) +
					GetColumnWidth(5);
	int col3size = GetClientSize().GetWidth()-colwidth-5;	
	SetColumnWidth(3, col3size > 50 ? col3size : 50);
}

void wxWebUpdateListCtrl::SetRemotePackages(const wxWebUpdatePackageArray &arr)
{
	m_bDownloaded.Clear();
	m_bInstalled.Clear();
	
	// init
	m_bDownloaded.Add(FALSE, arr.GetCount());
	m_bInstalled.Add(FALSE, arr.GetCount());
	m_arrRemotePackages = arr; 
	
	// load smoothly the size of the remote packages
 	CacheDownloadSizes(); 
}

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

void wxWebUpdateListCtrl::RebuildPackageList(wxWebUpdateListCtrlFilter filter)
{
	// remove old contents
	DeleteAllItems();

	int idx = 0;		// could go out of synch with 'i' because
						// some packages could not be added to the list....
	for (int i=0; i < (int)m_arrRemotePackages.GetCount(); i++, idx++) {

		wxWebUpdatePackage &curr = m_arrRemotePackages[i];
		wxLogDebug(wxT("wxWebUpdateListCtrl::RebuildPackageList - Adding the '") + 
			curr.GetName() + wxT("' package..."));


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
			
		if (IsToDiscard(filter, idx, f)) {
			
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
		
		
		// set as item data the index in our remote package array
		SetItemData(idx, i);
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
	for (int j=0; j<(int)m_arrRemotePackages.GetCount(); j++)
		if (m_arrRemotePackages[j].GetName() == name)
			return m_arrRemotePackages[j];
	return wxEmptyWebUpdatePackage;
}

void wxWebUpdateListCtrl::OnItemCheck(wxListEvent &ev)
{
	int n = ev.GetIndex();
	wxASSERT_MSG(IsChecked(n), wxT("Something broken in wxCheckedListCtrl"));
	
	if (!CanBeChecked(n)) { 
		Check(n, FALSE);
		return;
	}

	// check also all the packages required by the package that the 
	// user has just checked...
	wxString required = m_arrRemotePackages[GetPackageIndexForItem(n)].GetPrerequisites();
	for (int i=0; i < GetItemCount(); i++) {

		wxString str = m_arrRemotePackages[GetPackageIndexForItem(i)].GetName();
		if (i != n &&							// don't process the just-checked item
			required.Contains(str) &&
			CanBeChecked(i))
			Check(i, TRUE);
	}
}

void wxWebUpdateListCtrl::OnItemUncheck(wxListEvent &ev)
{
	int n = ev.GetIndex();
	wxASSERT_MSG(!IsChecked(n), wxT("Something broken in wxCheckedListCtrl"));
	if (!CanBeUnchecked(n)) { 
		Check(n, TRUE);
		return;
	}
	
	// collect some info
	wxString str = m_arrRemotePackages[GetPackageIndexForItem(n)].GetName();

	// uncheck also all the packages which require the package that the
	// user has just unchecked...
	for (int i=0; i < GetItemCount(); i++) {
	
		wxString req = m_arrRemotePackages[GetPackageIndexForItem(i)].GetPrerequisites();
		if (i != n &&							// don't process the just-unchecked item
			req.Contains(str) &&
			CanBeUnchecked(i))
			Check(i, FALSE);
	}
}

void wxWebUpdateListCtrl::OnCacheSizeComplete(wxCommandEvent &ev)
{
	wxArrayLong *arr = (wxArrayLong *)ev.GetClientData();
	
	for (int i=0; i<(int)m_arrRemotePackages.GetCount(); i++)
		m_arrRemotePackages[i].GetDownload().SetDownloadSize(arr->Item(i));
	delete arr;

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
	for (int i=0; i < (int)m_arrRemotePackages.GetCount(); i++) {
		wxString u = m_arrRemotePackages[i].GetDownload().GetDownloadString();
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

wxWebUpdateCheckFlag wxWebUpdateListCtrl::IsPackageUp2date(const wxWebUpdatePackage &remote)
{
	// check the packages	
	if (!remote.IsOk()) {
	
		wxLogDebug(wxT("wxWebUpdateListCtrl::IsPackageUp2date - the ") + 
  					remote.GetName() +
  					wxT(" package does not exist !"));
		return wxWUCF_FAILED;
	}
	
	wxWebUpdateLocalPackage &local = GetLocalPackage(remote.GetName());
	if (!local.IsOk())
		return wxWUCF_NOTINSTALLED;
	
	return remote.Check(local.GetVersion());
}

bool wxWebUpdateListCtrl::IsToDiscard(wxWebUpdateListCtrlFilter filter, int, wxWebUpdateCheckFlag f)
{
	if (filter == wxWULCF_ONLY_OUTOFDATE && f == wxWUCF_UPDATED)
 		return TRUE;		// this is not out-of-date
	if (f == wxWUCF_FAILED)
		return TRUE;
	return FALSE;
}

void wxWebUpdateListCtrl::UpdatePackagesVersions(wxWebUpdateListCtrlFilter filter)
{
	for (int j=0; j < GetItemCount(); j++) {
			
		wxWebUpdatePackage &curr = m_arrRemotePackages[GetPackageIndexForItem(j)]; 
		wxWebUpdateCheckFlag f = SetLocalVersionFor(j, curr);
			
		if (IsToDiscard(filter, j, f)) {
			
			DeleteItem(j);
			j--;
			continue;		// continue with next package
		}
	}
}

int wxWebUpdateListCtrl::GetPackageIndexForItem(int i) const
{
	// in #RebuildPackageList we set as item data the index in our
	// remote package array...
	return GetItemData(i);
}

bool wxWebUpdateListCtrl::CanBeChecked(int n)
{
	if (m_bLocked) return FALSE;		// discard user interaction in any case.

	// cannot check packages which are already up-to-date...
	wxWebUpdateCheckFlag f = IsPackageUp2date(m_arrRemotePackages[GetPackageIndexForItem(n)]);
	if (f == wxWUCF_UPDATED || f == wxWUCF_FAILED)
		return FALSE;
	return TRUE;
}

bool wxWebUpdateListCtrl::CanBeUnchecked(int n)
{
	if (m_bLocked) 
		return FALSE;		// discard user interaction in any case.
	
	wxWebUpdateCheckFlag f = IsPackageUp2date(m_arrRemotePackages[GetPackageIndexForItem(n)]);
	wxASSERT_MSG(f != wxWUCF_UPDATED && f != wxWUCF_FAILED,
				wxT("That item should not have been checked !"));
	return TRUE;
}

wxWebUpdatePackage *wxWebUpdateListCtrl::GetNextPackageToDownload()
{
	if (!IsShown()) {
	
		// we are hidden; thus the user cannot check the packages he want
		// to install... we will just select automatically *all* available
		// packages...
		for (int i=0; i < GetItemCount(); i++)
			Check(i, TRUE);
	}
	
	// launch the download of the selected packages
	int todownload = -1;
	for (int i=0; i<GetItemCount(); i++) {
	
		// download the first package which is checked and that has not been
		// downloaded yet...
		if (IsChecked(i) && !m_bDownloaded[GetPackageIndexForItem(i)]) {
			todownload = i;
			break;
		}
	}

	if (todownload == -1)
		return NULL;

	return 	&m_arrRemotePackages[GetPackageIndexForItem(todownload)];
}

bool wxWebUpdateListCtrl::IsReadyForInstallation(int n)
{
	wxString str = m_arrRemotePackages[n].GetPrerequisites();
	if (str.IsEmpty()) return TRUE;
	
	// extract each required package from the list
	wxStringTokenizer tkz(str, wxT(","));
	while ( tkz.HasMoreTokens() )
	{
	    wxString token = tkz.GetNextToken();
	
	    // is this package installed & up2date ?
	    if (IsPackageUp2date(token) != wxWUCF_UPDATED)
     		return FALSE;
	}
	
	return TRUE;
}

wxWebUpdatePackage *wxWebUpdateListCtrl::GetNextPackageToInstall()
{
	// find the first package whose requirements have already been installed
	int toinstall = -1;
	for (int i=0; i < GetItemCount(); i++) {
	
		int n = GetPackageIndexForItem(i);
		if (IsReadyForInstallation(n) && 
  			m_bDownloaded[n] && 
     		!m_bInstalled[n]) {
			toinstall = i;
			break;
		}
	}

	if (toinstall == -1)
		return NULL;

	return &m_arrRemotePackages[GetPackageIndexForItem(toinstall)];
}

void wxWebUpdateListCtrl::SetDownloadStatus(const wxWebUpdatePackage &p, bool status)
{
	int idx = m_arrRemotePackages.Index(p);
	if (idx == wxNOT_FOUND) return;
	
	m_bDownloaded[idx] = status;
}

void wxWebUpdateListCtrl::SetInstallStatus(const wxWebUpdatePackage &p, bool status)
{
	int idx = m_arrRemotePackages.Index(p);
	if (idx == wxNOT_FOUND) return;
	
	m_bInstalled[idx] = status;
}

bool wxWebUpdateListCtrl::IsInstalled(const wxWebUpdatePackage &p) const
{
	int idx = m_arrRemotePackages.Index(p);
	if (idx == wxNOT_FOUND) return FALSE;

	return m_bInstalled[idx];
}

bool wxWebUpdateListCtrl::IsDownloaded(const wxWebUpdatePackage &p) const
{
	int idx = m_arrRemotePackages.Index(p);
	if (idx == wxNOT_FOUND) return FALSE;

	return m_bDownloaded[idx];
}
