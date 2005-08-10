/////////////////////////////////////////////////////////////////////////////
// Name:        webupdatectrl.h
// Purpose:     wxWebUpdateListCtrl
// Author:      Francesco Montorsi
// Created:     2005/08/7
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_WEBUPDATECTRL_H_
#define _WX_WEBUPDATECTRL_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "webupdatectrl.h"
#endif

// wxWidgets headers
#include "wx/webupdate.h"
#include "wx/download.h"
#include "wx/checkedlistctrl.h"
#include "wx/installer.h"
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>

//! Returns a string with a short size description for the given number of bytes.
WXDLLIMPEXP_WEBUPDATE wxString wxGetSizeStr(unsigned long bytesize);

//! The possible filters for the wxWebUpdateListCtrl.
enum wxWebUpdateListCtrlFilter {

	//! The listctrl will show only the local packages which are out-of-date 
	//! or the remote packages which are not installed.
	wxWULCF_ONLY_OUTOFDATE,
	
	//! The listctrl will show all the local & remote packages.
	wxWULCF_ALL
};


// a flag array used by wxWebUpdateDlg
WX_DECLARE_USER_EXPORTED_OBJARRAY(bool, wxArrayBool, WXDLLIMPEXP_WEBUPDATE);

#if wxUSE_CHECKEDLISTCTRL
	#define wxWUDLC_BASECLASS wxCheckedListCtrl
#else
	#define wxWUDLC_BASECLASS wxListCtrl
#endif

//! The wxListCtrl which lists the REMOTE packages available as they are
//! parsed from the remote XML WebUpdate script.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateListCtrl : public wxWUDLC_BASECLASS
{
protected:

	//! The local packages currently installed.
	wxWebUpdateLocalPackageArray m_arrLocalPackages;	

	//! The packages we have downloaded from the web.
	wxWebUpdatePackageArray m_arrRemotePackages;
	
	//! An array of flags; TRUE for the n-th entry means that the n-th package
	//! (referring to the m_pUpdatesList listctrl package array) has already
	//! been downloaded and is ready for installation.
	wxArrayBool m_bDownloaded;
	
	//! An array of flags; TRUE for the n-th entry means that the n-th package
	//! (referring to the m_pUpdatesList listctrl package array) has already
	//! been installed.
	wxArrayBool m_bInstalled;

	//! TRUE if this listctrl is currently "locked". See Lock() for more info.
	bool m_bLocked;

protected:		// event handlers

	void OnItemCheck(wxListEvent &ev);
	void OnItemUncheck(wxListEvent &ev);
	void OnSize(wxSizeEvent &);

	// for event raised by our wxCacheSizerThread....
	void OnCacheSizeComplete(wxCommandEvent &);

	//! Launches our thread for caching the sizes of the packages shown
	//! in this listctrl.	
	//! You need to call #RebuildPackageList after this function.
	void CacheDownloadSizes();
	
	//! Returns the index in our #m_arrRemotePackages array for the
	//! n-th item currently shown in this listctrl.
	//! The two indexes could not match when, for example, we are
	//! showing only outdated packages in the listctrl.
	int GetPackageIndexForItem(int i) const;

	//! Sets the local version field for the listctrl item at idx #idx
	//! using the idx-th item of the local package array for retrieving
	//! the version string.
	//! Uses the given remote package for checking its update state.
	wxWebUpdateCheckFlag SetLocalVersionFor(int idx, wxWebUpdatePackage &curr);

	//! Returns TRUE if the item-idx item shown in the listctrl is to discard
	//! when applying the given filter.
	virtual bool IsToDiscard(wxWebUpdateListCtrlFilter filter, int itemidx, wxWebUpdateCheckFlag f);

public:

	//! Constructs a wxWebUpdateAdvPanel.
	wxWebUpdateListCtrl::wxWebUpdateListCtrl(wxWindow* parent, wxWindowID id, 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, 
		long style = wxLC_ICON, const wxValidator& validator = wxDefaultValidator, 
		const wxString& name = wxListCtrlNameStr);

	virtual ~wxWebUpdateListCtrl() {}

public:			// miscellaneous

	//! Rebuilds the list of the packages inside the main wxListCtrl
	//! using the #m_arrUpdatedPackages array. Removes any old content.
	void RebuildPackageList(wxWebUpdateListCtrlFilter filter);

	//! Updates the local version fields of all items in the listctrl using
	//! the version strings stored in the local package array.
	void UpdatePackagesVersions(wxWebUpdateListCtrlFilter filter);

	//! Returns a wxWebUpdateCheckFlag for the given package
	//! (both the remote & local package arrays are used, obviously!).
	wxWebUpdateCheckFlag IsPackageUp2date(const wxString &name)
		{ return IsPackageUp2date(GetRemotePackage(name)); }
	wxWebUpdateCheckFlag IsPackageUp2date(const wxWebUpdatePackage &p);

	//! Returns the first remote package which is checked and which has not
	//! been marked as already downloaded.
	wxWebUpdatePackage *GetNextPackageToDownload();

	//! Returns the first remote package which is checked, which has
	//! been marked as downloaded, has not been marked as installed yet
 	//! and whose requirements are all met.
	wxWebUpdatePackage *GetNextPackageToInstall();
	
	//! Returns TRUE if the n-th item of the remote package array
	//! (not the n-th item of the listctrl!) is ready to be installed.
	bool IsReadyForInstallation(int n);
	
	//! Makes impossible for the user to modify the check status of the
	//! current packages.
	void Lock()	
 		{ m_bLocked = TRUE; }
	
	//! Restore the user ability to check/uncheck items.
	void Unlock()
		{ m_bLocked = FALSE; }
	
public:		// getters

	//! Returns the array of updated packages parsed from the WebUpdate XML Script.
	wxWebUpdatePackageArray &GetRemotePackages()
		{ return m_arrRemotePackages; }

	//! Returns the array of updated packages taken from the wxWebUpdater.
	wxWebUpdateLocalPackageArray &GetLocalPackages()
		{ return m_arrLocalPackages; }

	//! Returns a reference to the local package with the given name or 
 	//! wxEmptyWebUpdateLocalPackage if such package could not be found.
	wxWebUpdateLocalPackage &GetLocalPackage(const wxString &name);

	//! Returns the remote package with the given name or 
 	//! wxEmptyWebUpdatePackage if such package could not be found.
	wxWebUpdatePackage &GetRemotePackage(const wxString &name);
	
	//! Returns the download status for the given package.
	bool IsDownloaded(const wxWebUpdatePackage &) const;
	
	//! Returns the installation status for the given package.
	bool IsInstalled(const wxWebUpdatePackage &) const;	

	//! Returns TRUE if the n-th item can be checked (by the user).
	bool CanBeChecked(int n);

	//! Returns TRUE if the n-th item can be unchecked (by the user).
	bool CanBeUnchecked(int n);

public:		// setters

	//! Sets the array of remote packages.
	//! You need to call #RebuildPackageList after this function.
	void SetRemotePackages(const wxWebUpdatePackageArray &arr);
		
	//! Sets the array of local packages.
	void SetLocalPackages(const wxWebUpdateLocalPackageArray &arr)
		{ m_arrLocalPackages = arr; }
		
	//! Sets the download status for the given package.
	void SetDownloadStatus(const wxWebUpdatePackage &p, bool status);
	
	//! Sets the install status for the given package.
	void SetInstallStatus(const wxWebUpdatePackage &p, bool status);

private:
	DECLARE_CLASS(wxWebUpdateListCtrl)
	DECLARE_EVENT_TABLE()
};

#endif // _WX_WEBUPDATECTRL_H_

