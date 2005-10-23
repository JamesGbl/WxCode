/////////////////////////////////////////////////////////////////////////////
// Name:        webupdatedlg.cpp
// Purpose:     wxWebUpdateDlg, wxWebUpdateAdvPanel, wxWebUpdateAboutDlg
// Author:      Francesco Montorsi
// Created:     2005/06/23
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////



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
#include <wx/tokenzr.h>
#include <wx/settings.h>

#if wxUSE_HTTPENGINE
    #include <wx/proxysettingsdlg.h>
    #include <wx/authdlg.h>
#endif


// wxWidgets RTTI
IMPLEMENT_CLASS(wxWebUpdateAdvPanel, wxPanel)
IMPLEMENT_CLASS(wxWebUpdateDlg, wxDialog)
IMPLEMENT_CLASS(wxWebUpdateAboutDlg, wxDialog)

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
    EVT_SIZE(wxWebUpdateDlg::OnSize)

    // miscellaneous
    EVT_TEXT_URL(XRCID("IDWUD_DESCRIPTION"), wxWebUpdateDlg::OnTextURL)

    // from threads
    EVT_DOWNLOAD_COMPLETE(-1, wxWebUpdateDlg::OnDownloadComplete)
    EVT_INSTALLATION_COMPLETE(-1, wxWebUpdateDlg::OnInstallationComplete)

END_EVENT_TABLE()

BEGIN_EVENT_TABLE(wxWebUpdateAboutDlg, wxDialog)

    // link events will be added when wxWidgets will give a wxHyperlink control

END_EVENT_TABLE()

BEGIN_EVENT_TABLE(wxWebUpdateAdvPanel, wxPanel)

    // buttons
EVT_BUTTON(XRCID("IDWUAP_BROWSE"), wxWebUpdateAdvPanel::OnBrowse)
EVT_BUTTON(XRCID("IDWUAP_PROXYSETTINGS"), wxWebUpdateAdvPanel::OnProxySettings)
EVT_BUTTON(XRCID("IDWUAP_AUTHSETTINGS"), wxWebUpdateAdvPanel::OnAuthSettings)

// checkboxes
EVT_CHECKBOX(XRCID("IDWUAP_RESTART"), wxWebUpdateAdvPanel::OnRestart)
EVT_CHECKBOX(XRCID("IDWUAP_SAVELOG"), wxWebUpdateAdvPanel::OnSaveLog)

END_EVENT_TABLE()




// -----------------------
// GLOBALS
// -----------------------

bool g_processingEvent = FALSE;

void StopUIUpdates()
{ g_processingEvent = TRUE; }

void RestoreUIUpdates()
{ g_processingEvent = FALSE; }

class wxUIUpdateStopper
{
public:
    wxUIUpdateStopper() { StopUIUpdates(); }
    virtual ~wxUIUpdateStopper() { RestoreUIUpdates(); }
};

// a little utility function
wxString wxWUDGetStatus(wxWebUpdateDlgStatus s)
{
    switch (s) {
    case wxWUDS_DOWNLOADING:
        return wxT("wxWUDS_DOWNLOADING");
    case wxWUDS_DOWNLOADINGXML:
        return wxT("wxWUDS_DOWNLOADINGXML");
    case wxWUDS_WAITING:
        return wxT("wxWUDS_WAITING");
    case wxWUDS_WAITINGXML:
        return wxT("wxWUDS_WAITINGXML");
    case wxWUDS_EXITING:
        return wxT("wxWUDS_EXITING");
    default:
        return wxString::Format(wxT("unknown: %d"), (int)s);
    }
}


// a little utility macro
#define wxWUD_CHECK_STATUS(allowed)                                 \
    { wxASSERT_MSG(m_nStatus & (allowed),                           \
                wxT("invalid status mode (") +                      \
                wxWUDGetStatus(m_nStatus) + wxT(")")); }






// ---------------------
// wxWEBUPDATEDLG
// ---------------------

wxWebUpdateDlg::wxWebUpdateDlg(wxWindow *parent,
                                const wxWebUpdateLocalXMLScript &script)
                            : wxDialog()
{
    PreInit();
    Create(parent, script);
}

void wxWebUpdateDlg::PreInit()
{
    // miscellaneous
    m_bUserAborted=FALSE;
    m_nStatus=wxWUDS_WAITINGXML;
    m_nDownloadCount=0;

    // pointers
    m_dThread = NULL;
    m_iThread = NULL;

    m_pAppNameText = NULL;
    m_pSpeedText = NULL;
    m_pTimeText = NULL;
    m_pGauge = NULL;
    m_pDescription = NULL;
    m_pShowOnlyOOD = NULL;
    m_pLog = NULL;
    m_pUpdatesList = NULL;
    m_pAdvPanel = NULL;
    m_pOkBtn = NULL;
    m_pCancelBtn = NULL;
    m_pShowHideAdvBtn = NULL;
}

bool wxWebUpdateDlg::Create(wxWindow *parent,
                            const wxWebUpdateLocalXMLScript &script)
{
    // copy the local XML script
    m_xmlLocal = script;

    // init our GUI
    if (!InitWidgetsFromXRC(parent))
        return FALSE;

    // and then our worker threads
    if (!InitThreads())
        return FALSE;

    return TRUE;
}

