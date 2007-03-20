////////////////////////////////////////////////////////////////////////////
// Name:        app.cpp
// Purpose:     The WebUpdater application
// Author:      Francesco Montorsi
// Modified by:
// Created:     29/07/05
// RCS-ID:      $Id: webapp.cpp 470 2007-03-20 11:35:51Z frm $
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// defines
// ----------------------------------------------------------------------------

//! The local XML script which is loaded by default if no --xml option is specified.
#define wxWU_LOCAL_XMLSCRIPT            wxT("local.xml")

//! The local XRC file which is loaded if no --xrc option is specified.
#define wxWU_LOCAL_XRC                  wxT("webupdatedlg.xrc")

//! The name of the XRC resource which is loaded if no --res option is specified.
#define wxWU_XRC_RESOURCE               wxT("wxWebUpdateDlg")

//! The name of our default log file.
#define wxWU_LOGFILENAME                wxT("webupdatelog.txt")


#ifdef MODDED
    extern bool wxInitializeMod();
    extern void wxUninitializeMod();
#endif

// for debugging
#if defined( __WXMSW__ ) && defined( __VISUALC__ )
#include <crtdbg.h>
#define mcDUMP_ON_EXIT      \
    { _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF); }
#else
#define mcDUMP_ON_EXIT      /* expand to nothing */
#endif


// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_all.h>
#include <wx/fs_mem.h>
#include <wx/image.h>
#include <wx/wfstream.h>
#include <wx/cmdline.h>
#include <wx/textfile.h>
#include <wx/apptrait.h>
#include <wx/config.h>

#include "webupdatedlg.h"
#include "wx/webupdatectrl.h"
#include "wx/stdactions.h"


// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

#include "www.xpm"

// the application icon (under Windows and OS/2 it is in resources and even
// though we could still include the XPM here it would be unused)
#if !defined(__WXMSW__) && !defined(__WXPM__)
    #include "app.xpm"
#endif


// ----------------------------------------------------------------------------
// usage instructions for this app
// ----------------------------------------------------------------------------

#define OPTION_XMLSCRIPT        wxT("l")
#define OPTION_XRC              wxT("x")
#define OPTION_RESOURCE         wxT("e")
#define OPTION_URI              wxT("u")
#define OPTION_ASKURI           wxT("a")

#define SWITCH_RESTART          wxT("r")
#define SWITCH_SAVELOG          wxT("s")

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
    // options
    { wxCMD_LINE_OPTION, OPTION_XMLSCRIPT, wxT("xml"),
        wxTRANSLATE("Use the given local XML file"),
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_OPTION, OPTION_XRC, wxT("xrc"),
        wxTRANSLATE("Use the given local XRC file"),
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_OPTION, OPTION_RESOURCE, wxT("res"),
        wxTRANSLATE("Use the given resource name when loading the XRC"),
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_OPTION, OPTION_URI, wxT("uri"),
        wxTRANSLATE("Use the given URI to load the remote XML file"),
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_OPTION, OPTION_ASKURI, wxT("ask-uri"),
        wxTRANSLATE("Asks the user the URI of the remote XML file"),
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },

    // switches
    { wxCMD_LINE_SWITCH, SWITCH_RESTART, wxT("restart"),
        wxTRANSLATE("Restart the updated application when WebUpdater quits"),
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_SWITCH, SWITCH_SAVELOG, wxT("savelog"),
        wxTRANSLATE("Saves the log messages to '") wxWU_LOGFILENAME wxT("'"),
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

    // help
    { wxCMD_LINE_SWITCH, wxT("h"), wxT("help"),
        wxTRANSLATE("Show this help message"),
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },

    { wxCMD_LINE_NONE }
};



// ----------------------------------------------------------------------------
// the application
// ----------------------------------------------------------------------------

class WebUpdaterApp : public wxApp
{
public:

    // our main dialog
    wxWebUpdateDlg *m_dlg;

    // our local script
    wxWebUpdateLocalXMLScript m_script;

    // did we create the file logger ?
    bool m_bLoggerInstalled;

    // our log window
    wxWebUpdateLog *m_log;

