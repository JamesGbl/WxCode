/////////////////////////////////////////////////////////////////////////////
// Name:        webupdatedlg.cpp
// Purpose:     wxWebUpdateDlg, wxWebUpdateAdvPanel, wxWebUpdateAboutDlg
// Author:      Francesco Montorsi
// Created:     2005/06/23
// RCS-ID:      $Id: webupdatedlg.cpp 448 2007-03-06 22:06:38Z frm $
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
    #include <wx/intl.h>        // for _() support
#endif

// includes
#include "webupdatedlg.h"
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
    EVT_SIZE(wxWebUpdateDlg::OnSize)

    // miscellaneous
    EVT_TEXT_URL(XRCID("IDWUD_DESCRIPTION"), wxWebUpdateDlg::OnTextURL)
    EVT_LIST_ITEM_SELECTED(XRCID("IDWUD_LISTCTRL"), wxWebUpdateDlg::OnPackageSelected)

    // from threads
    EVT_DOWNLOAD_CONNECTED(wxWUD_DOWNLOAD_THREAD_ID, wxWebUpdateDlg::OnDownloadConnect)
    EVT_DOWNLOAD_UPDATE(wxWUD_DOWNLOAD_THREAD_ID, wxWebUpdateDlg::OnDownloadUpdate)
    //EVT_DOWNLOAD_PAUSED
    EVT_DOWNLOAD_ABORTED(wxWUD_DOWNLOAD_THREAD_ID, wxWebUpdateDlg::OnDownloadAbort)
    EVT_DOWNLOAD_USER_ABORTED(wxWUD_DOWNLOAD_THREAD_ID, wxWebUpdateDlg::OnDownloadUserAbort)
    EVT_DOWNLOAD_COMPLETE(wxWUD_DOWNLOAD_THREAD_ID, wxWebUpdateDlg::OnDownloadComplete)

    EVT_INSTALLATION_COMPLETE(wxWUD_INSTALL_THREAD_ID, wxWebUpdateDlg::OnInstallationComplete)

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

bool g_processingEvent = TRUE;

void StopUIUpdates()
{ g_processingEvent = FALSE; }

void RestoreUIUpdates()
{ g_processingEvent = TRUE; }

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
        return wxS("wxWUDS_DOWNLOADING");
    case wxWUDS_DOWNLOADINGXML:
        return wxS("wxWUDS_DOWNLOADINGXML");
    case wxWUDS_WAITING:
        return wxS("wxWUDS_WAITING");
    case wxWUDS_WAITINGXML:
        return wxS("wxWUDS_WAITINGXML");
    case wxWUDS_EXITING:
        return wxS("wxWUDS_EXITING");
    default:
        return wxString::Format(wxS("unknown: %d"), (int)s);
    }
}


// a little utility macro
#define wxWUD_CHECK_STATUS(allowed)                                 \
    { wxASSERT_MSG(m_nStatus & (allowed),                           \
                wxS("invalid status mode (") +                      \
                wxWUDGetStatus(m_nStatus) + wxS(")")); }

// declared in webupdate.cpp
bool wxIsFileProtocol(const wxString &uri);
bool wxIsHTTPProtocol(const wxString &uri);
bool wxIsWebProtocol(const wxString &uri);




// ---------------------
// wxWEBUPDATEDLG
// ---------------------

wxWebUpdateDlg::wxWebUpdateDlg(wxWindow *parent,
                                const wxWebUpdateLocalXMLScript &script,
                                const wxLocale *lang)
                            : wxDialog()
{
    PreInit();
    Create(parent, script, lang);
}

void wxWebUpdateDlg::PreInit()
{
    // miscellaneous
    m_nStatus=wxWUDS_WAITINGXML;
    m_nDownloadCount=0;

    // for us conventionally wxDTF_ABORT == no download thread running
    m_dThreadFlag = wxDTF_ABORT;

    // pointers
    m_iThread = NULL;

    m_pAppNameText = NULL;
    m_pProgressText = NULL;
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

    m_ok = TRUE;
}

bool wxWebUpdateDlg::Create(wxWindow *parent,
                            const wxWebUpdateLocalXMLScript &script,
                            const wxLocale *lang)
{
    m_ok = FALSE;

    // copy the local XML script
    m_xmlLocal = script;
    m_pLocale = lang;

    // init our GUI
    if (!InitWidgetsFromXRC(parent))
        return FALSE;

    // and then our worker threads
    if (!InitThreads())
        return FALSE;

    m_ok = TRUE;
    return TRUE;
}