bool wxWebUpdateDlg::InitWidgetsFromXRC(wxWindow *parent)
{
    // be sure local XML script was correctly set
    // ------------------------------------------

    wxASSERT_MSG(m_xmlLocal.IsOk(),
        wxT("You must provide a valid XML local script before building a wxWebUpdateDlg"));


    // build the dialog
    // ----------------

    wxString res = m_xmlLocal.GetXRCResName();
    wxLogAdvMsg(wxT("wxWebUpdateDlg::InitWidgetsFromXRC - loading the [") + res + wxT("] resource..."));

    if (!wxXmlResource::Get()->LoadDialog(this, parent, res)) {
        wxLogAdvMsg(wxT("Error while building wxWebUpdateDlg; ")
            wxT("check that the given XRC (webupdatedlg.xrc by default) is valid !"));

        return FALSE;
    }



    // init control pointers
    // ---------------------

    m_pAppNameText = wxWU_XRCCTRL(*this, "IDWUD_APPNAME_TEXT", wxStaticText);       // can be NULL
    m_pSpeedText = wxWU_XRCCTRL(*this, "IDWUD_PROGRESS_TEXT", wxStaticText);
    m_pTimeText = wxWU_XRCCTRL(*this, "IDWUD_TIME_TEXT", wxStaticText);
    m_pGauge = wxWU_XRCCTRL(*this, "IDWUD_GAUGE", wxGauge);
    m_pDescription = wxWU_XRCCTRL(*this, "IDWUD_DESCRIPTION", wxTextCtrl);  // can be NULL
    m_pShowOnlyOOD = wxWU_XRCCTRL(*this, "IDWUD_SHOWFILTER", wxCheckBox);
    m_pLog = wxWU_XRCCTRL(*this, "IDWUD_LOG", wxTextCtrl);  // can be NULL

    // these two *CANNOT* be NULL, just hidden
    m_pUpdatesList =  wxWU_XRCCTRL(*this, "IDWUD_LISTCTRL", wxWebUpdateListCtrl);
    m_pAdvPanel = wxWU_XRCCTRL(*this, "IDWUD_ADVPANEL", wxWebUpdateAdvPanel);

    // our buttons
    m_pOkBtn = wxWU_XRCCTRL(*this,"IDWUD_OK", wxButton);
    m_pCancelBtn = wxWU_XRCCTRL(*this,"IDWUD_CANCEL", wxButton);
    m_pShowHideAdvBtn = wxWU_XRCCTRL(*this, "IDWUD_SHOWHIDEADV", wxButton); // can be NULL




    // init control data
    // -----------------

    // update core controls
    wxASSERT_MSG(m_pUpdatesList && m_pAdvPanel, wxT("Cannot be NULL, just hidden"));
    m_pUpdatesList->SetLocalPackages(m_xmlLocal.GetAllPackages());
    m_pAdvPanel->SetData(&m_xmlLocal);

    // aesthetic
    if (m_pAppNameText)
        m_pAppNameText->SetLabel(GetAppName());
    if (m_pShowHideAdvBtn) {

        wxCommandEvent fake;
        OnShowHideAdv(fake);    // begin with adv options hidden
    }
    if (m_pLog) {

        // cannot use wxDynamicCast since wxLog does not derive from wxObject
        wxWebUpdateLog *logger = wx_static_cast(wxWebUpdateLog*, wxLog::GetActiveTarget());
        if (logger) logger->WriteUsrMsgAlsoToTextCtrl(m_pLog);
    }

    // this is a little hardcoded value to make wxWebUpdateSimpleDlg looks nicer...
    wxSize sz = GetMinSize();
    int w = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
    if (w != -1) sz.SetWidth((int)(w*0.6));
    SetMinSize(sz);



    // relayout
    // --------

    // we have changed the appname statictext control so maybe we need
    // to expand our dialog... force a layout recalculation
    GetSizer()->CalcMin();
    GetSizer()->Layout();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);

    return TRUE;
}

bool wxWebUpdateDlg::ConnectionRequired()
{
    wxLogAdvMsg(wxT("wxWebUpdateDlg::ConnectionRequired - creating the wxDialUpManager"));
    bool res = FALSE;

    // are we connected ?
    wxDialUpManager *mng = wxDialUpManager::Create();
    if (mng->IsOk()) {

        if (!mng->IsOnline()) {

            wxMessageBox(wxT("You are not connected to Internet !\n")
                        wxT("WebUpdater needs to download the update list from the World Wide Web...\n")
                        wxT("Please connect and then retry."),
                        wxT("Error"), wxOK | wxICON_ERROR);
            wxLogUsrMsg(wxT("wxWebUpdateDlg::ConnectionRequired - not connected to Internet !"));
            res = FALSE;

        } else {

            wxLogUsrMsg(wxT("wxWebUpdateDlg::ConnectionRequired - found a valid connection"));
            res = TRUE;
        }
    }

    // cleanup & exit
    delete mng;
    return res;
}