    // for i18n support
    wxLocale m_locale;


public:
    WebUpdaterApp() : m_dlg(NULL), m_log(NULL) { m_bLoggerInstalled=FALSE; }
    virtual ~WebUpdaterApp() {}


    // helpers of OnPreInit()
    // NOTE: they are listed in the same order they are called by OnPreInit()
    void CreateFileLogger();
    void SetupLocale();

    // called before OnInit() and before any window is created
    bool OnPreInit();

    // Initializes the WebUpdaterApp.
    virtual bool OnInit();

    // Closes the WebUpdaterApp.
    virtual int OnExit();

public:         // we need to intercept the execute event; see wxWebUpdateActionRun::Run
                // for more info

    void OnExecute(wxCommandEvent &event);

private:
    DECLARE_EVENT_TABLE()
};


// ----------------------------------------------------------------------------
// the dialog of the application
// ----------------------------------------------------------------------------

class WebUpdaterDlg : public wxWebUpdateDlg
{
public:
    WebUpdaterDlg(const wxWebUpdateLocalXMLScript &script);
    virtual ~WebUpdaterDlg() {}


public:         // we need to intercept some events in order to make
                // this dialog work as 'frame' of our application

    void OnQuit(wxCloseEvent &event);

    virtual void EndModal(int retCode);

private:
    DECLARE_EVENT_TABLE()
};


// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

IMPLEMENT_APP(WebUpdaterApp)
BEGIN_EVENT_TABLE(WebUpdaterApp, wxApp)
    EVT_EXECUTE(-1, WebUpdaterApp::OnExecute)
END_EVENT_TABLE()
BEGIN_EVENT_TABLE(WebUpdaterDlg, wxWebUpdateDlg)
    EVT_CLOSE(WebUpdaterDlg::OnQuit)
END_EVENT_TABLE()


// ----------------------------------------------------------------------------
// WEBUPDATERAPP
// ----------------------------------------------------------------------------

void wxInitRequiredImageHandlers()
{
#if wxUSE_LIBPNG
    //wxImage::AddHandler( new wxPNGHandler );
#endif
#if wxUSE_XPM
    wxImage::AddHandler( new wxXPMHandler );
#endif
}

// using this function instead of wxXmlResource::Get()->InitAllHandlers()
// let us to avoid the ADV and HTML wx libraries (unused in this app) and
// thus make the executable smaller
void wxInitRequiredXmlHandlers()
{
    wxXmlResource *p = wxXmlResource::Get();

    p->AddHandler(new wxBitmapXmlHandler);
    p->AddHandler(new wxIconXmlHandler);
    p->AddHandler(new wxDialogXmlHandler);
    p->AddHandler(new wxPanelXmlHandler);
    p->AddHandler(new wxSizerXmlHandler);
    p->AddHandler(new wxButtonXmlHandler);
    p->AddHandler(new wxBitmapButtonXmlHandler);
    p->AddHandler(new wxStaticTextXmlHandler);
    p->AddHandler(new wxStaticBoxXmlHandler);
    p->AddHandler(new wxStaticBitmapXmlHandler);
    p->AddHandler(new wxTreeCtrlXmlHandler);
    p->AddHandler(new wxListCtrlXmlHandler);
#if wxUSE_CHOICE
    p->AddHandler(new wxChoiceXmlHandler);
#endif
#if wxUSE_SLIDER
    p->AddHandler(new wxSliderXmlHandler);
#endif
#if wxUSE_GAUGE
    p->AddHandler(new wxGaugeXmlHandler);
#endif
#if wxUSE_CHECKBOX
    p->AddHandler(new wxCheckBoxXmlHandler);
#endif
#if wxUSE_SPINBTN
    p->AddHandler(new wxSpinButtonXmlHandler);
#endif
#if wxUSE_SPINCTRL
    p->AddHandler(new wxSpinCtrlXmlHandler);
#endif
#if wxUSE_RADIOBOX
    p->AddHandler(new wxRadioBoxXmlHandler);
    p->AddHandler(new wxRadioButtonXmlHandler);
#endif
#if wxUSE_COMBOBOX
    p->AddHandler(new wxComboBoxXmlHandler);
#endif
#if wxUSE_LISTBOOK
    p->AddHandler(new wxListbookXmlHandler);
#endif
#if wxUSE_CHOICEBOOK
    p->AddHandler(new wxChoicebookXmlHandler);
#endif
    p->AddHandler(new wxTextCtrlXmlHandler);
#if wxUSE_LISTBOX
    p->AddHandler(new wxListBoxXmlHandler);
#endif
#if wxUSE_STATLINE
    p->AddHandler(new wxStaticLineXmlHandler);
#endif
    p->AddHandler(new wxUnknownWidgetXmlHandler);
    p->AddHandler(new wxHyperlinkCtrlXmlHandler);

    // our custom handlers
    p->AddHandler(new wxWebUpdateListCtrlXmlHandler);
    p->AddHandler(new wxWebUpdateAdvPanelXmlHandler);
}

