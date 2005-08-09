////////////////////////////////////////////////////////////////////////////
// Name:        app.cpp
// Purpose:     The WebUpdater application
// Author:      Francesco Montorsi
// Modified by: 
// Created:     29/07/05
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// defines
// ----------------------------------------------------------------------------

//! The local XML m_script which is loaded by default by the WebUpdater application.
#define wxWU_LOCAL_XMLSCRIPT		wxT("local.xml")

//! The local XRC file which is loaded.
#define wxWU_LOCAL_XRC				wxT("webupdatedlg.xrc")

//! The name of our default log file.
#define wxWU_LOGFILENAME			wxT("webupdatelog.txt")


#if defined( __WXMSW__ ) && defined( __VISUALC__ )
#include <crtdbg.h>
#define mcDUMP_ON_EXIT		\
	{ _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF); }
#else
#define mcDUMP_ON_EXIT		/* expand to nothing */
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

#include "wx/webupdatedlg.h"
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

#define OPTION_XMLSCRIPT		wxT("l")
#define OPTION_XRC				wxT("x")
#define SWITCH_RESTART			wxT("r")
#define SWITCH_SAVELOG			wxT("s")

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
	// options
	{ wxCMD_LINE_OPTION, OPTION_XMLSCRIPT, wxT("xml"), 
		wxT("Use the given local XML file"),
		wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_OPTION, OPTION_XRC, wxT("xrc"), 
		wxT("Use the given local XRC file"),
		wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
		
	// switches
	{ wxCMD_LINE_SWITCH, SWITCH_RESTART, wxT("restart"), 
		wxT("Restart the updated application when WebUpdater is closed"),
		wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_SWITCH, SWITCH_SAVELOG, wxT("savelog"), 
		wxT("Saves the log messages to '") wxWU_LOGFILENAME wxT("'"),
		wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },

	// help
	{ wxCMD_LINE_SWITCH, wxT("h"), wxT("help"), wxT("Show this help message"),
		wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },

	{ wxCMD_LINE_NONE }
};


// -----------------------------------------------------------------------------
// a little log class which saves log messages to a file (see /savelog switch)
// by Francesco Montorsi <frm@users.sourceforge.net>; wxWidgets-licensed
// -----------------------------------------------------------------------------

class wxFileLog : public wxLogPassThrough
{
	wxTextFile out;

public:
	wxFileLog(const wxString &filename, const wxString &header = wxEmptyString) { 
		wxRemoveFile(filename);
		out.Create(filename);

		// create a little header to make our log nicer
		if (!header.IsEmpty()) {
			out.AddLine(wxEmptyString);
			out.AddLine(header);
			out.AddLine(wxT(" ") + wxString(wxT('-'), 70));
			out.AddLine(wxEmptyString);
		}

		PassMessages(TRUE);
	}

	virtual ~wxFileLog() 
		{ out.Write(); out.Close(); }

	// writes to the file the log message
	virtual void DoLog(wxLogLevel level, const wxChar *msg, time_t timestamp)
		{ wxLogPassThrough::DoLog(level, msg, timestamp);
		  wxString str; TimeStamp(&str);
		  out.AddLine(wxT(" ") + str + msg); }
};


// ----------------------------------------------------------------------------
// the application
// ----------------------------------------------------------------------------

class WebUpdaterApp : public wxApp
{
public:

	// our main dialog
	wxDialog *m_dlg;

	// our local script
	wxWebUpdateLocalXMLScript m_script;
	
	// our extra options (taken from command line)
	wxWebUpdateExtraOptions m_optExtra;

	// our log window
	wxLogWindow *m_log;

public:
	WebUpdaterApp() : m_dlg(NULL), m_log(NULL) {}
	virtual ~WebUpdaterApp() {}

	// Initializes the WebUpdaterApp.
    virtual bool OnInit();
    