bool wxWebUpdateDlg::InitThreads()
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
        wxLogAdvMsg(wxT("wxWebUpdateDlg::InitThreads - cannot run the download thread !"));
        AbortDialog();

        return FALSE;
    }

    // init also our installer thread
    if (m_iThread->Create() != wxTHREAD_NO_ERROR ||
        m_iThread->Run() != wxTHREAD_NO_ERROR) {
        wxMessageBox(wxT("Low resources; cannot run the WebUpdate dialog...\n")
            wxT("Close some applications and then retry."),
            wxT("Error"), wxOK | wxICON_ERROR);
        wxLogAdvMsg(wxT("wxWebUpdateDlg::InitThreads - cannot run the installer thread !"));
        AbortDialog();

        return FALSE;
    }

    wxLogAdvMsg(wxT("wxWebUpdateDlg::InitThreads - successfully initialized the download & install threads..."));
    return TRUE;
}

bool wxWebUpdateDlg::CheckForAllUpdated(bool forcedefaultmsg)
{
    wxWebUpdatePackageArray &arr = GetRemotePackages();
    bool allupdated = TRUE;

    for (int j=0; j < (int)arr.GetCount(); j++) {
        if (m_pUpdatesList->IsPackageUp2date(arr[j]) != wxWUCF_UPDATED) {
            allupdated = FALSE;
            break;      // not all packages are uptodate
        }
    }

    if (allupdated) {

        // CHANGE OUR STATUS
        m_nStatus = wxWUDS_WAITING;

        // show to the user the "update not available" message
        wxString defaultmsg = wxT("You have the latest version of all packages of ") +
                            GetAppName() + wxT("... exiting the update dialog.");
        wxString usermsg = m_xmlRemote.GetUpdateNotAvailableMsg();
        wxWebUpdateInstaller::Get()->ShowNotificationMsg((usermsg.IsEmpty() || forcedefaultmsg) ? defaultmsg : usermsg);

        AbortDialog();
        return TRUE;        // TRUE = exit this dialog

    } else {

        // some updates are available... show the message only if the webupdate
        // script contains an explicit user-customized message for this evenience
        wxString usermsg = m_xmlRemote.GetUpdateAvailableMsg();
        if (!usermsg.IsEmpty())
            wxWebUpdateInstaller::Get()->ShowNotificationMsg(usermsg);
    }

    return FALSE;           // FALSE = do not exit the dialog
}

wxWebUpdateListCtrlFilter wxWebUpdateDlg::GetPackageFilter() const
{
    // if the "show only out of date" checkbox exists in the XRC...
    if (m_pShowOnlyOOD)
        return m_pShowOnlyOOD->GetValue() ? wxWULCF_ONLY_OUTOFDATE : wxWULCF_ALL;

    // if not...
    return wxWULCF_ONLY_OUTOFDATE;
}

void wxWebUpdateDlg::OnScriptDownload(const wxString &xmluri)
{
    // remove this download from our counter
    wxASSERT_MSG(m_nDownloadCount == 1,
        wxT("Only the WebUpdate XML script should have been downloaded"));
    m_nDownloadCount--;

    // ok, we can now parse the XML doc
    if (!m_xmlRemote.Load(xmluri)) {
        wxWebUpdateInstaller::Get()->ShowErrorMsg(wxT("Cannot parse the XML update script downloaded as: ") +
                        m_dThread->m_strOutput);
        AbortDialog();      // this is a unrecoverable error !
        return;
    }

    // update our remote package array (which is contained in
    // our wxWebUpdateListCtrl to be exact)
    wxLogUsrMsg(wxT("wxWebUpdateDlg::OnScriptDownload - XML script loaded successfully"));
    m_pUpdatesList->SetRemotePackages(m_xmlRemote.GetAllPackages());
    m_pUpdatesList->RebuildPackageList(GetPackageFilter());

    // is everything up to date ?
    if (CheckForAllUpdated())
        return;     // all is updated, we must exit this dialog

    // what if we could not found any valid package in the webupdate script ?
    if (m_pUpdatesList->GetItemCount() == 0) {

        wxWebUpdateInstaller::Get()->ShowNotificationMsg(wxT("Could not found any valid package for ") + GetAppName()
                    + wxT(" in the WebUpdate script. Exiting the update dialog."),
                    wxT("Warning"));
        AbortDialog();
        return;
    }

    // we have successfully complete step #1
    wxASSERT_MSG(m_nStatus == wxWUDS_DOWNLOADINGXML,
                wxT("Wrong status setting !"));
    m_nStatus = wxWUDS_WAITING;         // CHANGE OUR STATUS
}