void WebUpdaterApp::CreateFileLogger()
{
    if (m_bLoggerInstalled)
        return;     // already installed !

    // it automatically installs itself as the new logger

    wxLogDebug(wxT("WebUpdaterApp::CreateFileLogger - creating the logfile [") +
            wxString(wxWU_LOGFILENAME) + wxT("]"));
    m_log->WriteAllMsgAlsoToFile(wxWU_LOGFILENAME);
    wxLogNewSection(wxT(" LOG OF WEBUPDATER ") +
                    wxWebUpdateInstaller::Get()->GetVersion() +
                    wxT(" SESSION BEGAN AT ") +
                    wxDateTime::Now().Format(wxT("%x %X")));
    m_bLoggerInstalled = TRUE;
}

void WebUpdaterApp::SetupLocale()
{
    wxConfigBase *config = wxConfig::Get();
    wxString strLangCanonical;

    // load, if present, the setting of desired language from wxConfig
    config->SetPath(wxT("/General"));
    config->Read(wxT("Language"), &strLangCanonical, wxEmptyString);

    int langID = wxLANGUAGE_DEFAULT;
    if (!strLangCanonical.IsEmpty()) {

        // there is a desired language defined...
        const wxLanguageInfo* pLangInfo = m_locale.FindLanguageInfo(strLangCanonical);
        if (pLangInfo != NULL) {

            // and it's valid...
            langID = pLangInfo->Language;

        } else {

            // leave in langID the wxLANGUAGE_DEFAULT value
            wxLogAdvMsg(wxT("WebUpdaterApp::SetupLocale - the language [%s] found in the config file of WebUpdater is NOT valid; using default one"),
                        strLangCanonical.c_str());
        }

        // in case user wrote in config file something like 'Italian', we use this
        // to put in strLangCanonical the 'it_IT' string
        strLangCanonical = m_locale.GetLanguageInfo(langID)->CanonicalName;

    } else {

        // the configuration file for webupdater does not contain any info about
        // preferred language... use the default one...
        strLangCanonical = m_locale.GetLanguageInfo(langID)->CanonicalName;

        // write the default language in our config object
        config->Write(wxT("Language"), strLangCanonical);
    }

    // Initialization...
    m_locale.Init(langID);

    // test if there is a LANG subfolder in the directory in our working dir
    wxString path = wxGetCwd() + wxFileName::GetPathSeparator() + wxT("lang");
    wxLogAdvMsg(wxT("WebUpdaterApp::SetupLocale - searching for [") + path + wxT("]"));
    if (::wxDirExists(path)) {
        wxLogAdvMsg(wxT("WebUpdaterApp::SetupLocale - found the LANG subfolder"));

        // .mo-files are located in subdir "lang" (on Windows)...
        m_locale.AddCatalogLookupPathPrefix(wxT("lang"));
    }

    // Initialize the catalog we'll be using
    if (!strLangCanonical.IsEmpty()) {

        wxString fn = strLangCanonical.Before('_') + wxT(".mo");
        wxLogAdvMsg(wxT("WebUpdaterApp::SetupLocale - loading %s catalog"), fn.c_str());
        if (m_locale.AddCatalog(fn))
            wxLogAdvMsg(wxT("WebUpdaterApp::SetupLocale - successfully set the %s catalog"), fn.c_str());
        else
            wxLogAdvMsg(wxT("WebUpdaterApp::SetupLocale - problems setting the %s catalog"), fn.c_str());
    } else {

        wxLogAdvMsg(wxT("WebUpdaterApp::SetupLocale - no custom catalog loaded from wxConfig"));
    }
}