    // Closes the WebUpdaterApp.
	virtual int OnExit();

public:			// we need to intercept the execute event; see wxWebUpdateActionRun::Run
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
	WebUpdaterDlg(const wxWebUpdateLocalXMLScript &script,
					wxWebUpdateExtraOptions *opt);
	virtual ~WebUpdaterDlg() {}


public:			// we need to intercept some events in order to make
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

// using this function instead of wxXmlResource::Get()->InitAllHandlers()
// let us to avoid the ADV and HTML wx libraries (unused in this app) and
// thus make the executable smaller
void wxInitRequiredXmlHandlers()
{
	wxXmlResource *p = wxXmlResource::Get();
		
	p->AddHandler(new wxBitmapXmlHandler);
    p->AddHandler(new wxIconXmlHandler);
    p->AddHandler(new wxMenuXmlHandler);
    p->AddHandler(new wxMenuBarXmlHandler);
    p->AddHandler(new wxDialogXmlHandler);
    p->AddHandler(new wxPanelXmlHandler);
    p->AddHandler(new wxSizerXmlHandler);
    p->AddHandler(new wxStdDialogButtonSizerXmlHandler);
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
#if wxUSE_SCROLLBAR
    p->AddHandler(new wxScrollBarXmlHandler);
#endif
#if wxUSE_RADIOBOX
    p->AddHandler(new wxRadioBoxXmlHandler);
    p->AddHandler(new wxRadioButtonXmlHandler);
#endif
#if wxUSE_COMBOBOX
    p->AddHandler(new wxComboBoxXmlHandler);
#endif
#if wxUSE_NOTEBOOK
    p->AddHandler(new wxNotebookXmlHandler);
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
#if wxUSE_TOOLBAR
    p->AddHandler(new wxToolBarXmlHandler);
#endif
#if wxUSE_STATLINE
    p->AddHandler(new wxStaticLineXmlHandler);
#endif
    p->AddHandler(new wxUnknownWidgetXmlHandler);
#if wxUSE_DIRDLG
    p->AddHandler(new wxGenericDirCtrlXmlHandler);
#endif
    p->AddHandler(new wxFrameXmlHandler);
    p->AddHandler(new wxScrolledWindowXmlHandler);
    p->AddHandler(new wxSplitterWindowXmlHandler);
}

bool WebUpdaterApp::OnInit()
{
	mcDUMP_ON_EXIT;			// for debugging only	
	//_CrtSetBreakAlloc(31100);

	// parse the command line
	wxLogDebug(wxT("WebUpdaterApp::OnInit - parsing the command line"));
    wxCmdLineParser parser(g_cmdLineDesc, argc, argv);
    if (parser.Parse() != 0)
		return 0;		// help was shown / an error occurred

#ifdef __WXDEBUG__XXXXX				// the logframe causes problems in the exit stage
	// create an useful log window
	m_log = new wxLogWindow(NULL, wxT("WebUpdater log"));
	m_log->GetFrame()->Move(50, 50+350);
	m_log->GetFrame()->SetSize(800, 300);
#endif
	
	// check for other options & switches (ORDER IS IMPORTANT !)
	wxString xml, xrc;
	
	m_optExtra.m_bRestart = parser.Found(SWITCH_RESTART);
	m_optExtra.m_bSaveLog = parser.Found(SWITCH_SAVELOG);
	
	if (parser.Found(SWITCH_SAVELOG))
		new wxFileLog(wxWU_LOGFILENAME, wxT(" LOG OF WEBUPDATER ") + 
						wxWebUpdateInstaller::Get()->GetVersion() + 
						wxT(" SESSION BEGAN AT ") + 
						wxDateTime::Now().Format(wxT("%x %X")));		// it automatically installs itself as the new logger
	if (!parser.Found(OPTION_XMLSCRIPT, &xml))
		xml = wxWU_LOCAL_XMLSCRIPT;
	if (!parser.Found(OPTION_XRC, &xrc))
		xrc = wxWU_LOCAL_XRC;

	// this is for using wxDownloadThread
	wxLogDebug(wxT("WebUpdaterApp::OnInit - initializing sockets & handlers"));
	wxSocketBase::Initialize();

	// this little snippet allows us to embed our WWW bitmap directly in the
	// webupdater app's executable so that the users of WebUpdate are not required
	// to ship together with the EXE also the www image...
	wxInitAllImageHandlers();
	//wxXmlResource::Get()->InitAllHandlers();
	wxInitRequiredXmlHandlers();
	wxFileSystem::AddHandler(new wxMemoryFSHandler);
    wxMemoryFSHandler::AddFile(wxT("www.xpm"), wxBitmap(www_xpm), wxBITMAP_TYPE_XPM);	

	// load the local XML webupdate script
	wxLogDebug(wxT("WebUpdaterApp::OnInit - loading the local XML webupdate script ") + xml);	
	wxFileName fn(xml);
	fn.MakeAbsolute(wxGetCwd());
	if (!m_script.Load(fn.GetFullPath())) {
		wxWebUpdateInstaller::Get()->ShowErrorMsg(
					wxT("The installation of the WebUpdater component of this application\n")
					wxT("is corrupted; the file:\n\n\t") + 
					fn.GetFullPath() + 
					wxT("\n\n is missing (or invalid); please reinstall the program."));
		return FALSE;
	}
	
    // load our XRC file
	wxLogDebug(wxT("WebUpdaterApp::OnInit - loading the XRC file ") + xrc);    
    if (!wxXmlResource::Get()->Load(xrc)) {
		wxWebUpdateInstaller::Get()->ShowErrorMsg(
					wxT("The WebUpdater configuration file is corrupted; the file:\n\n\t") +				
					xrc + 
					wxT("\n\n is missing (or invalid); please reinstall the program."));
		return FALSE;
	}

	// check that the program-to-update EXE exists
	wxString tmp = m_script.GetAppFile() + 
		wxWebUpdateInstaller::Get()->GetKeywordValue(wxT("exe"));
	wxFileName app2update(tmp);
	app2update.MakeAbsolute(wxGetCwd());
	if (!wxFileName::FileExists(app2update.GetFullPath())) {
		wxWebUpdateInstaller::Get()->ShowErrorMsg(
					wxT("The WebUpdater configuration file is corrupted; the file:\n\n\t") +					
					app2update.GetFullPath() + 
					wxT("\n\n is missing (or invalid); please reinstall the program."));
		return FALSE;
	}

	// create our main dialog
#if 1
	wxLogDebug(wxT("WebUpdaterApp::OnInit - creating the WebUpdaterDlg"));
	m_dlg = new WebUpdaterDlg(m_script, &m_optExtra);
	SetTopWindow(m_dlg);
	SetExitOnFrameDelete(TRUE);
#else

	// to test wxWebUpdateAdvPanel alone
	m_dlg = new wxDialog(NULL, -1, wxT(""), wxDefaultPosition, wxDefaultSize, 
		wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
	new wxWebUpdateAdvPanel(m_dlg);
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
	// remove the singleton objects
	wxLogDebug(wxT("WebUpdaterApp::OnExit - deleting the WebUpdate installer"));	
	delete wxWebUpdateInstaller::Set(NULL);
	
	// before exiting this app, rerun the program we've just updated
	if (m_optExtra.m_bRestart) {
	
		wxString cmd =
#ifdef __WXMSW__
			m_script.GetAppFile() + wxT(".exe");
#else
			wxT("./") + m_script.GetAppFile();
#endif 
		
		wxLogDebug(wxT("WebUpdaterApp::OnExit - restarting the updated application [") + 
					cmd + wxT("]..."));	
		wxExecute(cmd, wxEXEC_ASYNC);
	}

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
	wxLogDebug(wxT("WebUpdaterApp::OnExecute - executing the command:\n\n\t\t") +
				cmd + wxT("\n\n with flags: %d; the exit code is: %d"), flags, res);	
	cond->Broadcast();		// let the wxWebUpdateActionRun know that we have finished
}


// ----------------------------------------------------------------------------
// WEBUPDATERDLG
// ----------------------------------------------------------------------------

WebUpdaterDlg::WebUpdaterDlg(const wxWebUpdateLocalXMLScript &script, wxWebUpdateExtraOptions *opt)
						: wxWebUpdateDlg(NULL, script, opt) 
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
#ifdef __WXDEBUG__
	wxFrame *f = NULL;
	if (wxGetApp().m_log) f = wxGetApp().m_log->GetFrame();
	if (f) f->Close(true);
#endif
    Destroy();
	wxLogDebug(wxT("WebUpdaterDlg::OnQuit - quitting"));
}

void WebUpdaterDlg::EndModal(int)
{
	// we are not showing the dlg as modal, thus just close it
	Close(true);
}