void wxWebUpdateDlg::AbortDialog()
{
    wxLogAdvMsg(wxT("wxWebUpdateDlg::AbortDialog - stopping the download thread"));
    if (m_dThread->IsPaused())
        m_dThread->Run();           // we need the thread running if we want to delete it !
    if (m_dThread->IsRunning())
        m_dThread->Delete();
    wxLogAdvMsg(wxT("wxWebUpdateDlg::AbortDialog - stopped & deleted the download thread"));

    wxLogAdvMsg(wxT("wxWebUpdateDlg::AbortDialog - stopping the installer thread"));
    if (m_iThread->IsPaused())
        m_iThread->Run();           // we need the thread running if we want to delete it !
    if (m_iThread->IsRunning())
        m_iThread->Delete();
    wxLogAdvMsg(wxT("wxWebUpdateDlg::AbortDialog - stopped & deleted the installer thread"));

    m_nStatus = wxWUDS_EXITING;
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

wxString wxWebUpdateDlg::GetOutputFilenameFor(const wxWebUpdatePackage &p)
{
    wxWebUpdateDownload &dl = p.GetDownload();
    wxASSERT(dl.IsOk());
    return m_pAdvPanel->GetDownloadPath() + dl.GetFileName();
}

bool wxWebUpdateDlg::DownloadNextPackage()
{
    wxASSERT_MSG(m_nStatus == wxWUDS_DOWNLOADING,
                    wxT("Invalid state mode"));

    // be sure not to access directly the listctrl array of packages;
    // this is required since we cannot use it safely:
    // a filter could have been applied on the listctrl and thus
    // there maybe some hidden packages... leave all this handling to our
    // listctrl class...
    m_current = m_pUpdatesList->GetNextPackageToDownload();
    if (m_current == NULL) return FALSE;        // no other packages to download
    wxWebUpdateDownload &dl = m_current->GetDownload();

    // init thread variables
    m_dThread->m_strOutput = GetOutputFilenameFor(*m_current);
    m_dThread->m_strURI = dl.GetDownloadString();
    m_dThread->m_strMD5 = dl.GetMD5Checksum();
    m_dThread->m_strResName = m_current->GetName() + wxT(" package");
    m_dThread->m_strID = m_current->GetName();

    // reset the gauge GUI
    m_pGauge->SetValue(0);
    m_pGauge->SetRange(dl.GetDownloadSize());

    // (eventually) check that we are online
    if (wxIsWebProtocol(m_dThread->m_strURI))
        if (!ConnectionRequired())
            return FALSE;       // not connected...

    // launch the download
    m_dThread->BeginNewDownload();
    wxLogUsrMsg(wxT("wxWebUpdateDlg::DownloadNextPackage - launching download of ") +
            m_dThread->m_strURI);

    return TRUE;
}

bool wxWebUpdateDlg::InstallNextPackage()
{
    wxASSERT_MSG(m_nStatus == wxWUDS_INSTALLING,
                    wxT("Invalid state mode"));

    m_current = m_pUpdatesList->GetNextPackageToInstall();
    if (m_current == NULL) return FALSE;        // no other packages to install
    wxWebUpdateDownload &download = m_current->GetDownload();

    // launch the installation
    m_iThread->m_pDownload = &download;
    m_iThread->m_strUpdateFile = GetOutputFilenameFor(*m_current);
    m_iThread->BeginNewInstall();
    wxLogUsrMsg(wxT("wxWebUpdateDlg::InstallNextPackage - launching installation of ") +
            m_iThread->m_strUpdateFile);

    return TRUE;
}

void wxWebUpdateDlg::RemoveCurrentPackage()
{
    // if the user wants to remove the update packages, do it now
    if (m_pAdvPanel->RemoveFiles()) {

        wxASSERT(m_current);

        wxString file = GetOutputFilenameFor(*m_current);
        wxLogAdvMsg(wxT("wxWebUpdateDlg::RemoveCurrentPackage - removing the package ")
                    + m_current->GetName() + wxT(" [") + file + wxT("]"));

        wxRemoveFile(file);
    }
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

void wxWebUpdateDlg::OnSize(wxSizeEvent &ev)
{
    if (m_pUpdatesList && m_pUpdatesList->IsShown())
        m_pUpdatesList->AdjustColumnSizes();

    // very important ! otherwise sizer won't work
    ev.Skip();
}

void wxWebUpdateDlg::OnDownload(wxCommandEvent &)
{
    wxASSERT_MSG(!m_dThread->IsDownloading(),
        wxT("The wxWUD_OK button should be disabled !"));

#if wxUSE_HTTPENGINE
    // first update the advanced options
    m_dThread->m_proxy = m_pAdvPanel->GetProxySettings();
    m_dThread->m_auth = m_pAdvPanel->GetHTTPAuthSettings();
#endif

    // clear old file counts
    m_nDownloadCount = 0;
    m_nInstallCount = 0;

    // check if we already downloaded the XML webupdate script
    if (m_nStatus == wxWUDS_WAITINGXML) {

        wxASSERT_MSG(!m_xmlRemote.IsOk(),
                    wxT("The XML remote script should have not been loaded yet !"));

        if (wxIsWebProtocol(m_xmlLocal.GetRemoteScriptURI()))
            if (!ConnectionRequired())
                return;         // not connected

        // keep our status in wxWUDS_WAITINGXML even if we are downloading the XML
        m_dThread->m_strURI = m_xmlLocal.GetRemoteScriptURI();
        m_dThread->m_strResName = wxT("XML WebUpdate script");
        m_dThread->m_strOutput = wxFileName::CreateTempFileName(wxT("webupdate"));
        m_dThread->m_strID = wxWUD_XMLSCRIPT_ID;
        m_dThread->BeginNewDownload();

        // CHANGE OUR STATUS
        m_nStatus = wxWUDS_DOWNLOADINGXML;
        UpdateWindowUI();
        return;
    }

    // safety checks
    wxWUD_CHECK_STATUS(wxWUDS_WAITING);
    wxASSERT_MSG(m_xmlRemote.IsOk(),
                    wxT("The XML remote script should have been already loaded !"));

    // CHANGE OUR STATUS
    m_nStatus = wxWUDS_DOWNLOADING;

    // launch the first download of the list
#ifdef __WXDEBUG__
    bool atleastone =           // avoid warnings in release mode
#endif
        DownloadNextPackage();
    wxASSERT_MSG(atleastone, wxT("The wxWUD_OK button should be enabled only when ")
                            wxT("one or more packages are ready for download"));

    // FIXME: is this required ?
    UpdateWindowUI();
}

void wxWebUpdateDlg::OnCancel(wxCommandEvent &)
{
    wxLogUsrMsg(wxT("wxWebUpdateDlg::OnCancel - user hit the cancel button..."));

    if (m_nStatus == wxWUDS_DOWNLOADING || m_nStatus == wxWUDS_DOWNLOADINGXML) {

        // we are now labeled as wxWUD_CANCEL_DOWNLOAD...
        // thus we only stop the download
        if (m_dThread->IsDownloading()) {
            m_bUserAborted = TRUE;
            m_dThread->AbortDownload();
        }

        // CHANGE OUR STATUS
        if (m_nStatus == wxWUDS_DOWNLOADING) m_nStatus = wxWUDS_WAITING;
        if (m_nStatus == wxWUDS_DOWNLOADINGXML) m_nStatus = wxWUDS_WAITINGXML;

        return;

    } else if (m_nStatus == wxWUDS_INSTALLING) {

        // we are now labeled as wxWUD_CANCEL_INSTALLATION...
        // thus we only stop the installation
        m_iThread->AbortInstall();
        return;
    }

    // we are in wxWUDS_WAITING or wxWUDS_WAITINGXML mode
    AbortDialog();
}

void wxWebUpdateDlg::OnAbout(wxCommandEvent &)
{
    wxWebUpdateAboutDlg dlg(this);
    dlg.CenterOnScreen();
    dlg.ShowModal();
}

void wxWebUpdateDlg::OnShowFilter(wxCommandEvent &)
{
    // hide/show items in the listctrl
    m_pUpdatesList->RebuildPackageList(GetPackageFilter());
}

void wxWebUpdateDlg::OnShowHideAdv(wxCommandEvent &)
{
    // since our wxWebUpdateAdvPanel is built using a <unknown> tag
    // we need to show/hide both the real panel and its container.
    wxWindow *p = ShowHideChild(wxT("IDWUD_ADVPANEL"));
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
    // we need to change our internal status variable before any other UI update event occurs
    wxUIUpdateStopper stopper;

    bool downloadingScript = (m_nStatus == wxWUDS_DOWNLOADINGXML);
#ifdef __WXDEBUG__
    if (downloadingScript)
        wxASSERT(m_dThread->m_strID == wxWUD_XMLSCRIPT_ID);
#endif

    // first of all, we need to know if download was successful
    if (!m_dThread->DownloadWasSuccessful()) {

        // CHANGE OUR STATUS back to wait mode
        if (downloadingScript) m_nStatus = wxWUDS_WAITINGXML;
        if (!downloadingScript) m_nStatus = wxWUDS_WAITING;

        if (m_bUserAborted)
            wxWebUpdateInstaller::Get()->ShowNotificationMsg(wxT("Download aborted..."), wxT("Warning"));
        else
            wxWebUpdateInstaller::Get()->ShowErrorMsg(wxT("Could not download the ") + m_dThread->m_strResName +
                    wxT(" from\n\n") + m_dThread->m_strURI + wxT("\n\nURL... "));

        if (downloadingScript) {
            wxLogUsrMsg(wxT("wxWebUpdateDlg::OnDownloadComplete - failed while downloading the XML script... aborting dialog"));
            AbortDialog();      // this is a unrecoverable error !
        }

        m_bUserAborted = FALSE;     // reset flag
        return;

    } else {

        wxLogUsrMsg(wxT("wxWebUpdateDlg::OnDownloadComplete - Download status: successfully completed"));
        m_nDownloadCount++;

        if (downloadingScript) {

            // handle the XML parsing & control update
            OnScriptDownload(m_dThread->m_strOutput);

        } else {

            if (m_dThread->m_strMD5.IsEmpty() || m_dThread->IsMD5Ok()) {

                wxASSERT_MSG(!m_pUpdatesList->IsDownloaded(*m_current) &&
                            !m_pUpdatesList->IsInstalled(*m_current),
                    wxT("We are downloading/installing the same package again ?"));
                m_pUpdatesList->SetDownloadStatus(*m_current, TRUE);

                // if this is the last package which must be downloaded
                // then we can begin the installation
                if (!DownloadNextPackage()) {

                    // the order of the package installation cannot be
                    // randomly chosen
                    wxLogUsrMsg(wxT("wxWebUpdateDlg::OnDownloadComplete - beginning installation of downloaded packages"));
                    m_nStatus = wxWUDS_INSTALLING;  // CHANGE OUR STATUS
                    InstallNextPackage();
                }

            } else {

                wxWebUpdateInstaller::Get()->ShowErrorMsg(wxT("The downloaded file \"") + m_dThread->m_strOutput +
                        wxT("\"\nis corrupted. MD5 checksum is:\n\n\t") +
                        m_dThread->GetComputedMD5() +
                        wxT("\n\ninstead of:\n\n\t") +
                        m_dThread->m_strMD5 +
                        wxT("\n\nPlease retry the download."));
                m_pUpdatesList->SetDownloadStatus(*m_current, FALSE);

                // CHANGE OUR STATUS back to wait mode
                m_nStatus = wxWUDS_WAITING;
            }
        }
    }
}

void wxWebUpdateDlg::OnInstallationComplete(wxCommandEvent &)
{
    // we need to change our internal status variable before any other UI update event occurs
    wxUIUpdateStopper stopper;

    m_nInstallCount++;

    if (m_iThread->InstallationWasSuccessful()) {

        m_pUpdatesList->SetInstallStatus(*m_current, TRUE);

        // find the installed package
        wxWebUpdateLocalPackageArray arr(GetLocalPackages());       // do a copy
        wxLogUsrMsg(wxT("wxWebUpdateDlg::OnInstallationComplete - completed installation of \"")
                    + m_current->GetName() + wxT("\"..."));

        // update the version fields for the local package...
        int found = wxNOT_FOUND;
        for (int j=0; j < (int)arr.GetCount(); j++) {
            if (arr[j].GetName() == m_current->GetName()) {
                found = j;
                break;
            }
        }
        if (found == wxNOT_FOUND)
            // this package was not previously installed... add it
            arr.Add(wxWebUpdateLocalPackage(m_current->GetName(),
                                        m_current->GetLatestVersion()));
        else
            // update the local package info since we updated it
            for (int i=0; i < (int)arr.GetCount(); i++)
                if (arr[i].GetName() == m_current->GetName())
                    arr[i].SetVersion(m_current->GetLatestVersion());

        // update & save the XML file
        m_xmlLocal.SetPackageArray(arr);
        if (!m_xmlLocal.Save()) {

            // don't save the new version in the listctrl array
            // since something went wrong...

        } else {

            // save the new local array in our listctrl
            m_pUpdatesList->SetLocalPackages(arr);

            // and update the listctrl versions
            m_pUpdatesList->UpdatePackagesVersions(GetPackageFilter());
        }

        // if the user asked to remove the downloaded packages, do it now
        RemoveCurrentPackage();

        // proceed with next
        if (!InstallNextPackage()) {

            wxLogUsrMsg(wxT("wxWebUpdateDlg::OnInstallationComplete - completed the installation of all packages"));

            // notify the user
            wxWebUpdateInstaller::Get()->ShowNotificationMsg(wxT("The installation of the selected packages has been completed successfully !"));

            // CHANGE OUR STATUS back to wait mode
            m_nStatus = wxWUDS_WAITING;
            CheckForAllUpdated(TRUE);
        }

    } else {

        // warn the user
        m_pUpdatesList->SetInstallStatus(*m_current, TRUE);
        wxWebUpdateInstaller::Get()->ShowErrorMsg(wxT("The downloaded package \"") + m_dThread->m_strOutput +
                        wxT("\"\ncould not be installed.") +
                        wxT("\n\nPlease contact the support team for more info."));
        RemoveCurrentPackage();

        // CHANGE OUR STATUS back to wait mode
        m_nStatus = wxWUDS_WAITING;
    }
}

void wxWebUpdateDlg::OnUpdateUI(wxUpdateUIEvent &)
{
    // help us to avoid useless calls to SetLabel() function which
    // cause flickering; the initial value ensures that at least a
    // label update takes place when showing the dialog
    static int nLabelMode = 0xFFFFFF;
    static wxDateTime lastupdate = wxDateTime::UNow();

    // special
    if (m_nStatus == wxWUDS_EXITING || g_processingEvent == TRUE)
        return;

    // change the description label eventually
    if (m_pDescription && m_pUpdatesList->GetSelectedItemCount() > 0) {

        static int firstselected = -1;

        for (int i=0; i<m_pUpdatesList->GetItemCount(); i++) {
            if (m_pUpdatesList->GetItemState(i, wxLIST_STATE_SELECTED)) {

                if (firstselected == i) break;

                // show the description of the first selected item
                wxString name = m_pUpdatesList->GetItemText(i);
                wxWebUpdatePackage &pkg = m_pUpdatesList->GetRemotePackage(name);

                wxLogAdvMsg(wxT("wxWebUpdateDlg::OnUpdateUI - updating the description with value: ") + pkg.GetDescription());
                m_pDescription->SetValue(pkg.GetDescription());
                firstselected = i;
                break;
            }
        }
    }

#ifdef __WXDEBUG__
    // check our state var looking at the thread status
    /*if (m_dThread->IsDownloading() || m_dThread->IsComputingMD5())
        wxWUD_CHECK_STATUS(wxWUDS_DOWNLOADINGXML | wxWUDS_DOWNLOADING)
    else
        wxWUD_CHECK_STATUS(~(wxWUDS_DOWNLOADINGXML | wxWUDS_DOWNLOADING))

    if (m_iThread->IsInstalling())
        wxWUD_CHECK_STATUS(wxWUDS_INSTALLING)
    else
        wxWUD_CHECK_STATUS(~wxWUDS_INSTALLING)*/
#endif

    // change UI labels according to the current status
    if (m_nStatus == wxWUDS_DOWNLOADING ||
        m_nStatus == wxWUDS_DOWNLOADINGXML) {

        // need to change labels ?
        if (m_dThread->IsDownloading() && nLabelMode != wxWUDS_DOWNLOADING) {
            m_pOkBtn->Disable();
            m_pCancelBtn->SetLabel(wxWUD_CANCEL_DOWNLOAD);
            m_pUpdatesList->Lock();

            nLabelMode = wxWUDS_DOWNLOADING;
        }
        if (m_dThread->IsComputingMD5() && nLabelMode != wxDTS_COMPUTINGMD5) {
            m_pSpeedText->SetLabel(wxWUD_SPEEDTEXT_PREFIX wxT("computing the file hash"));
            m_pTimeText->SetLabel(wxT("No downloads running..."));
            m_pCancelBtn->SetLabel(wxWUD_CANCEL_DOWNLOAD);
            m_pOkBtn->Disable();

            nLabelMode = wxDTS_COMPUTINGMD5;
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

    } else if (m_nStatus == wxWUDS_INSTALLING) {

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
        int d = m_nDownloadCount,           // see #m_nFileCount description
            i = m_nInstallCount;

        // did we download our WebUpdate script ?
        bool scriptOk = (m_nStatus != wxWUDS_WAITINGXML);
#ifdef __WXDEBUG__          // avoid warnings in release builds
        if (scriptOk) wxASSERT_MSG(m_xmlRemote.IsOk(),
                                wxT("invalid status mode (") +
                                wxWUDGetStatus(m_nStatus) + wxT(")"));
#endif

        // re-enable what we disabled when we launched the thread
        wxASSERT(m_nStatus == wxWUDS_WAITING || m_nStatus == wxWUDS_WAITINGXML);
        if (nLabelMode != m_nStatus) {

            if (d > 0 && i > 0)
                m_pSpeedText->SetLabel(
                    wxString::Format(wxWUD_SPEEDTEXT_PREFIX wxT("downloaded %d package(s) and installed %d package(s)"), d, i));
            else if (d > 0 && i == 0)
                m_pSpeedText->SetLabel(
                    wxString::Format(wxWUD_SPEEDTEXT_PREFIX wxT("downloaded %d package(s)"), d));
            else if (d == 0 && scriptOk)
                m_pSpeedText->SetLabel(wxWUD_SPEEDTEXT_PREFIX wxT("WebUpdate script successfully downloaded"));
            else if (d == 0 && !scriptOk)
                m_pSpeedText->SetLabel(wxWUD_SPEEDTEXT_PREFIX wxT("waiting WebUpdate script download"));

            // in any case (wxWUDS_INSTALLING/wxWUDS_UNDEFINED) we are not downloading
            // anything now...
            m_pTimeText->SetLabel(wxT("No downloads running..."));

            if (scriptOk)
                m_pOkBtn->SetLabel(wxT("Download && install"));
            else
                m_pOkBtn->SetLabel(wxT("Get update list"));

            m_pCancelBtn->SetLabel(wxWUD_CANCEL_DEFAULT_LABEL);

            m_pUpdatesList->Unlock();
            nLabelMode = m_nStatus;
        }

        // are there checked items in the package listctrl ?
        if (scriptOk) {

            if (m_pUpdatesList->GetCheckedItemCount() > 0)
                m_pOkBtn->Enable();         // yes, there are
            else
                m_pOkBtn->Disable();        // no, there aren't

        } else {

            m_pOkBtn->Enable();
        }
    }
}




// ---------------------
// wxWEBUPDATEADVPANEL
// ---------------------

bool wxWebUpdateAdvPanel::Create(wxWindow* parent)
{
    if (!InitWidgetsFromXRC(parent))
        return FALSE;
    return TRUE;
}

bool wxWebUpdateAdvPanel::InitWidgetsFromXRC(wxWindow *parent)
{
    // build the dialog
    // ----------------

    // and build our dialog window
    if (!wxXmlResource::Get()->LoadPanel(this, parent, wxT("wxWebUpdateAdvPanel"))) {
        wxLogAdvMsg(wxT("Error while creating the wxWebUpdateAdvPanel - be sure you loaded webupdatedlg.xrc !"));
        return FALSE;
    }


    // init control pointers
    // ---------------------

    m_pDownloadPathTextCtrl = wxWU_XRCCTRL(*this, "IDWUAP_DOWNLOAD_PATH", wxTextCtrl);      // can be NULL
    m_pRestart = wxWU_XRCCTRL(*this, "IDWUAP_RESTART", wxCheckBox);                 // can be NULL
    m_pRemoveFiles = wxWU_XRCCTRL(*this, "IDWUAP_REMOVE", wxCheckBox);                  // can be NULL
    m_pSaveLog =  wxWU_XRCCTRL(*this, "IDWUAP_SAVELOG", wxCheckBox);                // can be NULL

#if !wxUSE_HTTPENGINE
    // if not using wxHttpEngine component, hide our ADV conn settings buttons
    wxButton *b = wxWU_XRCCTRL(*this, "IDWUAP_PROXYSETTINGS", wxButton);
    if (b) b->Hide();
    wxButton *b2 = wxWU_XRCCTRL(*this, "IDWUAP_AUTHSETTINGS", wxButton);
    if (b2) b2->Hide();
#endif


    // init control data
    // -----------------

    if (m_pDownloadPathTextCtrl) {

        // get the temporary folder where we put by default the updates
        wxFileName str(wxFileName::CreateTempFileName(wxEmptyString, NULL));
        str.SetFullName(wxEmptyString);     // remove the filename and keep only the path
        m_pDownloadPathTextCtrl->SetValue(str.GetLongPath());
    }



    // relayout
    // --------

    //GetSizer()->CalcMin();
    GetSizer()->Layout();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);

    return TRUE;
}

void wxWebUpdateAdvPanel::SetData(wxWebUpdateLocalXMLScript *script)
{
    m_xmlLocal = script;
    if (m_pRestart) {

        // update the restart checkbox, if present
        m_pRestart->SetLabel(wxT("Restart ") + m_xmlLocal->GetAppName() +
                            wxT(" after the update is finished"));
        m_pRestart->SetValue(m_xmlLocal->IsAppToRestart());
    }

    if (m_pSaveLog) {

        // update the savelog checkbox, if present
        m_pSaveLog->SetValue(m_xmlLocal->IsLogToSave());
    }
}

void wxWebUpdateAdvPanel::OnBrowse(wxCommandEvent &)
{
    // get the current value of the the "download path" from the textctrl
    wxString path = m_pDownloadPathTextCtrl->GetValue();
    wxDirDialog dlg(this, wxT("Choose a directory"),
                    path, wxDD_DEFAULT_STYLE | wxDD_NEW_DIR_BUTTON);
    if (dlg.ShowModal() == wxID_OK) {

        m_pDownloadPathTextCtrl->SetValue(dlg.GetPath());
        wxLogUsrMsg(wxT("wxWebUpdateDlg::OnBrowse - New output path is ") + dlg.GetPath());

        // save it also in our update installer
        wxWebUpdateInstaller::Get()->SetKeywordValue(wxT("downloaddir"), dlg.GetPath());

    } else {

        // just don't change nothing
    }
}

void wxWebUpdateAdvPanel::OnProxySettings(wxCommandEvent &)
{
#if wxUSE_HTTPENGINE
    wxProxySettingsDlg dlg(this, -1, wxT("WebUpdate proxy settings"));

    wxTopLevelWindow *tw = wxDynamicCast(GetParent(), wxTopLevelWindow);
    if (tw) dlg.SetIcon( tw->GetIcon() );

    dlg.CenterOnScreen();
    dlg.SetProxySettings(m_proxy);

    if (dlg.ShowModal() == wxID_OK)
        m_proxy = dlg.GetProxySettings();
#endif
}

void wxWebUpdateAdvPanel::OnAuthSettings(wxCommandEvent &)
{
#if wxUSE_HTTPENGINE
    wxAuthenticateDlg dlg(this, -1, wxT("WebUpdate authentication settings"));

    wxTopLevelWindow *tw = wxDynamicCast(GetParent(), wxTopLevelWindow);
    if (tw) dlg.SetIcon( tw->GetIcon() );

    dlg.CenterOnScreen();
    dlg.SetAuthSettings(m_auth);

    if (dlg.ShowModal() == wxID_OK)
        m_auth = dlg.GetAuthSettings();
#endif
}

void wxWebUpdateAdvPanel::OnRestart(wxCommandEvent &ev)
{
    // update the restart field
    m_xmlLocal->OverrideRestartFlag(ev.IsChecked());
}

void wxWebUpdateAdvPanel::OnSaveLog(wxCommandEvent &ev)
{
    // update the restart field
    m_xmlLocal->OverrideSaveLogFlag(ev.IsChecked());
}



// -------------------------------
// wxWEBUPDATEADVPANELXMLHANDLER
// -------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxWebUpdateAdvPanelXmlHandler, wxXmlResourceHandler)

wxWebUpdateAdvPanelXmlHandler::wxWebUpdateAdvPanelXmlHandler() : wxXmlResourceHandler()
{
    AddWindowStyles();
}

wxObject *wxWebUpdateAdvPanelXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(panel, wxWebUpdateAdvPanel)

    panel->Create(m_parentAsWindow);
        panel->SetId(GetID());
        // GetPosition(), GetSize(),
        // GetStyle(wxT("style"), wxTAB_TRAVERSAL),
    panel->SetName(GetName());

    SetupWindow(panel);
    CreateChildren(panel);

    return panel;
}

bool wxWebUpdateAdvPanelXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxWebUpdateAdvPanel"));
}






// ---------------------
// wxWEBUPDATEABOUTDLG
// ---------------------

bool wxWebUpdateAboutDlg::InitWidgetsFromXRC(wxWindow *parent)
{
    // build the dialog
    // ----------------

    if (!wxXmlResource::Get()->LoadDialog(this, parent, wxT("wxWebUpdateAboutDlg"))) {
        wxLogAdvMsg(wxT("Error while creating the wxWebUpdateAboutDlg - be sure you loaded webupdatedlg.xrc !"));
        return FALSE;
    }


    // eventually set the version of the WebUpdater...
    wxStaticText *ver = wxWU_XRCCTRL(*this, "IDWUAD_VERSION", wxStaticText);
    if (ver) ver->SetLabel(wxWUAD_PREFIX + wxWebUpdateInstaller::Get()->GetVersion() + wxWUAD_POSTFIX);

    GetSizer()->SetSizeHints(this);

    return TRUE;
}