bool WebUpdaterApp::OnPreInit()
{
    // create the default wx logger
    wxLog::SetActiveTarget(GetTraits()->CreateLogTarget());
    m_log = new wxWebUpdateLog();       // start this app's logger

    // show the user that we are running
    //wxBusyCursor wait;

    // support multiple language !
    SetAppName(wxT("WebUpdater"));      // before accessing wxConfig objects, let's set this
    wxConfig::Get()->SetRecordDefaults();
    SetupLocale();

    // parse the command line
    wxLogUsrMsg(_("WebUpdaterApp::OnPreInit - parsing the command line"));
    wxCmdLineParser parser(g_cmdLineDesc, argc, argv);
    if (parser.Parse() != 0)
        return 0;           // help was shown / an error occurred

    // check for other options & switches (ORDER IS IMPORTANT !)
    wxString xml, xrc, res, uri, askuri;

    if (parser.Found(SWITCH_SAVELOG))
        CreateFileLogger();             // if this switch is in the command line, 
                                        // create the logger immediately
    if (!parser.Found(OPTION_XMLSCRIPT, &xml))
        xml = wxWU_LOCAL_XMLSCRIPT;

    // don't load the default for these options now
    parser.Found(OPTION_XRC, &xrc);
    parser.Found(OPTION_RESOURCE, &res);
    bool hasUri = parser.Found(OPTION_URI, &uri);
    bool hasAskUri = parser.Found(OPTION_ASKURI, &askuri);

    // check for valid options
    wxFileName askurifn(askuri);
    if ((hasAskUri && hasUri) || (hasAskUri && !askurifn.IsOk())) {
        wxLogError(_("WebUpdaterApp::OnPreInit - you cannot use both '--ask-uri' and '--uri' !"));
        return FALSE;
    }

    // show our platform in log files
    wxLogAdvMsg(wxT("WebUpdaterApp::OnPreInit - current platform is [")
        + wxWebUpdatePlatform::GetThisPlatform().GetAsString() + wxT("]"));

    // this is for using wxDownloadThread
    wxLogAdvMsg(wxT("WebUpdaterApp::OnPreInit - initializing sockets & handlers"));
    wxSocketBase::Initialize();

    // load only required handles (so that the linker can remove all
    // the unused libraries); instead of
    //    wxInitAllImageHandlers();
    //    wxXmlResource::Get()->InitAllHandlers();
    // we do:
    wxInitRequiredImageHandlers();
    wxInitRequiredXmlHandlers();

    // this little snippet allows us to embed our WWW bitmap directly in the
    // webupdater app's executable so that the users of WebUpdate are not required
    // to ship together with the EXE also the www image...
    wxFileSystem::AddHandler(new wxMemoryFSHandler);
    wxMemoryFSHandler::AddFile(wxT("www.xpm"), wxBitmap(www_xpm), wxBITMAP_TYPE_XPM);

    // load the local XML webupdate script
    wxLogUsrMsg(_("WebUpdaterApp::OnPreInit - loading the local XML webupdate script %s"), xml.c_str());
    wxFileName fn(xml);
    fn.MakeAbsolute(wxGetCwd());
    if (!m_script.Load(fn.GetFullPath())) {
        wxWebUpdateInstaller::Get()->ShowErrorMsg(wxString::Format(
                    _("The installation of the WebUpdater component of this application\nis corrupted; the file:\n\n\t%s\n\nis missing (or invalid); please reinstall the program."), fn.GetFullPath().c_str()));
        return FALSE;
    }

    // eventually ask the user the remote XML file location
    if (hasAskUri) {
        wxString xmlname = askurifn.GetFullName();
        if (xmlname.IsEmpty()) {
            if (m_script.GetRemoteScriptURI()) {
                xmlname = m_script.GetRemoteScriptURI().AfterLast(wxFileName::GetPathSeparator());
            } else {
                wxLogError(_("WebUpdaterApp::OnPreInit - the --ask-uri option is incomplete"));
                return FALSE;
            }
        }

        // create the folder selector dialog
        wxString startpath = askurifn.GetPath();
        if (startpath.IsEmpty()) startpath = wxGetCwd();
        wxDirDialog dd(NULL, _("Choose the folder containing the updated packages"),
                            startpath);
        wxString path, file;
        do {
            if (dd.ShowModal() == wxID_OK)
                path = dd.GetPath();
            if (!path.IsEmpty() && path.Last() == wxFileName::GetPathSeparator())
                path.RemoveLast();
            file = path + wxFileName::GetPathSeparator() + xmlname;
        } while (path.IsEmpty() || !wxFileName(file).FileExists());

        // override the remote XML script
        wxFileName newremotexml(file);
        wxLogAdvMsg(wxT("WebUpdaterApp::OnPreInit - overriding <remoteuri> with [") +
                newremotexml.GetFullPath() + wxT("]"));
        uri = wxFileSystem::FileNameToURL(newremotexml);
    }

    // now set the option values
    if (!res.IsEmpty())
        m_script.OverrideXRCResName(res);           // command line options have precedence
    if (!xrc.IsEmpty())
        m_script.OverrideXRC(xrc);
    if (!uri.IsEmpty())
        m_script.OverrideRemoteScriptURI(uri);
    if (parser.Found(SWITCH_RESTART))
        m_script.OverrideRestartFlag(TRUE);
    if (parser.Found(SWITCH_SAVELOG))
        m_script.OverrideSaveLogFlag(TRUE);

    if (m_script.GetXRCResName().IsEmpty())
        m_script.OverrideXRCResName(wxWU_XRC_RESOURCE);     // load the defaults
    if (m_script.GetXRC().IsEmpty())
        m_script.OverrideXRC(wxWU_LOCAL_XRC);

    // do we need to create the logger as specified in the local XML file ?
    if (m_script.IsLogToSave())
        CreateFileLogger();

    // do not proceed if in this stage we are still missing some required info
    if (!m_script.IsComplete()) {
        wxWebUpdateInstaller::Get()->ShowErrorMsg(
                    _("The WebUpdater configuration file is corrupted; the local XML script is missing some required info. Please correct the local XML script or give these info to WebUpdater through the command line options."));
        return FALSE;
    }

    wxASSERT(m_script.IsOk());

    // translate only once ;)
    wxString msg = _("The WebUpdater configuration file is corrupted; the file:\n\n\t%s\n\nis missing (or invalid); please reinstall the program.");

    // load our XRC file
    wxLogUsrMsg(_("WebUpdaterApp::OnInit - loading the XRC file [%s]"), m_script.GetXRC().c_str());
    if (!wxXmlResource::Get()->Load(m_script.GetXRC())) {
        wxWebUpdateInstaller::Get()->ShowErrorMsg(wxString::Format(msg,
                    m_script.GetXRC().c_str()));
        return FALSE;
    }

    // check that the program-to-update executable exists
    wxString tmp = m_script.GetAppFile() +
        wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("exe"));
    wxFileName app2update(tmp);
    app2update.MakeAbsolute(wxGetCwd());
    if (!wxFileName::FileExists(app2update.GetFullPath())) {
        wxWebUpdateInstaller::Get()->ShowErrorMsg(wxString::Format(msg,
                    app2update.GetFullPath().c_str()));
        return FALSE;
    }

    // initialize MOD, if present
