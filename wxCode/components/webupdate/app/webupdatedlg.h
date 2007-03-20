/////////////////////////////////////////////////////////////////////////////
// Name:        webupdatedlg.h
// Purpose:     wxWebUpdateDlg, wxWebUpdateAdvPanel, wxWebUpdateAboutDlg
// Author:      Francesco Montorsi
// Created:     2005/06/28
// RCS-ID:      $Id: webupdatedlg.h 470 2007-03-20 11:35:51Z frm $
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_WEBUPDATEDLG_H_
#define _WX_WEBUPDATEDLG_H_

// wxWidgets headers
#include "wx/webupdate.h"
#include "wx/download.h"
#include "wx/webupdatectrl.h"
#include "wx/installer.h"
#include <wx/dialog.h>
#include <wx/panel.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/wfstream.h>

#if wxUSE_HTTPENGINE
    #include "wx/httpbuilder.h"
#endif

// defined later
class wxStaticText;
class wxButton;
class wxTextCtrl;
class wxGauge;


//! The prefix of the static text control which shows the remaining time.
#define wxWUD_TIMETEXT_PREFIX			_("Time remaining: ")

//! The prefix of the static text control which shows the current status.
#define wxWUD_PROGRESSTEXT_PREFIX			_("Download status: ")

//! The possible labels of the wxWUD_OK button.
#define wxWUD_OK_DEFAULT_LABEL			_("Download")
#define wxWUD_OK_INSTALL				_("Install")

//! The possible labels of the wxWUD_CANCEL button
#define wxWUD_CANCEL_DEFAULT_LABEL		_("Cancel")
#define wxWUD_CANCEL_DOWNLOAD			_("Stop download")
#define wxWUD_CANCEL_INSTALLATION		_("Stop installation")

//! The possible labels of the wxWUD_SHOWHIDEADV button
#define wxWUD_SHOWHIDEADV_SHOW			_("Show advanced settings >>")
#define wxWUD_SHOWHIDEADV_HIDE			_("Hide advanced settings <<")


//! Our wxWUD_GAUGE control accepts values from zero to wxWUD_GAUGE_RANGE.
#define wxWUD_GAUGE_RANGE				1000

//! The text prefix for the IDWUAD_VERSION.
#define wxWUAD_VERSIONSTR				_("This is WebUpdater version %s\nby Francesco Montorsi (c) 2005-2007")


//! The ID of the XML webupdate resource script.
#define wxWUD_XMLSCRIPT_ID				wxT("XMLSCRIPT")

//! The ID of the threads
#define wxWUD_INSTALL_THREAD_ID         654321
#define wxWUD_DOWNLOAD_THREAD_ID        123456


//! wxWebUpdateDlg and related classes are very flexible when loading the XRC resources:
//! if they find that some IDs do not exist (i.e. the user has removed some controls from
//! the XRC - he probably did not like them) they simply ignore them.
//! Since XRCCTRL macro can be used only for IDs that you are sure that exist in the XRC
//! (otherwise a wxStaticCast will throw an assert failure!), we need to use a custom
//! macro which is more flexible... and here comes in wxWU_XRCCTRL.
#define wxWU_XRCCTRL(window, id, type) \
    (wxDynamicCast((window).FindWindow(XRCID(id)), type))		// note the dynamic cast vs static cast



//! The possible values of the wxWebUpdateDlg::m_nStatus variable.
//! These should be used internally by wxWebUpdateDlg only and user
//! should not care about it; however they give a good idea of the
//! update process 'steps'.
//! \note We assign to these enum items values like those used typically
//!       for a variable which holds a bit mask.
//!       This is only to make it easier to write the wxWUD_CHECK_STATUS
//!       macro: wxWebUpdateDlg::m_nStatus can hold only *one* of the
//!       following values in each moment !
enum wxWebUpdateDlgStatus {

	//! The user must still hit the "Get update list" button and
	//! the XML remote script has not been loaded yet
	wxWUDS_WAITINGXML = 1,

	//! We are downloading the remote XML script.
	wxWUDS_DOWNLOADINGXML = 2,

	//! We have successfully loaded the remote XML script and we are
	//! waiting that the user chooses the packages to download & install
	wxWUDS_WAITING = 4,

	//! The user has chosen the packages to download & install and we
	//! are downloading them.
	wxWUDS_DOWNLOADING = 8,

	//! The download of *all* user-chosen packaged has been completed and
	//! we are installing them, one by one, in an order which is allowed
	//! by the package dependencies.
	wxWUDS_INSTALLING = 16,