bool wxWebUpdateDlg::InitWidgetsFromXRC(wxWindow *parent)
{
    // be sure local XML script was correctly set
    // ------------------------------------------

    wxASSERT_MSG(m_xmlLocal.IsOk(),
        wxS("You must provide a valid XML local script before building a wxWebUpdateDlg"));


    // build the dialog
    // ----------------

    {
        wxUIUpdateStopper stopper;

        wxString res = m_xmlLocal.GetXRCResName();
        wxLogAdvMsg(wxS("wxWebUpdateDlg::InitWidgetsFromXRC - loading the [") + res + wxS("] resource..."));

        if (!wxXmlResource::Get()->LoadDialog(this, parent, res)) {
            wxLogAdvMsg(wxS("Error while building wxWebUpdateDlg; ")
                    wxS("check that the given XRC [") + res + wxS("] is valid !"));

            return FALSE;
        }
    }



    // init control pointers
    // ---------------------

    m_pAppNameText = wxWU_XRCCTRL(*this, "IDWUD_APPNAME_TEXT", wxStaticText);  // can be NULL
    m_pProgressText = wxWU_XRCCTRL(*this, "IDWUD_PROGRESS_TEXT", wxStaticText);
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
    wxASSERT_MSG(m_pUpdatesList && m_pAdvPanel, wxS("Cannot be NULL, just hidden"));
    m_pUpdatesList->SetLocalPackages(m_xmlLocal.GetAllPackages());
    m_pAdvPanel->SetData(&m_xmlLocal);

    m_pGauge->SetRange(wxWUD_GAUGE_RANGE);

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

        // FIXME this adjustements cannot be encoded in the XRC ?
        m_pLog->SetBackgroundColour(*wxBLACK);
        m_pLog->SetForegroundColour(*wxGREEN);
        //m_pLog->SetDefaultStyle(wxTextAttr(*wxGREEN, *wxBLACK)); seems not to work
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
    wxLogAdvMsg(wxS("wxWebUpdateDlg::ConnectionRequired - creating the wxDialUpManager"));
    bool res = FALSE;

    // are we connected ?
    wxDialUpManager *mng = wxDialUpManager::Create();
    if (mng->IsOk()) {

        if (!mng->IsOnline()) {

            wxMessageBox(_("You are not connected to Internet !\nWebUpdater needs to download the update list from Internet...\nPlease connect and then retry."),
                        _("Error"), wxOK | wxICON_ERROR);
            wxLogUsrMsg(_("wxWebUpdateDlg::ConnectionRequired - not connected to Internet !"));
            res = FALSE;

        } else {

            wxLogUsrMsg(_("wxWebUpdateDlg::ConnectionRequired - found a valid connection"));
            res = TRUE;
        }
    }

    // cleanup & exit
    delete mng;
    return res;
}

bool wxWebUpdateDlg::InitThreads()
{
    wxASSERT_MSG(m_iThread == NULL, wxS("initializing threads twice ?"));

    // the installer thread is a joinable thread and thus we are allowed to
    // store a pointer to it:
    m_iThread = new wxWebUpdateInstallThread(this, wxWUD_INSTALL_THREAD_ID);

    // init also our installer thread
    if (m_iThread->Create() != wxTHREAD_NO_ERROR ||
        m_iThread->Run() != wxTHREAD_NO_ERROR) {
        wxMessageBox(_("Low resources; cannot run the WebUpdate dialog...\nClose some applications and then retry."), 
                     _("Error"), wxOK | wxICON_ERROR);
        wxLogAdvMsg(wxS("wxWebUpdateDlg::InitThreads - cannot run the installer thread !"));
        AbortDialog();

        return FALSE;
    }

    wxLogAdvMsg(wxS("wxWebUpdateDlg::InitThreads - successfully initialized the installer thread..."));
    return TRUE;
}