#ifdef MODDED
    if (!wxInitializeMod())
    return FALSE;
#endif

    return TRUE;
}

bool WebUpdaterApp::OnInit()
{
    mcDUMP_ON_EXIT;         // for debugging only

    if (!OnPreInit()) {
        wxUninitializeWebUpdate();
        return FALSE;
    }

    // create our main dialog
#if 1
    wxLogUsrMsg(_("WebUpdaterApp::OnInit - creating the WebUpdaterDlg"));
    m_dlg = new WebUpdaterDlg(m_script);
    if (!m_dlg->IsOk())
        return FALSE;       // failure
    SetTopWindow(m_dlg);
    SetExitOnFrameDelete(TRUE);
#else

    // to test wxWebUpdateAdvPanel alone
    m_dlg = new wxDialog(NULL, -1, wxT(""), wxDefaultPosition, wxDefaultSize,
                         wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    wxBoxSizer *sz = new wxBoxSizer(wxVERTICAL);
    sz->Add(new wxWebUpdateAdvPanel(m_dlg, m_script), 1, 5, wxGROW);
    sz->SetSizeHints(m_dlg);
    SetTopWindow(m_dlg);
#endif

    // show the dialog...
    m_dlg->CenterOnScreen();
    m_dlg->Show(TRUE);

    // success
    return TRUE;
}

int WebUpdaterApp::OnExit()
{
    // uninitialize MOD, if present
#ifdef MODDED
    wxUninitializeMod();
#endif
    wxUninitializeWebUpdate();

    // before exiting this app, rerun the program we've just updated
    if (m_dlg->IsAppToRestart()) {

        wxString cmd =
#ifdef __WXMSW__
            m_script.GetAppFile() + wxT(".exe");
#else
            wxT("./") + m_script.GetAppFile();
#endif

        wxLogAdvMsg(wxT("WebUpdaterApp::OnExit - restarting the updated application [") +
                    cmd + wxT("]..."));
        wxExecute(cmd, wxEXEC_ASYNC);
    }

    wxLogAdvMsg(wxT("WebUpdaterApp::OnExit - calling wxApp::OnExit"));

    // before stopping do log an end-of-session message
    wxLogNewSection(wxT(" END OF LOG SESSION"));

    return wxApp::OnExit();
}

void WebUpdaterApp::OnExecute(wxCommandEvent &ce)
{
    wxString cmd = ce.GetString();
    int flags = ce.GetInt();
    wxMutex *m = (wxMutex*)ce.GetExtraLong();
    wxCondition *cond = (wxCondition *)ce.GetClientData();

    wxMutexLocker locker(*m);
    int res = wxExecute(cmd, flags);

    // we cannot save the return code in the event since the event we have here
    // is not the same event instance sent by the wxWebUpdateAction::wxExecute
    // function...
    // so we are forced to save the result directly in a global variable of
    // wxWebUpdateAction - hacky way to do it !!
    wxWebUpdateAction::m_nExecResult = res;

    // tell wxWebUpdateAction that we have finished
    wxLogAdvMsg(wxT("WebUpdaterApp::OnExecute - executing the command:\n\n\t\t") +
                cmd + wxT("\n\n with flags: %d; the exit code is: %d"), flags, res);
    cond->Broadcast();          // let the wxWebUpdateActionRun know that we have finished
}




// ----------------------------------------------------------------------------
// WEBUPDATERDLG
// ----------------------------------------------------------------------------

WebUpdaterDlg::WebUpdaterDlg(const wxWebUpdateLocalXMLScript &script)
                        : wxWebUpdateDlg(NULL, script, &(wxGetApp().m_locale))
{
    //wxIconBundle icons(wxT("app.ico"), wxBITMAP_TYPE_ICO);

    // our icon comes from the BlueSphere icon theme,
    // look under "apps/up2date.png", which I found at:
    //    http://svgicons.sourceforge.net/
    // Thanks to the author of the SVGIcons project, Vadim Plessky !
    SetIcon(wxICON(app));
}

void WebUpdaterDlg::OnQuit(wxCloseEvent &)
{
    // NOTE Since our main window is a dialog and not
    // ---- a frame we have to close it using Destroy
    //      rather than Close. In fact Close doesn't
    //      actually close a dialog but just hides it
    //      so that the application will hang there
    //      with his only window hidden and thus unable
    //      to get any user input.

    // --> Don't use Close with a wxDialog,
    //     use Destroy instead.
    Destroy();
    wxLogUsrMsg(_("WebUpdaterDlg::OnQuit - quitting"));
}

void WebUpdaterDlg::EndModal(int)
{
    // we are not showing the dlg as modal, thus just close it
    Close(true);
}