	// when the wxWUDS_INSTALLING phase has been completed, then the
	// dialog will automatically exit if all packages are up2date;
	// otherwise it will return to the wxWUDS_WAITING status.
	wxWUDS_EXITING = 32
};



//! A container of the extra (extra referred to the options already present in the
//! local XML script) options supported by wxWebUpdate* classes and provided to
//! WebUpdater through command line options.
//! These options override the eventually same options specified in the local XML script.
/*class wxWebUpdateExtraOptions
{
public:

	//! TRUE if the application-to-update asked to be restarted at the end
	//! of the update process.
	bool m_bRestart;

	//! TRUE if the application-to-update asked to save a log of this
	//! update session.
	bool m_bSaveLog;

	//! The name of the XRC resource to load as main WebUpdater dialog.
	wxString m_strRes;

public:
	wxWebUpdateExtraOptions() { m_bRestart=m_bSaveLog=FALSE; }
	virtual ~wxWebUpdateExtraOptions() {}
};*/


//! The advanced panel of a wxWebUpdateDlg which contains all connection
//! settings options and some miscellaneous others.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateAdvPanel : public wxPanel
{
protected:		// pointers to our controls

	wxTextCtrl *m_pDownloadPathTextCtrl;
	wxCheckBox *m_pRemoveFiles, *m_pRestart, *m_pSaveLog;

#if wxUSE_HTTPENGINE
	wxProxySettings m_proxy;
	wxHTTPAuthSettings m_auth;
#endif

	//! The local XML script used to get some info.
	wxWebUpdateLocalXMLScript *m_xmlLocal;

protected:

	//! Loads the XRC for this dialog and init the control pointers.
	bool InitWidgetsFromXRC(wxWindow *parent);

protected:		// event handlers

	void OnBrowse(wxCommandEvent &);
	void OnProxySettings(wxCommandEvent &);
	void OnAuthSettings(wxCommandEvent &);
	void OnRestart(wxCommandEvent &);
	void OnSaveLog(wxCommandEvent &);

public:

	//! Constructs a wxWebUpdateAdvPanel; for two-step creation.
	wxWebUpdateAdvPanel::wxWebUpdateAdvPanel()
			: m_xmlLocal(NULL) {}

	//! Constructs a wxWebUpdateAdvPanel.
	wxWebUpdateAdvPanel::wxWebUpdateAdvPanel(wxWindow* parent)
			: m_xmlLocal(NULL)
  		{ Create(parent); }

	//! Creates this panel as child of the given window.
	bool Create(wxWindow* parent);


	virtual ~wxWebUpdateAdvPanel() {}


public:		// setters

 	void SetData(wxWebUpdateLocalXMLScript *script);

public:		// getters

#if wxUSE_HTTPENGINE
	//! Returns the updated proxy settings.
	wxProxySettings GetProxySettings() const
		{ return m_proxy; }

	//! Returns the updated HTTP authentication settings.
	wxHTTPAuthSettings GetHTTPAuthSettings() const
		{ return m_auth; }
#endif

	//! Returns the path chosen by the user for the downloaded file.
	//! This one is initialized to the temporary folder for the current user.
	wxString GetDownloadPath() const
		{ return m_pDownloadPathTextCtrl->GetValue(); }

	//! Returns TRUE if the user has chosen to remove the downloaded files.
	bool RemoveFiles() const
		{ if (m_pRemoveFiles) return m_pRemoveFiles->GetValue(); return TRUE; }

	//! Returns TRUE if the user has chosen to restart the updated application
 	//! when exiting WebUpdater.
	bool Restart() const
		{ if (m_pRestart) return m_pRestart->GetValue(); return TRUE; }


private:
	DECLARE_CLASS(wxWebUpdateAdvPanel)
	DECLARE_EVENT_TABLE()
};



//! The XRC handler for wxWebUpdateAdvPanel.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateAdvPanelXmlHandler : public wxXmlResourceHandler
{
	DECLARE_DYNAMIC_CLASS(wxWebUpdateAdvPanelXmlHandler)

public:
    wxWebUpdateAdvPanelXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};


//! The about dialog shown by wxWebUpdateDlg.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateAboutDlg : public wxDialog
{
protected:

	//! Loads the XRC for this dialog and init the control pointers.
	bool InitWidgetsFromXRC(wxWindow *parent);

public:
	wxWebUpdateAboutDlg::wxWebUpdateAboutDlg(wxWindow *parent)
		{ InitWidgetsFromXRC(parent); }
	virtual ~wxWebUpdateAboutDlg() {}

private:
	DECLARE_CLASS(wxWebUpdateAboutDlg)
	DECLARE_EVENT_TABLE()
};