bool wxWebUpdateDlg::CheckForAllUpdated(wxWebUpdatePackageArray &arr, bool forcedefaultmsg)
{
    bool allupdated = TRUE;
    for (int j=0; j < (int)arr.GetCount(); j++) {
        wxWebUpdateCheckFlag f = m_pUpdatesList->CompareVersion(arr[j]);
        if (f == wxWUCF_OUTOFDATE || f == wxWUCF_NOTINSTALLED) {
            allupdated = FALSE;
            break;      // not all packages are up2date
        }

        if (f == wxWUCF_FAILED) {
            wxWebUpdateInstaller::Get()->ShowNotificationMsg(
                wxString::Format(
                    _("Update check for package '%1$s' is failed!\nThis can happen because of a corrupted XML local description file (%2$s)"),
                    arr[j].GetName().c_str(), m_xmlLocal.GetLocalScriptURI().c_str()));

            AbortDialog();
            return TRUE;        // TRUE = exit this dialog
        }
    }

    if (allupdated) {

        // CHANGE OUR STATUS
        m_nStatus = wxWUDS_WAITING;

        // show to the user the "update not available" message
        wxString defaultmsg = wxString::Format(_("You have the latest version of all packages of %s...\nexiting the update dialog."), GetAppName().c_str());
        wxString usermsg = m_xmlRemote.GetUpdateNotAvailableMsg();

        wxWebUpdateInstaller::Get()->ShowNotificationMsg(
            (usermsg.IsEmpty() || forcedefaultmsg) ? defaultmsg : usermsg);

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

bool wxWebUpdateDlg::FilterOtherPlatforms(wxWebUpdatePackageArray &arr)
{
    wxString removed;
    for (int j=0; j < (int)arr.GetCount(); j++) {
        if (arr[j].GetDownload() == wxEmptyWebUpdateDownload) {

            // remove this package from the list
            removed += wxS(",") + arr[j].GetName();
            arr.RemoveAt(j, 1);
            j--;    // recheck the j-th package
        }
    }

    // now remove those packages which depend on previously removed packages
    bool restart;
    int count = 0;
    do {
        restart = FALSE;
        for (int i=0; i < (int)arr.GetCount(); i++) {
            wxArrayString req(arr[i].GetParsedPrerequisites());

            for (int k=0; k < (int)req.GetCount(); k++) {
                if (removed.Contains(req[k])) {

                    // the i-th package depends on a removed package...
                    removed += wxS(",") + arr[i].GetName();
                    arr.RemoveAt(i, 1);
                    i--;

                    // start all the loops again since having removed this package
                    // could require us to remove also others which were previously
                    // checked...
                    restart = TRUE;
                    break;
                }
            }

            if (restart)
                break;
        }

        count++;
    } while (restart && count < 1000);  // 1000 check is to avoid endless loops

    // warn the user eventually
    if (arr.GetCount() == 0) {

        wxWebUpdateInstaller::Get()->ShowNotificationMsg(
            wxString::Format(_("No updates available for this platform (%s)... exiting the update dialog."),
                             wxWebUpdatePlatform::GetThisPlatform().GetAsString().c_str()));
        AbortDialog();

        return FALSE;        // FALSE = exit this dialog
    }

    return TRUE;
}

wxWebUpdateListCtrlFilter wxWebUpdateDlg::GetPackageFilter() const
{
    // if the "show only out of date" checkbox exists in the XRC...
    if (m_pShowOnlyOOD)
        return m_pShowOnlyOOD->GetValue() ? wxWULCF_ONLY_OUTOFDATE : wxWULCF_ALL;

    // if not...
    return wxWULCF_ONLY_OUTOFDATE;
}

void wxWebUpdateDlg::AbortDialog()
{
    wxLogAdvMsg(wxS("wxWebUpdateDlg::AbortDialog - stopping the installer thread"));
    if (m_iThread->IsPaused())
        m_iThread->Run();           // we need the thread running if we want to delete it !
    if (m_iThread->IsRunning())
        m_iThread->Delete();
    wxLogAdvMsg(wxS("wxWebUpdateDlg::AbortDialog - stopped & deleted the installer thread"));

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

void wxWebUpdateDlg::AfterScriptDownload(const wxString &xmluri)
{
    // remove this download from our counter
    wxASSERT_MSG(m_nDownloadCount == 1,
        wxS("Only the WebUpdate XML script should have been downloaded"));
    m_nDownloadCount--;

    // ok, we can now parse the XML doc
    wxString langid = m_pLocale ? m_pLocale->GetCanonicalName() : wxS("en");
    if (!m_xmlRemote.Load(xmluri, langid)) {
        wxWebUpdateInstaller::Get()->ShowErrorMsg(wxString::Format(
                        _("Cannot parse the XML update script downloaded as: %s"),
                        xmluri.c_str()));
        AbortDialog();      // this is a unrecoverable error !
        return;
    }

    // do a basic filtering: remove packages which don't have downloads
    // suitable for this platform
    wxWebUpdatePackageArray arr(m_xmlRemote.GetAllPackages());
    if (!FilterOtherPlatforms(arr))
        return;      // no downloads for this platform ? then exit the dialog
                     // (a message dialog has already been shown by FilterOtherPlatforms)

    // update our remote package array (which is contained only in
    // our wxWebUpdateListCtrl to avoid data duplications)
    wxLogUsrMsg(_("wxWebUpdateDlg::OnScriptDownload - XML script loaded successfully"));
    m_pUpdatesList->SetRemotePackages(arr);
    m_pUpdatesList->RebuildPackageList(GetPackageFilter());

    // is everything up to date ?
    if (CheckForAllUpdated(arr))
        return;     // all is updated, we must exit this dialog
                    // (a message dialog has already been shown by CheckForAllUpdated)

    // what if we could not found any valid package in the webupdate script ?
    if (m_pUpdatesList->GetItemCount() == 0) {

        wxWebUpdateInstaller::Get()->ShowNotificationMsg(
                wxString::Format(_("Could not find any valid package for %1$s in the WebUpdate script.\n\nThis could be because the local XML description file (%2$s) has been corrupted.\n\nExiting the update dialog."),
                                 GetAppName().c_str(),
                                 m_xmlLocal.GetLocalScriptURI().c_str()),
                _("Warning"));
        AbortDialog();
        return;
    }

    // we have successfully complete step #1
    wxASSERT_MSG(m_nStatus == wxWUDS_DOWNLOADINGXML,
                wxS("Wrong status setting !"));
    m_nStatus = wxWUDS_WAITING;         // CHANGE OUR STATUS
}

void wxWebUpdateDlg::StartDownload(const wxString &uri)
{
    wxASSERT(m_dThreadFlag == wxDTF_ABORT);

    // update the GUI _before_ launching the thread
    // buttons & other controls will be reenabled in OnDownloadComplete
    m_pOkBtn->Disable();
    m_pCancelBtn->SetLabel(wxWUD_CANCEL_DOWNLOAD);
    m_pUpdatesList->Lock();

    m_pProgressText->SetLabel(wxString(wxWUD_PROGRESSTEXT_PREFIX) + 
                           _("connecting..."));

    // we assume m_pOutputFile has already been correctly initialized
    m_dThreadFlag = wxDTF_CONTINUE;
    new wxDownloadThread(this, wxWUD_DOWNLOAD_THREAD_ID, &m_dThreadFlag, 
                         uri, m_pOutputFile);
}

bool wxWebUpdateDlg::DownloadNextPackage()
{
    wxASSERT_MSG(m_nStatus == wxWUDS_DOWNLOADING,
                    wxS("Invalid state mode"));

    // be sure not to access directly the listctrl array of packages;
    // this is required since we cannot use it safely:
    // a filter could have been applied on the listctrl and thus
    // there maybe some hidden packages... leave all this handling to our
    // listctrl class...
    m_current = m_pUpdatesList->GetNextPackageToDownload();
    if (m_current == NULL) return FALSE;        // no other packages to download
    wxWebUpdateDownload &dl = m_current->GetDownload();

    // init download properties
    m_strResName = m_current->GetName() + wxS(" package");
    m_strOutputFile = GetOutputFilenameFor(*m_current);
    m_pOutputFile = new wxFileOutputStream(m_strOutputFile);

    // reset the gauge GUI
    m_pGauge->SetValue(0);

    // (eventually) check that we are online
    wxString strURI = dl.GetDownloadString();
    if (wxIsWebProtocol(strURI))
        if (!ConnectionRequired())
            return FALSE;       // not connected...

    // launch the new thread: the download thread is a detached thread
    // and thus we cannot store a pointer to it !
    wxLogUsrMsg(_("wxWebUpdateDlg::DownloadNextPackage - launching download of [%s]"),
                strURI.c_str());
    StartDownload(strURI);

    return TRUE;
}

bool wxWebUpdateDlg::InstallNextPackage()
{
    wxASSERT_MSG(m_nStatus == wxWUDS_INSTALLING,
                    wxS("Invalid state mode"));

    m_current = m_pUpdatesList->GetNextPackageToInstall();
    if (m_current == NULL) return FALSE;        // no other packages to install
    wxWebUpdateDownload &download = m_current->GetDownload();

    // launch the installation
    m_iThread->m_pDownload = &download;
    m_iThread->m_strUpdateFile = GetOutputFilenameFor(*m_current);
    m_iThread->BeginNewInstall();
    wxLogUsrMsg(_("wxWebUpdateDlg::InstallNextPackage - launching installation of [%s]"),
                m_iThread->m_strUpdateFile.c_str());

    return TRUE;
}

void wxWebUpdateDlg::RemoveCurrentPackage()
{
    // if the user wants to remove the update packages, do it now
    if (m_pAdvPanel->RemoveFiles()) {

        wxASSERT(m_current);

        wxString file = GetOutputFilenameFor(*m_current);
        wxLogAdvMsg(wxS("wxWebUpdateDlg::RemoveCurrentPackage - removing the package ")
                    + m_current->GetName() + wxS(" [") + file + wxS("]"));

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
    wxWindow *p = ShowHideChild(wxS("IDWUD_ADVPANEL"));
    wxASSERT_MSG(p, wxS("Invalid XRC file !"));

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

void wxWebUpdateDlg::OnDownload(wxCommandEvent &)
{
    // conventionally we put m_dThreadFlag to wxDTF_ABORT when not downloading
    wxASSERT_MSG(m_dThreadFlag == wxDTF_ABORT,
        wxS("The wxWUD_OK button should be disabled if a download is in progress !"));

#if wxUSE_HTTPENGINE_FIXME_FIXME
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
                    wxS("The XML remote script should have not been loaded yet !"));

        if (wxIsWebProtocol(m_xmlLocal.GetRemoteScriptURI()))
            if (!ConnectionRequired())
                return;         // not connected

        // keep our status in wxWUDS_WAITINGXML even if we are downloading the XML
        //m_strURI = m_xmlLocal.GetRemoteScriptURI();
        m_strResName = _("XML WebUpdate script");
        m_strOutputFile = wxFileName::CreateTempFileName(wxS("webupdate"));
        m_pOutputFile = new wxFileOutputStream(m_strOutputFile);

        // CHANGE OUR STATUS
        m_nStatus = wxWUDS_DOWNLOADINGXML;

        // _after_ changing status, do start the download
        StartDownload(m_xmlLocal.GetRemoteScriptURI());

    } else {    // XML script has already been downloaded: download selected package

        // safety checks
        wxWUD_CHECK_STATUS(wxWUDS_WAITING);
        wxASSERT_MSG(m_xmlRemote.IsOk(),
                        wxS("The XML remote script should have been already loaded !"));

        // CHANGE OUR STATUS
        m_nStatus = wxWUDS_DOWNLOADING;

        // launch the first download of the list
    #ifdef __WXDEBUG__
        bool atleastone =           // avoid warnings in release mode
    #endif
            DownloadNextPackage();
        wxASSERT_MSG(atleastone, wxS("The wxWUD_OK button should be enabled only when ")
                                wxS("one or more packages are ready for download"));
    }

    // FIXME: is this required ?
    UpdateWindowUI();
}

void wxWebUpdateDlg::OnCancel(wxCommandEvent &)
{
    wxLogUsrMsg(_("wxWebUpdateDlg::OnCancel - user hit the cancel button..."));

    if (m_nStatus == wxWUDS_DOWNLOADING || m_nStatus == wxWUDS_DOWNLOADINGXML) {

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

void wxWebUpdateDlg::OnInstallationComplete(wxCommandEvent &)
{
    // we need to change our internal status variable before any other UI update event occurs
    wxUIUpdateStopper stopper;

    m_nInstallCount++;

    if (m_iThread->InstallationWasSuccessful()) {

        m_pUpdatesList->SetInstallStatus(*m_current, TRUE);

        // find the installed package
        wxWebUpdateLocalPackageArray arr(GetLocalPackages());       // do a copy
        wxLogUsrMsg(_("wxWebUpdateDlg::OnInstallationComplete - completed installation of [%s]..."),
                    m_current->GetName().c_str());

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

            wxLogUsrMsg(_("wxWebUpdateDlg::OnInstallationComplete - completed the installation of all packages"));

            // notify the user
            wxWebUpdateInstaller::Get()->ShowNotificationMsg(
                    _("The installation of the selected packages has been completed successfully !"));

            // CHANGE OUR STATUS back to wait mode
            m_nStatus = wxWUDS_WAITING;
            CheckForAllUpdated(GetRemotePackages(), TRUE);
        }

    } else {

        // warn the user
        m_pUpdatesList->SetInstallStatus(*m_current, TRUE);
        wxWebUpdateInstaller::Get()->ShowErrorMsg(wxString::Format(
                        _("The downloaded package '%s'\ncould not be installed.\n\nPlease contact the support team for more info."),
                        m_strOutputFile.c_str()));
        RemoveCurrentPackage();

        // CHANGE OUR STATUS back to wait mode
        m_nStatus = wxWUDS_WAITING;
    }
}


// -----------------------------------------------------
// wxWEBUPDATEDLG - event handlers for download events
// -----------------------------------------------------

void wxWebUpdateDlg::OnDownloadConnect(wxDownloadEvent &WXUNUSED(ev))
{
    wxASSERT(m_dThreadFlag != wxDTF_ABORT);
    m_pProgressText->SetLabel(wxString(wxWUD_PROGRESSTEXT_PREFIX) + _("connected to the server..."));
}

void wxWebUpdateDlg::OnDownloadUpdate(wxDownloadEvent &ev)
{
    m_pGauge->SetValue(ev.GetDownloadProgress()*wxWUD_GAUGE_RANGE);

    if (ev.GetDownloadSpeed() != 0)
        m_pProgressText->SetLabel(wxWUD_PROGRESSTEXT_PREFIX + 
            wxString::Format(_("downloading '%1$s' at %2$s"),
                            m_strResName.c_str(), 
                            ev.GetHumanReadableDownloadSpeed().c_str()));
    else
        m_pProgressText->SetLabel(wxWUD_PROGRESSTEXT_PREFIX + 
            wxString::Format(_("downloading '%1$s'"),
                            m_strResName.c_str()));

    m_pTimeText->SetLabel(wxWUD_TIMETEXT_PREFIX +
                          ev.GetEstimatedRemainingTime().Format());
}

void wxWebUpdateDlg::OnDownloadComplete(wxDownloadEvent &ev)
{
    // we need to change our internal status variable before any other UI update event occurs
    wxUIUpdateStopper stopper;

    // the download was successful!
    wxLogUsrMsg(_("wxWebUpdateDlg::OnDownloadComplete - Download status: successfully completed"));
    m_nDownloadCount++;
    wxDELETE(m_pOutputFile);

    bool downloadingScript = (m_nStatus == wxWUDS_DOWNLOADINGXML);
    if (downloadingScript) {

        // handle the XML parsing & control update
        AfterScriptDownload(m_strOutputFile);

    } else {

        wxString validMD5 = m_current->GetDownload().GetMD5Checksum();
        wxString ourMD5;

        if (validMD5.IsEmpty() // this download is not MD5-signed
            || validMD5 == ourMD5) {

            wxASSERT_MSG(!m_pUpdatesList->IsDownloaded(*m_current) &&
                        !m_pUpdatesList->IsInstalled(*m_current),
                wxS("We are downloading/installing the same package again ?"));
            m_pUpdatesList->SetDownloadStatus(*m_current, TRUE);

            // if this is the last package which must be downloaded
            // then we can begin the installation
            if (!DownloadNextPackage()) {

                // the order of the package installation cannot be
                // randomly chosen
                wxLogUsrMsg(_("wxWebUpdateDlg::OnDownloadComplete - beginning installation of downloaded packages"));
                m_nStatus = wxWUDS_INSTALLING;  // CHANGE OUR STATUS
                InstallNextPackage();
            }

        } else {

            wxWebUpdateInstaller::Get()->ShowErrorMsg(wxString::Format(
                    _("The downloaded file '%1$s'\nis corrupted. MD5 checksum is:\n\n\t%2$s\n\ninstead of the expected:\n\n\t%3$s\n\nPlease retry the download."),
                    m_strOutputFile.c_str(),
                    ourMD5.c_str(),
                    validMD5.c_str()));
            m_pUpdatesList->SetDownloadStatus(*m_current, FALSE);

            // CHANGE OUR STATUS back to wait mode
            m_nStatus = wxWUDS_WAITING;
        }
    }

    // did we download our WebUpdate script ?
    bool scriptOk = (m_nStatus != wxWUDS_WAITINGXML);
#ifdef __WXDEBUG__          // avoid warnings in release builds
    if (scriptOk) wxASSERT_MSG(m_xmlRemote.IsOk(),
                            wxS("invalid status mode (") +
                            wxWUDGetStatus(m_nStatus) + wxS(")"));
#endif

    // reset our gauge control
    m_pGauge->SetValue(0);

    // update our meters
    int d = m_nDownloadCount,           // see #m_nFileCount description
        i = m_nInstallCount;

    // re-enable what we disabled when we launched the thread

    if (d > 0 && i > 0)
        m_pProgressText->SetLabel(
            wxString::Format(wxString(wxWUD_PROGRESSTEXT_PREFIX) +
                _("downloaded %1$d package(s) and installed %2$d package(s)"), d, i));
    else if (d > 0 && i == 0)
        m_pProgressText->SetLabel(
            wxString::Format(wxString(wxWUD_PROGRESSTEXT_PREFIX) + _("downloaded %d package(s)"), d));
    else if (d == 0 && scriptOk)
        m_pProgressText->SetLabel(wxString(wxWUD_PROGRESSTEXT_PREFIX) + _("WebUpdate script successfully downloaded"));
    else if (d == 0 && !scriptOk)
        m_pProgressText->SetLabel(wxString(wxWUD_PROGRESSTEXT_PREFIX) + _("waiting WebUpdate script download"));

    // in any case (wxWUDS_INSTALLING/wxWUDS_UNDEFINED) we are not downloading
    // anything now...
    m_pTimeText->SetLabel(_("No downloads running..."));

    if (scriptOk)
        m_pOkBtn->SetLabel(_("Download && install"));
    else
        m_pOkBtn->SetLabel(_("Get update list"));

    m_pCancelBtn->SetLabel(wxWUD_CANCEL_DEFAULT_LABEL);
    m_pUpdatesList->Unlock();
}

void wxWebUpdateDlg::OnDownloadAbort(wxDownloadEvent &ev)
{
    wxDELETE(m_pOutputFile);
    bool downloadingScript = (m_nStatus == wxWUDS_DOWNLOADINGXML);

    // CHANGE OUR STATUS back to wait mode
    if (downloadingScript) m_nStatus = wxWUDS_WAITINGXML;
    if (!downloadingScript) m_nStatus = wxWUDS_WAITING;

    wxWebUpdateInstaller::Get()->ShowErrorMsg(wxString::Format(
            _("Could not download the %s from\n\n%s\n\nURL..."),
            m_strResName.c_str(), ev.GetURL().c_str()));

    if (downloadingScript) {
        wxLogUsrMsg(_("wxWebUpdateDlg::OnDownloadComplete - failed while downloading the XML script... aborting dialog"));
        AbortDialog();      // this is a unrecoverable error !
    }
}

void wxWebUpdateDlg::OnDownloadUserAbort(wxDownloadEvent &)
{
    wxDELETE(m_pOutputFile);
    bool downloadingScript = (m_nStatus == wxWUDS_DOWNLOADINGXML);

    // CHANGE OUR STATUS back to wait mode
    if (downloadingScript) m_nStatus = wxWUDS_WAITINGXML;
    if (!downloadingScript) m_nStatus = wxWUDS_WAITING;

    wxWebUpdateInstaller::Get()->ShowNotificationMsg(_("Download aborted..."), 
                                                     _("Warning"));

    if (downloadingScript) {
        wxLogUsrMsg(_("wxWebUpdateDlg::OnDownloadComplete - failed while downloading the XML script... aborting dialog"));
        AbortDialog();      // this is a unrecoverable error !
    }

//    m_bUserAborted = FALSE;     // reset flag
}

void wxWebUpdateDlg::OnPackageSelected(wxListEvent &ev)
{
    // change the description label eventually
    if (!m_pDescription)
        return;

    if (m_pUpdatesList->GetSelectedItemCount() == 0)
    {
        m_pDescription->SetValue(_("No package selected"));
        return;
    }

    for (int i=0; i<m_pUpdatesList->GetItemCount(); i++) {
        if (m_pUpdatesList->GetItemState(i, wxLIST_STATE_SELECTED)) {

            // show the description of the first selected item
            wxString name = m_pUpdatesList->GetItemText(i);
            const wxWebUpdatePackage &pkg = m_pUpdatesList->GetRemotePackage(name);

            wxLogAdvMsg(wxS("wxWebUpdateDlg::OnUpdateUI - updating the description with value: ") + pkg.GetDescription());
            m_pDescription->SetValue(pkg.GetDescription());
            break;
        }
    }
}

/*
/*
        if (m_dThread->IsComputingMD5() &&*//* nLabelMode != wxDTS_COMPUTINGMD5) {
            m_pProgressText->SetLabel(wxString(wxWUD_PROGRESSTEXT_PREFIX) + _("computing the file hash"));
            m_pTimeText->SetLabel(_("No downloads running..."));
            m_pCancelBtn->SetLabel(wxWUD_CANCEL_DOWNLOAD);
            m_pOkBtn->Disable();

            nLabelMode = wxDTS_COMPUTINGMD5;
        }*/

/*
    } else if (m_nStatus == wxWUDS_INSTALLING) {

        m_pGauge->SetValue(0);
        if (nLabelMode != wxWUDS_INSTALLING) {

            // use the "speed" label for installing status
            m_pProgressText->SetLabel(wxWUD_PROGRESSTEXT_PREFIX + wxString::Format(_("installing '%s'"),
                                   m_strResName.c_str()));
            m_pTimeText->SetLabel(_("No downloads running..."));
            m_pCancelBtn->SetLabel(wxWUD_CANCEL_INSTALLATION);
            m_pOkBtn->Disable();

            nLabelMode = wxWUDS_INSTALLING;
        }

    } else {


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

*/


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
    if (!wxXmlResource::Get()->LoadPanel(this, parent, wxS("wxWebUpdateAdvPanel"))) {
        wxLogAdvMsg(wxS("Error while creating the wxWebUpdateAdvPanel - be sure you loaded webupdatedlg.xrc !"));
        return FALSE;
    }


    // init control pointers
    // ---------------------

    m_pDownloadPathTextCtrl = 
        wxWU_XRCCTRL(*this, "IDWUAP_DOWNLOAD_PATH", wxTextCtrl);      // can be NULL
    m_pRestart = 
        wxWU_XRCCTRL(*this, "IDWUAP_RESTART", wxCheckBox);            // can be NULL
    m_pRemoveFiles = 
        wxWU_XRCCTRL(*this, "IDWUAP_REMOVE", wxCheckBox);             // can be NULL
    m_pSaveLog =  
        wxWU_XRCCTRL(*this, "IDWUAP_SAVELOG", wxCheckBox);            // can be NULL

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
        wxFileName str(wxFileName::CreateTempFileName(wxEmptyString));
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
        m_pRestart->SetLabel(wxString::Format(
                    _("Restart %s after the update is finished"),
                    m_xmlLocal->GetAppName().c_str()));
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
    wxDirDialog dlg(this, _("Choose a directory"),
                    path, wxDD_DEFAULT_STYLE | wxDD_NEW_DIR_BUTTON);
    if (dlg.ShowModal() == wxID_OK) {

        m_pDownloadPathTextCtrl->SetValue(dlg.GetPath());
        wxLogUsrMsg(_("wxWebUpdateDlg::OnBrowse - new output path is [%s]"), dlg.GetPath().c_str());

        // save it also in our update installer
        wxWebUpdateInstaller::Get()->SetKeywordValue(wxS("downloaddir"), dlg.GetPath());

    } else {

        // just don't change nothing
    }
}

void wxWebUpdateAdvPanel::OnProxySettings(wxCommandEvent &)
{
#if wxUSE_HTTPENGINE
    wxProxySettingsDlg dlg(this, -1, _("WebUpdate proxy settings"));

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
    wxAuthenticateDlg dlg(this, -1, _("WebUpdate authentication settings"));

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
        // GetStyle(wxS("style"), wxTAB_TRAVERSAL),
    panel->SetName(GetName());

    SetupWindow(panel);
    CreateChildren(panel);

    return panel;
}

bool wxWebUpdateAdvPanelXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxS("wxWebUpdateAdvPanel"));
}






// ---------------------
// wxWEBUPDATEABOUTDLG
// ---------------------

bool wxWebUpdateAboutDlg::InitWidgetsFromXRC(wxWindow *parent)
{
    // build the dialog
    // ----------------

    if (!wxXmlResource::Get()->LoadDialog(this, parent, wxS("wxWebUpdateAboutDlg"))) {
        wxLogAdvMsg(wxS("Error while creating the wxWebUpdateAboutDlg - be sure you loaded webupdatedlg.xrc !"));
        return FALSE;
    }


    // eventually set the version of the WebUpdater...
    wxStaticText *ver = wxWU_XRCCTRL(*this, "IDWUAD_VERSION", wxStaticText);
    if (ver) ver->SetLabel(wxString::Format(wxWUAD_VERSIONSTR, wxWebUpdateInstaller::Get()->GetVersion().GetAsString().c_str()));

    GetSizer()->SetSizeHints(this);

    return TRUE;
}