//! The dialog which lets the user update this program.
//! NOTE: it's quite difficult to derive a new class from wxProgressDialog
//!       (which would be more suited rather than the generic wxDialog as
//!       base class) so we emulate some of wxProgressDialog features
//!       even if we are not using it.
class WXDLLIMPEXP_WEBUPDATE wxWebUpdateDlg : public wxDialog
{
protected:		// pointers to our controls

	wxStaticText *m_pAppNameText, *m_pTimeText, *m_pProgressText;
	wxButton *m_pOkBtn, *m_pCancelBtn, *m_pShowHideAdvBtn;
	wxGauge *m_pGauge;
	wxTextCtrl *m_pDescription, *m_pLog;
	wxCheckBox *m_pShowOnlyOOD;

    // used to check if inside wxWebUpdateDlg everything went good
    bool m_ok;

	// our listctrl
	wxWebUpdateListCtrl *m_pUpdatesList;

	// we store advanced settings here:
	wxWebUpdateAdvPanel *m_pAdvPanel;

	//! The number of files downloaded by our #m_dThread.
	//! Even if wxDownloadThread::GetDownloadCount() could be used for
	//! retrieving this info, we keep a separed field here so that we
	//! can remove from the filecount the webupdate XML remote script.
	//! We could also do a check in each UpdateUI event and avoid this
	//! field but it's much more easier to keep this and remove the check ;-)
	//! Also, using a separate variable, we can reset this value each time
	//! the user starts a new download+install process.
	int m_nDownloadCount;

	//! wxWebUpdateInstallThread::GetInstallationCount() could be used
	//! using a separate variable we can reset this value each time
	//! the user starts a new download+install process.
	int m_nInstallCount;

protected:		// XML scripts

	//! The webupdate XML script. This variable becomes valid only once
	//! the first download has been completed.
	wxWebUpdateXMLScript m_xmlRemote;

	//! The local XML script.
	wxWebUpdateLocalXMLScript m_xmlLocal;

protected:		// remote-related stuff

    //! This variable is continuosly checked by our download thread and
    //! we use it to communicate to it the PAUSE/CONTINUE/ABORT commands.
    wxDownloadThreadFlag m_dThreadFlag;

	//! The thread we use to install the packages.
	wxWebUpdateInstallThread *m_iThread;

	//! The package that we are currently downloading if m_nStatus == wxWUDS_DOWNLADING;
	//! the package that we are installing if m_nStatus == wxWUDS_INSTALLING.
	wxWebUpdatePackage *m_current;

    //! The locale to use when choosing among localized messages from the XML remote script.
    const wxLocale *m_pLocale;

protected:		// wxWebUpdateDlg-internals

	//! TRUE if we intentionally called wxDownloadThread::AbortDownload
	//! because user asked us to do so.
//	bool m_bUserAborted;

	//! The current status of the dialog. Used by the dialog functions to
	//! let #OnUpdateUI event handler know which label must be set in the
	//! various static text controls of the dialog.
	wxWebUpdateDlgStatus m_nStatus;

protected:      // properties of the file being downloaded
    
    // a descriptive name for the resource being downloaded
    wxString m_strResName;
    
    // put the downloaded stuff directly on a file on the hard disk:
    wxFileOutputStream *m_pOutputFile;

    // the name of the file where data being downloaded is written:
    // m_pOutputFile is a stream opened on this file:
    wxString m_strOutputFile;

protected:		// init helpers

	//! Loads the XRC for this dialog and init the control pointers.
	bool InitWidgetsFromXRC(wxWindow *parent);

	//! Initializes the threads.
	bool InitThreads();

protected:		// utilities

	//! Shows or hides the child window with the given name and then returns
	//! a pointer to it.
	wxWindow *ShowHideChild(const wxString &name);

	//! Returns the name of the local file where the given remote package
	//! will be/has been saved after the download.
	wxString GetOutputFilenameFor(const wxWebUpdatePackage &p);

protected:		// pseudo event handlers

    //! Low level utility which launchs a new wxDownloadThread
    void StartDownload(const wxString &uri);

	//! Called when the XML script file has been downloaded.
	void AfterScriptDownload(const wxString &xmluri);

	//! Downloads the first item in the listctrl which is checked and which
	//! has not been already downloaded.
	//! Returns TRUE if there were checked & not-downloaded packages.
	bool DownloadNextPackage();

	//! Installs the first item in the listctrl which is checked and has been
	//! downloaded.
	//! Returns TRUE if there were a package to install.
	bool InstallNextPackage();

	//! Checks if all packages are up to date and shows the right message to the
	//! user in the two cases (yes, all updated / no, someone needs to be updated).
	bool CheckForAllUpdated(wxWebUpdatePackageArray &arr, bool forcedefaultmsg = FALSE);

    //! Filters out all those packages which don't have a download suitable for the
    //! current platform. Also those packages which depend from a non-available
    //! package are filtered out.
    //! Returns TRUE if there are no packages left in the given array.
    bool FilterOtherPlatforms(wxWebUpdatePackageArray &arr);

	//! Removes the current package if the user has chosen to do that after
	//! that the current package has been installed.
	void RemoveCurrentPackage();

protected:		// event handlers

	void OnDownload(wxCommandEvent &);
	void OnCancel(wxCommandEvent &);
	void OnAbout(wxCommandEvent &);
	void OnShowFilter(wxCommandEvent &);
	void OnShowHideAdv(wxCommandEvent &);
	void OnTextURL(wxTextUrlEvent &);
	void OnSize(wxSizeEvent &);
    void OnPackageSelected(wxListEvent &ev);

	// for event raised by our wxDownloadThread....
    void OnDownloadConnect(wxDownloadEvent &);
    void OnDownloadComplete(wxDownloadEvent &);
    void OnDownloadUpdate(wxDownloadEvent &);
    void OnDownloadAbort(wxDownloadEvent &);
    void OnDownloadUserAbort(wxDownloadEvent &);

	// for event raised by our wxWebUpdateInstallThread...
	void OnInstallationComplete(wxCommandEvent &);

public:

	//! For two-step creations.
	wxWebUpdateDlg::wxWebUpdateDlg() { PreInit(); }

	//! Constructs a wxWebUpdateDlg.
	//! This function does not take so many parameters like a typical
	//! wxWindow or wxDialog does because it uses XRC to set all the
	//! attributes: size, position, id, names...
	//! NOTE: the line
	//!         wxDialog(parent, id, title, pos, size, style, name)
	//! is not required since we are using XRC system
	wxWebUpdateDlg::wxWebUpdateDlg(
 						wxWindow *parent,
						const wxWebUpdateLocalXMLScript &script,
                        const wxLocale *lang = NULL);

	//! Inits the values of the pointers and others var to NULL.
	void PreInit();

	//! Creates the dialog.
	bool Create(wxWindow *parent,
				const wxWebUpdateLocalXMLScript &script,
                const wxLocale *lang = NULL);

    //! Returns TRUE if in the creation process everything was successful.
    bool IsOk() const
        { return m_ok; }

	virtual ~wxWebUpdateDlg()
		{ wxDELETE(m_iThread); }


public:		// main functions

	//! Checks that the user is connected to internet.
	//! Aborts this dialog if not and in this case returns FALSE.
	bool ConnectionRequired();

	//! Call this function instead of EndModal(wxCANCEL) to abort this dialog.
	//! This function takes care of our wxDownloadThread, infact.
	void AbortDialog();

	//! Returns the array of updated packages parsed from the WebUpdate XML Script.
	wxWebUpdatePackageArray &GetRemotePackages()
		{ return m_pUpdatesList->GetRemotePackages(); }

	//! Returns the array of local packages taken from wxWebUpdater.
	wxWebUpdateLocalPackageArray &GetLocalPackages()
		{ return m_pUpdatesList->GetLocalPackages(); }

	//! Sets the local XML script which acts as configuration file for the
	//!	entire dialog (it contains the name of the application to update and
	//! the XRC resource which should be used for this dialog).
	void SetLocalScript(const wxWebUpdateLocalXMLScript &script)
		{ m_xmlLocal = script; }

	//! Returns the name of the application to update.
	wxString GetAppName() const
		{ return m_xmlLocal.GetAppName(); }

	//! Returns TRUE if the application updated should be restarted.
	bool IsAppToRestart() const
		{ return m_xmlLocal.IsAppToRestart(); }

	//! Returns the filter currently in use for the package listctrl.
	wxWebUpdateListCtrlFilter GetPackageFilter() const;

private:
	DECLARE_CLASS(wxWebUpdateDlg)
	DECLARE_EVENT_TABLE()
};


#endif // _WX_WEBUPDATEDLG_H_

